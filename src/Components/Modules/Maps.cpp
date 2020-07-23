#include "STDInclude.hpp"

namespace Components
{
	Maps::UserMapContainer Maps::UserMap;
	std::string Maps::CurrentMainZone;
	std::vector<std::pair<std::string, std::string>> Maps::DependencyList;
	std::vector<std::string> Maps::CurrentDependencies;

	bool Maps::SPMap;
	std::vector<Maps::DLC> Maps::DlcPacks;

	const char* Maps::UserMapFiles[4] =
	{
		".ff",
		"_load.ff",
		".iwd",
		".arena",
	};

	Maps::UserMapContainer* Maps::GetUserMap()
	{
		return &Maps::UserMap;
	}

	void Maps::UserMapContainer::loadIwd()
	{
		if (this->isValid() && !this->searchPath.iwd)
		{
			std::string iwdName = Utils::String::VA("%s.iwd", this->mapname.data());
			std::string path = Utils::String::VA("%s\\usermaps\\%s\\%s", Dvar::Var("fs_basepath").get<const char*>(), this->mapname.data(), iwdName.data());

			if (Utils::IO::FileExists(path))
			{
				this->searchPath.iwd = Game::FS_IsShippedIWD(path.data(), iwdName.data());

				if (this->searchPath.iwd)
				{
					this->searchPath.bLocalized = false;
					this->searchPath.ignore = 0;
					this->searchPath.ignorePureCheck = 0;
					this->searchPath.language = 0;
					this->searchPath.dir = nullptr;
					this->searchPath.next = *Game::fs_searchpaths;
					*Game::fs_searchpaths = &this->searchPath;
				}
			}
		}
	}

	void Maps::UserMapContainer::reloadIwd()
	{
		if (this->isValid() && this->wasFreed)
		{
			this->wasFreed = false;
			this->searchPath.iwd = nullptr;
			this->loadIwd();
		}
	}

	void Maps::UserMapContainer::handlePackfile(void* packfile)
	{
		if (this->isValid() && this->searchPath.iwd == packfile)
		{
			this->wasFreed = true;
		}
	}

	void Maps::UserMapContainer::freeIwd()
	{
		if (this->isValid() && this->searchPath.iwd && !this->wasFreed)
		{
			this->wasFreed = true;

			// Unchain our searchpath
			for (Game::searchpath_t** pathPtr = Game::fs_searchpaths; *pathPtr; pathPtr = &(*pathPtr)->next)
			{
				if (*pathPtr == &this->searchPath)
				{
					*pathPtr = (*pathPtr)->next;
					break;
				}
			}

			Game::unzClose(this->searchPath.iwd->handle);

			auto _free = Utils::Hook::Call<void(void*)>(0x6B5CF2);
			_free(this->searchPath.iwd->buildBuffer);
			_free(this->searchPath.iwd);

			ZeroMemory(&this->searchPath, sizeof this->searchPath);
		}
	}

	const char* Maps::LoadArenaFileStub(const char* name, char* buffer, int size)
	{
		std::string data  = Game::LoadModdableRawfile(0, name);

		if(Maps::UserMap.isValid())
		{
			std::string mapname = Maps::UserMap.getName();
			std::string arena = Utils::String::VA("usermaps/%s/%s.arena", mapname.data(), mapname.data());

			if (Utils::IO::FileExists(arena))
			{
				data.append(Utils::IO::ReadFile(arena));
			}
		}

		strncpy_s(buffer, size, data.data(), data.size());
		return buffer;
	}

	void Maps::UnloadMapZones(Game::XZoneInfo* zoneInfo, unsigned int zoneCount, int sync)
	{
		Game::DB_LoadXAssets(zoneInfo, zoneCount, sync);

		if (Maps::UserMap.isValid())
		{
			Maps::UserMap.freeIwd();
			Maps::UserMap.clear();
		}
	}

