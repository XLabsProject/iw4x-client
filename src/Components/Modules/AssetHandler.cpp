#include "STDInclude.hpp"

namespace Components
{
	thread_local int AssetHandler::BypassState = 0;
    bool AssetHandler::ShouldSearchTempAssets = false;
	std::map<Game::XAssetType, AssetHandler::IAsset*> AssetHandler::AssetInterfaces;
	std::map<Game::XAssetType, Utils::Slot<AssetHandler::Callback>> AssetHandler::TypeCallbacks;
	Utils::Signal<AssetHandler::RestrictCallback> AssetHandler::RestrictSignal;

	std::map<void*, void*> AssetHandler::Relocations;

	std::vector<std::pair<Game::XAssetType, std::string>> AssetHandler::EmptyAssets;

	std::map<std::string, Game::XAssetHeader> AssetHandler::TemporaryAssets[Game::XAssetType::ASSET_TYPE_COUNT];

	void AssetHandler::RegisterInterface(IAsset* iAsset)
	{
		if (!iAsset) return;
		if (iAsset->getType() == Game::XAssetType::ASSET_TYPE_INVALID)
		{
			delete iAsset;
			return;
		}

		if (AssetHandler::AssetInterfaces.find(iAsset->getType()) != AssetHandler::AssetInterfaces.end())
		{
			Logger::Print("Duplicate asset interface: %s\n", Game::DB_GetXAssetTypeName(iAsset->getType()));
			delete AssetHandler::AssetInterfaces[iAsset->getType()];
		}
		else
		{
			Logger::Print("Asset interface registered: %s\n", Game::DB_GetXAssetTypeName(iAsset->getType()));
		}

		AssetHandler::AssetInterfaces[iAsset->getType()] = iAsset;
	}

	void AssetHandler::ClearTemporaryAssets()
	{
		for (int i = 0; i < Game::XAssetType::ASSET_TYPE_COUNT; ++i)
		{
			AssetHandler::TemporaryAssets[i].clear();
		}
	}

	void AssetHandler::StoreTemporaryAsset(Game::XAssetType type, Game::XAssetHeader asset)
	{
		AssetHandler::TemporaryAssets[type][Game::DB_GetXAssetNameHandlers[type](&asset)] = asset;
	}

	Game::XAssetHeader AssetHandler::FindAsset(Game::XAssetType type, const char* filename)
	{
		Game::XAssetHeader header = { nullptr };

		if (filename)
		{
			// Allow call DB_FindXAssetHeader within the hook
			AssetHandler::SetBypassState(true);

			if (AssetHandler::TypeCallbacks.find(type) != AssetHandler::TypeCallbacks.end())
			{
				header = AssetHandler::TypeCallbacks[type](type, filename);
			}

			// Disallow calling DB_FindXAssetHeader ;)
			AssetHandler::SetBypassState(false);
		}

		return header;
	}

    Game::XAssetHeader AssetHandler::FindTemporaryAsset(Game::XAssetType type, const char* filename)
    {
        Game::XAssetHeader header = { nullptr };
        if (type >= Game::XAssetType::ASSET_TYPE_COUNT) return header;

        auto tempPool = &AssetHandler::TemporaryAssets[type];
        auto entry = tempPool->find(filename);
        if (entry != tempPool->end())
        {
            header = { entry->second };
        }

        return header;
    }    

	int AssetHandler::HasThreadBypass()
	{
		return AssetHandler::BypassState > 0;
	}

	void AssetHandler::SetBypassState(bool value)
	{
		AssetHandler::BypassState += (value ? 1 : -1);
		if (AssetHandler::BypassState < 0)
		{
			AssetHandler::BypassState = 0;
			Logger::Error("Bypass state is below 0!");
		}
	}

	void AssetHandler::ResetBypassState()
	{
		if (AssetHandler::HasThreadBypass())
		{
			// Maybe just decrement it?
			AssetHandler::BypassState = 0;
		}
	}

