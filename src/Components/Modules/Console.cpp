#include "STDInclude.hpp"

namespace Components
{
	WINDOW* Console::OutputWindow;
	WINDOW* Console::InputWindow;
	WINDOW* Console::InfoWindow;

	int Console::OutputTop = 0;
	int Console::OutBuffer = 0;
	int Console::LastRefresh = 0;

	int Console::Height = 25;
	int Console::Width = 80;

	char Console::LineBuffer[1024] = { 0 };
	char Console::LineBuffer2[1024] = { 0 };
	int Console::LineBufferIndex = 0;

	bool Console::HasConsole = false;
	bool Console::SkipShutdown = false;

	std::thread Console::ConsoleThread;

	Game::SafeArea Console::OriginalSafeArea;

	char** Console::GetAutoCompleteFileList(const char *path, const char *extension, Game::FsListBehavior_e behavior, int *numfiles, int allocTrackType)
	{
		if (path == reinterpret_cast<char*>(0xBAADF00D) || path == reinterpret_cast<char*>(0xCDCDCDCD) || ::Utils::Memory::IsBadReadPtr(path)) return nullptr;
		return Game::FS_GetFileList(path, extension, behavior, numfiles, allocTrackType);
	}

	void Console::ToggleConsole()
	{
		// possibly cls.keyCatchers?
		Utils::Hook::Xor<DWORD>(0xB2C538, 1);

		// g_consoleField
		Game::Field_Clear(reinterpret_cast<void*>(0xA1B6B0));

		// show console output?
		Utils::Hook::Set<BYTE>(0xA15F38, 0);
	}

	void Console::RefreshStatus()
	{
		std::string mapname = Dvar::Var("mapname").get<const char*>();
		std::string hostname = Colors::Strip(Dvar::Var("sv_hostname").get<const char*>());

		if (Console::HasConsole)
		{
			SetConsoleTitleA(hostname.data());

			int clientCount = 0;
			int maxclientCount = *Game::svs_numclients;

			if (maxclientCount)
			{
				for (int i = 0; i < maxclientCount; ++i)
				{
					if (Game::svs_clients[i].state >= 3)
					{
						++clientCount;
					}
				}
			}
			else
			{
				maxclientCount = Dvar::Var("party_maxplayers").get<int>();
				//maxclientCount = Game::Party_GetMaxPlayers(*Game::partyIngame);
				clientCount = Game::PartyHost_CountMembers(reinterpret_cast<Game::PartyData_s*>(0x1081C00));
			}

			wclear(Console::InfoWindow);
			wprintw(Console::InfoWindow, "%s : %d/%d players : map %s", hostname.data(), clientCount, maxclientCount, (mapname.size() ? mapname.data() : "none"));
			wnoutrefresh(Console::InfoWindow);
		}
		else if (IsWindow(Console::GetWindow()) != FALSE)
		{
			SetWindowTextA(Console::GetWindow(), Utils::String::VA("IW4x(" VERSION ") : %s", hostname.data()));
		}
	}

	void Console::ShowPrompt()
	{
		wattron(Console::InputWindow, COLOR_PAIR(10) | A_BOLD);
		wprintw(Console::InputWindow, "%s> ", VERSION);
	}

	void Console::RefreshOutput()
	{
		prefresh(Console::OutputWindow, ((Console::OutputTop > 0) ? (Console::OutputTop - 1) : 0), 0, 1, 0, Console::Height - 2, Console::Width - 1);
	}

	void Console::ScrollOutput(int amount)
	{
		Console::OutputTop += amount;

		if (Console::OutputTop > OUTPUT_MAX_TOP)
		{
			Console::OutputTop = OUTPUT_MAX_TOP;
		}
		else if (Console::OutputTop < 0)
		{
			Console::OutputTop = 0;
		}

		// make it only scroll the top if there's more than HEIGHT lines
		if (Console::OutBuffer >= 0)
		{
			Console::OutBuffer += amount;

			if (Console::OutBuffer >= Console::Height)
			{
				Console::OutBuffer = -1;
			}

			if (Console::OutputTop < Console::Height)
			{
				Console::OutputTop = 0;
			}
		}
	}

