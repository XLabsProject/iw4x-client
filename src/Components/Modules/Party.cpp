#include "STDInclude.hpp"

namespace Components
{
	Party::JoinContainer Party::Container;
	std::map<uint64_t, Network::Address> Party::LobbyMap;
	Dvar::Var Party::PartyEnable;

	SteamID Party::GenerateLobbyId()
	{
		SteamID id;

		id.accountID = Game::Sys_Milliseconds();
		id.universe = 1;
		id.accountType = 8;
		id.accountInstance = 0x40000;

		return id;
	}

	Network::Address Party::Target()
	{
		return Party::Container.target;
	}

	void Party::Connect(Network::Address target)
	{
		Node::Add(target);

		Party::Container.valid = true;
		Party::Container.awaitingPlaylist = false;
		Party::Container.joinTime = Game::Sys_Milliseconds();
		Party::Container.target = target;
		Party::Container.challenge = Utils::Cryptography::Rand::GenerateChallenge();

		Network::SendCommand(Party::Container.target, "getinfo", Party::Container.challenge);

		Command::Execute("openmenu popup_reconnectingtoparty");
	}

	const char* Party::GetLobbyInfo(SteamID lobby, const std::string& key)
	{
		if (Party::LobbyMap.find(lobby.bits) != Party::LobbyMap.end())
		{
			Network::Address address = Party::LobbyMap[lobby.bits];

			if (key == "addr")
			{
				return Utils::String::VA("%d", address.getIP().full);
			}
			else if (key == "port")
			{
				return Utils::String::VA("%d", address.getPort());
			}
		}

		return "212";
	}

	void Party::RemoveLobby(SteamID lobby)
	{
		if (Party::LobbyMap.find(lobby.bits) != Party::LobbyMap.end())
		{
			Party::LobbyMap.erase(Party::LobbyMap.find(lobby.bits));
		}
	}

	void Party::ConnectError(const std::string& message)
	{
		Localization::ClearTemp();
		Command::Execute("closemenu popup_reconnectingtoparty");
		Dvar::Var("partyend_reason").set(message);
		Command::Execute("openmenu menu_xboxlive_partyended");
	}

	std::string Party::GetMotd()
	{
		return Party::Container.motd;
	}

	Game::dvar_t* Party::RegisterMinPlayers(const char* name, int /*value*/, int /*min*/, int max, Game::dvar_flag flag, const char* description)
	{
		return Dvar::Register<int>(name, 1, 1, max, Game::dvar_flag::DVAR_FLAG_WRITEPROTECTED | flag, description).get<Game::dvar_t*>();
	}

	bool Party::PlaylistAwaiting()
	{
		return Party::Container.awaitingPlaylist;
	}

	void Party::PlaylistContinue()
	{
		Dvar::Var("xblive_privateserver").set(false);

		// Ensure we can join
		*Game::g_lobbyCreateInProgress = false;

		Party::Container.awaitingPlaylist = false;

		SteamID id = Party::GenerateLobbyId();

		// Temporary workaround
		// TODO: Patch the 127.0.0.1 -> loopback mapping in the party code
		if (Party::Container.target.isLoopback())
		{
			if (*Game::numIP)
			{
				Party::Container.target.setIP(*Game::localIP);
				Party::Container.target.setType(Game::netadrtype_t::NA_IP);

				Logger::Print("Trying to connect to party with loopback address, using a local ip instead: %s\n", Party::Container.target.getCString());
			}
			else
			{
				Logger::Print("Trying to connect to party with loopback address, but no local ip was found.\n");
			}
		}

		Party::LobbyMap[id.bits] = Party::Container.target;

		Game::Steam_JoinLobby(id, 0);
	}

	void Party::PlaylistError(const std::string& error)
	{
		Party::Container.valid = false;
		Party::Container.awaitingPlaylist = false;

		Party::ConnectError(error);
	}

	DWORD Party::UIDvarIntStub(char* dvar)
	{
		if (!_stricmp(dvar, "onlinegame") && !Stats::IsMaxLevel())
		{
			return 0x649E660;
		}

		return Utils::Hook::Call<DWORD(char*)>(0x4D5390)(dvar);
	}

