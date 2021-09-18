#pragma once

namespace Components
{
	class Party : public Component
	{
	public:
		Party();
		~Party();

		static Network::Address Target();
		static void Connect(Network::Address target);
		static const char* GetLobbyInfo(SteamID lobby, const std::string& key);
		static void RemoveLobby(SteamID lobby);

		static bool PlaylistAwaiting();
		static void PlaylistContinue();
		static void PlaylistError(const std::string& error);

		static void ConnectError(const std::string& message);

		static bool IsInUserMapLobby();
		static bool IsInLobby();
		static bool IsEnabled();

		static std::string GetMotd();

	private:
		class JoinContainer
		{
		public:
			Network::Address target;
			std::string challenge;
			std::string motd;
			DWORD joinTime;
			bool valid;
			int matchType;

			Utils::InfoString info;

			// Party-specific stuff
			DWORD requestTime;
			bool awaitingPlaylist;
		};

		static JoinContainer Container;
		static std::map<uint64_t, Network::Address> LobbyMap;

		static SteamID GenerateLobbyId();

		static Game::dvar_t* RegisterMinPlayers(const char* name, int value, int min, int max, Game::dvar_flag flag, const char* description);

		static DWORD UIDvarIntStub(char* dvar);
		static Dvar::Var PartyEnable;
	};
}