	void Maps::LoadMapZones(Game::XZoneInfo* zoneInfo, unsigned int zoneCount, int sync)
	{
		if (!zoneInfo) return;

		Maps::SPMap = false;
		Maps::CurrentMainZone = zoneInfo->name;
		
		Maps::CurrentDependencies.clear();
		for (auto i = Maps::DependencyList.begin(); i != Maps::DependencyList.end(); ++i)
		{
			if (std::regex_match(zoneInfo->name, std::regex(i->first)))
			{
				if (std::find(Maps::CurrentDependencies.begin(), Maps::CurrentDependencies.end(), i->second) == Maps::CurrentDependencies.end())
				{
					Maps::CurrentDependencies.push_back(i->second);
				}
			}
		}

		Utils::Memory::Allocator allocator;
		auto teams = Maps::GetTeamsForMap(Maps::CurrentMainZone);

		auto dependencies = Maps::GetDependenciesForMap(Maps::CurrentMainZone);
		Utils::Merge(&Maps::CurrentDependencies, dependencies.data(), dependencies.size());

		std::vector<Game::XZoneInfo> data;
		Utils::Merge(&data, zoneInfo, zoneCount);
		
		Game::XZoneInfo team;
		team.allocFlags = zoneInfo->allocFlags;
		team.freeFlags = zoneInfo->freeFlags;

		team.name = allocator.duplicateString(Utils::String::VA("iw4x_team_%s", teams.first.data()));
		data.push_back(team);

		team.name = allocator.duplicateString(Utils::String::VA("iw4x_team_%s", teams.second.data()));
		data.push_back(team);

		for (unsigned int i = 0; i < Maps::CurrentDependencies.size(); ++i)
		{
			Game::XZoneInfo info;

			info.name = (&Maps::CurrentDependencies[i])->data();
			info.allocFlags = zoneInfo->allocFlags;
			info.freeFlags = zoneInfo->freeFlags;

			data.push_back(info);
		}

		// Load patch files
		std::string patchZone = Utils::String::VA("patch_%s", zoneInfo->name);
		if (FastFiles::Exists(patchZone))
		{
			data.push_back({patchZone.data(), zoneInfo->allocFlags, zoneInfo->freeFlags});
		}

		return FastFiles::LoadLocalizeZones(data.data(), data.size(), sync);
	}

	void Maps::OverrideMapEnts(Game::MapEnts* ents)
	{
		auto callback = [] (Game::XAssetHeader header, void* ents)
			{
				Game::MapEnts* mapEnts = reinterpret_cast<Game::MapEnts*>(ents);
				Game::clipMap_t* clipMap = header.clipMap;

				if (clipMap && mapEnts && !_stricmp(mapEnts->name, clipMap->name))
				{
					clipMap->mapEnts = mapEnts;
					//*Game::marMapEntsPtr = mapEnts;
					//Game::G_SpawnEntitiesFromString();
				}
			};

		// Internal doesn't lock the thread, as locking is impossible, due to executing this in the thread that holds the current lock
		Game::DB_EnumXAssets_Internal(Game::XAssetType::ASSET_TYPE_CLIPMAP_MP, callback, ents, true);
		Game::DB_EnumXAssets_Internal(Game::XAssetType::ASSET_TYPE_CLIPMAP_SP, callback, ents, true);
	}