	__declspec(naked) void AssetHandler::FindAssetStub()
	{
		__asm
		{
			push ecx
			push ebx
			push ebp
			push esi
			push edi


			push eax
			pushad

			// Check if custom handler should be bypassed
			call AssetHandler::HasThreadBypass

			mov [esp + 20h], eax
			popad
			pop eax


			test al, al
			jnz checkTempAssets

			mov ecx, [esp + 18h] // Asset type
			mov ebx, [esp + 1Ch] // Filename


			push eax
			pushad

			push ebx
			push ecx

			call AssetHandler::FindAsset

			add esp, 8h

			mov [esp + 20h], eax
			popad
			pop eax


			test eax, eax
			jnz finishFound
            
        checkTempAssets:
            mov al, AssetHandler::ShouldSearchTempAssets // check to see if enabled
            test eax, eax
            jz finishOriginal

            mov ecx, [esp + 18h] // Asset type
            mov ebx, [esp + 1Ch] // Filename

            push ebx
            push ecx

            call AssetHandler::FindTemporaryAsset

            add esp, 8h

            test eax, eax
            jnz finishFound
            
        finishOriginal:
			// Asset not found using custom handlers or in temp assets or bypasses were enabled
            // redirect to DB_FindXAssetHeader
			mov ebx, ds:6D7190h // InterlockedDecrement
			mov eax, 40793Bh
			jmp eax

		finishFound:
			pop edi
			pop esi
			pop ebp
			pop ebx
			pop ecx
			retn
		}
	}

