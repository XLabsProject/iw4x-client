#include "STDInclude.hpp"

namespace Components
{
	void Client::AddFunctions()
	{
		//File functions

		Script::AddFunction("fileWrite", [](Game::scr_entref_t) // gsc: fileWrite(<filepath>, <string>, <mode>)
		{
			std::string path = Game::Scr_GetString(0);
			auto text = Game::Scr_GetString(1);
			auto mode = Game::Scr_GetString(2);

			if (path.empty())
			{
				Game::Com_Printf(0, "^1fileWrite: filepath not defined!\n");
				return;
			}

			std::vector<const char*> queryStrings = { R"(..)", R"(../)", R"(..\)" };
			for (auto i = 0u; i < queryStrings.size(); i++)
			{
				if (path.find(queryStrings[i]) != std::string::npos)
				{
					Game::Com_Printf(0, "^1fileWrite: directory traversal is not allowed!\n");
					return;
				}
			}

			if (mode != "append"s && mode != "write"s)
			{
				Game::Com_Printf(0, "^3fileWrite: mode not defined or was wrong, defaulting to 'write'\n");
				mode = "write";
			}

			auto fileHandle = 0;
			if (mode == "write"s)
			{
				fileHandle = Game::FS_FOpenFileWrite(path.data());
			}
			else if (mode == "append"s)
			{
				fileHandle = Game::FS_FOpenFileAppend(path.data());
			}
			Game::FS_Write(text, strlen(text), fileHandle);
			Game::FS_FCloseFile(fileHandle);
		});

		Script::AddFunction("fileRead", [](Game::scr_entref_t) // gsc: fileRead(<filepath>)
		{
			std::string path = Game::Scr_GetString(0);

			if (path.empty())
			{
				Game::Com_Printf(0, "^1fileRead: filepath not defined!\n");
				return;
			}

			std::vector<const char*> queryStrings = { R"(..)", R"(../)", R"(..\)" };
			for (auto i = 0u; i < queryStrings.size(); i++)
			{
				if (path.find(queryStrings[i]) != std::string::npos)
				{
					Game::Com_Printf(0, "^1fileRead: directory traversal is not allowed!\n");
					return;
				}
			}

			if (Game::FS_FileExists(path.data()) == false)
			{
				Game::Com_Printf(0, "^1fileRead: file not found!\n");
				return;
			}

			char* buffer = nullptr;
			Game::FS_ReadFile(path.data(), &buffer);
			Game::Scr_AddString(buffer);
		});

		Script::AddFunction("fileExists", [](Game::scr_entref_t) // gsc: fileExists(<filepath>)
		{
			std::string path = Game::Scr_GetString(0);

			if (path.empty())
			{
				Game::Com_Printf(0, "^1fileExists: filepath not defined!\n");
				return;
			}

			std::vector<const char*> queryStrings = { R"(..)", R"(../)", R"(..\)" };
			for (auto i = 0u; i < queryStrings.size(); i++)
			{
				if (path.find(queryStrings[i]) != std::string::npos)
				{
					Game::Com_Printf(0, "^1fileExists: directory traversal is not allowed!\n");
					return;
				}
			}

			Game::Scr_AddInt(Game::FS_FileExists(path.data()));
		});

		Script::AddFunction("fileRemove", [](Game::scr_entref_t) // gsc: fileRemove(<filepath>)
		{
			std::string path = Game::Scr_GetString(0);

			if (path.empty())
			{
				Game::Com_Printf(0, "^1fileRemove: filepath not defined!\n");
				return;
			}

			std::vector<const char*> queryStrings = { R"(..)", R"(../)", R"(..\)" };
			for (auto i = 0u; i < queryStrings.size(); i++)
			{
				if (path.find(queryStrings[i]) != std::string::npos)
				{
					Game::Com_Printf(0, "^1fileRemove: directory traversal is not allowed!\n");
					return;
				}
			}

			Game::Scr_AddInt(Game::FS_Remove(path.data()));
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