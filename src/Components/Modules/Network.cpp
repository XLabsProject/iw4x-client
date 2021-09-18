#include "STDInclude.hpp"

namespace Components
{
	std::string Network::SelectedPacket;
	Utils::Signal<Network::CallbackRaw> Network::StartupSignal;
	std::map<std::string, Utils::Slot<Network::Callback>> Network::PacketHandlers;

	Network::Address::Address(const std::string& addrString)
	{
		Game::NET_StringToAdr(addrString.data(), &this->address);
	}
	Network::Address::Address(sockaddr* addr)
	{
		Game::SockadrToNetadr(addr, &this->address);
	}
	bool Network::Address::operator==(const Network::Address& obj) const
	{
		return Game::NET_CompareAdr(this->address, obj.address);
	}
	void Network::Address::setPort(unsigned short port)
	{
		this->address.port = htons(port);
	}
	unsigned short Network::Address::getPort()
	{
		return ntohs(this->address.port);
	}
	void Network::Address::setIP(DWORD ip)
	{
		this->address.ip.full = ip;
	}
	void Network::Address::setIP(Game::netIP_t ip)
	{
		this->address.ip = ip;
	}
	Game::netIP_t Network::Address::getIP()
	{
		return this->address.ip;
	}
	void Network::Address::setType(Game::netadrtype_t type)
	{
		this->address.type = type;
	}
	Game::netadrtype_t Network::Address::getType()
	{
		return this->address.type;
	}
	sockaddr Network::Address::getSockAddr()
	{
		sockaddr addr;
		this->toSockAddr(&addr);
		return addr;
	}
	void Network::Address::toSockAddr(sockaddr* addr)
	{
		if (addr)
		{
			Game::NetadrToSockadr(&this->address, addr);
		}
	}
	void Network::Address::toSockAddr(sockaddr_in* addr)
	{
		this->toSockAddr(reinterpret_cast<sockaddr*>(addr));
	}
	Game::netadr_t* Network::Address::get()
	{
		return &this->address;
	}
	const char* Network::Address::getCString() const
	{
		return Game::NET_AdrToString(this->address);
	}
	std::string Network::Address::getString() const
	{
		return this->getCString();
	}
	bool Network::Address::isLocal()
	{
		// According to: https://en.wikipedia.org/wiki/Private_network

		// 10.X.X.X
		if (this->getIP().bytes[0] == 10) return true;

		// 192.168.X.X
		if (this->getIP().bytes[0] == 192 && this->getIP().bytes[1] == 168) return true;

		// 172.16.X.X - 172.31.X.X
		if (this->getIP().bytes[0] == 172 && (this->getIP().bytes[1] >= 16) && (this->getIP().bytes[1] < 32)) return true;

		// 127.0.0.1
		if (this->getIP().full == 0x0100007F) return true;

		// TODO: Maybe check for matching localIPs and subnet mask

		return false;
	}
	bool Network::Address::isSelf()
	{
		if (Game::NET_IsLocalAddress(this->address)) return true; // Loopback
		if (this->getPort() != Network::GetPort()) return false; // Port not equal

		for (int i = 0; i < *Game::numIP; ++i)
		{
			if (this->getIP().full == Game::localIP[i].full)
			{
				return true;
			}
		}

		return false;
	}
	bool Network::Address::isLoopback()
	{
		if (this->getIP().full == 0x100007f) // 127.0.0.1
		{
			return true;
		}

		return Game::NET_IsLocalAddress(this->address);
	}
	bool Network::Address::isValid()
	{
		return (this->getType() != Game::netadrtype_t::NA_BAD && this->getType() >= Game::netadrtype_t::NA_BOT && this->getType() <= Game::netadrtype_t::NA_IP);
	}
	void Network::Handle(const std::string& packet, Utils::Slot<Network::Callback> callback)
	{
		Network::PacketHandlers[Utils::String::ToLower(packet)] = callback;
	}

	void Network::OnStart(Utils::Slot<Network::CallbackRaw> callback)
	{
		Network::StartupSignal.connect(callback);
	}

	void Network::Send(Game::netsrc_t type, Network::Address target, const std::string& data)
	{
		// NET_OutOfBandPrint only supports non-binary data!
		//Game::NET_OutOfBandPrint(type, *target.Get(), data.data());

		std::string rawData;
		rawData.append("\xFF\xFF\xFF\xFF", 4);
		rawData.append(data);
		//rawData.append("\0", 1);

		Network::SendRaw(type, target, rawData);
	}

