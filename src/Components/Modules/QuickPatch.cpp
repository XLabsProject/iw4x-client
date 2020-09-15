#include "STDInclude.hpp"

namespace Components
{
	int QuickPatch::FrameTime = 0;

	void QuickPatch::UnlockStats()
	{
		if (Dedicated::IsEnabled()) return;

		if (Game::CL_IsCgameInitialized())
		{
			Toast::Show("cardicon_locked", "^1Error", "Not allowed while ingame.", 3000);
			return;
		}

		Command::Execute("setPlayerData prestige 10");
		Command::Execute("setPlayerData experience 2516000");
		Command::Execute("setPlayerData iconUnlocked cardicon_prestige10_02 1");

		// Unlock challenges
		Game::StringTable* challengeTable = Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_STRINGTABLE, "mp/allchallengestable.csv").stringTable;

		if (challengeTable)
		{
			for (int i = 0; i < challengeTable->rowCount; ++i)
			{
				// Find challenge
				const char* challenge = Game::TableLookup(challengeTable, i, 0);

				int maxState = 0;
				int maxProgress = 0;

				// Find correct tier and progress
				for (int j = 0; j < 10; ++j)
				{
					int progress = atoi(Game::TableLookup(challengeTable, i, 6 + j * 2));
					if (!progress) break;

					maxState = j + 2;
					maxProgress = progress;
				}

				Command::Execute(Utils::String::VA("setPlayerData challengeState %s %d", challenge, maxState));
				Command::Execute(Utils::String::VA("setPlayerData challengeProgress %s %d", challenge, maxProgress));
			}
		}
	}

	int QuickPatch::MsgReadBitsCompressCheckSV(const char *from, char *to, int size)
	{
		static char buffer[0x8000];

		if (size > 0x800) return 0;
		size = Game::MSG_ReadBitsCompress(from, buffer, size);

		if (size > 0x800) return 0;
		std::memcpy(to, buffer, size);

		return size;
	}

	int QuickPatch::MsgReadBitsCompressCheckCL(const char *from, char *to, int size)
	{
		static char buffer[0x100000];

		if (size > 0x20000) return 0;
		size = Game::MSG_ReadBitsCompress(from, buffer, size);

		if (size > 0x20000) return 0;
		std::memcpy(to, buffer, size);

		return size;
	}

	int QuickPatch::SVCanReplaceServerCommand(Game::client_t* /*client*/, const char* /*cmd*/)
	{
		// This is a fix copied from V2. As I don't have time to investigate, let's simply trust them
		return -1;
	}

	long QuickPatch::AtolAdjustPlayerLimit(const char* string)
	{
		return std::min(atol(string), 18l);
	}

	void QuickPatch::SelectStringTableEntryInDvarStub()
	{
		Command::ClientParams args;

		if (args.length() >= 4)
		{
			std::string cmd = args[0];
			std::string table = args[1];
			std::string col = args[2];
			std::string dvarName = args[3];
			Game::dvar_t* dvar = Game::Dvar_FindVar(dvarName.data());

			if (Command::Find(dvarName) || (dvar && (dvar->flags & (Game::DVAR_FLAG_WRITEPROTECTED | Game::DVAR_FLAG_CHEAT | Game::DVAR_FLAG_READONLY))))
			{
				return;
			}
		}

		Game::CL_SelectStringTableEntryInDvar_f();
	}

	__declspec(naked) void QuickPatch::JavelinResetHookStub()
	{
		__asm
		{
			mov eax, 577A10h;
			call eax;
			pop edi;
			mov dword ptr [esi+34h], 0;
			pop esi;
			pop ebx;
			retn;
		}
	}

	__declspec(naked) int QuickPatch::SVGameClientNum()
	{
		_asm
		{
			mov eax, [esp + 4] // index
			mov ecx, 1A831A8h
			mov ecx, [ecx]
			imul eax, 366Ch
			lea edx, [eax + ecx + 3134h]
			cmp edx, 0
			jnz valid_player_state
			mov eax, 0
			ret

			valid_player_state:
			mov eax, [eax + ecx + 3134h]
			ret
		}
	}

	bool QuickPatch::InvalidNameCheck(char *dest, char *source, int size)
	{
		strncpy(dest, source, size - 1);
		dest[size - 1] = 0;

		for (int i = 0; i < size - 1; i++)
		{
			if (!dest[i]) break;

			if (dest[i] > 125 || dest[i] < 32 || dest[i] == '%') 
			{
				return false;
			}
		}

		return true;
	}

	__declspec(naked) void QuickPatch::InvalidNameStub()
	{
		static char* kick_reason = "Invalid name detected.";

		__asm
		{
			call InvalidNameCheck;
			test al, al

			jnz returnSafe;

			pushad;
			push 1;
			push kick_reason;
			push edi;
			mov eax, 0x004D1600;
			call eax;
			add esp, 12;
			popad;

		returnSafe:
			push 0x00401988;
			retn;
		}
	}

	Game::dvar_t* QuickPatch::sv_enableBounces;
	__declspec(naked) void QuickPatch::BounceStub()
	{
		__asm
		{
			// check the value of sv_enableBounces
			push eax;
			mov eax, sv_enableBounces;
			cmp byte ptr[eax + 16], 1;
			pop eax;

			// always bounce if sv_enableBounces is set to 1
			je bounce;

			// original code
			cmp dword ptr[esp + 24h], 0;
			jnz dontBounce;

		bounce:
			push 0x004B1B34;
			retn;

		dontBounce:
			push 0x004B1B48;
			retn;
		}
	}

	Game::dvar_t* QuickPatch::r_customAspectRatio;
	Game::dvar_t* QuickPatch::Dvar_RegisterAspectRatioDvar(const char* name, char**, int defaultVal, int flags, const char* description)
	{
		static std::vector < char * > values =
		{
			"auto",
			"standard",
			"wide 16:10",
			"wide 16:9",
			"custom",
			nullptr,
		};

		// register custom aspect ratio dvar
		r_customAspectRatio = Game::Dvar_RegisterFloat("r_customAspectRatio", 16.0f / 9.0f, 4.0f / 3.0f, 63.0f / 9.0f, flags, "Screen aspect ratio. Divide the width by the height in order to get the aspect ratio value. For example: 16 / 9 = 1,77");

		// register enumeration dvar
		return Game::Dvar_RegisterEnum(name, values.data(), defaultVal, flags, description);
	}

	void QuickPatch::SetAspectRatio()
	{
		// set the aspect ratio
		Utils::Hook::Set<float>(0x66E1C78, r_customAspectRatio->current.value);
	}

	__declspec(naked) void QuickPatch::SetAspectRatioStub()
	{
		__asm
		{
			cmp eax, 4;
			ja goToDefaultCase;
			je useCustomRatio;

			// execute switch statement code
			push 0x005063FC;
			retn;

		goToDefaultCase:
			push 0x005064FC;
			retn;

		useCustomRatio:
			// set custom resolution
			pushad;
			call SetAspectRatio;
			popad;

			// set widescreen to 1
			mov eax, 1;

			// continue execution
			push 0x00506495;
			retn;
		}
	}

	Game::dvar_t* QuickPatch::g_playerCollision;
	__declspec(naked) void QuickPatch::PlayerCollisionStub()
	{
		__asm
		{
			// check the value of g_playerCollision
			push eax;
			mov eax, g_playerCollision;
			cmp byte ptr[eax + 16], 0;
			pop eax;

			// dont collide if g_playerCollision is set to 0
			je dontcollide;

			// original code
			mov eax, dword ptr[esp + 0xa0];
			jmp collide;

		collide:
			push 0x00478376;
			retn;

		dontcollide:
			mov eax, dword ptr[esp + 0xa0];
			mov ecx, dword ptr[esp + 9ch];
			push eax;
			push ecx;
			lea edx, [esp + 48h];
			push edx;
			mov eax, esi;
			push 0x0047838b;
			retn;
		}
	}

	Game::dvar_t* QuickPatch::g_playerEjection;
	__declspec(naked) void QuickPatch::PlayerEjectionStub()
	{
		__asm
		{
			// check the value of g_playerEjection
			push eax;
			mov eax, g_playerEjection;
			cmp byte ptr[eax + 16], 0;
			pop eax;

			// dont eject if g_playerEjection is set to 0
			je donteject;

			// original code
			cmp dword ptr[ebx + 19ch], edi;
			jle eject;

		eject:
			push 0x005d8152;
			retn;

		donteject:
			push 0x005d815b;
			retn;
		}
	}

	template <typename T> std::function < T > ImportFunction(const std::string& dll, const std::string& function)
	{
		auto dllHandle = GetModuleHandleA(&dll[0]);
		auto procAddr = GetProcAddress(dllHandle, &function[0]);

		return std::function < T >(reinterpret_cast<T*>(procAddr));
	}

	QuickPatch::QuickPatch()
	{
		QuickPatch::FrameTime = 0;
		Scheduler::OnFrame([]()
		{
			QuickPatch::FrameTime = Game::Sys_Milliseconds();
		});

		// quit_hard
		Command::Add("quit_hard", [](Command::Params*)
		{
			typedef enum _HARDERROR_RESPONSE_OPTION {
				OptionAbortRetryIgnore,
				OptionOk,
				OptionOkCancel,
				OptionRetryCancel,
				OptionYesNo,
				OptionYesNoCancel,
				OptionShutdownSystem
			} HARDERROR_RESPONSE_OPTION, *PHARDERROR_RESPONSE_OPTION;

			typedef enum _HARDERROR_RESPONSE {
				ResponseReturnToCaller,
				ResponseNotHandled,
				ResponseAbort,
				ResponseCancel,
				ResponseIgnore,
				ResponseNo,
				ResponseOk,
				ResponseRetry,
				ResponseYes
			} HARDERROR_RESPONSE, *PHARDERROR_RESPONSE;

			BOOLEAN hasPerms;
			HARDERROR_RESPONSE response;

			auto result = ImportFunction<NTSTATUS __stdcall(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN)>("ntdll.dll", "RtlAdjustPrivilege")
				(19, true, false, &hasPerms);

			result = ImportFunction<NTSTATUS __stdcall(NTSTATUS, ULONG, LPCSTR, PVOID, HARDERROR_RESPONSE_OPTION, PHARDERROR_RESPONSE)>("ntdll.dll", "NtRaiseHardError")
				(0xC000007B /*0x0000000A*/, 0, nullptr, nullptr, OptionShutdownSystem, &response);
		});

		// bounce dvar
		sv_enableBounces = Game::Dvar_RegisterBool("sv_enableBounces", false, Game::DVAR_FLAG_REPLICATED, "Enables bouncing on the server");
		Utils::Hook(0x4B1B2D, QuickPatch::BounceStub, HOOK_JUMP).install()->quick();

		// Intermission time dvar
		Game::Dvar_RegisterFloat("scr_intermissionTime", 10, 0, 120, Game::DVAR_FLAG_REPLICATED | Game::DVAR_FLAG_SAVED | Game::DVAR_FLAG_DEDISAVED, "Time in seconds before match server loads the next map");

		// Player Collision dvar
		g_playerCollision = Game::Dvar_RegisterBool("g_playerCollision", true, Game::DVAR_FLAG_REPLICATED, "Flag whether player collision is on or off");
		Utils::Hook(0x47836F, QuickPatch::PlayerCollisionStub, HOOK_JUMP).install()->quick();

		// Player Ejection dvar
		g_playerEjection = Game::Dvar_RegisterBool("g_playerEjection", true, Game::DVAR_FLAG_REPLICATED, "Flag whether player ejection is on or off");
		Utils::Hook(0x5D814A, QuickPatch::PlayerEjectionStub, HOOK_JUMP).install()->quick();

		// Disallow invalid player names
		Utils::Hook(0x401983, QuickPatch::InvalidNameStub, HOOK_JUMP).install()->quick();

		// Javelin fix
		Utils::Hook(0x578F52, QuickPatch::JavelinResetHookStub, HOOK_JUMP).install()->quick();

		// Add ultrawide support
		Utils::Hook(0x0051B13B, QuickPatch::Dvar_RegisterAspectRatioDvar, HOOK_CALL).install()->quick();
		Utils::Hook(0x005063F3, QuickPatch::SetAspectRatioStub, HOOK_JUMP).install()->quick();

		// Make sure preDestroy is called when the game shuts down
		Scheduler::OnShutdown(Loader::PreDestroy);

		// protocol version (workaround for hacks)
		Utils::Hook::Set<int>(0x4FB501, PROTOCOL);

		// protocol command
		Utils::Hook::Set<int>(0x4D36A9, PROTOCOL);
		Utils::Hook::Set<int>(0x4D36AE, PROTOCOL);
		Utils::Hook::Set<int>(0x4D36B3, PROTOCOL);

		// internal version is 99, most servers should accept it
		Utils::Hook::Set<int>(0x463C61, 208);

		// remove system pre-init stuff (improper quit, disk full)
		Utils::Hook::Set<BYTE>(0x411350, 0xC3);

		// remove STEAMSTART checking for DRM IPC
		Utils::Hook::Nop(0x451145, 5);
		Utils::Hook::Set<BYTE>(0x45114C, 0xEB);

		// LSP disabled
		Utils::Hook::Set<BYTE>(0x435950, 0xC3); // LSP HELLO
		Utils::Hook::Set<BYTE>(0x49C220, 0xC3); // We wanted to send a logging packet, but we haven't connected to LSP!
		Utils::Hook::Set<BYTE>(0x4BD900, 0xC3); // main LSP response func
		Utils::Hook::Set<BYTE>(0x682170, 0xC3); // Telling LSP that we're playing a private match
		Utils::Hook::Nop(0x4FD448, 5);          // Don't create lsp_socket

		// Don't delete config files if corrupted
		Utils::Hook::Set<BYTE>(0x47DCB3, 0xEB);
		Utils::Hook::Set<BYTE>(0x4402B6, 0);

		// hopefully allow alt-tab during game, used at least in alt-enter handling
		Utils::Hook::Set<DWORD>(0x45ACE0, 0xC301B0);

		// fs_basegame
		Utils::Hook::Set<char*>(0x6431D1, BASEGAME);

		// UI version string
		Utils::Hook::Set<char*>(0x43F73B, "IW4x: " VERSION);

		// console version string
		Utils::Hook::Set<char*>(0x4B12BB, "IW4x " VERSION " (built " __DATE__ " " __TIME__ ")");

		// version string
		Utils::Hook::Set<char*>(0x60BD56, "IW4x (" VERSION ")");

		// version string color
		static float buildLocColor[] = { 1.0f, 1.0f, 1.0f, 0.8f };
		Utils::Hook::Set(0x43F710, buildLocColor);

		// Shift ui version string to the left (ui_buildlocation)
		Utils::Hook::Nop(0x6310A0, 5); // Don't register the initial dvar
		Utils::Hook::Nop(0x6310B8, 5); // Don't write the result
		Dvar::OnInit([]()
		{
			*reinterpret_cast<Game::dvar_t**>(0x62E4B64) = Game::Dvar_RegisterVec2("ui_buildLocation", -60.0f, 474.0f, -10000.0, 10000.0, Game::DVAR_FLAG_READONLY, "Where to draw the build number");
		});

		// console title
		if (ZoneBuilder::IsEnabled())
		{
			Utils::Hook::Set<char*>(0x4289E8, "IW4x (" VERSION "): ZoneBuilder");
		}
		else if (Dedicated::IsEnabled())
		{
			Utils::Hook::Set<char*>(0x4289E8, "IW4x (" VERSION "): Dedicated");
		}
		else
		{
			Utils::Hook::Set<char*>(0x4289E8, "IW4x (" VERSION "): Console");
		}

		// window title
		Utils::Hook::Set<char*>(0x5076A0, "IW4x: Multiplayer");

		// sv_hostname
		Utils::Hook::Set<char*>(0x4D378B, "IW4Host");

		// shortversion
		Utils::Hook::Set<char*>(0x60BD91, SHORTVERSION);

		// console logo
		Utils::Hook::Set<char*>(0x428A66, BASEGAME "/images/logo.bmp");

		// splash logo
		Utils::Hook::Set<char*>(0x475F9E, BASEGAME "/images/splash.bmp");

		Utils::Hook::Set<char*>(0x4876C6, "Successfully read stats data\n");

		// Numerical ping (cg_scoreboardPingText 1)
		Utils::Hook::Set<BYTE>(0x45888E, 1);
		Utils::Hook::Set<BYTE>(0x45888C, Game::dvar_flag::DVAR_FLAG_CHEAT);

		// increase font sizes for chat on higher resolutions
		static float float13 = 13.0f;
		static float float10 = 10.0f;
		Utils::Hook::Set<float*>(0x5814AE, &float13);
		Utils::Hook::Set<float*>(0x5814C8, &float10);

		// Enable commandline arguments
		Utils::Hook::Set<BYTE>(0x464AE4, 0xEB);

		// remove limit on IWD file loading
		Utils::Hook::Set<BYTE>(0x642BF3, 0xEB);

		// dont run UPNP stuff on main thread
		Utils::Hook::Set<BYTE>(0x48A135, 0xC3);
		Utils::Hook::Set<BYTE>(0x48A151, 0xC3);
		Utils::Hook::Nop(0x684080, 5); // Don't spam the console

		// spawn upnp thread when UPNP_init returns
		Utils::Hook::Hook(0x47982B, []()
		{
			std::thread([]()
			{
				// check natpmpstate
				// state 4 is no more devices to query
				while (Utils::Hook::Get<int>(0x66CE200) < 4)
				{
					Utils::Hook::Call<void()>(0x4D7030)();
					std::this_thread::sleep_for(500ms);
				}
			}).detach();
		}, HOOK_JUMP).install()->quick();

		// disable the IWNet IP detection (default 'got ipdetect' flag to 1)
		Utils::Hook::Set<BYTE>(0x649D6F0, 1);

		// Fix stats sleeping
		Utils::Hook::Set<BYTE>(0x6832BA, 0xEB);
		Utils::Hook::Set<BYTE>(0x4BD190, 0xC3);

		// remove 'impure stats' checking
		Utils::Hook::Set<BYTE>(0x4BB250, 0x33);
		Utils::Hook::Set<BYTE>(0x4BB251, 0xC0);
		Utils::Hook::Set<DWORD>(0x4BB252, 0xC3909090);

		// default sv_pure to 0
		Utils::Hook::Set<BYTE>(0x4D3A74, 0);

		// Force debug logging
		Utils::Hook::Set<BYTE>(0x60AE4A, 1);
		//Utils::Hook::Nop(0x60AE49, 8);
		//Utils::Hook::Set<BYTE>(0x6FF53C, 0);

		// remove activeAction execution (exploit in mods)
		Utils::Hook::Set<BYTE>(0x5A1D43, 0xEB);

		// disable bind protection
		Utils::Hook::Set<BYTE>(0x4DACA2, 0xEB);

		// require Windows 5
		Utils::Hook::Set<BYTE>(0x467ADF, 5);
		Utils::Hook::Set<char>(0x6DF5D6, '5');

		// disable 'ignoring asset' notices
		Utils::Hook::Nop(0x5BB902, 5);

		// disable migration_dvarErrors
		Utils::Hook::Set<BYTE>(0x60BDA7, 0);

		// allow joining 'developer 1' servers
		Utils::Hook::Set<BYTE>(0x478BA2, 0xEB);

		// fs_game fixes
		Utils::Hook::Nop(0x4A5D74, 2); // remove fs_game profiles
		Utils::Hook::Set<BYTE>(0x4081FD, 0xEB); // defaultweapon
		Utils::Hook::Set<BYTE>(0x452C1D, 0xEB); // LoadObj weaponDefs

		// filesystem init default_mp.cfg check
		Utils::Hook::Nop(0x461A9E, 5);
		Utils::Hook::Nop(0x461AAA, 5);
		Utils::Hook::Set<BYTE>(0x461AB4, 0xEB);

		// vid_restart when ingame
		Utils::Hook::Nop(0x4CA1FA, 6);

		// Filter log (initially com_logFilter, but I don't see why that dvar is needed)
		// Seems like it's needed for B3, so there is a separate handling for dedicated servers in Dedicated.cpp
		if (!Dedicated::IsEnabled())
		{
			Utils::Hook::Nop(0x647466, 5); // 'dvar set' lines
			Utils::Hook::Nop(0x5DF4F2, 5); // 'sending splash open' lines
		}

		// intro stuff
		Utils::Hook::Nop(0x60BEE9, 5); // Don't show legals
		Utils::Hook::Nop(0x60BEF6, 5); // Don't reset the intro dvar
		Utils::Hook::Set<char*>(0x60BED2, "unskippablecinematic IW_logo\n");
		Utils::Hook::Set<char*>(0x51C2A4, "%s\\" BASEGAME "\\video\\%s.bik");
		Utils::Hook::Set<DWORD>(0x51C2C2, 0x78A0AC);

		// Redirect logs
		Utils::Hook::Set<char*>(0x5E44D8, "logs/games_mp.log");
		Utils::Hook::Set<char*>(0x60A90C, "logs/console_mp.log");
		Utils::Hook::Set<char*>(0x60A918, "logs/console_mp.log");

		// Rename config
		Utils::Hook::Set<char*>(0x461B4B, CLIENT_CONFIG);
		Utils::Hook::Set<char*>(0x47DCBB, CLIENT_CONFIG);
		Utils::Hook::Set<char*>(0x6098F8, CLIENT_CONFIG);
		Utils::Hook::Set<char*>(0x60B279, CLIENT_CONFIG);
		Utils::Hook::Set<char*>(0x60BBD4, CLIENT_CONFIG);

		// Disable profile system
//		Utils::Hook::Nop(0x60BEB1, 5);          // GamerProfile_InitAllProfiles - Causes an error, when calling a harrier killstreak.
//		Utils::Hook::Nop(0x60BEB8, 5);          // GamerProfile_LogInProfile
//		Utils::Hook::Nop(0x4059EA, 5);          // GamerProfile_RegisterCommands
		Utils::Hook::Nop(0x4059EF, 5);          // GamerProfile_RegisterDvars
		Utils::Hook::Nop(0x47DF9A, 5);          // GamerProfile_UpdateSystemDvars
		Utils::Hook::Set<BYTE>(0x5AF0D0, 0xC3); // GamerProfile_SaveProfile
		Utils::Hook::Set<BYTE>(0x4E6870, 0xC3); // GamerProfile_UpdateSystemVarsFromProfile
		Utils::Hook::Set<BYTE>(0x4C37F0, 0xC3); // GamerProfile_UpdateProfileAndSaveIfNeeded
		Utils::Hook::Set<BYTE>(0x633CA0, 0xC3); // GamerProfile_SetPercentCompleteMP

		Utils::Hook::Nop(0x5AF1EC, 5); // Profile loading error
		Utils::Hook::Set<BYTE>(0x5AE212, 0xC3); // Profile reading

		// GamerProfile_RegisterCommands
		// Some random function used as nullsub :P
		Utils::Hook::Set<DWORD>(0x45B868, 0x5188FB); // profile_menuDvarsSetup
		Utils::Hook::Set<DWORD>(0x45B87E, 0x5188FB); // profile_menuDvarsFinish
		Utils::Hook::Set<DWORD>(0x45B894, 0x5188FB); // profile_toggleInvertedPitch
		Utils::Hook::Set<DWORD>(0x45B8AA, 0x5188FB); // profile_setViewSensitivity
		Utils::Hook::Set<DWORD>(0x45B8C3, 0x5188FB); // profile_setButtonsConfig
		Utils::Hook::Set<DWORD>(0x45B8D9, 0x5188FB); // profile_setSticksConfig
		Utils::Hook::Set<DWORD>(0x45B8EF, 0x5188FB); // profile_toggleAutoAim
		Utils::Hook::Set<DWORD>(0x45B905, 0x5188FB); // profile_SetHasEverPlayed_MainMenu
		Utils::Hook::Set<DWORD>(0x45B91E, 0x5188FB); // profile_SetHasEverPlayed_SP
		Utils::Hook::Set<DWORD>(0x45B934, 0x5188FB); // profile_SetHasEverPlayed_SO
		Utils::Hook::Set<DWORD>(0x45B94A, 0x5188FB); // profile_SetHasEverPlayed_MP
		Utils::Hook::Set<DWORD>(0x45B960, 0x5188FB); // profile_setVolume
		Utils::Hook::Set<DWORD>(0x45B979, 0x5188FB); // profile_setGamma
		Utils::Hook::Set<DWORD>(0x45B98F, 0x5188FB); // profile_setBlacklevel
		Utils::Hook::Set<DWORD>(0x45B9A5, 0x5188FB); // profile_toggleCanSkipOffensiveMissions

		// Patch SV_IsClientUsingOnlineStatsOffline
		Utils::Hook::Set<DWORD>(0x46B710, 0x90C3C033);

		// Fix mouse lag
		Utils::Hook::Nop(0x4731F5, 8);
		Scheduler::OnFrame([]()
		{
			SetThreadExecutionState(ES_DISPLAY_REQUIRED);
		});

		// Fix mouse pitch adjustments
		Dvar::Register<bool>("ui_mousePitch", false, Game::DVAR_FLAG_SAVED, "");
		UIScript::Add("updateui_mousePitch", [](UIScript::Token)
		{
			if (Dvar::Var("ui_mousePitch").get<bool>())
			{
				Dvar::Var("m_pitch").set(-0.022f);
			}
			else
			{
				Dvar::Var("m_pitch").set(0.022f);
			}
		});

		// Exploit fixes
		Utils::Hook::Set<BYTE>(0x412370, 0xC3);                                                      // SV_SteamAuthClient
		Utils::Hook::Set<BYTE>(0x5A8C70, 0xC3);                                                      // CL_HandleRelayPacket
		Utils::Hook(0x414D92, QuickPatch::MsgReadBitsCompressCheckSV, HOOK_CALL).install()->quick(); // SV_ExecuteClientCommands
		Utils::Hook(0x4A9F56, QuickPatch::MsgReadBitsCompressCheckCL, HOOK_CALL).install()->quick(); // CL_ParseServerMessage
		Utils::Hook(0x407376, QuickPatch::SVCanReplaceServerCommand , HOOK_CALL).install()->quick(); // SV_CanReplaceServerCommand
		Utils::Hook(0x5B67ED, QuickPatch::AtolAdjustPlayerLimit     , HOOK_CALL).install()->quick(); // PartyHost_HandleJoinPartyRequest


		// Patch selectStringTableEntryInDvar
		Utils::Hook::Set(0x405959, QuickPatch::SelectStringTableEntryInDvarStub);

		// Patch SV_GameClientNum for edge case generating status
		Utils::Hook(0x624DE2, QuickPatch::SVGameClientNum, HOOK_CALL).install()->quick();

		// Ignore call to print 'Offhand class mismatch when giving weapon...'
		Utils::Hook(0x5D9047, 0x4BB9B0, HOOK_CALL).install()->quick();

		// Nop call to print 'Unexpected text \'%s\' when trying to find \'%s\' in map\'s effect file\n' because the _fx gsc can be anything the user wants
		// Fixes the 
		// Error: Unexpected text main()
		// {
		//		
		// }
		// on ported maps
		Utils::Hook::Nop(0x57E65A, 5);

		Command::Add("unlockstats", [](Command::Params*)
		{
			QuickPatch::UnlockStats();
		});

		Command::Add("crash", [](Command::Params*)
		{
			throw new std::exception();
		});

		Command::Add("dumptechsets", [](Command::Params* param)
		{
			if (param->length() != 2)
			{
				Logger::Print("usage: dumptechsets <fastfile> | all\n");
				return;
			}
			std::vector<std::string> fastfiles;

			if (param->get(1) == "all"s)
			{
				for (std::string f : Utils::IO::ListFiles("zone/english"))
					fastfiles.push_back(f.substr(7, f.length() - 10));
				for (std::string f : Utils::IO::ListFiles("zone/dlc"))
					fastfiles.push_back(f.substr(3, f.length() - 6));
				for (std::string f : Utils::IO::ListFiles("zone/patch"))
					fastfiles.push_back(f.substr(5, f.length() - 8));
			}
			else
			{
				fastfiles.push_back(param->get(1));
			}

			int count = 0;

			AssetHandler::OnLoad([](Game::XAssetType type, Game::XAssetHeader asset, const std::string& name, bool* /*restrict*/)
			{
				// they're basically the same right?
				if (type == Game::ASSET_TYPE_PIXELSHADER || type == Game::ASSET_TYPE_VERTEXSHADER)
				{
					Utils::IO::CreateDir("userraw/shader_bin");

					const char* formatString;
					if (type == Game::ASSET_TYPE_PIXELSHADER)
					{
						formatString = "userraw/shader_bin/%.ps";
					}
					else
					{
						formatString = "userraw/shader_bin/%.vs";
					}

					if (Utils::IO::FileExists(Utils::String::VA(formatString, name.data()))) return;

					Utils::Stream buffer(0x1000);
					Game::MaterialPixelShader* dest = buffer.dest<Game::MaterialPixelShader>();
					buffer.save(asset.pixelShader);

					if (asset.pixelShader->prog.loadDef.program)
					{
						buffer.saveArray(asset.pixelShader->prog.loadDef.program, asset.pixelShader->prog.loadDef.programSize);
						Utils::Stream::ClearPointer(&dest->prog.loadDef.program);
					}

					Utils::IO::WriteFile(Utils::String::VA(formatString, name.data()), buffer.toBuffer());
				}

				static std::map<const void*, unsigned int> pointerMap;

				// Check if the given pointer has already been mapped
				std::function<bool(const void*)> hasPointer = [](const void* pointer)
				{
					return (pointerMap.find(pointer) != pointerMap.end());
				};

				// Get stored offset for given file pointer
				std::function<unsigned int(const void*)> getPointer = [hasPointer](const void* pointer)
				{
					if (hasPointer(pointer))
					{
						return pointerMap[pointer];
					}

					return 0U;
				};

				std::function<void(const void*, unsigned int)> storePointer = [hasPointer](const void* ptr, unsigned int offset)
				{
					if (hasPointer(ptr)) return;
					pointerMap[ptr] = offset;
				};

				if (type == Game::ASSET_TYPE_TECHNIQUE_SET)
				{
					Utils::IO::CreateDir("userraw/techsets");
					Utils::Stream buffer(0x1000);
					Game::MaterialTechniqueSet* dest = buffer.dest<Game::MaterialTechniqueSet>();
					buffer.save(asset.techniqueSet);

					if (asset.techniqueSet->name)
					{
						buffer.saveString(asset.techniqueSet->name);
						Utils::Stream::ClearPointer(&dest->name);
					}

					for (int i = 0; i < ARRAYSIZE(Game::MaterialTechniqueSet::techniques); ++i)
					{
						Game::MaterialTechnique* technique = asset.techniqueSet->techniques[i];

						if (technique)
						{
							dest->techniques[i] = reinterpret_cast<Game::MaterialTechnique*>(getPointer(technique));
							if (!dest->techniques)
							{
								// Size-check is obsolete, as the structure is dynamic
								buffer.align(Utils::Stream::ALIGN_4);
								//storePointer(technique, buffer->);

								Game::MaterialTechnique* destTechnique = buffer.dest<Game::MaterialTechnique>();
								buffer.save(technique, 8);

								// Save_MaterialPassArray
								Game::MaterialPass* destPasses = buffer.dest<Game::MaterialPass>();
								buffer.saveArray(technique->passArray, technique->passCount);

								for (short j = 0; j < technique->passCount; ++j)
								{
									AssertSize(Game::MaterialPass, 20);

									Game::MaterialPass* destPass = &destPasses[j];
									Game::MaterialPass* pass = &technique->passArray[j];

									if (pass->vertexDecl)
									{

									}

									if (pass->args)
									{
										buffer.align(Utils::Stream::ALIGN_4);
										buffer.saveArray(pass->args, pass->perPrimArgCount + pass->perObjArgCount + pass->stableArgCount);
										Utils::Stream::ClearPointer(&destPass->args);
									}
								}

								if (technique->name)
								{
									buffer.saveString(technique->name);
									Utils::Stream::ClearPointer(&destTechnique->name);
								}

								Utils::Stream::ClearPointer(&dest->techniques[i]);
							}
						}
					}
				}
			});

			for (std::string fastfile : fastfiles)
			{
				if (!Game::DB_IsZoneLoaded(fastfile.data()))
				{
					Game::XZoneInfo info;
					info.name = fastfile.data();
					info.allocFlags = 0x20;
					info.freeFlags = 0;

					Game::DB_LoadXAssets(&info, 1, true);
				}

				// unload the fastfiles so we don't run out of memory or asset pools
				if (count % 5)
				{
					Game::XZoneInfo info;
					info.name = nullptr;
					info.allocFlags = 0x0;
					info.freeFlags = 0x20;
					Game::DB_LoadXAssets(&info, 1, true);
				}
				
				count++;
			}
		});

		AssetHandler::OnLoad([](Game::XAssetType type, Game::XAssetHeader asset, const std::string& /*name*/, bool* /*restrict*/)
		{
			if (type == Game::XAssetType::ASSET_TYPE_GFXWORLD)
			{
				std::string buffer;

				for (unsigned int i = 0; i < asset.gfxWorld->dpvs.staticSurfaceCount; ++i)
				{
					buffer.append(Utils::String::VA("%s\n", asset.gfxWorld->dpvs.surfaces[asset.gfxWorld->dpvs.sortedSurfIndex[i]].material->info.name));
				}

				Utils::IO::WriteFile("userraw/logs/matlog.txt", buffer);
			}
		});

		Dvar::OnInit([]
		{
			Dvar::Register<bool>("r_drawAabbTrees", false, Game::DVAR_FLAG_USERCREATED, "Draw aabb trees");
		});

		Scheduler::OnFrame([]()
		{
			if (!Game::CL_IsCgameInitialized() || !Dvar::Var("r_drawAabbTrees").get<bool>()) return;

			float cyan[4] = { 0.0f, 0.5f, 0.5f, 1.0f };
            float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

			Game::clipMap_t* clipMap = *reinterpret_cast<Game::clipMap_t**>(0x7998E0);
			//Game::GfxWorld* gameWorld = *reinterpret_cast<Game::GfxWorld**>(0x66DEE94);
			if (!clipMap) return;

            for (unsigned short i = 0; i < clipMap->smodelNodeCount; ++i)
            {
                Game::R_AddDebugBounds(cyan, &clipMap->smodelNodes[i].bounds);
            }

            for (unsigned int i = 0; i < clipMap->numStaticModels; i += 2)
            {
                Game::R_AddDebugBounds(red, &clipMap->staticModelList[i].absBounds);
            }			
		});


		// Dvars
		Dvar::Register<bool>("ui_streamFriendly", false, Game::DVAR_FLAG_SAVED, "Stream friendly UI");

		// Debug patches
#ifdef DEBUG
		// ui_debugMode 1
		//Utils::Hook::Set<bool>(0x6312E0, true);

		// fs_debug 1
		Utils::Hook::Set<bool>(0x643172, true);

		// developer 2
		Utils::Hook::Set<BYTE>(0x4FA425, 2);
		Utils::Hook::Set<BYTE>(0x51B087, 2);
		Utils::Hook::Set<BYTE>(0x60AE13, 2);

		// developer_Script 1
		Utils::Hook::Set<bool>(0x60AE2B, true);

		// Disable cheat protection for dvars
		Utils::Hook::Set<BYTE>(0x647682, 0xEB);

		// Constantly draw the mini console
		Utils::Hook::Set<BYTE>(0x412A45, 0xEB);

		Scheduler::OnFrame([]()
		{
			if (*reinterpret_cast<Game::Font_s**>(0x62E4BAC))
			{
				Game::Con_DrawMiniConsole(0, 2, 4, (Game::CL_IsCgameInitialized() ? 1.0f : 0.4f));
			}
		}, true);
#else
		// Remove missing tag message
		Utils::Hook::Nop(0x4EBF1A, 5);
#endif

		if (Flags::HasFlag("nointro"))
		{
			Utils::Hook::Set<BYTE>(0x60BECF, 0xEB);
		}
	}

	QuickPatch::~QuickPatch()
	{

	}

	bool QuickPatch::unitTest()
	{
		uint32_t randIntCount = 4'000'000;
		printf("Generating %d random integers...", randIntCount);

		auto startTime = std::chrono::high_resolution_clock::now();

		for (uint32_t i = 0; i < randIntCount; ++i)
		{
			Utils::Cryptography::Rand::GenerateInt();
		}

		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
		Logger::Print("took %llims\n", duration);

		printf("Testing ZLib compression...");

		std::string test = Utils::String::VA("%c", Utils::Cryptography::Rand::GenerateInt());

		for (int i = 0; i < 21; ++i)
		{
			std::string compressed = Utils::Compression::ZLib::Compress(test);
			std::string decompressed = Utils::Compression::ZLib::Decompress(compressed);

			if (test != decompressed)
			{
				printf("Error\n");
				printf("Compressing %d bytes and decompressing failed!\n", test.size());
				return false;
			}

			auto size = test.size();
			for (unsigned int j = 0; j < size; ++j)
			{
				test.append(Utils::String::VA("%c", Utils::Cryptography::Rand::GenerateInt()));
			}
		}

		printf("Success\n");

		printf("Testing trimming...");
		std::string trim1 = " 1 ";
		std::string trim2 = "   1";
		std::string trim3 = "1   ";

		Utils::String::Trim(trim1);
		Utils::String::LTrim(trim2);
		Utils::String::RTrim(trim3);

		if (trim1 != "1") return false;
		if (trim2 != "1") return false;
		if (trim3 != "1") return false;

		printf("Success\n");
		return true;
	}
}