	void AssetHandler::ModifyAsset(Game::XAssetType type, Game::XAssetHeader asset, const std::string& name)
	{
		if (type == Game::XAssetType::ASSET_TYPE_MATERIAL && (name == "gfx_distortion_knife_trail" || name == "gfx_distortion_heat_far" || name == "gfx_distortion_ring_light" || name == "gfx_distortion_heat") && asset.material->info.sortKey >= 43)
		{
			if (Zones::Version() >= VERSION_ALPHA2)
			{
				asset.material->info.sortKey = 44;
			}
			else
			{
				asset.material->info.sortKey = 43;
			}
		}

		if (type == Game::XAssetType::ASSET_TYPE_MATERIAL && (name == "wc/codo_ui_viewer_black_decal3" || name == "wc/codo_ui_viewer_black_decal2" || name == "wc/hint_arrows01" || name == "wc/hint_arrows02"))
		{
			asset.material->info.sortKey = 0xE;
		}

		if (type == Game::XAssetType::ASSET_TYPE_VEHICLE && Zones::Version() >= VERSION_ALPHA2)
		{
			asset.vehDef->turretWeapon = nullptr;
		}

		// Fix shader const stuff
		if (type == Game::XAssetType::ASSET_TYPE_TECHNIQUE_SET){
			if (Zones::Version() >= 359 && Zones::Version() < 448)
			{
				for (int i = 0; i < 48; ++i)
				{
					if (asset.techniqueSet->techniques[i])
					{
						for (int j = 0; j < asset.techniqueSet->techniques[i]->passCount; ++j)
						{
							Game::MaterialPass* pass = &asset.techniqueSet->techniques[i]->passArray[j];

							for (int k = 0; k < (pass->perPrimArgCount + pass->perObjArgCount + pass->stableArgCount); ++k)
							{
								if (pass->args[k].type == D3DSHADER_PARAM_REGISTER_TYPE::D3DSPR_CONSTINT)
								{
									if (pass->args[k].u.codeConst.index == -28132)
									{
										pass->args[k].u.codeConst.index = 2644;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	bool AssetHandler::IsAssetEligible(Game::XAssetType type, Game::XAssetHeader *asset)
	{
		const char* name = Game::DB_GetXAssetNameHandlers[type](asset);
		if (!name) return false;

		for (auto i = AssetHandler::EmptyAssets.begin(); i != AssetHandler::EmptyAssets.end();)
		{
			if (i->first == type && i->second == name)
			{
				i = AssetHandler::EmptyAssets.erase(i);
			}
			else
			{
				++i;
			}
		}

		if (Flags::HasFlag("entries"))
		{
			Components::Logger::Print(Utils::String::VA("%s: %d: %s\n", FastFiles::Current().data(), type, name));
		}

		bool restrict = false;
		AssetHandler::RestrictSignal(type, *asset, name, &restrict);

		if (!restrict)
		{
			AssetHandler::ModifyAsset(type, *asset, name);
		}

		// If no slot restricts the loading, we can load the asset
		return (!restrict);
	}

	__declspec(naked) void AssetHandler::AddAssetStub()
	{
		__asm
		{
			push eax
			pushad

			push [esp + 2Ch]
			push [esp + 2Ch]
			call AssetHandler::IsAssetEligible
			add esp, 8h

			mov [esp + 20h], eax
			popad
			pop eax

			test al, al
			jz doNotLoad

			mov eax, [esp + 8h]
			sub esp, 14h
			mov ecx, 5BB657h
			jmp ecx

		doNotLoad:
			mov eax, [esp + 8h]
			retn
		}
	}

	void AssetHandler::OnFind(Game::XAssetType type, Utils::Slot<AssetHandler::Callback> callback)
	{
		AssetHandler::TypeCallbacks[type] = callback;
	}

	void AssetHandler::OnLoad(Utils::Slot<AssetHandler::RestrictCallback> callback)
	{
		AssetHandler::RestrictSignal.connect(callback);
	}

	void AssetHandler::ClearRelocations()
	{
		AssetHandler::Relocations.clear();
	}

	void AssetHandler::Relocate(void* start, void* to, DWORD size)
	{
		for (DWORD i = 0; i < size; i += 4)
		{
			AssetHandler::Relocations[reinterpret_cast<char*>(start) + i] = reinterpret_cast<char*>(to) + i;
		}
	}

	void AssetHandler::OffsetToAlias(Utils::Stream::Offset* offset)
	{
		void* pointer = (*Game::g_streamBlocks)[offset->getUnpackedBlock()].data + offset->getUnpackedOffset();

		if (AssetHandler::Relocations.find(pointer) != AssetHandler::Relocations.end())
		{
			pointer = AssetHandler::Relocations[pointer];
		}

		offset->pointer = *reinterpret_cast<void**>(pointer);
	}

	void AssetHandler::ZoneSave(Game::XAsset asset, ZoneBuilder::Zone* builder)
	{
		if (AssetHandler::AssetInterfaces.find(asset.type) != AssetHandler::AssetInterfaces.end())
		{
			AssetHandler::AssetInterfaces[asset.type]->save(asset.header, builder);
		}
		else
		{
			Logger::Error("No interface for type '%s'!", Game::DB_GetXAssetTypeName(asset.type));
		}
	}

	void AssetHandler::ZoneMark(Game::XAsset asset, ZoneBuilder::Zone* builder)
	{
		if (AssetHandler::AssetInterfaces.find(asset.type) != AssetHandler::AssetInterfaces.end())
		{
			AssetHandler::AssetInterfaces[asset.type]->mark(asset.header, builder);
		}
		else
		{
			Logger::Error("No interface for type '%s'!", Game::DB_GetXAssetTypeName(asset.type));
		}
	}

	Game::XAssetHeader AssetHandler::FindAssetForZone(Game::XAssetType type, const std::string& filename, ZoneBuilder::Zone* builder, bool isSubAsset)
	{
		ZoneBuilder::Zone::AssetRecursionMarker _(builder);

		Game::XAssetHeader header = { nullptr };
		if (type >= Game::XAssetType::ASSET_TYPE_COUNT) return header;

		auto tempPool = &AssetHandler::TemporaryAssets[type];
		auto entry = tempPool->find(filename);
		if (entry != tempPool->end())
		{
			return { entry->second };
		}

		if (AssetHandler::AssetInterfaces.find(type) != AssetHandler::AssetInterfaces.end())
		{
			AssetHandler::AssetInterfaces[type]->load(&header, filename, builder);

			if (header.data)
			{
				Components::AssetHandler::StoreTemporaryAsset(type, header);
			}
		}

		if (!header.data && isSubAsset)
		{
			header = ZoneBuilder::GetEmptyAssetIfCommon(type, filename, builder);
		}

		if (!header.data)
		{
			header = Game::DB_FindXAssetHeader(type, filename.data());
			if (header.data) Components::AssetHandler::StoreTemporaryAsset(type, header); // Might increase efficiency...
		}

		return header;
	}

	Game::XAssetHeader AssetHandler::FindOriginalAsset(Game::XAssetType type, const char* filename)
	{
		AssetHandler::SetBypassState(true);
		Game::XAssetHeader header = Game::DB_FindXAssetHeader(type, filename);
		AssetHandler::SetBypassState(false);

		return header;
	}

	void AssetHandler::StoreEmptyAsset(Game::XAssetType type, const char* name)
	{
		AssetHandler::EmptyAssets.push_back({ type, name });
	}

	__declspec(naked) void AssetHandler::StoreEmptyAssetStub()
	{
		__asm
		{
			pushad
			push ebx
			push eax

			call AssetHandler::StoreEmptyAsset

			pop eax
			pop ebx
			popad

			push 5BB290h
			retn
		}
	}

	void AssetHandler::MissingAssetError(int severity, const char* format, const char* type, const char* name)
	{
		if (Dedicated::IsEnabled() && Game::DB_GetXAssetNameType(type) == Game::XAssetType::ASSET_TYPE_TECHNIQUE_SET) return;
		Utils::Hook::Call<void(int, const char*, const char*, const char*)>(0x4F8C70)(severity, format, type, name); // Print error
	}

	void AssetHandler::reallocateEntryPool()
	{
		AssertSize(Game::XAssetEntry, 16);

		size_t size = (ZoneBuilder::IsEnabled() ? 1183968 : 789312);
		Game::XAssetEntry* entryPool = Utils::Memory::GetAllocator()->allocateArray<Game::XAssetEntry>(size);

		// Apply new size
		Utils::Hook::Set<DWORD>(0x5BAEB0, size);

		// Apply new pool
		DWORD patches[] =
		{
			0x48E6F4,
			0x4C67E4,
			0x4C8584,
			0x5BAEA8,
			0x5BB0C4,
			0x5BB0F5,
			0x5BB1D4,
			0x5BB235,
			0x5BB278,
			0x5BB34C,
			0x5BB484,
			0x5BB570,
			0x5BB6B7,
			0x5BB844,
			0x5BB98D,
			0x5BBA66,
			0x5BBB8D,
			0x5BBCB1,
			0x5BBD9B,
			0x5BBE4C,
			0x5BBF14,
			0x5BBF54,
			0x5BBFB8
		};

		for (int i = 0; i < ARRAYSIZE(patches); ++i)
		{
			Utils::Hook::Set<Game::XAssetEntry*>(patches[i], entryPool);
		}

		Utils::Hook::Set<Game::XAssetEntry*>(0x5BAE91, entryPool + 1);
		Utils::Hook::Set<Game::XAssetEntry*>(0x5BAEA2, entryPool + 1);
	}

    void AssetHandler::ExposeTemporaryAssets(bool expose)
    {
        AssetHandler::ShouldSearchTempAssets = expose;
    }

	AssetHandler::AssetHandler()
	{
		this->reallocateEntryPool();

		Dvar::Register<bool>("r_noVoid", false, Game::DVAR_FLAG_SAVED, "Disable void model (red fx)");

		AssetHandler::ClearTemporaryAssets();

		// DB_FindXAssetHeader
		Utils::Hook(Game::DB_FindXAssetHeader, AssetHandler::FindAssetStub).install()->quick();

		// DB_ConvertOffsetToAlias
		Utils::Hook(0x4FDFA0, AssetHandler::OffsetToAlias, HOOK_JUMP).install()->quick();

		// DB_AddXAsset
		Utils::Hook(0x5BB650, AssetHandler::AddAssetStub, HOOK_JUMP).install()->quick();

		// Store empty assets
		Utils::Hook(0x5BB6EC, AssetHandler::StoreEmptyAssetStub, HOOK_CALL).install()->quick();

		// Intercept missing asset messages
		if (!ZoneBuilder::IsEnabled()) Utils::Hook(0x5BB3F2, AssetHandler::MissingAssetError, HOOK_CALL).install()->quick();

		// Log missing empty assets
		Scheduler::OnFrame([]()
		{
			if (FastFiles::Ready() && !AssetHandler::EmptyAssets.empty())
			{
				for (auto& asset : AssetHandler::EmptyAssets)
				{
					Game::Sys_Error(25, reinterpret_cast<char*>(0x724428), Game::DB_GetXAssetTypeName(asset.first), asset.second.data());
				}

				AssetHandler::EmptyAssets.clear();
			}
		});

		AssetHandler::OnLoad([](Game::XAssetType type, Game::XAssetHeader asset, std::string name, bool*)
		{
#ifdef DEBUG
// #define DUMP_TECHSETS
#ifdef DUMP_TECHSETS
			if (type == Game::XAssetType::ASSET_TYPE_VERTEXDECL && !name.empty() && name[0] != ',')
			{
				std::filesystem::create_directories("techsets/vertexdecl");

				auto vertexdecl = asset.vertexDecl;

				std::vector<json11::Json> routingData;
				for (int i = 0; i < vertexdecl->streamCount; i++)
				{
					routingData.push_back(json11::Json::object
						{
							{ "source", (int)vertexdecl->routing.data[i].source },
							{ "dest", (int)vertexdecl->routing.data[i].dest },
						});
				}

				std::vector<json11::Json> declData;
				for (int i = 0; i < 16; i++)
				{
					if (vertexdecl->routing.decl[i])
					{
						routingData.push_back(int(vertexdecl->routing.decl[i]));
					}
					else
					{
						routingData.push_back(nullptr);
					}
				}

				json11::Json vertexData = json11::Json::object
				{
					{ "name", vertexdecl->name },
					{ "streamCount", vertexdecl->streamCount },
					{ "hasOptionalSource", vertexdecl->hasOptionalSource },
					{ "routing", routingData },
					{ "decl", declData },
				};

				auto stringData = vertexData.dump();

				auto fp = fopen(Utils::String::VA("techsets/vertexdecl/%s.%s.json", vertexdecl->name, Zones::Version() > 276 ? "codo" : "iw4"), "wb");
				fwrite(&stringData[0], stringData.size(), 1, fp);
				fclose(fp);
			}

			if (type == Game::ASSET_TYPE_TECHNIQUE_SET && !name.empty() && name[0] != ',')
			{
				std::filesystem::create_directory("techsets");
				std::filesystem::create_directories("techsets/techniques");
				
				auto techset = asset.techniqueSet;

				std::vector<json11::Json> techniques;
				for (int technique = 0; technique < 48; technique++)
				{
					auto curTech = techset->techniques[technique];
					if (curTech)
					{
						std::vector<json11::Json> passDataArray;
						for (int pass = 0; pass < curTech->passCount; pass++)
						{
							auto curPass = &curTech->passArray[pass];

							std::vector<json11::Json> argDataArray;
							for (int arg = 0; arg < curPass->perObjArgCount + curPass->perPrimArgCount + curPass->stableArgCount; arg++)
							{
								auto curArg = &curPass->args[arg];

								if (curArg->type == 1 || curArg->type == 7)
								{
									std::vector<float> literalConsts;
									if (curArg->u.literalConst != 0)
									{
										literalConsts.push_back(curArg->u.literalConst[0]);
										literalConsts.push_back(curArg->u.literalConst[1]);
										literalConsts.push_back(curArg->u.literalConst[2]);
										literalConsts.push_back(curArg->u.literalConst[3]);
									}

									json11::Json argData = json11::Json::object
									{
										{ "type", curArg->type },
										{ "value", literalConsts },
									};
									argDataArray.push_back(argData);
								}
								else if (curArg->type == 3 || curArg->type == 5)
								{
									json11::Json argData = json11::Json::object
									{
										{ "type", curArg->type },
										{ "firstRow", curArg->u.codeConst.firstRow },
										{ "rowCount", curArg->u.codeConst.rowCount },
										{ "index", curArg->u.codeConst.index },
									};
									argDataArray.push_back(argData);
								}
								else
								{
									json11::Json argData = json11::Json::object
									{
										{ "type", curArg->type },
										{ "value", static_cast<int>(curArg->u.codeSampler) },
									};
									argDataArray.push_back(argData);
								}
							}

							json11::Json passData = json11::Json::object
							{
								{ "perObjArgCount", curPass->perObjArgCount },
								{ "perPrimArgCount", curPass->perPrimArgCount },
								{ "stableArgCount", curPass->stableArgCount },
								{ "args", argDataArray },
								{ "pixelShader", curPass->pixelShader ? curPass->pixelShader->name : "" },
								{ "vertexShader", curPass->vertexShader ? curPass->vertexShader->name : "" },
								{ "vertexDecl", curPass->vertexDecl ? curPass->vertexDecl->name : "" },
							};
							passDataArray.push_back(passData);
						}

						json11::Json techData = json11::Json::object
						{
							{ "name", curTech->name },
							{ "index", technique },
							{ "flags", curTech->flags },
							{ "numPasses", curTech->passCount },
							{ "pass", passDataArray },
						};

						auto stringData = techData.dump();
						
						auto fp = fopen(Utils::String::VA("techsets/techniques/%s.%s.json", curTech->name, Zones::Version() > 276 ? "codo" : "iw4"), "wb");
						fwrite(&stringData[0], stringData.size(), 1, fp);
						fclose(fp);
						
						json11::Json techsetTechnique = json11::Json::object
						{
							{ "name", curTech->name },
							{ "index", technique },
						};
						techniques.push_back(techsetTechnique);
					}
					else
					{
						techniques.push_back(nullptr);
					}
				}

				json11::Json techsetData = json11::Json::object
				{
					{ "name", techset->name },
					{ "techniques", techniques },
				};

				auto stringData = techsetData.dump();

				auto fp = fopen(Utils::String::VA("techsets/%s.%s.json", techset->name, Zones::Version() > 276 ? "codo" : "iw4"), "wb");
				fwrite(&stringData[0], stringData.size(), 1, fp);
				fclose(fp);
			}
#endif

			if (type == Game::XAssetType::ASSET_TYPE_TECHNIQUE_SET && Zones::Version() >= 460)
			{
				auto techset = asset.techniqueSet;
				if (techset)
				{
					for (int t = 0; t < 48; t++)
					{
						if (techset->techniques[t])
						{
							for (int p = 0; p < techset->techniques[t]->passCount; p++)
							{
								for (int a = 0; a < techset->techniques[t]->passArray[p].perObjArgCount +
									techset->techniques[t]->passArray[p].perPrimArgCount +
									techset->techniques[t]->passArray[p].stableArgCount; a++)
								{
									auto arg = &techset->techniques[t]->passArray[p].args[a];
									if (arg->type == 3 || arg->type == 5)
									{
										if (arg->u.codeConst.index > 140)
										{
											OutputDebugStringA(Utils::String::VA("codeConst %i is out of range for %s::%s[%i]\n", arg->u.codeConst.index,
												techset->name, techset->techniques[t]->name, p));

											if (!ZoneBuilder::IsEnabled())
											{
												__debugbreak();
											}
										}
									}
								}
							}
						}
					}
				}
			}
			
#endif
			
			if (Dvar::Var("r_noVoid").get<bool>() && type == Game::XAssetType::ASSET_TYPE_XMODEL && name == "void")
			{
				asset.model->numLods = 0;
			}
		});

		Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_GAMEWORLD_SP, 1);
		Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_IMAGE, ZoneBuilder::IsEnabled() ? 14336 * 2 : 7168);
		Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_LOADED_SOUND, 2700 * 2);
		Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_FX, 1200 * 2);
		Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_LOCALIZE_ENTRY, 14000);
		Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_XANIMPARTS, 8192 * 2);
		Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_XMODEL, 5125 * 2);
		Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_PHYSPRESET, 128);
		Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_PIXELSHADER, ZoneBuilder::IsEnabled() ? 0x4000 : 10000);
		Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_VERTEXSHADER, ZoneBuilder::IsEnabled() ? 0x2000 : 3072);
		Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_MATERIAL, 8192 * 2);
		Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_VERTEXDECL, ZoneBuilder::IsEnabled() ? 0x400 : 196);
		Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_WEAPON, WEAPON_LIMIT);
		Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_STRINGTABLE, 800);
		Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_IMPACT_FX, 8);

		// Register asset interfaces
		if (ZoneBuilder::IsEnabled())
		{
			Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_MAP_ENTS, 10);
			Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_XMODEL_SURFS, 8192 * 2);
			Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_TECHNIQUE_SET, 0x2000);
			Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_FONT, 32);
			Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_RAWFILE, 2048);
			Game::ReallocateAssetPool(Game::XAssetType::ASSET_TYPE_LEADERBOARD, 500);

			AssetHandler::RegisterInterface(new Assets::IFont_s());
            AssetHandler::RegisterInterface(new Assets::IWeapon());
			AssetHandler::RegisterInterface(new Assets::IXModel());
			AssetHandler::RegisterInterface(new Assets::IFxWorld());
			AssetHandler::RegisterInterface(new Assets::IMapEnts());
			AssetHandler::RegisterInterface(new Assets::IRawFile());
			AssetHandler::RegisterInterface(new Assets::IComWorld());
			AssetHandler::RegisterInterface(new Assets::IGfxImage());
			AssetHandler::RegisterInterface(new Assets::IGfxWorld());
			AssetHandler::RegisterInterface(new Assets::ISndCurve());
			AssetHandler::RegisterInterface(new Assets::IMaterial());
			AssetHandler::RegisterInterface(new Assets::IMenuList());
            AssetHandler::RegisterInterface(new Assets::IclipMap_t());
			AssetHandler::RegisterInterface(new Assets::ImenuDef_t());
            AssetHandler::RegisterInterface(new Assets::ITracerDef());
			AssetHandler::RegisterInterface(new Assets::IPhysPreset());
			AssetHandler::RegisterInterface(new Assets::IXAnimParts());
			AssetHandler::RegisterInterface(new Assets::IFxEffectDef());
			AssetHandler::RegisterInterface(new Assets::IGameWorldMp());
			AssetHandler::RegisterInterface(new Assets::IGameWorldSp());
			AssetHandler::RegisterInterface(new Assets::IGfxLightDef());
			AssetHandler::RegisterInterface(new Assets::ILoadedSound());
			AssetHandler::RegisterInterface(new Assets::IPhysCollmap());
			AssetHandler::RegisterInterface(new Assets::IStringTable());
			AssetHandler::RegisterInterface(new Assets::IXModelSurfs());
			AssetHandler::RegisterInterface(new Assets::ILocalizeEntry());
			AssetHandler::RegisterInterface(new Assets::Isnd_alias_list_t());
			AssetHandler::RegisterInterface(new Assets::IMaterialPixelShader());
			AssetHandler::RegisterInterface(new Assets::IMaterialTechniqueSet());
			AssetHandler::RegisterInterface(new Assets::IMaterialVertexShader());
			AssetHandler::RegisterInterface(new Assets::IStructuredDataDefSet());
			AssetHandler::RegisterInterface(new Assets::IMaterialVertexDeclaration());
		}
	}

	AssetHandler::~AssetHandler()
	{
		AssetHandler::ClearTemporaryAssets();

		for (auto i = AssetHandler::AssetInterfaces.begin(); i != AssetHandler::AssetInterfaces.end(); ++i)
		{
			delete i->second;
		}

		AssetHandler::Relocations.clear();
		AssetHandler::AssetInterfaces.clear();
		AssetHandler::RestrictSignal.clear();
		AssetHandler::TypeCallbacks.clear();
	}
}
