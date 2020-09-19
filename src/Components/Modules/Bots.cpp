#include "STDInclude.hpp"

namespace Components
{
	std::vector<std::string> Bots::BotNames;

	void Bots::BuildConnectString(char* buffer, const char* connectString, int num, int, int protocol, int checksum, int statVer, int statStuff, int port)
	{
		static int botId = 0;
		const char* botName;

		if (Bots::BotNames.empty())
		{
			FileSystem::File bots("bots.txt");

			if (bots.exists())
			{
				std::vector<std::string> names = Utils::String::Explode(bots.getBuffer(), '\n');

				for (auto name : names)
				{
					Utils::String::Replace(name, "\r", "");
					name = Utils::String::Trim(name);

					if (!name.empty())
					{
						Bots::BotNames.push_back(name);
					}
				}
			}
		}

		if (!Bots::BotNames.empty())
		{
			botId %= Bots::BotNames.size();
			botName = Bots::BotNames[botId++].data();
		}
		else
		{
			botName = Utils::String::VA("bot%d", botId++);
		}

		strncpy_s(buffer, 0x400, Utils::String::VA(connectString, num, botName, protocol, checksum, statVer, statStuff, port), 0x400);
	}

	void Bots::Spawn(unsigned int count)
	{
		for (unsigned int i = 0; i < count; ++i)
		{
			Scheduler::OnDelay([]()
			{
				for (int i = 0; i < 3; ++i)
				{
					Game::gentity_t* entRef = Game::SV_AddTestClient();
					if (entRef)
					{
						Scheduler::OnDelay([entRef]()
						{
							Game::Scr_AddString("autoassign");
							Game::Scr_AddString("team_marinesopfor");
							Game::Scr_Notify(entRef, Game::SL_GetString("menuresponse", 0), 2);

							Scheduler::OnDelay([entRef]()
							{
								Game::Scr_AddString(Utils::String::VA("class%d", Utils::Cryptography::Rand::GenerateInt() % 5));
								Game::Scr_AddString("changeclass");
								Game::Scr_Notify(entRef, Game::SL_GetString("menuresponse", 0), 2);
							}, 1s);
						}, 1s);

						break;
					}
				}
			}, 500ms * (i + 1));
		}
	}

	Bots::Bots()
	{
		// Replace connect string
		Utils::Hook::Set<const char*>(0x48ADA6, "connect bot%d \"\\cg_predictItems\\1\\cl_anonymous\\0\\color\\4\\head\\default\\model\\multi\\snaps\\20\\rate\\5000\\name\\%s\\protocol\\%d\\checksum\\%d\\statver\\%d %u\\qport\\%d\"");

		// Intercept sprintf for the connect string
		Utils::Hook(0x48ADAB, Bots::BuildConnectString, HOOK_CALL).install()->quick();

		Command::Add("spawnBot", [](Command::Params* params)
		{
			unsigned int count = 1;

			if (params->length() > 1)
			{
				if (params->get(1) == "all"s) count = static_cast<unsigned int>(-1);
				else count = atoi(params->get(1));
			}

			count = std::min(18u, count);

			// Check if ingame and host
			if (!Game::SV_Loaded())
			{
				Toast::Show("cardicon_headshot", "^1Error", "You need to be host to spawn bots!", 3000);
				Logger::Print("You need to be host to spawn bots!\n");
				return;
			}

			Toast::Show("cardicon_headshot", "^2Success", Utils::String::VA("Spawning %d %s...", count, (count == 1 ? "bot" : "bots")), 3000);
			Logger::Print("Spawning %d %s...\n", count, (count == 1 ? "bot" : "bots"));

			Bots::Spawn(count);
		});
	}

	Bots::~Bots()
	{
		Bots::BotNames.clear();
	}
}