	const char* Console::Input()
	{
		if (!Console::HasConsole)
		{
			Console::ShowPrompt();
			wrefresh(Console::InputWindow);
			Console::HasConsole = true;
		}

		int currentTime = static_cast<int>(GetTickCount64()); // Make our compiler happy
		if ((currentTime - Console::LastRefresh) > 250)
		{
			Console::RefreshOutput();
			Console::LastRefresh = currentTime;
		}

		int c = wgetch(Console::InputWindow);

		if (c == ERR)
		{
			return nullptr;
		}

		switch (c)
		{
		case '\r':
		case 459: // keypad enter
		{
			wattron(Console::OutputWindow, COLOR_PAIR(10) | A_BOLD);
			wprintw(Console::OutputWindow, "%s", "]");

			if (Console::LineBufferIndex)
			{
				wprintw(Console::OutputWindow, "%s", Console::LineBuffer);
			}

			wprintw(Console::OutputWindow, "%s", "\n");
			wattroff(Console::OutputWindow, A_BOLD);
			wclear(Console::InputWindow);

			Console::ShowPrompt();

			wrefresh(Console::InputWindow);

			Console::ScrollOutput(1);
			Console::RefreshOutput();

			if (Console::LineBufferIndex)
			{
				strcpy_s(Console::LineBuffer2, Console::LineBuffer);
				strcat_s(Console::LineBuffer, "\n");
				Console::LineBufferIndex = 0;
				return Console::LineBuffer;
			}

			break;
		}
		case 'c' - 'a' + 1: // ctrl-c
		case 27:
		{
			Console::LineBuffer[0] = '\0';
			Console::LineBufferIndex = 0;

			wclear(Console::InputWindow);

			Console::ShowPrompt();

			wrefresh(Console::InputWindow);
			break;
		}
		case 8: // backspace
		{
			if (Console::LineBufferIndex > 0)
			{
				Console::LineBufferIndex--;
				Console::LineBuffer[Console::LineBufferIndex] = '\0';

				wprintw(Console::InputWindow, "%c %c", static_cast<char>(c), static_cast<char>(c));
				wrefresh(Console::InputWindow);
			}
			break;
		}
		case KEY_PPAGE:
		{
			Console::ScrollOutput(-1);
			Console::RefreshOutput();
			break;
		}
		case KEY_NPAGE:
		{
			Console::ScrollOutput(1);
			Console::RefreshOutput();
			break;
		}
		case KEY_UP:
		{
			wclear(Console::InputWindow);
			Console::ShowPrompt();
			wprintw(Console::InputWindow, "%s", Console::LineBuffer2);
			wrefresh(Console::InputWindow);

			strcpy_s(Console::LineBuffer, Console::LineBuffer2);
			Console::LineBufferIndex = strlen(Console::LineBuffer);
			break;
		}
		default:
			if (c <= 127 && Console::LineBufferIndex < 1022)
			{
				// temporary workaround , find out what overwrites our index later on
				//consoleLineBufferIndex = strlen(consoleLineBuffer);

				Console::LineBuffer[Console::LineBufferIndex++] = static_cast<char>(c);
				Console::LineBuffer[Console::LineBufferIndex] = '\0';
				wprintw(Console::InputWindow, "%c", static_cast<char>(c));
				wrefresh(Console::InputWindow);
			}
			break;
		}

		return nullptr;
	}

	void Console::Destroy()
	{
		__try
		{
			delwin(Console::OutputWindow);
			delwin(Console::InputWindow);
			delwin(Console::InfoWindow);
			endwin();
			delscreen(SP);
		}
		__finally {}

		Console::OutputWindow = nullptr;
		Console::InputWindow = nullptr;
		Console::InfoWindow = nullptr;
	}