	void Network::Send(Network::Address target, const std::string& data)
	{
		Network::Send(Game::netsrc_t::NS_CLIENT1, target, data);
	}

	void Network::SendRaw(Game::netsrc_t type, Network::Address target, const std::string& data)
	{
		if (!target.isValid()) return;

		// NET_OutOfBandData doesn't seem to work properly
		//Game::NET_OutOfBandData(type, *target.Get(), data.data(), data.size());
		Game::Sys_SendPacket(type, data.size(), data.data(), *target.get());
	}

	void Network::SendRaw(Network::Address target, const std::string& data)
	{
		Network::SendRaw(Game::netsrc_t::NS_CLIENT1, target, data);
	}

	void Network::SendCommand(Game::netsrc_t type, Network::Address target, const std::string& command, const std::string& data)
	{
		// Use space as separator (possible separators are '\n', ' ').
		// Though, our handler only needs exactly 1 char as separator and doesn't care which char it is.
		// EDIT: Most 3rd party tools expect a line break, so let's use that instead!
		std::string packet;
		packet.append(command);
		packet.append("\n", 1);
		packet.append(data);

		Network::Send(type, target, packet);
	}

	void Network::SendCommand(Network::Address target, const std::string& command, const std::string& data)
	{
		Network::SendCommand(Game::netsrc_t::NS_CLIENT1, target, command, data);
	}

	void Network::Broadcast(unsigned short port, const std::string& data)
	{
		Address target;

		target.setPort(port);
		target.setIP(INADDR_BROADCAST);
		target.setType(Game::netadrtype_t::NA_BROADCAST);

		Network::Send(Game::netsrc_t::NS_CLIENT1, target, data);
	}

	void Network::BroadcastRange(unsigned int min, unsigned int max, const std::string& data)
	{
		for (unsigned int i = min; i < max; ++i)
		{
			Network::Broadcast(static_cast<unsigned short>(i & 0xFFFF), data);
		}
	}

	void Network::BroadcastAll(const std::string& data)
	{
		Network::BroadcastRange(100, 65536, data);
	}

