#include "STDInclude.hpp"

namespace Components
{
	std::recursive_mutex Node::Mutex;
	std::vector<Node::Entry> Node::Nodes;

	bool Node::wasIngame = false;

	bool Node::Entry::isValid()
	{
		return (this->lastResponse.has_value() && !this->lastResponse->elapsed(NODE_HALFLIFE * 2));
	}

	bool Node::Entry::isDead()
	{
		if (!this->lastResponse.has_value())
		{
			if (this->lastRequest.has_value() && this->lastRequest->elapsed(NODE_HALFLIFE))
			{
				return true;
			}
		}
		else if(this->lastResponse->elapsed(NODE_HALFLIFE * 2) && this->lastRequest.has_value() && this->lastRequest->after(*this->lastResponse))
		{
			return true;
		}

		return false;
	}

	bool Node::Entry::requiresRequest()
	{
		return (!this->isDead() && (!this->lastRequest.has_value() || this->lastRequest->elapsed(NODE_HALFLIFE)));
	}

	void Node::Entry::sendRequest()
	{
		if (!this->lastRequest.has_value()) this->lastRequest.emplace(Utils::Time::Point());
		this->lastRequest->update();

		Session::Send(this->address, "nodeListRequest");
		Node::SendList(this->address);
		NODE_LOG("Sent request to %s\n", this->address.getCString());
	}

	void Node::Entry::reset()
	{
		// this->lastResponse.reset(); // This would invalidate the node, but maybe we don't want that?
		this->lastRequest.reset();
	}

	json11::Json Node::Entry::to_json() const
	{
		return this->address.getString();
	}

	void Node::LoadNodeRemotePreset()
	{
		std::string nodes = Utils::Cache::GetFile("/iw4/nodes.txt");
		if (nodes.empty()) return;

		auto nodeList = Utils::String::Explode(nodes, '\n');
		for (auto& node : nodeList)
		{
			Utils::String::Replace(node, "\r", "");
			node = Utils::String::Trim(node);
			Node::Add(node);
		}
	}

	void Node::LoadNodePreset()
	{
		Proto::Node::List list;

		if (Monitor::IsEnabled())
		{
			std::string nodes = Utils::IO::ReadFile("players/nodes_default.dat");
			if (nodes.empty() || !list.ParseFromString(Utils::Compression::ZLib::Decompress(nodes))) return;
		}
		else
		{
			FileSystem::File defaultNodes("nodes_default.dat");
			if (!defaultNodes.exists() || !list.ParseFromString(Utils::Compression::ZLib::Decompress(defaultNodes.getBuffer()))) return;
		}

		for (int i = 0; i < list.nodes_size(); ++i)
		{
			const std::string& addr = list.nodes(i);

			if (addr.size() == sizeof(sockaddr))
			{
				Node::Add(reinterpret_cast<sockaddr*>(const_cast<char*>(addr.data())));
			}
		}
	}

	void Node::LoadNodes()
	{
		Proto::Node::List list;
		std::string nodes = Utils::IO::ReadFile("players/nodes.dat");
		if (nodes.empty() || !list.ParseFromString(Utils::Compression::ZLib::Decompress(nodes))) return;

		for (int i = 0; i < list.nodes_size(); ++i)
		{
			const std::string& addr = list.nodes(i);

			if (addr.size() == sizeof(sockaddr))
			{
				Node::Add(reinterpret_cast<sockaddr*>(const_cast<char*>(addr.data())));
			}
		}
	}

	void Node::StoreNodes(bool force)
	{
		if (Dedicated::IsEnabled() && Dvar::Var("sv_lanOnly").get<bool>()) return;

		static Utils::Time::Interval interval;
		if (!force && !interval.elapsed(1min)) return;
		interval.update();

		Proto::Node::List list;

		Node::Mutex.lock();
		for (auto& node : Node::Nodes)
		{
			if (node.isValid())
			{
				std::string* str = list.add_nodes();

				sockaddr addr = node.address.getSockAddr();
				str->append(reinterpret_cast<char*>(&addr), sizeof(addr));
			}
		}
		Node::Mutex.unlock();

		Utils::IO::WriteFile("players/nodes.dat", Utils::Compression::ZLib::Compress(list.SerializeAsString()));
	}

	void Node::Add(Network::Address address)
	{
#ifndef DEBUG
		if (address.isLocal() || address.isSelf()) return;
#endif

		if (!address.isValid()) return;

		std::lock_guard<std::recursive_mutex> _(Node::Mutex);
		for (auto& session : Node::Nodes)
		{
			if (session.address == address) return;
		}

		Node::Entry node;
		node.address = address;

		Node::Nodes.push_back(node);
	}

	std::vector<Node::Entry> Node::GetNodes()
	{
		std::lock_guard<std::recursive_mutex> _(Node::Mutex);

		return Node::Nodes;
	}

