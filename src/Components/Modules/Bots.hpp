#pragma once

namespace Components
{
	class Bots : public Component
	{
	public:
		static unsigned int Bots::GetClientNum(Game::client_s* cl);

		Bots();
		~Bots();

	private:
		static std::vector<std::string> BotNames;

		static void Bots::SV_BotUserMoveStub(Game::client_s* cl);

		static void BuildConnectString(char* buffer, const char* connectString, int num, int, int protocol, int checksum, int statVer, int statStuff, int port);

		static void Spawn(unsigned int count);
	};
}