	int Network::PacketInterceptionHandler(const char* packet)
	{
		// Packet rate limit.
		static uint32_t packets = 0;
		static int lastClean = 0;

		if ((Game::Sys_Milliseconds() - lastClean) > 1'000)
		{
			packets = 0;
			lastClean = Game::Sys_Milliseconds();
		}

		if ((++packets) > NETWORK_MAX_PACKETS_PER_SECOND)
		{
			return 1;
		}

		std::string packetCommand = packet;
		auto pos = packetCommand.find_first_of("\\\n ");
		if (pos != std::string::npos)
		{
			packetCommand = packetCommand.substr(0, pos);
		}

		packetCommand = Utils::String::ToLower(packetCommand);

		// Check if custom handler exists
		for (auto i = Network::PacketHandlers.begin(); i != Network::PacketHandlers.end(); ++i)
		{
			if (Utils::String::ToLower(i->first) == packetCommand)
			{
				Network::SelectedPacket = i->first;
				return 0;
			}
		}

		// No interception
		return 1;
	}

	void Network::DeployPacket(Game::netadr_t* from, Game::msg_t* msg)
	{
		if (Network::PacketHandlers.find(Network::SelectedPacket) != Network::PacketHandlers.end())
		{
			std::string data;

			size_t offset = Network::SelectedPacket.size() + 4 + 1;

			if (static_cast<size_t>(msg->cursize) > offset)
			{
				data.append(msg->data + offset, msg->cursize - offset);
			}

			// Remove trailing 0x00 byte
			// Actually, don't remove it, it might be part of the packet. Send correctly formatted packets instead!
			//if (data.size() && !data[data.size() - 1]) data.pop_back();

			Network::PacketHandlers[Network::SelectedPacket](from, data);
		}
		else
		{
			Logger::Print("Error: Network packet intercepted, but handler is missing!\n");
		}
	}

	void Network::NetworkStart()
	{
		Network::StartupSignal();
	}

	unsigned short Network::GetPort()
	{
		return static_cast<unsigned short>(Dvar::Var(0x64A3004).get<unsigned int>());
	}

	__declspec(naked) void Network::NetworkStartStub()
	{
		__asm
		{
			mov eax, 64D900h
			call eax
			jmp Network::NetworkStart
		}
	}

	bool Network::ShouldDeployClientPacket()
	{
		return Party::IsEnabled() || !Dedicated::IsEnabled();
	}

	__declspec(naked) void Network::ClientDeployPacketStub()
	{
		__asm
		{
			pushad

			call ShouldDeployClientPacket
			test al, al
			je skip

			lea eax, [esp + 0C54h + 20h]
			push ebp // Command
			push eax // Address pointer
			call Network::DeployPacket
			add esp, 8h
		skip:
			popad

			mov al, 1
			pop edi
			pop esi
			pop ebp
			pop ebx
			add esp, 0C40h
			retn
		}
	}

	__declspec(naked) void Network::ServerDeployPacketStub()
	{
		__asm
		{
			lea eax, [esp + 408h]

			pushad

			push esi // msg 
			push eax // from 
			call Network::DeployPacket
			add esp, 8

			popad

			push 6267EBh
			ret
		}
	}

	__declspec(naked) void Network::PacketErrorCheck()
	{
		__asm
		{
			cmp eax, 2746h
			jz returnIgnore

			cmp eax, WSAENETRESET
			jz returnIgnore

			push 465325h
			retn

		returnIgnore:
			push 4654C6h
			retn
		}
	}

	void Network::NET_DeferPacketToClientStub(Game::netadr_t* from, Game::msg_t* msg)
	{
		if (msg->cursize > 0 && msg->cursize <= 1404)
			Game::NET_DeferPacketToClient(from, msg);
	}

	void Network::SV_ExecuteClientMessageStub(Game::client_t* client, Game::msg_t* msg)
	{
		if (client->reliableAcknowledge < 0)
		{
			Logger::Print("Negative reliableAcknowledge from %s - cl->reliableSequence is %i, reliableAcknowledge is %i\n",
							client->name, client->reliableSequence, client->reliableAcknowledge);
			client->reliableAcknowledge = client->reliableSequence;
			Network::SendCommand(Game::NS_SERVER, client->netchan.remoteAddress, "error", "EXE_LOSTRELIABLECOMMANDS");
			return;
		}

		Utils::Hook::Call<void(Game::client_t*, Game::msg_t*)>(0x414D40)(client, msg);
	}

	Network::Network()
	{
		AssertSize(Game::netadr_t, 20);

		// maximum size in NET_OutOfBandPrint
		Utils::Hook::Set<DWORD>(0x4AEF08, 0x1FFFC);
		Utils::Hook::Set<DWORD>(0x4AEFA3, 0x1FFFC);

		// increase max port binding attempts from 10 to 100
		Utils::Hook::Set<BYTE>(0x4FD48A, 100);

		// increase cl_maxpackets limit
		Utils::Hook::Set<BYTE>(0x4050A1, 125);

		// increase snaps
		//Utils::Hook::Set<BYTE>(0x405357, 40);

		// default maxpackets and snaps
		Utils::Hook::Set<BYTE>(0x40535B, 30);
		Utils::Hook::Set<BYTE>(0x4050A5, 125);

		// Parse port as short in Net_AddrToString
		Utils::Hook::Set<const char*>(0x4698E3, "%u.%u.%u.%u:%hu");

		// Install startup handler
		Utils::Hook(0x4FD4D4, Network::NetworkStartStub, HOOK_JUMP).install()->quick();

		if (Dedicated::IsEnabled())
		{
			// Install interception handler for SV_ConnectionlessPacket
			Utils::Hook(0x6266D0, Network::PacketInterceptionHandler, HOOK_CALL).install()->quick();

			// Install packet deploy hook
			Utils::Hook::RedirectJump(0x6266DA, Network::ServerDeployPacketStub);
		}

		// Install interception handler for CL_ConnectionlessPacket
		Utils::Hook(0x5AA709, Network::PacketInterceptionHandler, HOOK_CALL).install()->quick();

		// Install packet deploy hook
		Utils::Hook::RedirectJump(0x5AA713, Network::ClientDeployPacketStub);

		// Prevent recvfrom error spam
		Utils::Hook(0x46531A, Network::PacketErrorCheck, HOOK_JUMP).install()->quick();



		// Fix packets causing buffer overflow
		Utils::Hook(0x6267E3, Network::NET_DeferPacketToClientStub, HOOK_CALL).install()->quick();

		// Fix server freezer exploit
		Utils::Hook(0x626996, Network::SV_ExecuteClientMessageStub, HOOK_CALL).install()->quick();

		Network::Handle("resolveAddress", [](Address address, const std::string& /*data*/)
		{
			Network::SendRaw(address, address.getString());
		});
	}

	Network::~Network()
	{
		Network::SelectedPacket.clear();
		Network::PacketHandlers.clear();
		Network::StartupSignal.clear();
	}
}
