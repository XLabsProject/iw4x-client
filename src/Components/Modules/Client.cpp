#include "STDInclude.hpp"

namespace Components
{
	void Client::AddFunctions()
	{
		// System time

		Script::AddFunction("GetSystemTime", [](Game::scr_entref_t) // gsc: GetSystemTime()
		{
			SYSTEMTIME time;
			GetSystemTime(&time);

			Game::Scr_AddInt(time.wSecond);
		});

		Script::AddFunction("GetSystemMilliseconds", [](Game::scr_entref_t) // gsc: GetSystemMilliseconds()
		{
			SYSTEMTIME time;
			GetSystemTime(&time);

			Game::Scr_AddInt(time.wMilliseconds);
		});

		// Print to console, even without being in 'developer 1'.

		Script::AddFunction("PrintConsole", [](Game::scr_entref_t) // gsc: PrintConsole(<string>)
		{
			auto str = Game::Scr_GetString(0);

			Game::Com_Printf(0, str);
		});

		// Executes command to the console

		Script::AddFunction("Exec", [](Game::scr_entref_t) // gsc: Exec(<string>)
		{
			auto str = Game::Scr_GetString(0);

			Command::Execute(str, false);
		});
	}

	void Client::AddMethods()
	{
		// Client methods

		Script::AddFunction("getIp", [](Game::scr_entref_t id) // gsc: self getIp()
		{
			Game::gentity_t* gentity = Script::getEntFromEntRef(id);
			Game::client_t* client = Script::getClientFromEnt(gentity);

			if (client->state >= 3)
			{
				std::string ip = Game::NET_AdrToString(client->addr);
				if (ip.find_first_of(":") != std::string::npos)
					ip.erase(ip.begin() + ip.find_first_of(":"), ip.end()); // erase port
				Game::Scr_AddString(ip.data());
			}
		});

		Script::AddFunction("getPing", [](Game::scr_entref_t id) // gsc: self getPing()
		{
			Game::gentity_t* gentity = Script::getEntFromEntRef(id);
			Game::client_t* client = Script::getClientFromEnt(gentity);

			if (client->state >= 3)
			{
				int ping = (int)client->ping;
				Game::Scr_AddInt(ping);
			}
		});
	
		Script::AddFunction("SetPing", [](Game::scr_entref_t id) // gsc: self SetPing(<int>)
		{
			auto ping = Game::Scr_GetInt(0);

			if (ping < 0 || ping > 999)
			{
				Game::Com_Printf(0, "^1SetPing: Ping needs to between 0 and 999!\n");
				return;
			}

			Game::gentity_t* gentity = Script::getEntFromEntRef(id);
			Game::client_t* client = Script::getClientFromEnt(gentity);

			if (!client->isBot)
			{
				Game::Com_Printf(0, "^1SetPing: Can only call on a bot!\n");
				return;
			}

			if (client->state >= 3)
			{
				client->ping = (short)ping;
			}
		});
	}

	void Client::AddCommands()
	{
		Command::Add("NULL", [](Command::Params*)
		{
			return NULL;
		});
	}

	Client::Client()
	{
		Client::AddFunctions();
		Client::AddMethods();
		Client::AddCommands();
	}

	Client::~Client()
	{

	}
}