	void Node::RunFrame()
	{
		if (Dedicated::IsEnabled())
		{
			bool isServerLoading = Utils::Hook::Call<bool()>(0x40BC50)();
			if(Dvar::Var("sv_lanOnly").get<bool>() || isServerLoading)
				return; // don't run if sv_lanOnly or while server is still loading
		}

		bool CL_IsCgameTrulyInitialized = Utils::Hook::Get<bool>(0xB2BB8B);
		if (!Dedicated::IsEnabled() && CL_IsCgameTrulyInitialized)
		{
			wasIngame = true;
			return; // don't run while ingame because it can crash the game and still cause lag spikes on lower end PCs
		}

		if (wasIngame) // our last frame we were ingame and now we aren't so touch all nodes
		{
			for (auto i = Node::Nodes.begin(); i != Node::Nodes.end();++i)
			{
				// clearing the last request and response times makes the 
				// dispatcher think its a new node and will force a refresh
				i->lastRequest.reset();
				i->lastResponse.reset();
			}
			wasIngame = false;
		}

		static Utils::Time::Interval frameLimit;
		int interval = static_cast<int>(1000.0f / Dvar::Var("net_serverFrames").get<int>());
		if (!frameLimit.elapsed(std::chrono::milliseconds(interval))) return;
		frameLimit.update();

		std::lock_guard<std::recursive_mutex> _(Node::Mutex);
		Dvar::Var queryLimit("net_serverQueryLimit");

		int sentRequests = 0;
		for (auto i = Node::Nodes.begin(); i != Node::Nodes.end();)
		{
			if (i->isDead())
			{
				i = Node::Nodes.erase(i);
				continue;
			}
			else if (sentRequests < queryLimit.get<int>() && i->requiresRequest())
			{
				++sentRequests;
				i->sendRequest();
			}

			++i;
		}
	}

	void Node::Synchronize()
	{
		std::lock_guard<std::recursive_mutex> _(Node::Mutex);
		for (auto& node : Node::Nodes)
		{
			//if (node.isValid()) // Comment out to simulate 'syncnodes' behaviour
			{
				node.reset();
			}
		}
	}

	void Node::HandleResponse(Network::Address address, const std::string& data)
	{
		Proto::Node::List list;
		if (!list.ParseFromString(data)) return;

		NODE_LOG("Received response from %s\n", address.getCString());

		std::lock_guard<std::recursive_mutex> _(Node::Mutex);

		for (int i = 0; i < list.nodes_size(); ++i)
		{
			const std::string& addr = list.nodes(i);

			if (addr.size() == sizeof(sockaddr))
			{
				Node::Add(reinterpret_cast<sockaddr*>(const_cast<char*>(addr.data())));
			}
		}

		if (list.isnode() && (!list.port() || list.port() == address.getPort()))
		{
			if (!Dedicated::IsEnabled() && ServerList::IsOnlineList() && list.protocol() == PROTOCOL)
			{
				NODE_LOG("Inserting %s into the serverlist\n", address.getCString());
				ServerList::InsertRequest(address);
			}
			else
			{
				NODE_LOG("Dropping serverlist insertion for %s\n", address.getCString());
			}

			for (auto& node : Node::Nodes)
			{
				if (address == node.address)
				{
					if (!node.lastResponse.has_value()) node.lastResponse.emplace(Utils::Time::Point());
					node.lastResponse->update();

					node.data.protocol = list.protocol();
					return;
				}
			}

			Node::Entry entry;
			entry.address = address;
			entry.data.protocol = list.protocol();
			entry.lastResponse.emplace(Utils::Time::Point());

			Node::Nodes.push_back(entry);
		}
	}

	void Node::SendList(Network::Address address)
	{
		Proto::Node::List list;
		list.set_isnode(Dedicated::IsEnabled());
		list.set_protocol(PROTOCOL);
		list.set_port(Node::GetPort());

		std::lock_guard<std::recursive_mutex> _(Node::Mutex);

		for (auto& node : Node::Nodes)
		{
			if (node.isValid())
			{
				std::string* str = list.add_nodes();

				sockaddr addr = node.address.getSockAddr();
				str->append(reinterpret_cast<char*>(&addr), sizeof(addr));
			}
		}

		Session::Send(address, "nodeListResponse", list.SerializeAsString());
	}

	unsigned short Node::GetPort()
	{
		if (Dvar::Var("net_natFix").get<bool>()) return 0;
		return Network::GetPort();
	}

	Node::Node()
	{
		if (ZoneBuilder::IsEnabled()) return;
		Dvar::Register<bool>("net_natFix", false, 0, "Fix node registration for certain firewalls/routers");

		Scheduler::OnFrameAsync([]()
		{
			Node::StoreNodes(false);
		});

		Scheduler::OnFrame(Node::RunFrame);
		Session::Handle("nodeListResponse", Node::HandleResponse);
		Session::Handle("nodeListRequest", [](Network::Address address, const std::string&)
		{
			Node::SendList(address);
		});

		// Load stored nodes
		auto loadNodes = []()
		{
			Node::LoadNodePreset();
			Node::LoadNodes();
		};

		if (Monitor::IsEnabled()) Network::OnStart(loadNodes);
		else Dvar::OnInit(loadNodes);

		Network::OnStart([]()
		{
			std::thread([]()
			{
				Node::LoadNodeRemotePreset();
			}).detach();
		});

		Command::Add("listnodes", [](Command::Params*)
		{
			Logger::Print("Nodes: %d\n", Node::Nodes.size());

			std::lock_guard<std::recursive_mutex> _(Node::Mutex);
			for (auto& node : Node::Nodes)
			{
				Logger::Print("%s\t(%s)\n", node.address.getCString(), node.isValid() ? "Valid" : "Invalid");
			}
		});

		Command::Add("addnode", [](Command::Params* params)
		{
			if (params->length() < 2) return;
			Node::Add({ params->get(1) });
		});
	}

	Node::~Node()
	{
		std::lock_guard<std::recursive_mutex> _(Node::Mutex);
		Node::StoreNodes(true);
		Node::Nodes.clear();
	}
}
