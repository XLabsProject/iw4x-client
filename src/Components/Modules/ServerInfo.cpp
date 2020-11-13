#include "STDInclude.hpp"

namespace Components
{
	ServerInfo::Container ServerInfo::PlayerContainer;

	unsigned int ServerInfo::GetPlayerCount()
	{
		return ServerInfo::PlayerContainer.playerList.size();
	}

	const char* ServerInfo::GetPlayerText(unsigned int index, int column)
	{
		if (index < ServerInfo::PlayerContainer.playerList.size())
		{
			switch (column)
			{
			case 0:
				return Utils::String::VA("%d", index);

			case 1:
				return ServerInfo::PlayerContainer.playerList[index].name.data();

			case 2:
				return Utils::String::VA("%d", ServerInfo::PlayerContainer.playerList[index].score);

			case 3:
				return Utils::String::VA("%d", ServerInfo::PlayerContainer.playerList[index].ping);
			default:
				break;
			}
		}

		return "";
	}

	void ServerInfo::SelectPlayer(unsigned int index)
	{
		ServerInfo::PlayerContainer.currentPlayer = index;
	}

	void ServerInfo::ServerStatus(UIScript::Token)
	{
		ServerInfo::PlayerContainer.currentPlayer = 0;
		ServerInfo::PlayerContainer.playerList.clear();

		ServerList::ServerInfo* info = ServerList::GetCurrentServer();

		if (info)
		{
			Dvar::Var("uiSi_ServerName").set(info->hostname);
			Dvar::Var("uiSi_MaxClients").set(info->clients);
			Dvar::Var("uiSi_Version").set(info->shortversion);
			Dvar::Var("uiSi_SecurityLevel").set(info->securityLevel);
			Dvar::Var("uiSi_isPrivate").set(info->password ? "@MENU_YES" : "@MENU_NO");
			Dvar::Var("uiSi_Hardcore").set(info->hardcore ? "@MENU_ENABLED" : "@MENU_DISABLED");
			Dvar::Var("uiSi_KillCam").set("@MENU_NO");
			Dvar::Var("uiSi_ffType").set("@MENU_DISABLED");
			Dvar::Var("uiSi_MapName").set(info->mapname);
			Dvar::Var("uiSi_MapNameLoc").set(Game::UI_LocalizeMapName(info->mapname.data()));
			Dvar::Var("uiSi_GameType").set(Game::UI_LocalizeGameType(info->gametype.data()));
			Dvar::Var("uiSi_ModName").set("");

			if (info->mod.size() > 5)
			{
				Dvar::Var("uiSi_ModName").set(info->mod.data() + 5);
			}

			ServerInfo::PlayerContainer.target = info->addr;
			Network::SendCommand(ServerInfo::PlayerContainer.target, "getstatus");
		}
	}

	void ServerInfo::DrawScoreboardInfo(void* a1)
	{
		Game::Font_s* font = Game::R_RegisterFont("fonts/bigfont", 0);
		void* cxt = Game::UI_GetContext(a1);

		std::string addressText = Network::Address(*Game::connectedHost).getString();
		if (addressText == "0.0.0.0:0" || addressText == "loopback") addressText = "Listen Server";

		// get x positions
		float fontSize = 0.35f;
		float y = (480.0f - Dvar::Var("cg_scoreboardHeight").get<float>()) * 0.5f;
		y += Dvar::Var("cg_scoreboardHeight").get<float>() + 6.0f;

		float x = 320.0f - Dvar::Var("cg_scoreboardWidth").get<float>() * 0.5f;
		float x2 = 320.0f + Dvar::Var("cg_scoreboardWidth").get<float>() * 0.5f;

		// draw only when stream friendly ui is not enabled
		if (!Dvar::Var("ui_streamFriendly").get<bool>())
		{
			Game::UI_DrawText(cxt, reinterpret_cast<const char*>(0x7ED3F8), 0x7FFFFFFF, font, x, y, 0, 0, fontSize, reinterpret_cast<float*>(0x747F34), 3);
			Game::UI_DrawText(cxt, addressText.data(), 0x7FFFFFFF, font, x2 - Game::UI_TextWidth(addressText.data(), 0, font, fontSize), y, 0, 0, fontSize, reinterpret_cast<float*>(0x747F34), 3);
		}
	}