	void Console::Create()
	{
		Console::OutputTop = 0;
		Console::OutBuffer = 0;
		Console::LastRefresh = 0;
		Console::LineBufferIndex = 0;
		Console::HasConsole = false;

		CONSOLE_SCREEN_BUFFER_INFO info;
		if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
		{
			Console::Width = info.dwSize.X;
			Console::Height = info.srWindow.Bottom - info.srWindow.Top + 1;
		}
		else
		{
			Console::Height = 25;
			Console::Width = 80;
		}

		initscr();
		raw();
		noecho();

		Console::OutputWindow = newpad(Console::Height - 1, Console::Width);
		Console::InputWindow = newwin(1, Console::Width, Console::Height - 1, 0);
		Console::InfoWindow = newwin(1, Console::Width, 0, 0);

		scrollok(Console::OutputWindow, true);
		idlok(Console::OutputWindow, true);
		scrollok(Console::InputWindow, true);
		nodelay(Console::InputWindow, true);
		keypad(Console::InputWindow, true);

		if (has_colors())
		{
			start_color();
			init_pair(1, COLOR_BLACK, COLOR_WHITE);
			init_pair(2, COLOR_WHITE, COLOR_BLACK);
			init_pair(3, COLOR_RED, COLOR_BLACK);
			init_pair(4, COLOR_GREEN, COLOR_BLACK);
			init_pair(5, COLOR_YELLOW, COLOR_BLACK);
			init_pair(6, COLOR_BLUE, COLOR_BLACK);
			init_pair(7, COLOR_CYAN, COLOR_BLACK);
			init_pair(8, COLOR_RED, COLOR_BLACK);
			init_pair(9, COLOR_WHITE, COLOR_BLACK);
			init_pair(10, COLOR_WHITE, COLOR_BLACK);
		}

		wbkgd(Console::InfoWindow, COLOR_PAIR(1));

		wrefresh(Console::InfoWindow);
		wrefresh(Console::InputWindow);

		Console::RefreshOutput();
	}

	void Console::Error(const char* format, ...)
	{
		static char buffer[32768];

		va_list va;
		va_start(va, format);
		_vsnprintf_s(buffer, sizeof(buffer), format, va);
		va_end(va);

		Game::Com_Printf(0, "ERROR:\n");
		Game::Com_Printf(0, buffer);

		Console::RefreshOutput();

		if (IsDebuggerPresent())
		{
			while (true)
			{
				std::this_thread::sleep_for(5s);
			}
		}

		TerminateProcess(GetCurrentProcess(), 0xDEADDEAD);
	}

	void Console::Print(const char* message)
	{
		if (!Console::OutputWindow) return;

		const char* p = message;
		while (*p != '\0')
		{
			if (*p == '^')
			{
				char color;
				++p;

				color = (*p - '0');

				if (color < 9 && color > 0)
				{
					wattron(Console::OutputWindow, COLOR_PAIR(color + 2));
					++p;
					continue;
				}
			}

			waddch(Console::OutputWindow, *p);

			++p;
		}

		wattron(Console::OutputWindow, COLOR_PAIR(9));

// 		int currentTime = static_cast<int>(GetTickCount64()); // Make our compiler happy
//
// 		if (!Console::HasConsole)
// 		{
// 			Console::RefreshOutput();
// 		}
// 		else if ((currentTime - Console::LastRefresh) > 100)
// 		{
// 			Console::RefreshOutput();
// 			Console::LastRefresh = currentTime;
// 		}

		Console::RefreshOutput();
	}