	bool Party::IsInLobby()
	{
		return (!Dvar::Var("sv_running").get<bool>() && Dvar::Var("party_enable").get<bool>() && Dvar::Var("party_host").get<bool>());
	}

	bool Party::IsInUserMapLobby()
	{
		return (Party::IsInLobby() && Maps::IsUserMap(Dvar::Var("ui_mapname").get<const char*>()));
	}

	bool Party::IsEnabled()
	{
		return PartyEnable.get<bool>();
	}

	Party::Party()
	{
		PartyEnable = Dvar::Register<bool>("party_enable", Dedicated::IsEnabled(), Game::dvar_flag::DVAR_FLAG_NONE, "Enable party system");
		Dvar::Register<bool>("xblive_privatematch", true, Game::dvar_flag::DVAR_FLAG_WRITEPROTECTED, "").get<Game::dvar_t*>();

		// various changes to SV_DirectConnect-y stuff to allow non-party joinees
		Utils::Hook::Set<WORD>(0x460D96, 0x90E9);
		Utils::Hook::Set<BYTE>(0x460F0A, 0xEB);
		Utils::Hook::Set<BYTE>(0x401CA4, 0xEB);
		Utils::Hook::Set<BYTE>(0x401C15, 0xEB);

		// disable configstring checksum matching (it's unreliable at most)
		Utils::Hook::Set<BYTE>(0x4A75A7, 0xEB); // SV_SpawnServer
		Utils::Hook::Set<BYTE>(0x5AC2CF, 0xEB); // CL_ParseGamestate
		Utils::Hook::Set<BYTE>(0x5AC2C3, 0xEB); // CL_ParseGamestate

		// AnonymousAddRequest
		Utils::Hook::Set<BYTE>(0x5B5E18, 0xEB);
		Utils::Hook::Set<BYTE>(0x5B5E64, 0xEB);
		Utils::Hook::Nop(0x5B5E5C, 2);

		// HandleClientHandshake
		Utils::Hook::Set<BYTE>(0x5B6EA5, 0xEB);
		Utils::Hook::Set<BYTE>(0x5B6EF3, 0xEB);
		Utils::Hook::Nop(0x5B6EEB, 2);

		// Allow local connections
		Utils::Hook::Set<BYTE>(0x4D43DA, 0xEB);

		// LobbyID mismatch
		Utils::Hook::Nop(0x4E50D6, 2);
		Utils::Hook::Set<BYTE>(0x4E50DA, 0xEB);

		// causes 'does current Steam lobby match' calls in Steam_JoinLobby to be ignored
		Utils::Hook::Set<BYTE>(0x49D007, 0xEB);

		// functions checking party heartbeat timeouts, cause random issues
		Utils::Hook::Nop(0x4E532D, 5);

		// Steam_JoinLobby call causes migration
		Utils::Hook::Nop(0x5AF851, 5);
		Utils::Hook::Set<BYTE>(0x5AF85B, 0xEB);

		// Allow xpartygo in public lobbies
		Utils::Hook::Set<BYTE>(0x5A969E, 0xEB);
		Utils::Hook::Nop(0x5A96BE, 2);

		// Always open lobby menu when connecting
		// It's not possible to entirely patch it via code
		//Utils::Hook::Set<BYTE>(0x5B1698, 0xEB);
		//Utils::Hook::Nop(0x5029F2, 6);
		//Utils::Hook::SetString(0x70573C, "menu_xboxlive_lobby");

		// Disallow selecting team in private match
		//Utils::Hook::Nop(0x5B2BD8, 6);

		// Force teams, even if not private match
		Utils::Hook::Set<BYTE>(0x487BB2, 0xEB);

		// Force xblive_privatematch 0 and rename it
		//Utils::Hook::Set<BYTE>(0x420A6A, 4);
		Utils::Hook::Set<BYTE>(0x420A6C, 0);
		Utils::Hook::Set<const char*>(0x420A6E, "xblive_privateserver");

		// Remove migration shutdown, it causes crashes and will be destroyed when erroring anyways
		Utils::Hook::Nop(0x5A8E1C, 12);
		Utils::Hook::Nop(0x5A8E33, 11);

		// Enable XP Bar
		Utils::Hook(0x62A2A7, Party::UIDvarIntStub, HOOK_CALL).install()->quick();

		// Set NAT to open
		Utils::Hook::Set<int>(0x79D898, 1);

		// Disable host migration
		Utils::Hook::Set<BYTE>(0x5B58B2, 0xEB);
		Utils::Hook::Set<BYTE>(0x4D6171, 0);
		Utils::Hook::Nop(0x4077A1, 5); // PartyMigrate_Frame

		// Patch playlist stuff for non-party behavior
		Game::dvar_t* partyEnable = PartyEnable.get<Game::dvar_t*>();
		Utils::Hook::Set<Game::dvar_t**>(0x4A4093, &partyEnable);
		Utils::Hook::Set<Game::dvar_t**>(0x4573F1, &partyEnable);
		Utils::Hook::Set<Game::dvar_t**>(0x5B1A0C, &partyEnable);

		// Invert corresponding jumps
		Utils::Hook::Xor<BYTE>(0x4A409B, 1);
		Utils::Hook::Xor<BYTE>(0x4573FA, 1);
		Utils::Hook::Xor<BYTE>(0x5B1A17, 1);

		// Fix xstartlobby
		//Utils::Hook::Set<BYTE>(0x5B71CD, 0xEB);

		// Patch party_minplayers to 1 and protect it
		//Utils::Hook(0x4D5D51, Party::RegisterMinPlayers, HOOK_CALL).install()->quick();

		// Set ui_maxclients to sv_maxclients
		Utils::Hook::Set<const char*>(0x42618F, "sv_maxclients");
		Utils::Hook::Set<const char*>(0x4D3756, "sv_maxclients");
		Utils::Hook::Set<const char*>(0x5E3772, "sv_maxclients");

		// Unlatch maxclient dvars
		Utils::Hook::Xor<BYTE>(0x426187, Game::dvar_flag::DVAR_FLAG_LATCHED);
		Utils::Hook::Xor<BYTE>(0x4D374E, Game::dvar_flag::DVAR_FLAG_LATCHED);
		Utils::Hook::Xor<BYTE>(0x5E376A, Game::dvar_flag::DVAR_FLAG_LATCHED);
		Utils::Hook::Xor<DWORD>(0x4261A1, Game::dvar_flag::DVAR_FLAG_LATCHED);
		Utils::Hook::Xor<DWORD>(0x4D376D, Game::dvar_flag::DVAR_FLAG_LATCHED);
		Utils::Hook::Xor<DWORD>(0x5E3789, Game::dvar_flag::DVAR_FLAG_LATCHED);

		// Patch Live_PlayerHasLoopbackAddr
		//Utils::Hook::Set<DWORD>(0x418F30, 0x90C3C033);

		Command::Add("connect", [](Command::Params* params)
		{
			if (params->length() < 2)
			{
				return;
			}

			if (Game::CL_IsCgameInitialized())
			{
				Command::Execute("disconnect", false);
				Command::Execute(Utils::String::VA("%s", params->join(0).data()), false);
			}
			else
			{
				Party::Connect(Network::Address(params->get(1)));
			}
		});
		Command::Add("reconnect", [](Command::Params*)
		{
			Party::Connect(Party::Container.target);
		});

		Scheduler::OnFrame([]()
		{
			if (Party::Container.valid)
			{
				if ((Game::Sys_Milliseconds() - Party::Container.joinTime) > 10'000)
				{
					Party::Container.valid = false;
					Party::ConnectError("Server connection timed out.");
				}
			}

			if (Party::Container.awaitingPlaylist)
			{
				if ((Game::Sys_Milliseconds() - Party::Container.requestTime) > 5'000)
				{
					Party::Container.awaitingPlaylist = false;
					Party::ConnectError("Playlist request timed out.");
				}
			}
		}, true);

		// Basic info handler
		Network::Handle("getInfo", [](Network::Address address, const std::string& data)
		{
			int botCount = 0;
			int clientCount = 0;
			int maxclientCount = *Game::svs_numclients;

			if (maxclientCount)
			{
				for (int i = 0; i < maxclientCount; ++i)
				{
					if (Game::svs_clients[i].state >= 3)
					{
						if (Game::svs_clients[i].isBot) ++botCount;
						else ++clientCount;
					}
				}
			}
			else
			{
				maxclientCount = Dvar::Var("party_maxplayers").get<int>();
				//maxclientCount = Game::Party_GetMaxPlayers(*Game::partyIngame);
				clientCount = Game::PartyHost_CountMembers(reinterpret_cast<Game::PartyData_s*>(0x1081C00));
			}

			Utils::InfoString info;
			info.set("challenge", Utils::ParseChallenge(data));
			info.set("gamename", "IW4");
			info.set("hostname", Dvar::Var("sv_hostname").get<const char*>());
			info.set("gametype", Dvar::Var("g_gametype").get<const char*>());
			info.set("fs_game", Dvar::Var("fs_game").get<const char*>());
			info.set("xuid", Utils::String::VA("%llX", Steam::SteamUser()->GetSteamID().bits));
			info.set("clients", Utils::String::VA("%i", clientCount));
			info.set("bots", Utils::String::VA("%i", botCount));
			info.set("sv_maxclients", Utils::String::VA("%i", maxclientCount));
			info.set("protocol", Utils::String::VA("%i", PROTOCOL));
			info.set("shortversion", SHORTVERSION);
			info.set("checksum", Utils::String::VA("%d", Game::Sys_Milliseconds()));
			info.set("mapname", Dvar::Var("mapname").get<const char*>());
			info.set("isPrivate", (Dvar::Var("g_password").get<std::string>().size() ? "1" : "0"));
			info.set("hc", (Dvar::Var("g_hardcore").get<bool>() ? "1" : "0"));
			info.set("securityLevel", Utils::String::VA("%i", Dvar::Var("sv_securityLevel").get<int>()));
			info.set("sv_running", (Dvar::Var("sv_running").get<bool>() ? "1" : "0"));

			// Ensure mapname is set
			if (info.get("mapname").empty() || Party::IsInLobby())
			{
				info.set("mapname", Dvar::Var("ui_mapname").get<const char*>());
			}

			if (Maps::GetUserMap()->isValid())
			{
				info.set("usermaphash", Utils::String::VA("%i", Maps::GetUserMap()->getHash()));
			}
			else if (Party::IsInUserMapLobby())
			{
				info.set("usermaphash", Utils::String::VA("%i", Maps::GetUsermapHash(info.get("mapname"))));
			}

			if (Dedicated::IsEnabled())
			{
				info.set("sv_motd", Dvar::Var("sv_motd").get<std::string>());
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

			info.set("wwwDownload", (Dvar::Var("sv_wwwDownload").get<bool>() ? "1" : "0"));
			info.set("wwwUrl", Dvar::Var("sv_wwwBaseUrl").get<std::string>());

			Network::SendCommand(address, "infoResponse", "\\" + info.build());
		});

		Network::Handle("infoResponse", [](Network::Address address, const std::string& data)
		{
			Utils::InfoString info(data);

			// Handle connection
			if (Party::Container.valid)
			{
				if (Party::Container.target == address)
				{
					// Invalidate handler for future packets
					Party::Container.valid = false;
					Party::Container.info = info;

					Party::Container.matchType = atoi(info.get("matchtype").data());
					uint32_t securityLevel = static_cast<uint32_t>(atoi(info.get("securityLevel").data()));
					bool isUsermap = !info.get("usermaphash").empty();
					unsigned int usermapHash = atoi(info.get("usermaphash").data());

					std::string mod = Dvar::Var("fs_game").get<std::string>();

					// set fast server stuff here so its updated when we go to download stuff
					if (info.get("wwwDownload") == "1"s)
					{
						Dvar::Var("sv_wwwDownload").set(true);
						Dvar::Var("sv_wwwBaseUrl").set(info.get("wwwUrl"));
					}
					else
					{
						Dvar::Var("sv_wwwDownload").set(false);
						Dvar::Var("sv_wwwBaseUrl").set("");
					}

					if (info.get("challenge") != Party::Container.challenge)
					{
						Party::ConnectError("Invalid join response: Challenge mismatch.");
					}
					else if (securityLevel > Auth::GetSecurityLevel())
					{
						//Party::ConnectError(Utils::VA("Your security level (%d) is lower than the server's (%d)", Auth::GetSecurityLevel(), securityLevel));
						Command::Execute("closemenu popup_reconnectingtoparty");
						Auth::IncreaseSecurityLevel(securityLevel, "reconnect");
					}
					else if (!Party::Container.matchType)
					{
						Party::ConnectError("Server is not hosting a match.");
					}
					else if (Party::Container.matchType > 2 || Party::Container.matchType < 0)
					{
						Party::ConnectError("Invalid join response: Unknown matchtype");
					}
					else if (Party::Container.info.get("mapname").empty() || Party::Container.info.get("gametype").empty())
					{
						Party::ConnectError("Invalid map or gametype.");
					}
					else if (Party::Container.info.get("isPrivate") == "1"s && !Dvar::Var("password").get<std::string>().length())
					{
						Party::ConnectError("A password is required to join this server! Set it at the bottom of the serverlist.");
					}
					else if (isUsermap && usermapHash != Maps::GetUsermapHash(info.get("mapname")))
					{
						Command::Execute("closemenu popup_reconnectingtoparty");
						Download::InitiateMapDownload(info.get("mapname"), info.get("isPrivate") == "1");
					}
					else if (!info.get("fs_game").empty() && Utils::String::ToLower(mod) != Utils::String::ToLower(info.get("fs_game")))
					{
						Command::Execute("closemenu popup_reconnectingtoparty");
						Download::InitiateClientDownload(info.get("fs_game"), info.get("isPrivate") == "1"s);
					}
					else if (!Dvar::Var("fs_game").get<std::string>().empty() && info.get("fs_game").empty())
					{
						Dvar::Var("fs_game").set("");

						if (Dvar::Var("cl_modVidRestart").get<bool>())
						{
							Command::Execute("vid_restart", false);
						}

						Command::Execute("reconnect", false);
					}
					else
					{
						if (!Maps::CheckMapInstalled(Party::Container.info.get("mapname").data(), true)) return;

						Party::Container.motd = info.get("sv_motd");

						if (Party::Container.matchType == 1) // Party
						{
							// Send playlist request
							Party::Container.requestTime = Game::Sys_Milliseconds();
							Party::Container.awaitingPlaylist = true;
							Network::SendCommand(Party::Container.target, "getplaylist", Dvar::Var("password").get<std::string>());

							// This is not a safe method
							// TODO: Fix actual error!
							if (Game::CL_IsCgameInitialized())
							{
								Command::Execute("disconnect", true);
							}
						}
						else if (Party::Container.matchType == 2) // Match
						{
							if (atoi(Party::Container.info.get("clients").data()) >= atoi(Party::Container.info.get("sv_maxclients").data()))
							{
								Party::ConnectError("@EXE_SERVERISFULL");
							}
							else
							{
								Dvar::Var("xblive_privateserver").set(true);

								Game::Menus_CloseAll(Game::uiContext);

								Game::_XSESSION_INFO hostInfo;
								Game::CL_ConnectFromParty(0, &hostInfo, *Party::Container.target.get(), 0, 0, Party::Container.info.get("mapname").data(), Party::Container.info.get("gametype").data());
							}
						}
					}
				}
			}

			ServerList::Insert(address, info);
			Friends::UpdateServer(address, info.get("hostname"), info.get("mapname"));
		});
	}

	Party::~Party()
	{
		Party::LobbyMap.clear();
	}
}