	__declspec(naked) void ServerInfo::DrawScoreboardStub()
	{
		__asm
		{
			pushad
			push eax
			call ServerInfo::DrawScoreboardInfo
			pop eax
			popad

			push 591B70h
			retn
		}
	}

	Utils::InfoString ServerInfo::GetHostInfo()
	{
		Utils::InfoString info;

		// TODO: Possibly add all Dvar starting with _
		info.set("admin", Dvar::Var("_Admin").get<const char*>());
		info.set("website", Dvar::Var("_Website").get<const char*>());
		info.set("email", Dvar::Var("_Email").get<const char*>());
		info.set("location", Dvar::Var("_Location").get<const char*>());

		return info;
	}

	Utils::InfoString ServerInfo::GetInfo()
	{
		int maxclientCount = *Game::svs_numclients;

		if (!maxclientCount)
		{
			maxclientCount = Dvar::Var("party_maxplayers").get<int>();
			//maxclientCount = Game::Party_GetMaxPlayers(*Game::partyIngame);
		}

		Utils::InfoString info(Game::Dvar_InfoString_Big(1024));
		info.set("gamename", "IW4");
		info.set("sv_maxclients", Utils::String::VA("%i", maxclientCount));
		info.set("protocol", Utils::String::VA("%i", PROTOCOL));
		info.set("shortversion", SHORTVERSION);
		info.set("mapname", Dvar::Var("mapname").get<const char*>());
		info.set("isPrivate", (Dvar::Var("g_password").get<std::string>().empty() ? "0" : "1"));
		info.set("checksum", Utils::String::VA("%X", Utils::Cryptography::JenkinsOneAtATime::Compute(Utils::String::VA("%u", Game::Sys_Milliseconds()))));

		// Ensure mapname is set
		if (info.get("mapname").empty())
		{
			info.set("mapname", Dvar::Var("ui_mapname").get<const char*>());
		}

		// Set matchtype
		// 0 - No match, connecting not possible
		// 1 - Party, use Steam_JoinLobby to connect
		// 2 - Match, use CL_ConnectFromParty to connect

		if (Dvar::Var("party_enable").get<bool>() && Dvar::Var("party_host").get<bool>()) // Party hosting
		{
			info.set("matchtype", "1");
		}
		else if (Dvar::Var("sv_running").get<bool>()) // Match hosting
		{
			info.set("matchtype", "2");
		}
		else
		{
			info.set("matchtype", "0");
		}

		return info;
	}