	void Maps::LoadAssetRestrict(Game::XAssetType type, Game::XAssetHeader asset, const std::string& name, bool* restrict)
	{
		if (std::find(Maps::CurrentDependencies.begin(), Maps::CurrentDependencies.end(), FastFiles::Current()) != Maps::CurrentDependencies.end()
			&& (FastFiles::Current() != "mp_shipment_long" || Maps::CurrentMainZone != "mp_shipment")) // Shipment is a special case
		{
			if (type == Game::XAssetType::ASSET_TYPE_CLIPMAP_MP || type == Game::XAssetType::ASSET_TYPE_CLIPMAP_SP || type == Game::XAssetType::ASSET_TYPE_GAMEWORLD_SP || type == Game::XAssetType::ASSET_TYPE_GAMEWORLD_MP || type == Game::XAssetType::ASSET_TYPE_GFXWORLD || type == Game::XAssetType::ASSET_TYPE_MAP_ENTS || type == Game::XAssetType::ASSET_TYPE_COMWORLD || type == Game::XAssetType::ASSET_TYPE_FXWORLD)
			{
				*restrict = true;
				return;
			}
		}

		if (type == Game::XAssetType::ASSET_TYPE_ADDON_MAP_ENTS)
		{
			*restrict = true;
			return;
		}

		if (type == Game::XAssetType::ASSET_TYPE_WEAPON)
		{
			if ((!strstr(name.data(), "_mp") && name != "none" && name != "destructible_car") || Zones::Version() >= VERSION_ALPHA2)
			{
				*restrict = true;
				return;
			}
		}

		if (type == Game::XAssetType::ASSET_TYPE_STRINGTABLE)
		{
			if (FastFiles::Current() == "mp_cross_fire")
			{
				*restrict = true;
				return;
			}
		}

		if (type == Game::XAssetType::ASSET_TYPE_MAP_ENTS)
		{
			if (Flags::HasFlag("dump"))
			{
				Utils::IO::WriteFile(Utils::String::VA("raw/%s.ents", name.data()), asset.mapEnts->entityString);
			}

			static std::string mapEntities;
			FileSystem::File ents(name + ".ents");
			if (ents.exists())
			{
				mapEntities = ents.getBuffer();
				asset.mapEnts->entityString = const_cast<char*>(mapEntities.data());
				asset.mapEnts->numEntityChars = mapEntities.size() + 1;
			}
		}
		
		// This is broken
		if ((type == Game::XAssetType::ASSET_TYPE_MENU || type == Game::XAssetType::ASSET_TYPE_MENULIST) && Zones::Version() >= 359)
		{
			*restrict = true;
			return;
		}
	}

	Game::G_GlassData* Maps::GetWorldData()
	{
		Logger::Print("Waiting for database...\n");
		while (!Game::Sys_IsDatabaseReady()) std::this_thread::sleep_for(10ms);

		if (!Game::DB_XAssetPool[Game::XAssetType::ASSET_TYPE_GAMEWORLD_MP].gameWorldMp || !Game::DB_XAssetPool[Game::XAssetType::ASSET_TYPE_GAMEWORLD_MP].gameWorldMp->name || 
			!Game::DB_XAssetPool[Game::XAssetType::ASSET_TYPE_GAMEWORLD_MP].gameWorldMp->g_glassData || Maps::SPMap)
		{
			return Game::DB_XAssetPool[Game::XAssetType::ASSET_TYPE_GAMEWORLD_SP].gameWorldSp->g_glassData;
		}

		return Game::DB_XAssetPool[Game::XAssetType::ASSET_TYPE_GAMEWORLD_MP].gameWorldMp->g_glassData;
	}

	__declspec(naked) void Maps::GetWorldDataStub()
	{
		__asm
		{
			push eax
			pushad

			call Maps::GetWorldData

			mov [esp + 20h], eax
			popad
			pop eax

			retn
		}
	}

	void Maps::LoadRawSun()
	{
		Game::R_FlushSun();

		Game::GfxWorld* world = *reinterpret_cast<Game::GfxWorld**>(0x66DEE94);

		if (FileSystem::File(Utils::String::VA("sun/%s.sun", Maps::CurrentMainZone.data())).exists())
		{
			Game::R_LoadSunThroughDvars(Maps::CurrentMainZone.data(), &world->sun);
		}
	}

	void Maps::GetBSPName(char* buffer, size_t size, const char* format, const char* mapname)
	{
		if (!Utils::String::StartsWith(mapname, "mp_") && !Utils::String::StartsWith(mapname, "zm_"))
		{
			format = "maps/%s.d3dbsp";
		}

		// Redirect shipment to shipment long
		if (mapname == "mp_shipment"s)
		{
			mapname = "mp_shipment_long";
		}

		_snprintf_s(buffer, size, size, format, mapname);
	}

	void Maps::HandleAsSPMap()
	{
		Maps::SPMap = true;
	}

	void Maps::AddDependency(const std::string& expression, const std::string& zone)
	{
		// Test expression before adding it
		try
		{
			std::regex _(expression);
		}
		catch (const std::exception e)
		{
			MessageBoxA(nullptr, Utils::String::VA("Invalid regular expression: %s", expression.data()), "Warning", MB_ICONEXCLAMATION);
			return;
		}

		Maps::DependencyList.push_back({expression, zone});
	}

