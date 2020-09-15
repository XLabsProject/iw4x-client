#pragma once

// This enables version filtering
//#define VERSION_FILTER

namespace Components
{
	class ServerList : public Component
	{
	public:
		typedef int(SortCallback)(const void*, const void*);

		class ServerInfo
		{
		public:
			Network::Address addr;
			std::string hostname;
			std::string mapname;
			std::string gametype;
			std::string mod;
			std::string shortversion;
			int clients;
			int bots;
			int maxClients;
			bool password;
			int ping;
			int matchType;
			int securityLevel;
			bool hardcore;
			bool svRunning;
		};

		ServerList();
		~ServerList();

		static void Refresh(UIScript::Token);
		static void RefreshVisibleList(UIScript::Token);
		static void UpdateVisibleList(UIScript::Token);
		static void InsertRequest(Network::Address address);
		static void Insert(Network::Address address, Utils::InfoString info);

		static ServerInfo* GetCurrentServer();

		static bool IsFavouriteList();
		static bool IsOfflineList();
		static bool IsOnlineList();

		static void Frame();
		static std::vector<ServerInfo>* GetList();

		static void UpdateVisibleInfo();

	private:
		enum Column
		{
			Password,
			Matchtype,
			Hostname,
			Mapname,
			Players,
			Gametype,
			Mod,
			Ping,
		};

#pragma pack(push, 1)
		union MasterEntry
		{
			char token[7];
			struct
			{
				uint32_t ip;
				uint16_t port;
			};

			bool IsEndToken()
			{
				// End of transmission or file token
				return (token[0] == 'E' && token[1] == 'O' && (token[2] == 'T' || token[2] == 'F'));
			}

			bool HasSeparator()
			{
				return (token[6] == '\\');
			}
		};
#pragma pack(pop)

		class Container
		{
		public:
			class ServerContainer
			{
			public:
				bool sent;
				int sendTime;
				std::string challenge;
				Network::Address target;
			};

			bool awatingList;
			int awaitTime;

			int sentCount;
			int sendCount;

			Network::Address host;
			std::vector<ServerContainer> servers;
			std::recursive_mutex mutex;
		};

		static unsigned int GetServerCount();
		static const char* GetServerText(unsigned int index, int column);
		static const char* GetServerInfoText(ServerInfo* server, int column, bool sorting = false);
		static void SelectServer(unsigned int index);

		static void UpdateSource();
		static void UpdateGameType();

		static void SortList();

		static void LoadFavourties();
		static void StoreFavourite(const std::string& server);
		static void RemoveFavourite(const std::string& server);

		static ServerInfo* GetServer(unsigned int index);

		static bool CompareVersion(const std::string& version1, const std::string& version2);

		static int SortKey;
		static bool SortAsc;

		static unsigned int CurrentServer;
		static Container RefreshContainer;

		static std::vector<ServerInfo> OnlineList;
		static std::vector<ServerInfo> OfflineList;
		static std::vector<ServerInfo> FavouriteList;

		static std::vector<unsigned int> VisibleList;
	};
}
