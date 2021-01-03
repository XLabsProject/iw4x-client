#include "STDInclude.hpp"

namespace Components
{
	std::recursive_mutex Localization::LocalizeMutex;
	Dvar::Var Localization::UseLocalization;
	std::unordered_map<std::string, Game::LocalizeEntry*> Localization::LocalizeMap;
	std::unordered_map<std::string, Game::LocalizeEntry*> Localization::TempLocalizeMap;

	void Localization::Set(const std::string& key, const std::string& value)
	{
		std::lock_guard<std::recursive_mutex> _(Localization::LocalizeMutex);
		Utils::Memory::Allocator* allocator = Utils::Memory::GetAllocator();

		if (Localization::LocalizeMap.find(key) != Localization::LocalizeMap.end())
		{
			Game::LocalizeEntry* entry = Localization::LocalizeMap[key];

			char* newStaticValue = allocator->duplicateString(value);
			if (!newStaticValue) return;
			if (entry->value) allocator->free(entry->value);
			entry->value = newStaticValue;
			return;
		}

		Game::LocalizeEntry* entry = allocator->allocate<Game::LocalizeEntry>();
		if (!entry) return;

		entry->name = allocator->duplicateString(key);
		if (!entry->name)
		{
			allocator->free(entry);
			return;
		}

		entry->value = allocator->duplicateString(value);
		if (!entry->value)
		{
			allocator->free(entry->name);
			allocator->free(entry);
			return;
		}

		Localization::LocalizeMap[key] = entry;
	}

	const char* Localization::Get(const char* key)
	{
		if (!Localization::UseLocalization.get<bool>()) return key;

		Game::LocalizeEntry* entry = nullptr;
		{
			std::lock_guard<std::recursive_mutex> _(Localization::LocalizeMutex);

			if (Localization::TempLocalizeMap.find(key) != Localization::TempLocalizeMap.end())
			{
				entry = Localization::TempLocalizeMap[key];
			}
			else if (Localization::LocalizeMap.find(key) != Localization::LocalizeMap.end())
			{
				entry = Localization::LocalizeMap[key];
			}
		}

		if (!entry || !entry->value)
		{
			entry = Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_LOCALIZE_ENTRY, key).localize;
		}

		if (entry && entry->value)
		{
			return entry->value;
		}