	int Maps::IgnoreEntityStub(const char* entity)
	{
		return (Utils::String::StartsWith(entity, "dyn_") || Utils::String::StartsWith(entity, "node_") || Utils::String::StartsWith(entity, "actor_"));
	}

	std::vector<std::string> Maps::GetDependenciesForMap(const std::string& map)
	{
		for (int i = 0; i < *Game::arenaCount; ++i)
		{
			Game::newMapArena_t* arena = &ArenaLength::NewArenas[i];
			if (arena->mapName == map)
			{
				for (int j = 0; j < ARRAYSIZE(arena->keys); ++j)
				{
					if (arena->keys[j] == "dependency"s)
					{
						return Utils::String::Explode(arena->values[j], ' ');
					}
				}
			}
		}

		return {};
	}

	std::pair<std::string, std::string> Maps::GetTeamsForMap(const std::string& map)
	{
		std::string team_axis = "opforce_composite";
		std::string team_allies = "us_army";

		for (int i = 0; i < *Game::arenaCount; ++i)
		{
			Game::newMapArena_t* arena = &ArenaLength::NewArenas[i];
			if (arena->mapName == map)
			{
				for (int j = 0; j < ARRAYSIZE(arena->keys); ++j)
				{
					if (arena->keys[j] == "allieschar"s)
					{
						team_allies = arena->values[j];
					}
					else if (arena->keys[j] == "axischar"s)
					{
						team_axis = arena->values[j];
					}
				}

				break;
			}
		}

		return {team_axis, team_allies};
	}

	void Maps::PrepareUsermap(const char* mapname)
	{
		if (Maps::UserMap.isValid())
		{
			Maps::UserMap.freeIwd();
			Maps::UserMap.clear();
		}

		if (Maps::IsUserMap(mapname))
		{
			Maps::UserMap = Maps::UserMapContainer(mapname);
			Maps::UserMap.loadIwd();
		}
		else
		{
			Maps::UserMap.clear();
		}
	}

	unsigned int Maps::GetUsermapHash(const std::string& map)
	{
		if (Utils::IO::DirectoryExists(Utils::String::VA("usermaps/%s", map.data())))
		{
			std::string hash;

			for(int i = 0; i < ARRAYSIZE(Maps::UserMapFiles); ++i)
			{
				std::string filePath = Utils::String::VA("usermaps/%s/%s%s", map.data(), map.data(), Maps::UserMapFiles[i]);
				if (Utils::IO::FileExists(filePath))
				{
					hash.append(Utils::Cryptography::SHA256::Compute(Utils::IO::ReadFile(filePath)));
				}
			}

			return Utils::Cryptography::JenkinsOneAtATime::Compute(hash);
		}

		return 0;
	}

	void Maps::LoadNewMapCommand(char* buffer, size_t size, const char* /*format*/, const char* mapname, const char* gametype)
	{
		unsigned int hash = Maps::GetUsermapHash(mapname);
		_snprintf_s(buffer, size, size, "loadingnewmap\n%s\n%s\n%d", mapname, gametype, hash);
	}

	int Maps::TriggerReconnectForMap(Game::msg_t* msg, const char* mapname)
	{
		Theatre::StopRecording();

		char hashBuf[100] = { 0 };
		unsigned int hash = atoi(Game::MSG_ReadStringLine(msg, hashBuf, sizeof hashBuf));

		if(!Maps::CheckMapInstalled(mapname, false, true) || hash && hash != Maps::GetUsermapHash(mapname))
		{
			// Reconnecting forces the client to download the new map
			Command::Execute("disconnect", false);
			Command::Execute("awaitDatabase", false); // Wait for the database to load
			Command::Execute("wait 100", false);
			Command::Execute("openmenu popup_reconnectingtoparty", false);
			Command::Execute("delayReconnect", false);
			return true;
		}

		return false;
	}

