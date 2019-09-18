#include "STDInclude.hpp"

namespace Main
{
	void Initialize()
	{
		Utils::SetEnvironment();
		Utils::Cryptography::Initialize();
		Components::Loader::Initialize();

#if defined(DEBUG) || defined(FORCE_UNIT_TESTS)
		if (Components::Loader::IsPerformingUnitTests())
		{
			DWORD result = (Components::Loader::PerformUnitTests() ? 0 : -1);
			Components::Loader::Uninitialize();
			ExitProcess(result);
		}
#else
		if (Components::Flags::HasFlag("tests"))
		{
			Components::Logger::Print("Unit tests are disabled outside the debug environment!\n");
		}
#endif
	}

	void Uninitialize()
	{
		Components::Loader::Uninitialize();
		Utils::Cache::Uninitialize();
		google::protobuf::ShutdownProtobufLibrary();
	}

	__declspec(naked) void EntryPoint()
	{
		__asm
		{
			pushad
			call Main::Initialize
			popad

			push 6BAA2Fh // Continue init routine
			push 6CA062h // ___security_init_cookie
			retn
		}
	}
}

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD  ul_reason_for_call, LPVOID /*lpReserved*/)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		// Not sure if it conflicts with our TLS variables
		//DisableThreadLibraryCalls(hModule);

		if (!IsWindows8Point1OrGreater())
		{
			std::string buffer;
			std::string path = "players/warning.txt";

			//check if file exists
			if (Utils::IO::FileExists(path))
			{
				//if it does, open it and read it
				buffer = Utils::IO::ReadFile(path);

				//check the contents of the buffer
				if (buffer == "0" || buffer.empty())
				{
					//if its 0 OR empty, show the warning
					int reponse = MessageBoxA(nullptr, "Your version of Windows is unsupported.\nYou may encounter undefined behavior such as crashes.", "Unsupported Operating System", MB_OKCANCEL | MB_ICONWARNING);

					//check the response
					if (reponse == IDCANCEL)
					{
						//user clicked cancel, they saw the warning, lets write 1 to file and exit
						Utils::IO::WriteFile(path, "1");
						exit(-1);
					}
					else
					{
						//otherwise they saw it, write 1, continue with execution
						Utils::IO::WriteFile(path, "1");
					}
				}
				else if(buffer == "1")
				{
					//if the file exists and 1 is written, just continue
				}
			}
			else //the file doesnt exist
			{
				//create the file
				Utils::IO::WriteFile(path, "");

				//give warning
				int reponse = MessageBoxA(nullptr, "Your version of Windows is unsupported.\nYou may encounter unexpected behaviour such as crashes.", "Unsupported Operating System", MB_OKCANCEL | MB_ICONWARNING);

				//check the response
				if (reponse == IDCANCEL)
				{
					//user clicked cancel, they saw the warning, lets write 1 to file and exit
					Utils::IO::WriteFile(path, "1");
					exit(-1);
				}
				else
				{
					//otherwise they saw it, write 1, continue with execution
					Utils::IO::WriteFile(path, "1");
				}
			}
		}

		Steam::Proxy::RunMod();

		// Ensure we're working with our desired binary
		char* module = reinterpret_cast<char*>(0x400000);
		auto hash1 = Utils::Cryptography::JenkinsOneAtATime::Compute(module + 0x1000, 0x2D531F);  // .text
		auto hash2 = Utils::Cryptography::JenkinsOneAtATime::Compute(module + 0x2D75FC, 0xBDA04); // .rdata
		if ((hash1 != 0x54684DBE
#ifdef DEBUG
			&& hash1 != 0x8AADE716
#endif
			) || hash2 != 0x8030ec53)
		{
			return FALSE;
		}

#ifndef DISABLE_ANTICHEAT
		[]()
		{
			if (!Components::Dedicated::IsEnabled() && !Components::Loader::IsPerformingUnitTests())
			{
				Components::AntiCheat::ProtectProcess();
				Components::AntiCheat::PatchThreadCreation();
			}
		}();
#endif

		DWORD oldProtect;
		VirtualProtect(module + 0x1000, 0x2D6000, PAGE_EXECUTE_READ, &oldProtect); // Protect the .text segment

		// Install entry point hook
		Utils::Hook(0x6BAC0F, Main::EntryPoint, HOOK_JUMP).install()->quick();
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		Main::Uninitialize();
	}
	else if (ul_reason_for_call == DLL_THREAD_ATTACH)
	{
#ifndef DISABLE_ANTICHEAT
		[]()
		{
			if (!Components::Dedicated::IsEnabled() && !Components::Loader::IsPerformingUnitTests())
			{
				Components::AntiCheat::VerifyThreadIntegrity();
			}
		}();
#endif
	}

	return TRUE;
}
