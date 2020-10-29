#include "STDInclude.hpp"

#define NEWS_MOTD_DEFAULT "Welcome to IW4x Multiplayer!"

namespace Components
{
	bool News::Terminate;
	std::thread News::Thread;

	bool News::unitTest()
	{
		bool result = true;

		if (News::Thread.joinable())
		{
			Logger::Print("Awaiting thread termination...\n");
			News::Thread.join();

			if (!strcmp(Localization::Get("MPUI_MOTD_TEXT"), NEWS_MOTD_DEFAULT))
			{
				Logger::Print("Failed to fetch motd!\n");
				result = false;
			}
			else
			{
				Logger::Print("Successfully fetched motd.\n");
			}
		}

		return result;
	}

	const char* News::GetNewsText()
	{
		return Localization::Get("MPUI_MOTD_TEXT");
	}

	News::News()
	{
		if (ZoneBuilder::IsEnabled() || Dedicated::IsEnabled()) return; // Maybe also dedi?

		Dvar::Register<bool>("g_firstLaunch", true, Game::DVAR_FLAG_SAVED, "");

		Dvar::Register<int>("cl_updateoldversion", REVISION, REVISION, REVISION, Game::DVAR_FLAG_WRITEPROTECTED, "Current version number.");

		UIScript::Add("checkFirstLaunch", [](UIScript::Token)
		{
			if (Dvar::Var("g_firstLaunch").get<bool>())
			{
				Command::Execute("openmenu menu_first_launch", false);
				//Dvar::Var("g_firstLaunch").set(false);
			}
		});

		UIScript::Add("visitWebsite", [](UIScript::Token)
		{
			Utils::OpenUrl(Utils::Cache::GetStaticUrl(""));
		});

		UIScript::Add("visitWiki", [](UIScript::Token)
		{
			//Utils::OpenUrl(Utils::Cache::GetStaticUrl("/wiki/"));
			Utils::OpenUrl("https://github.com/Jawesome99/IW4x/wiki");
		});

		UIScript::Add("visitDiscord", [](UIScript::Token)
		{
			Utils::OpenUrl("https://discord.gg/sKeVmR3");
		});

		Localization::Set("MPUI_CHANGELOG_TEXT", "Loading...");
		Localization::Set("MPUI_MOTD_TEXT", NEWS_MOTD_DEFAULT);

		// make newsfeed (ticker) menu items not cut off based on safe area
		Utils::Hook::Nop(0x63892D, 5);

		// hook for getting the news ticker string
		Utils::Hook::Nop(0x6388BB, 2); // skip the "if (item->text[0] == '@')" localize check
		Utils::Hook(0x6388C1, News::GetNewsText, HOOK_CALL).install()->quick();

		if (!Utils::IsWineEnvironment() && !Loader::IsPerformingUnitTests())
		{
			News::Terminate = false;
			News::Thread = std::thread([]()
			{
				Changelog::LoadChangelog();
				if (News::Terminate) return;

				std::string data = Utils::Cache::GetFile("/iw4/motd.txt");
				if (!data.empty())
				{
					Localization::Set("MPUI_MOTD_TEXT", data);
				}

				if (!Loader::IsPerformingUnitTests() && !News::Terminate)
				{
					while (!News::Terminate)
					{
						// Sleep for 3 minutes
						for (int i = 0; i < 180 && !News::Terminate; ++i)
						{
							std::this_thread::sleep_for(1s);
						}
					}
				}
			});
		}
	}

	void News::preDestroy()
	{
		News::Terminate = true;

		if (News::Thread.joinable())
		{
			News::Thread.join();
		}

		News::Thread = std::thread();
	}
}