	__declspec(naked) void Maps::RotateCheckStub()
	{
		__asm
		{
			push eax
			pushad

			push [esp + 28h]
			push ebp
			call Maps::TriggerReconnectForMap
			add esp, 8h

			mov [esp + 20h], eax

			popad
			pop eax

			test eax, eax
			jnz skipRotation

			push 487C50h // Rotate map

		skipRotation:
			retn
		}
	}

	__declspec(naked) void Maps::SpawnServerStub()
	{
		__asm
		{
			pushad

			push [esp + 24h]
			call Maps::PrepareUsermap
			pop eax

			popad

			push 4A7120h // SV_SpawnServer
			retn
		}
	}

	__declspec(naked) void Maps::LoadMapLoadscreenStub()
	{
		__asm
		{
			pushad

			push [esp + 24h]
			call Maps::PrepareUsermap
			pop eax

			popad

			push 4D8030h // LoadMapLoadscreen
			retn
		}
	}

	void Maps::AddDlc(Maps::DLC dlc)
	{
		for (auto& pack : Maps::DlcPacks)
		{
			if (pack.index == dlc.index)
			{
				pack.maps = dlc.maps;
				Maps::UpdateDlcStatus();
				return;
			}
		}

		Dvar::Register<bool>(Utils::String::VA("isDlcInstalled_%d", dlc.index), false, Game::DVAR_FLAG_USERCREATED | Game::DVAR_FLAG_WRITEPROTECTED, "");

		Maps::DlcPacks.push_back(dlc);
		Maps::UpdateDlcStatus();
	}

	void Maps::UpdateDlcStatus()
	{
		bool hasAllDlcs = true;
		std::vector<bool> hasDlc;
		for (auto& pack : Maps::DlcPacks)
		{
			bool hasAllMaps = true;
			for (auto map : pack.maps)
			{
				if (!FastFiles::Exists(map))
				{
					hasAllMaps = false;
					hasAllDlcs = false;
					break;
				}
			}

			hasDlc.push_back(hasAllMaps);
			Dvar::Var(Utils::String::VA("isDlcInstalled_%d", pack.index)).setRaw(hasAllMaps ? 1 : 0);
		}

		// Must have all of dlc 3 to 5 or it causes issues
		static bool sentMessage = false;
		if (hasDlc.size() >= 5 && (hasDlc[2] || hasDlc[3] || hasDlc[4]) && (!hasDlc[2] || !hasDlc[3] || !hasDlc[4]) && !sentMessage)
		{
			StartupMessages::AddMessage("Warning:\n You only have some of DLCs 3-5 which are all required to be installed to work. There may be issues with those maps.");
			sentMessage = true;
		}

		Dvar::Var("isDlcInstalled_All").setRaw(hasAllDlcs ? 1 : 0);
	}

	bool Maps::IsCustomMap()
	{
		Game::GfxWorld*& gameWorld = *reinterpret_cast<Game::GfxWorld**>(0x66DEE94);
		if(gameWorld) return gameWorld->checksum == 0xDEADBEEF;
		return false;
	}

	bool Maps::IsUserMap(const std::string& mapname)
	{
		return Utils::IO::DirectoryExists(Utils::String::VA("usermaps/%s", mapname.data())) && Utils::IO::FileExists(Utils::String::VA("usermaps/%s/%s.ff", mapname.data(), mapname.data()));
	}

	Game::XAssetEntry* Maps::GetAssetEntryPool()
	{
		return *reinterpret_cast<Game::XAssetEntry**>(0x48E6F4);
	}

	// dlcIsTrue serves as a check if the map is a custom map and if it's missing
	bool Maps::CheckMapInstalled(const char* mapname, bool error, bool dlcIsTrue)
	{
		if (FastFiles::Exists(mapname)) return true;

		for (auto& pack : Maps::DlcPacks)
		{
			for (auto map : pack.maps)
			{
				if (map == std::string(mapname))
				{
					if (error)
					{
						Components::Logger::SoftError("Missing DLC pack %s (%d) containing map %s (%s).\nPlease download it to play this map.",
							pack.name.data(), pack.index, Game::UI_LocalizeMapName(mapname), mapname);
					}

					return dlcIsTrue;
				}
			}
		}

		if (error) Components::Logger::SoftError("Missing map file %s.\nYou may have a damaged installation or are attempting to load a non-existant map.", mapname);
		return false;
	}

