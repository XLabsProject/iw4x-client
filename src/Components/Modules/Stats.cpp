#include "STDInclude.hpp"

namespace Components
{
	int64_t* Stats::GetStatsID()
	{
		static int64_t id = 0x110000100001337;
		return &id;
	}

	bool Stats::IsMaxLevel()
	{
		// 2516000 should be the max experience.
		return (Game::Live_GetXp(0) >= Game::CL_GetMaxXP());
	}

	void Stats::SendStats()
	{
		// check if we're connected to a server...
		if (*reinterpret_cast<std::uint32_t*>(0xB2C540) >= 7)
		{
			for (unsigned char i = 0; i < 7; ++i)
			{
				Game::Com_Printf(0, "Sending stat packet %i to server.\n", i);

				// alloc
				Game::msg_t msg;
				char buffer[2048];
				ZeroMemory(&msg, sizeof(msg));
				ZeroMemory(&buffer, sizeof(buffer));

				// init
				Game::MSG_Init(&msg, buffer, sizeof(buffer));
				Game::MSG_WriteString(&msg, "stats");

				// get stat buffer
				char *statbuffer = nullptr;
				if (Utils::Hook::Call<int(int)>(0x444CA0)(0))
				{
					statbuffer = &Utils::Hook::Call<char *(int)>(0x4C49F0)(0)[1240 * i];
				}

				// Client port?
				Game::MSG_WriteShort(&msg, *reinterpret_cast<short*>(0xA1E878));

				// Stat packet index
				Game::MSG_WriteByte(&msg, i);

				// write stat packet data
				if (statbuffer)
				{
					Game::MSG_WriteData(&msg, statbuffer, std::min(8192 - (i * 1240), 1240));
				}

				// send statpacket
				Network::SendRaw(Game::NS_CLIENT1, *reinterpret_cast<Game::netadr_t*>(0xA1E888), std::string(msg.data, msg.cursize));
			}
		}
	}

	void Stats::UpdateClasses(UIScript::Token)
	{
		Stats::SendStats();
	}

	int Stats::SaveStats(char* dest, const char* folder, const char* buffer, size_t length)
	{
		const auto fs_game = Game::Dvar_FindVar("fs_game");
		if (fs_game && fs_game->current.string && strlen(fs_game->current.string) && !strncmp(fs_game->current.string, "mods/", 5))
		{
			auto directory = folder + "/"s + fs_game->current.string;
			folder = directory.data();
		}

		return Utils::Hook::Call<int(char*, const char*, const char*, size_t)>(0x426450)(dest, folder, buffer, length);
	}

	int Stats::ReadStats(char* buffer, size_t buf_size, const char* format)
	{
		const auto fs_game = Game::Dvar_FindVar("fs_game");
		if (fs_game && fs_game->current.string && strlen(fs_game->current.string) && !strncmp(fs_game->current.string, "mods/", 5))
		{
			auto directory = fs_game->current.string + "/"s + format;
			format = directory.data();
		}

		return Utils::Hook::Call<int(char*, size_t, const char*)>(0x413DE0)(buffer, buf_size, format);
	}

	Stats::Stats()
	{
		// This UIScript should be added in the onClose code of the cac_popup menu,
		// so everytime the create-a-class window is closed, and a client is connected
		// to a server, the stats data of the client will be reuploaded to the server.
		// allowing the player to change their classes while connected to a server.
		UIScript::Add("UpdateClasses", Stats::UpdateClasses);

		// Allow playerdata to be changed while connected to a server
		Utils::Hook::Set<BYTE>(0x4376FD, 0xEB);

		// ToDo: Allow playerdata changes in setPlayerData UI script.

		// Rename stat file
		Utils::Hook::SetString(0x71C048, "iw4x.stat");

		// Patch stats steamid
		Utils::Hook::Nop(0x682EBF, 20);
		Utils::Hook::Nop(0x6830B1, 20);
		Utils::Hook(0x682EBF, Stats::GetStatsID, HOOK_CALL).install()->quick();
		Utils::Hook(0x6830B1, Stats::GetStatsID, HOOK_CALL).install()->quick();
		//Utils::Hook::Set<BYTE>(0x68323A, 0xEB);

		// Never use remote stat saving
		Utils::Hook::Set<BYTE>(0x682F39, 0xEB);

		// Don't create stat backup
		Utils::Hook::Nop(0x402CE6, 2);

		// Write stats to mod specific folder if a mod is loaded
		Utils::Hook(0x682F7B, Stats::SaveStats, HOOK_CALL).install()->quick();

		// Read stats from mod specific folder if a mod is loaded
		Utils::Hook(0x683168, Stats::ReadStats, HOOK_CALL).install()->quick();
	}

	Stats::~Stats()
	{

	}
}