	void Console::ConsoleRunner()
	{
		Console::SkipShutdown = false;
		Game::Sys_ShowConsole();

		MSG message;
		while (IsWindow(Console::GetWindow()) != FALSE && GetMessageA(&message, nullptr, 0, 0))
		{
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}

		if (Console::SkipShutdown) return;

		if (Game::Sys_Milliseconds() - Console::LastRefresh > 100 &&
			MessageBoxA(nullptr, "The application is not responding anymore, do you want to force its termination?", "Application is not responding", MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
		{
			// Force process termination
			// if the main thread is not responding
			OutputDebugStringA("Process termination forced, as the main thread is not responding!");

			// We can not force the termination in this thread
			// The destructor would be called in this thread
			// and would try to join this thread, which is impossible
			TerminateProcess(GetCurrentProcess(), 0xFFFFFFFF);
		}
		else
		{
			// Send quit command to safely terminate the application
			Command::Execute("wait 200;quit\n", false);
		}
	}

	void Console::StdOutPrint(const char* message)
	{
		printf("%s", message);
		fflush(stdout);
	}

	void Console::StdOutError(const char* format, ...)
	{
		char buffer[0x1000] = { 0 };

		va_list ap;
		va_start(ap, format);
		_vsnprintf_s(buffer, sizeof(buffer), format, ap);
		va_end(ap);

		perror(buffer);
		fflush(stderr);

		ExitProcess(1);
	}

	__declspec(naked) void Console::DrawSolidConsoleStub()
	{
		__asm
		{
			pushad
			call Console::StoreSafeArea
			popad

			// We need esi preserved here, so we have to backup 'all' registers when storing the safearea
			call Game::Con_DrawSolidConsole

			pushad
			call Console::RestoreSafeArea
			popad
			retn
		}
	}

	void Console::StoreSafeArea()
	{
		// Backup the original safe area
		Console::OriginalSafeArea = *Game::safeArea;

		// Apply new safe area and border
		float border = 6.0f;
		Game::safeArea->top = border;
		Game::safeArea->left = border;
		Game::safeArea->bottom = static_cast<float>(Renderer::Height()) - border;
		Game::safeArea->right = static_cast<float>(Renderer::Width()) - border;

		Game::safeArea->textHeight = static_cast<int>((Game::safeArea->bottom - Game::safeArea->top - (2 * Game::safeArea->fontHeight) - 24.0) / Game::safeArea->fontHeight);
		Game::safeArea->textWidth = static_cast<int>(Game::safeArea->right - Game::safeArea->left - 10.0f - 18.0);
	}

	void Console::RestoreSafeArea()
	{
		// Restore the initial safe area
		*Game::safeArea = Console::OriginalSafeArea;
	}

	void Console::SetSkipShutdown()
	{
		Console::SkipShutdown = true;
	}

	void Console::FreeNativeConsole()
	{
		if (!Monitor::IsEnabled() && !Flags::HasFlag("stdout") && (!Dedicated::IsEnabled() || Flags::HasFlag("console")) && !Loader::IsPerformingUnitTests())
		{
			FreeConsole();
		}
	}

	HWND Console::GetWindow()
	{
		return *reinterpret_cast<HWND*>(0x64A3288);
	}

	void Console::ShowAsyncConsole()
	{
		Console::ConsoleThread = std::thread(Console::ConsoleRunner);
	}

	Game::dvar_t* Console::RegisterConColor(const char* name, float r, float g, float b, float a, float min, float max, int flags, const char* description)
	{
		static struct
		{
			const char* name;
			float color[4];
		} patchedColors[] =
		{
			{ "con_inputBoxColor",     { 0.20f, 0.20f, 0.20f, 1.00f } },
			{ "con_inputHintBoxColor", { 0.30f, 0.30f, 0.30f, 1.00f } },
			{ "con_outputBarColor",    { 0.50f, 0.50f, 0.50f, 0.60f } },
			{ "con_outputSliderColor", { 0.70f, 1.00f, 0.00f, 1.00f } },
			{ "con_outputWindowColor", { 0.25f, 0.25f, 0.25f, 0.85f } },
		};

		for (int i = 0; i < ARRAYSIZE(patchedColors); ++i)
		{
			if (std::string(name) == patchedColors[i].name)
			{
				r = patchedColors[i].color[0];
				g = patchedColors[i].color[1];
				b = patchedColors[i].color[2];
				a = patchedColors[i].color[3];
				break;
			}
		}

		return reinterpret_cast<Game::Dvar_RegisterVec4_t>(0x471500)(name, r, g, b, a, min, max, flags, description);
	}

	Console::Console()
	{
		// Console '%s: %s> ' string
		Utils::Hook::Set<char*>(0x5A44B4, "IW4x: " VERSION "> ");

		// Patch console color
		static float consoleColor[] = { 0.70f, 1.00f, 0.00f, 1.00f };
		Utils::Hook::Set<float*>(0x5A451A, consoleColor);
		Utils::Hook::Set<float*>(0x5A4400, consoleColor);

		// Internal console
		Utils::Hook(0x4F690C, Console::ToggleConsole, HOOK_CALL).install()->quick();
		Utils::Hook(0x4F65A5, Console::ToggleConsole, HOOK_JUMP).install()->quick();

		// Patch safearea for ingame-console
		Utils::Hook(0x5A50EF, Console::DrawSolidConsoleStub, HOOK_CALL).install()->quick();

		// Check for bad food ;)
		Utils::Hook(0x4CB9F4, Console::GetAutoCompleteFileList, HOOK_CALL).install()->quick();

		// Patch console dvars
		Utils::Hook(0x4829AB, Console::RegisterConColor, HOOK_CALL).install()->quick();
		Utils::Hook(0x4829EE, Console::RegisterConColor, HOOK_CALL).install()->quick();
		Utils::Hook(0x482A31, Console::RegisterConColor, HOOK_CALL).install()->quick();
		Utils::Hook(0x482A7A, Console::RegisterConColor, HOOK_CALL).install()->quick();
		Utils::Hook(0x482AC3, Console::RegisterConColor, HOOK_CALL).install()->quick();

		// Modify console style
		Utils::Hook::Set<BYTE>(0x428A8E, 0);    // Adjust logo Y pos
		Utils::Hook::Set<BYTE>(0x428A90, 0);    // Adjust logo X pos
		Utils::Hook::Set<BYTE>(0x428AF2, 67);   // Adjust output Y pos
		Utils::Hook::Set<DWORD>(0x428AC5, 397); // Adjust input Y pos
		Utils::Hook::Set<DWORD>(0x428951, 609); // Reduce window width
		Utils::Hook::Set<DWORD>(0x42895D, 423); // Reduce window height
		Utils::Hook::Set<DWORD>(0x428AC0, 597); // Reduce input width
		Utils::Hook::Set<DWORD>(0x428AED, 596); // Reduce output width

		// Don't resize the console
		Utils::Hook(0x64DC6B, 0x64DCC2, HOOK_JUMP).install()->quick();

		if (Dedicated::IsEnabled() && !ZoneBuilder::IsEnabled())
		{
			Scheduler::OnFrame(Console::RefreshStatus);
		}

		// Code below is not necessary when performing unit tests!
		if (Loader::IsPerformingUnitTests()) return;

		// External console
		if (Flags::HasFlag("stdout") || Monitor::IsEnabled())
		{
#ifndef DEBUG
			if (!Monitor::IsEnabled())
#endif
			{
				Utils::Hook(0x4B2080, Console::StdOutPrint, HOOK_JUMP).install()->quick();
				Utils::Hook(0x43D570, Console::StdOutError, HOOK_JUMP).install()->quick();
			}
		}
		else if (Flags::HasFlag("console") || ZoneBuilder::IsEnabled()) // ZoneBuilder uses the game's console, until the native one is adapted.
		{
			Utils::Hook::Nop(0x60BB58, 11);

			// Redirect input (]command)
			Utils::Hook(0x47025A, 0x4F5770, HOOK_CALL).install()->quick();

			Utils::Hook(0x60BB68, []()
			{
				Console::ShowAsyncConsole();
			}, HOOK_CALL).install()->quick();

			Utils::Hook(0x4D69A2, []()
			{
				Console::SetSkipShutdown();

				// Sys_DestroyConsole
				Utils::Hook::Call<void()>(0x4528A0)();

				if (Console::ConsoleThread.joinable())
				{
					Console::ConsoleThread.join();
				}
			}, HOOK_CALL).install()->quick();

			Scheduler::OnFrame([]()
			{
				Console::LastRefresh = Game::Sys_Milliseconds();
			});
		}
		else if (Dedicated::IsEnabled()/* || ZoneBuilder::IsEnabled()*/)
		{
			DWORD type = GetFileType(GetStdHandle(STD_INPUT_HANDLE));
			if (type != FILE_TYPE_CHAR)
			{
				MessageBoxA(nullptr, "Console not supported, please use '-stdout' or '-console' flag!", "ERRROR", MB_ICONERROR);
				TerminateProcess(GetCurrentProcess(), 1);
			}

			Utils::Hook::Nop(0x60BB58, 11);

			Utils::Hook(0x4305E0, Console::Create, HOOK_JUMP).install()->quick();
			Utils::Hook(0x4528A0, Console::Destroy, HOOK_JUMP).install()->quick();
			Utils::Hook(0x4B2080, Console::Print, HOOK_JUMP).install()->quick();
			Utils::Hook(0x43D570, Console::Error, HOOK_JUMP).install()->quick();
			Utils::Hook(0x4859A5, Console::Input, HOOK_CALL).install()->quick();
		}
		else if(!Loader::IsPerformingUnitTests())
		{
			FreeConsole();
		}
	}

	Console::~Console()
	{
		Console::SetSkipShutdown();
		if (Console::ConsoleThread.joinable())
		{
			Console::ConsoleThread.join();
		}
	}
}