	ServerInfo::ServerInfo()
	{
		ServerInfo::PlayerContainer.currentPlayer = 0;
		ServerInfo::PlayerContainer.playerList.clear();

		// Draw IP and hostname on the scoreboard
		Utils::Hook(0x4FC6EA, ServerInfo::DrawScoreboardStub, HOOK_CALL).install()->quick();

		// Ignore native getStatus implementation
		Utils::Hook::Nop(0x62654E, 6);

		// Add uiscript
		UIScript::Add("ServerStatus", ServerInfo::ServerStatus);

		// Add uifeeder
		UIFeeder::Add(13.0f, ServerInfo::GetPlayerCount, ServerInfo::GetPlayerText, ServerInfo::SelectPlayer);

		Network::Handle("getStatus", [](Network::Address address, const std::string& data)
		{
			std::string playerList;

			Utils::InfoString info = ServerInfo::GetInfo();
			info.set("challenge", Utils::ParseChallenge(data));

			if (Game::SV_Loaded())
			{
				for (int i = 0; i < atoi(info.get("sv_maxclients").data()); ++i) // Maybe choose 18 here?
				{
					int score = 0;
					int ping = 0;
					std::string name;

					if (Dvar::Var("sv_running").get<bool>())
					{
						if (Game::svs_clients[i].state < 3) continue;

						score = Game::SV_GameClientNum_Score(i);
						ping = Game::svs_clients[i].ping;
						name = Game::svs_clients[i].name;
					}
					else
					{
						// Score and ping are irrelevant
						const char* namePtr = Game::PartyHost_GetMemberName(reinterpret_cast<Game::PartyData_t*>(0x1081C00), i);
						if (!namePtr || !namePtr[0]) continue;

						name = namePtr;
					}

					playerList.append(Utils::String::VA("%i %i \"%s\"\n", score, ping, name.data()));
				}
			}

			Network::SendCommand(address, "statusResponse", "\\" + info.build() + "\n" + playerList + "\n");
		});

		Network::Handle("statusResponse", [](Network::Address address, const std::string& data)
		{
			if (ServerInfo::PlayerContainer.target == address)
			{
				Utils::InfoString info(data.substr(0, data.find_first_of("\n")));

				Dvar::Var("uiSi_ServerName").set(info.get("sv_hostname"));
				Dvar::Var("uiSi_MaxClients").set(info.get("sv_maxclients"));
				Dvar::Var("uiSi_Version").set(info.get("shortversion"));
				Dvar::Var("uiSi_SecurityLevel").set(info.get("sv_securityLevel"));
				Dvar::Var("uiSi_isPrivate").set(info.get("isPrivate") == "0" ? "@MENU_NO" : "@MENU_YES");
				Dvar::Var("uiSi_Hardcore").set(info.get("g_hardcore") == "0" ? "@MENU_DISABLED" : "@MENU_ENABLED");
				Dvar::Var("uiSi_KillCam").set(info.get("scr_game_allowkillcam") == "0" ? "@MENU_NO" : "@MENU_YES");
				Dvar::Var("uiSi_MapName").set(info.get("mapname"));
				Dvar::Var("uiSi_MapNameLoc").set(Game::UI_LocalizeMapName(info.get("mapname").data()));
				Dvar::Var("uiSi_GameType").set(Game::UI_LocalizeGameType(info.get("g_gametype").data()));
				Dvar::Var("uiSi_ModName").set("");

				switch (atoi(info.get("scr_team_fftype").data()))
				{
				default:
					Dvar::Var("uiSi_ffType").set("@MENU_DISABLED");
					break;

				case 1:
					Dvar::Var("uiSi_ffType").set("@MENU_ENABLED");
					break;

				case 2:
					Dvar::Var("uiSi_ffType").set("@MPUI_RULES_REFLECT");
					break;

				case 3:
					Dvar::Var("uiSi_ffType").set("@MPUI_RULES_SHARED");
					break;
				}

				if (info.get("fs_game").size() > 5)
				{
					Dvar::Var("uiSi_ModName").set(info.get("fs_game").data() + 5);
				}

				auto lines = Utils::String::Explode(data, '\n');

				if (lines.size() <= 1) return;

				for (unsigned int i = 1; i < lines.size(); ++i)
				{
					ServerInfo::Container::Player player;

					std::string currentData = lines[i];

					if (currentData.size() < 3) continue;

					// Insert score
					player.score = atoi(currentData.substr(0, currentData.find_first_of(" ")).data());

					// Remove score
					currentData = currentData.substr(currentData.find_first_of(" ") + 1);

					// Insert ping
					player.ping = atoi(currentData.substr(0, currentData.find_first_of(" ")).data());

					// Remove ping
					currentData = currentData.substr(currentData.find_first_of(" ") + 1);

					if (currentData[0] == '\"')
					{
						currentData = currentData.substr(1);
					}

					if (currentData.back() == '\"')
					{
						currentData.pop_back();
					}

					player.name = currentData;

					ServerInfo::PlayerContainer.playerList.push_back(player);
				}
			}
		});
	}

	ServerInfo::~ServerInfo()
	{
		ServerInfo::PlayerContainer.playerList.clear();
	}
}