		return key;
	}

	void Localization::SetTemp(const std::string& key, const std::string& value)
	{
		std::lock_guard<std::recursive_mutex> _(Localization::LocalizeMutex);
		Utils::Memory::Allocator* allocator = Utils::Memory::GetAllocator();

		if (Localization::TempLocalizeMap.find(key) != Localization::TempLocalizeMap.end())
		{
			Game::LocalizeEntry* entry = Localization::TempLocalizeMap[key];
			if (entry->value) allocator->free(entry->value);
			entry->value = allocator->duplicateString(value);
		}
		else
		{
			Game::LocalizeEntry* entry = allocator->allocate<Game::LocalizeEntry>();
			if (!entry) return;

			entry->name = allocator->duplicateString(key);
			if (!entry->name)
			{
				allocator->free(entry);
				return;
			}

			entry->value = allocator->duplicateString(value);
			if (!entry->value)
			{
				allocator->free(entry->name);
				allocator->free(entry);
				return;
			}

			Localization::TempLocalizeMap[key] = entry;
		}
	}

	void Localization::ClearTemp()
	{
		std::lock_guard<std::recursive_mutex> _(Localization::LocalizeMutex);
		Utils::Memory::Allocator* allocator = Utils::Memory::GetAllocator();

		for (auto i = Localization::TempLocalizeMap.begin(); i != Localization::TempLocalizeMap.end(); ++i)
		{
			if (i->second)
			{
				if (i->second->name)  allocator->free(i->second->name);
				if (i->second->value) allocator->free(i->second->value);
				allocator->free(i->second);
			}
		}

		Localization::TempLocalizeMap.clear();
	}

	void __stdcall Localization::SetStringStub(const char* key, const char* value, bool /*isEnglish*/)
	{
		Localization::Set(key, value);
	}

	void Localization::LoadLanguageStrings()
	{
		//if (ZoneBuilder::IsEnabled())
		{
			if (FileSystem::File(Utils::String::VA("localizedstrings/iw4x_%s.str", Game::Win_GetLanguage())).exists())
			{
				Game::SE_Load(Utils::String::VA("localizedstrings/iw4x_%s.str", Game::Win_GetLanguage()), 0);
			}
			else if (FileSystem::File("localizedstrings/iw4x_english.str").exists())
			{
				Game::SE_Load("localizedstrings/iw4x_english.str", 0);
			}
		}
	}

	__declspec(naked) void Localization::SELoadLanguageStub()
	{
		__asm
		{
			pushad
			call Localization::LoadLanguageStrings
			popad

			push 629E20h
			retn
		}
	}

	void Localization::SetCredits()
	{
		static const char* staff[] =
		{
			"/dev/../",
			"/dev/console",
			"/dev/full",
			"/dev/sdb",
			"/dev/sr0",
			"/dev/tty0",
			"/dev/urandom",
			"Snake",
			"lsb_release -a"
		};

		static const char* contributors[] =
		{
			"a231",
			"AmateurHailbut",
			"Aoki",
			"civil",
			"Dasfonia",
			"Deity",
			"Dizzy",
			"Dss0",
			"H3X1C",
			"HardNougat",
			"Homura",
			"INeedGames",
			"Killera",
			"Lithium",
			"OneFourOne",
			"quaK",
			"RaidMax",
			"Revo",
			"RezTech",
			"Shadow the Hedgehog",
			"Slykuiper",
			"st0rm",
			"VVLNT",
			"X3RX35"
		};

		static const char* specials[] =
		{
			"NTAuthority",
			"aerosoul94",
			"ReactIW4",
			"IW4Play",
			"V2",
			"luckyy"
		};

		std::string credits = "^2The IW4x Team:^7\n";

		for (int i = 0; i < ARRAYSIZE(staff); ++i)
		{
			credits.append(staff[i]);
			credits.append("\n");
		}

		credits.append("\n^3Contributors:^7\n");

		for (int i = 0; i < ARRAYSIZE(contributors); ++i)
		{
			credits.append(contributors[i]);
			credits.append("\n");
		}

		credits.append("\n^5Special thanks to:^7\n");

		for (int i = 0; i < ARRAYSIZE(specials); ++i)
		{
			credits.append(specials[i]);
			credits.append("\n");
		}

		// I have no idea why, but the last 2 lines are invisible!
		credits.append("-\n-");

		Localization::Set("IW4X_CREDITS", credits);
	}

	Localization::Localization()
	{
		Localization::SetCredits();

		AssetHandler::OnFind(Game::XAssetType::ASSET_TYPE_LOCALIZE_ENTRY, [](Game::XAssetType, const std::string& filename)
		{
			Game::XAssetHeader header = { nullptr };
			std::lock_guard<std::recursive_mutex> _(Localization::LocalizeMutex);

			if (Localization::TempLocalizeMap.find(filename) != Localization::TempLocalizeMap.end())
			{
				header.localize = Localization::TempLocalizeMap[filename];
			}
			else if (Localization::LocalizeMap.find(filename) != Localization::LocalizeMap.end())
			{
				header.localize = Localization::LocalizeMap[filename];
			}

			return header;
		});

		// Resolving hook
		Utils::Hook(0x629B90, Localization::Get, HOOK_JUMP).install()->quick();

		// Set loading entry point
		Utils::Hook(0x41D859, Localization::SELoadLanguageStub, HOOK_CALL).install()->quick();

		// Overwrite SetString
		Utils::Hook(0x4CE5EE, Localization::SetStringStub, HOOK_CALL).install()->quick();

		Localization::UseLocalization = Dvar::Register<bool>("ui_localize", true, Game::dvar_flag::DVAR_FLAG_NONE, "Use localization strings");

		// Generate localized entries for custom classes above 10
		AssetHandler::OnLoad([](Game::XAssetType type, Game::XAssetHeader asset, const std::string& name, bool* /*restrict*/)
		{
			if (type != Game::XAssetType::ASSET_TYPE_LOCALIZE_ENTRY) return;

			if (name == "CLASS_SLOT1"s)
			{
				for (int i = 11; i <= NUM_CUSTOM_CLASSES; ++i)
				{
					std::string key = Utils::String::VA("CLASS_SLOT%i", i);

					std::string value = asset.localize->value;
					Utils::String::Replace(value, "1", Utils::String::VA("%i", i)); // Pretty ugly, but it should work

					Localization::Set(key, value);
				}
			}
		});

// #ifndef DISABLE_ANTICHEAT
// 		if (!Dedicated::IsEnabled() && !ZoneBuilder::IsEnabled() && !Utils::IsWineEnvironment() && !Loader::IsPerformingUnitTests())
// 		{
// 			AntiCheat::PatchVirtualProtect(VirtualProtect, VirtualProtectEx);
// 		}
// #endif
	}

	Localization::~Localization()
	{
		Localization::ClearTemp();
		Localization::LocalizeMap.clear();
	}
}