	void Maps::HideModel()
	{
		Game::GfxWorld*& gameWorld = *reinterpret_cast<Game::GfxWorld**>(0x66DEE94);
		if (!gameWorld) return;

		std::string model = Dvar::Var("r_hideModel").get<std::string>();
		if (model.empty()) return;

		for (unsigned int i = 0; i < gameWorld->dpvs.smodelCount; ++i)
		{
			if (gameWorld->dpvs.smodelDrawInsts[i].model->name == model)
			{
				gameWorld->dpvs.smodelVisData[0][i] = 0;
				gameWorld->dpvs.smodelVisData[1][i] = 0;
				gameWorld->dpvs.smodelVisData[2][i] = 0;
			}
		}
	}

	__declspec(naked) void Maps::HideModelStub()
	{
		__asm
		{
			pushad
			call Maps::HideModel
			popad

			push 541E40h
			retn
		}
	}

	Game::dvar_t* Maps::GetDistortionDvar()
	{
		Game::dvar_t*& r_distortion = *reinterpret_cast<Game::dvar_t**>(0x69F0DCC);

		if(Maps::IsCustomMap())
		{
			static Game::dvar_t noDistortion;
			ZeroMemory(&noDistortion, sizeof noDistortion);
			return &noDistortion;
		}

		return r_distortion;
	}

	__declspec(naked) void Maps::SetDistortionStub()
	{
		__asm
		{
			push eax
			pushad
			call Maps::GetDistortionDvar

			mov [esp + 20h], eax
			popad

			pop eax
			retn
		}
	}

	Game::dvar_t* Maps::GetSpecularDvar()
	{
		Game::dvar_t*& r_specular = *reinterpret_cast<Game::dvar_t**>(0x69F0D94);
		static Game::dvar_t* r_specularCustomMaps = Game::Dvar_RegisterBool("r_specularCustomMaps", true, Game::DVAR_FLAG_SAVED, "Allows shaders to use phong specular lighting on custom maps");

		if (Maps::IsCustomMap())
		{
			if (!r_specularCustomMaps->current.enabled)
			{
				static Game::dvar_t noSpecular;
				ZeroMemory(&noSpecular, sizeof noSpecular);
				return &noSpecular;
			}
		}

		return r_specular;
	}

	__declspec(naked) void Maps::SetSpecularStub1()
	{
		__asm
		{
			push eax
			pushad
			call Maps::GetSpecularDvar

			mov [esp + 20h], eax
			popad

			pop eax
			retn
		}
	}

	__declspec(naked) void Maps::SetSpecularStub2()
	{
		__asm
		{
			push eax
			pushad
			call Maps::GetSpecularDvar

			mov [esp + 20h], eax
			popad

			pop edx
			retn
		}
	}

	void Maps::G_SpawnTurretHook(Game::gentity_s* ent, int unk, int unk2)
	{
		if (Maps::CurrentMainZone == "mp_backlot_sh"s || Maps::CurrentMainZone == "mp_con_spring"s || 
			Maps::CurrentMainZone == "mp_mogadishu_sh"s || Maps::CurrentMainZone == "mp_nightshift_sh"s)
		{
			return;
		}

		Utils::Hook::Call<void(Game::gentity_s*, int, int)>(0x408910)(ent, unk, unk2);
	}
	
	Maps::Maps()
	{
		Dvar::OnInit([]()
		{
			Dvar::Register<bool>("isDlcInstalled_All", false, Game::DVAR_FLAG_USERCREATED | Game::DVAR_FLAG_WRITEPROTECTED, "");
			Dvar::Register<bool>("r_listSModels", false, Game::DVAR_FLAG_NONE, "Display a list of visible SModels");

			Maps::AddDlc({ 1, "Stimulus Pack", {"mp_complex", "mp_compact", "mp_storm", "mp_overgrown", "mp_crash"} });
			Maps::AddDlc({ 2, "Resurgence Pack", {"mp_abandon", "mp_vacant", "mp_trailerpark", "mp_strike", "mp_fuel2"} });
			Maps::AddDlc({ 3, "Nuketown", {"mp_nuked"} });
			Maps::AddDlc({ 4, "Classics Pack #1", {"mp_cross_fire", "mp_cargoship", "mp_bloc"} });
			Maps::AddDlc({ 5, "Classics Pack #2", {"mp_killhouse", "mp_bog_sh"} });
			Maps::AddDlc({ 6, "Freighter", {"mp_cargoship_sh"} });
			Maps::AddDlc({ 7, "Resurrection Pack", {"mp_shipment_long", "mp_rust_long", "mp_firingrange"} });
			Maps::AddDlc({ 8, "Recycled Pack", {"mp_bloc_sh", "mp_crash_tropical", "mp_estate_tropical", "mp_fav_tropical", "mp_storm_spring"} });

			Maps::UpdateDlcStatus();

			UIScript::Add("downloadDLC", [](UIScript::Token token)
			{
				int dlc = token.get<int>();

				for (auto pack : Maps::DlcPacks)
				{
					if (pack.index == dlc)
					{
						News::LaunchUpdater(Utils::String::VA("-dlc %i -c", pack.index));
						//ShellExecuteA(nullptr, "open", pack.url.data(), nullptr, nullptr, SW_SHOWNORMAL);
						return;
					}
				}

				Game::ShowMessageBox(Utils::String::VA("DLC %d does not exist!", dlc), "ERROR");
			});
		});

		// disable turrets on CoD:OL 448+ maps for now
		Utils::Hook(0x5EE577, Maps::G_SpawnTurretHook, HOOK_CALL).install()->quick();
		Utils::Hook(0x44A4D5, Maps::G_SpawnTurretHook, HOOK_CALL).install()->quick();
		
//#define SORT_SMODELS
#if !defined(DEBUG) || !defined(SORT_SMODELS)
		// Don't sort static models
		Utils::Hook::Nop(0x53D815, 2);
#endif

		// Don't draw smodels
		//Utils::Hook::Set<BYTE>(0x541E40, 0xC3);

		// Restrict asset loading
		AssetHandler::OnLoad(Maps::LoadAssetRestrict);

		// hunk size (was 300 MiB)
		Utils::Hook::Set<DWORD>(0x64A029, 0x1C200000); // 450 MiB
		Utils::Hook::Set<DWORD>(0x64A057, 0x1C200000);

#if DEBUG
		// Hunk debugging
		Utils::Hook::Set<BYTE>(0x4FF57B, 0xCC);
		Utils::Hook::Nop(0x4FF57C, 4);
#else
		// Temporarily disable distortion warnings
		Utils::Hook::Nop(0x50DBFF, 5);
		Utils::Hook::Nop(0x50DC4F, 5);
		Utils::Hook::Nop(0x50DCA3, 5);
		Utils::Hook::Nop(0x50DCFE, 5);
#endif

		// Intercept BSP name resolving
		Utils::Hook(0x4C5979, Maps::GetBSPName, HOOK_CALL).install()->quick();

		// Intercept map zone loading/unloading
		Utils::Hook(0x42C2AF, Maps::LoadMapZones, HOOK_CALL).install()->quick();
		Utils::Hook(0x60B477, Maps::UnloadMapZones, HOOK_CALL).install()->quick();

		// Ignore SP entities
		Utils::Hook(0x444810, Maps::IgnoreEntityStub, HOOK_JUMP).install()->quick();

		// WorldData pointer replacement
		Utils::Hook(0x4D90B6, Maps::GetWorldDataStub, HOOK_CALL).install()->quick();

		// Allow loading raw suns
		Utils::Hook(0x51B46A, Maps::LoadRawSun, HOOK_CALL).install()->quick();

		// Disable distortion on custom maps
		//Utils::Hook(0x50AA47, Maps::SetDistortionStub, HOOK_CALL).install()->quick();

		// Disable speculars on custom maps
		Utils::Hook(0x525EA6, Maps::SetSpecularStub1, HOOK_CALL).install()->quick();
		Utils::Hook(0x51FBC7, Maps::SetSpecularStub2, HOOK_CALL).install()->quick();
		Utils::Hook(0x522A2E, Maps::SetSpecularStub2, HOOK_CALL).install()->quick();
		Utils::Hook::Nop(0x51FBCC, 1);
		Utils::Hook::Nop(0x522A33, 1);

		// Intercept map loading for usermap initialization
		Utils::Hook(0x6245E3, Maps::SpawnServerStub, HOOK_CALL).install()->quick();
		Utils::Hook(0x62493E, Maps::SpawnServerStub, HOOK_CALL).install()->quick();
		Utils::Hook(0x42CF58, Maps::LoadMapLoadscreenStub, HOOK_CALL).install()->quick();
		Utils::Hook(0x487CDD, Maps::LoadMapLoadscreenStub, HOOK_CALL).install()->quick();
		Utils::Hook(0x4CA3E9, Maps::LoadMapLoadscreenStub, HOOK_CALL).install()->quick();
		Utils::Hook(0x5A9D51, Maps::LoadMapLoadscreenStub, HOOK_CALL).install()->quick();
		Utils::Hook(0x5B34DD, Maps::LoadMapLoadscreenStub, HOOK_CALL).install()->quick();

		Command::Add("delayReconnect", [](Command::Params*)
		{
			Scheduler::OnDelay([]()
			{
				Command::Execute("closemenu popup_reconnectingtoparty", false);
				Command::Execute("reconnect", false);
			}, 10s, true);
		});

		if(Dedicated::IsEnabled())
		{
			Utils::Hook(0x4A7251, Maps::LoadNewMapCommand, HOOK_CALL).install()->quick();
		}

		// Download the map before a maprotation if necessary
		// Conflicts with Theater's SV map rotation check, but this one is safer!
		Utils::Hook(0x5AA91C, Maps::RotateCheckStub, HOOK_CALL).install()->quick();

		// Load usermap arena file
		Utils::Hook(0x630A88, Maps::LoadArenaFileStub, HOOK_CALL).install()->quick();

		// Dependencies
		//Maps::AddDependency("oilrig", "mp_subbase");
		//Maps::AddDependency("gulag", "mp_subbase");
		//Maps::AddDependency("invasion", "mp_rust");
		//Maps::AddDependency("co_hunted", "mp_storm");
		//Maps::AddDependency("mp_shipment", "mp_shipment_long");

		// Allow hiding specific smodels
		Utils::Hook(0x50E67C, Maps::HideModelStub, HOOK_CALL).install()->quick();

		Scheduler::OnFrame([]()
		{
			Game::GfxWorld*& gameWorld = *reinterpret_cast<Game::GfxWorld**>(0x66DEE94);
			if (!Game::CL_IsCgameInitialized() || !gameWorld || !Dvar::Var("r_listSModels").get<bool>()) return;

			std::map<std::string, int> models;
			for (unsigned int i = 0; i < gameWorld->dpvs.smodelCount; ++i)
			{
				if (gameWorld->dpvs.smodelVisData[0][i])
				{
					std::string name = gameWorld->dpvs.smodelDrawInsts[i].model->name;

					if (models.find(name) == models.end()) models[name] = 1;
					else models[name]++;
				}
			}

			Game::Font_s* font = Game::R_RegisterFont("fonts/smallFont", 0);
			int height = Game::R_TextHeight(font);
			float scale = 0.75;
			float color[4] = { 0, 1.0f, 0, 1.0f };

			unsigned int i = 0;
			for (auto& model : models)
			{
				Game::R_AddCmdDrawText(Utils::String::VA("%d %s", model.second, model.first.data()), 0x7FFFFFFF, font, 15.0f, (height * scale + 1) * (i++ + 1) + 15.0f, scale, scale, 0.0f, color, Game::ITEM_TEXTSTYLE_NORMAL);
			}
		}, true);
	}

	Maps::~Maps()
	{
		Maps::DlcPacks.clear();
		Maps::DependencyList.clear();
		Maps::CurrentMainZone.clear();
		Maps::CurrentDependencies.clear();
	}
}
