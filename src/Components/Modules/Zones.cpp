#include "STDInclude.hpp"

#pragma optimize( "", off )
namespace Components
{
	int Zones::ZoneVersion;
	int Zones::EntitiesVersion;

	int Zones::FxEffectIndex;
	char* Zones::FxEffectStrings[64];

	static std::unordered_map<std::string, std::string> shellshock_replace_list = {
		{ "66","bg_shock_screenType" },
		{ "67","bg_shock_screenBlurBlendTime"},
		{ "68","bg_shock_screenBlurBlendFadeTime"},
		{ "69","bg_shock_screenFlashWhiteFadeTime"},
		{ "70","bg_shock_screenFlashShotFadeTime"},
		{ "71","bg_shock_viewKickPeriod"},
		{ "72","bg_shock_viewKickRadius"},
		{ "73","bg_shock_viewKickFadeTime"},
		{ "78","bg_shock_sound"},
		{ "74","bg_shock_soundLoop"},
		{ "75","bg_shock_soundLoopSilent"},
		{ "76","bg_shock_soundEnd"},
		{ "77","bg_shock_soundEndAbort"},
		{ "79","bg_shock_soundFadeInTime"},
		{ "80","bg_shock_soundFadeOutTime"},
		{ "81","bg_shock_soundLoopFadeTime"},
		{ "82","bg_shock_soundLoopEndDelay"},
		{ "83","bg_shock_soundRoomType"},
		{ "84","bg_shock_soundDryLevel"},
		{ "85","bg_shock_soundWetLevel"},
		{ "86","bg_shock_soundModEndDelay"},

		// guessed, not sure
		{ "87","bg_shock_lookControl"},
		{ "88","bg_shock_lookControl_maxpitchspeed"},
		{ "89","bg_shock_lookControl_maxyawspeed"},
		{ "90","bg_shock_lookControl_mousesensitivityscale"},
		{ "91","bg_shock_lookControl_fadeTime"},
		{ "92","bg_shock_movement"}
	};

	static std::unordered_map<std::string, std::string> vision_replace_list = {
		{ "511","r_glow" },
		{ "516","r_glowRadius0" },
		{ "512","r_glowBloomCutoff" },
		{ "513","r_glowBloomDesaturation" },
		{ "514","r_glowBloomIntensity0" },
		{ "520","r_filmEnable" },
		{ "522","r_filmContrast" },
		{ "521","r_filmBrightness" },
		{ "523","r_filmDesaturation" },
		{ "524","r_filmDesaturationDark" },
		{ "525","r_filmInvert" },
		{ "526","r_filmLightTint" },
		{ "527","r_filmMediumTint" },
		{ "528","r_filmDarkTint" },
		{ "529","r_primaryLightUseTweaks" },
		{ "530","r_primaryLightTweakDiffuseStrength" },
		{ "531","r_primaryLightTweakSpecularStrength" },
	};

	Game::XAssetType currentAssetType = Game::XAssetType::ASSET_TYPE_INVALID;
	Game::XAssetType previousAssetType = Game::XAssetType::ASSET_TYPE_INVALID;
	
	bool Zones::LoadFxEffectDef(bool atStreamStart, char* buffer, int size)
	{
		int count = 0;

		if (Zones::Version() >= VERSION_ALPHA2)
		{
			size /= 252;
			count = size;
			size *= 260;
		}

		bool result = Game::Load_Stream(atStreamStart, buffer, size);

		Zones::FxEffectIndex = 0;

		if (Zones::Version() >= VERSION_ALPHA2)
		{
			Utils::Memory::Allocator allocator;
			Game::FxElemDef* elems = allocator.allocateArray<Game::FxElemDef>(count);

			for (int i = 0; i < count; ++i)
			{
				AssetHandler::Relocate(buffer + (260 * i), buffer + (252 * i), 252);
				std::memcpy(&elems[i], buffer + (260 * i), 252);
				Zones::FxEffectStrings[i] = *reinterpret_cast<char**>(buffer + (260 * i) + 256);
			}

			std::memcpy(buffer, elems, sizeof(Game::FxElemDef) * count);
		}

		return result;
	}

	bool Zones::LoadFxElemDefStub(bool atStreamStart, Game::FxElemDef* fxElem, int size)
	{
		if (Zones::Version() >= VERSION_ALPHA2)
		{
			if (fxElem->elemType == 3)
			{
				fxElem->elemType = 2;
			}
			else if (fxElem->elemType >= 5)
			{
				fxElem->elemType -= 2;
			}
		}

		return Game::Load_Stream(atStreamStart, fxElem, size);
	}

	void Zones::LoadFxElemDefArrayStub(bool atStreamStart)
	{
		Game::Load_FxElemDef(atStreamStart);

		if (Zones::Version() >= VERSION_ALPHA2)
		{
			*Game::varXString = &Zones::FxEffectStrings[Zones::FxEffectIndex++];
			Game::Load_XString(false);
		}
	}

	bool Zones::LoadXModel(bool atStreamStart, char* xmodel, int size)
	{
		if (Zones::Version() >= VERSION_ALPHA2)
		{
			if (Zones::Version() == VERSION_ALPHA2)
			{
				size = 0x16C;
			}
			else
			{
				size = 0x168;
			}
		}

		bool result = Game::Load_Stream(atStreamStart, xmodel, size);
		
		if (Zones::Version() >= VERSION_ALPHA2)
		{
			Game::XModel model[2]; // Allocate 2 models, as we exceed the buffer

			std::memcpy(model, xmodel, 36);
			std::memcpy(&model->boneNames, &xmodel[44], 28);

			for (int i = 0; i < 4; ++i)
			{
				AssertOffset(Game::XModelLodInfo, partBits, 12);

				std::memcpy(&model->lodInfo[i], &xmodel[72 + (i * 56)], 12);
				std::memcpy(&model->lodInfo[i].partBits, &xmodel[72 + (i * 56) + 16], 32);

				std::memcpy(reinterpret_cast<char*>(&model) + (size - 4) - (i * 4), &xmodel[72 + (i * 56) + 12], 4);
			}

			std::memcpy(&model->lodInfo[3].lod, &xmodel[292], (size - 292 - 4)/*68*/);
			std::memcpy(&model->physPreset, &xmodel[(size - 8)], 8);

			model[1].name = reinterpret_cast<char*>(0xDEADC0DE);

			std::memcpy(xmodel, &model, size);
		}

		return result;
	}

	void Zones::LoadXModelLodInfo(int i)
	{
		if (Zones::Version() >= VERSION_ALPHA2)
		{
			int elSize = (Zones::ZoneVersion == VERSION_ALPHA2) ? 364 : 360;
			*Game::varXString = reinterpret_cast<char**>(reinterpret_cast<char*>(*Game::varXModel) + (elSize - 4) - (4 * (4 - i)));
			Game::Load_XString(false);
		}
	}

	__declspec(naked) void Zones::LoadXModelLodInfoStub()
	{
		__asm
		{
			pushad
			push edi
			call Zones::LoadXModelLodInfo
			add esp, 4h
			popad

			mov eax, [esp + 8h]
			push eax
			add eax, 8
			push eax
			call Game::Load_XModelSurfsFixup
			add esp, 8h

			retn
		}
	}

	bool Zones::LoadXSurfaceArray(bool atStreamStart, char* buffer, int size)
	{
		int count = 0;

		if (Zones::Version() >= VERSION_ALPHA2)
		{
			size >>= 6;

			count = size;
			size *= 84;
		}

		bool result = Game::Load_Stream(atStreamStart, buffer, size);

		if (Zones::Version() >= VERSION_ALPHA2)
		{
			Utils::Memory::Allocator allocator;
			Game::XSurface* tempSurfaces = allocator.allocateArray<Game::XSurface>(count);

			for (int i = 0; i < count; ++i)
			{
				char* source = &buffer[i * 84];

				std::memcpy(&tempSurfaces[i], source, 12);
				std::memcpy(&tempSurfaces[i].triIndices, source + 16, 20);
				std::memcpy(&tempSurfaces[i].vertListCount, source + 40, 8);
				std::memcpy(&tempSurfaces[i].partBits, source + 52, 24);

				if (Zones::ZoneVersion >= 332)
				{
					struct
					{
						short pad;                // +0
						char flag;                // +2
						char zoneHandle;          // +3
						unsigned short vertCount; // +4
						unsigned short triCount;  // +6
						// [...]
					} surface332;

					// Copy the data to our new structure
					std::memcpy(&surface332, &tempSurfaces[i], sizeof(surface332));

					// Check if that special flag is set
					if (!(surface332.flag & 0x20))
					{
						Logger::Error("We're not able to handle XSurface buffer allocation yet!");
					}

					// Copy the correct data back to our surface
					tempSurfaces[i].zoneHandle = surface332.zoneHandle;
					tempSurfaces[i].vertCount = surface332.vertCount;
					tempSurfaces[i].triCount = surface332.triCount;

					//std::memmove(&tempSurfaces[i].numVertices, &tempSurfaces[i].numPrimitives, 6);
				}
			}

			std::memcpy(buffer, tempSurfaces, sizeof(Game::XSurface) * count);
		}

		return result;
	}

	void Zones::LoadWeaponCompleteDef()
	{
		if (Zones::ZoneVersion < VERSION_ALPHA2)
		{
			return Utils::Hook::Call<void(bool)>(0x4AE7B0)(true);
		}

		// setup structures we use
		char* varWeaponCompleteDef = *reinterpret_cast<char**>(0x112A9F4);

		int size = 3112;

		if (Zones::ZoneVersion >= 461)
			size = 4124;
		else if (Zones::ZoneVersion >= 460)
			size = 4120;
		else if (Zones::ZoneVersion >= 365)
			size = 3124;
		else if (Zones::ZoneVersion >= 359)
			size = 3120;
		else if (Zones::ZoneVersion >= 332)
			size = 3068;
		else if (Zones::ZoneVersion >= 318)
			size = 3156;

		int offsetShift = (Zones::ZoneVersion >= 461) ? 4 : 0;

		// and do the stuff
		Game::Load_Stream(true, varWeaponCompleteDef, size);

		Game::DB_PushStreamPos(3);

		*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 0);
		Game::Load_XString(false);

		*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 4);
		Game::Load_XString(false);

		*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 8);
		Game::Load_XString(false);

		*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 12);
		Game::Load_XString(false);

		*Game::varXModelPtr = reinterpret_cast<Game::XModel**>(varWeaponCompleteDef + 16);
		Game::Load_XModelPtr(false);

		if (Zones::ZoneVersion >= 359)
		{
			auto count = (Zones::Version() >= 460) ? 52 : 56;
			for (int offset = 20; offset <= count; offset += 4)
			{
				*Game::varXModelPtr = reinterpret_cast<Game::XModel**>(varWeaponCompleteDef + offset);
				Game::Load_XModelPtr(false);
			}
		}
		else
		{
			for (int i = 0, offset = 20; i < 32; ++i, offset += 4)
			{
				*Game::varXModelPtr = reinterpret_cast<Game::XModel**>(varWeaponCompleteDef + offset);
				Game::Load_XModelPtr(false);
			}

			// 148
			for (int offset = 148; offset <= 168; offset += 4)
			{
				*Game::varXModelPtr = reinterpret_cast<Game::XModel**>(varWeaponCompleteDef + offset);
				Game::Load_XModelPtr(false);
			}
		}


		// 172
		// 32 scriptstrings, should not need to be loaded

		if (Zones::ZoneVersion >= 359)
		{
			auto stringCount = (Zones::Version() >= 460) ? 62 : 52;
			auto arraySize = stringCount * 4;
			
			// 236
			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 124);
			Game::Load_XStringArray(false, stringCount);

			// 428
			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 124 + arraySize);
			Game::Load_XStringArray(false, stringCount);

			// 620
			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 124 + (arraySize * 2));
			Game::Load_XStringArray(false, stringCount);
		}
		else
		{
			// 236
			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 236);
			Game::Load_XStringArray(false, 48);

			// 428
			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 428);
			Game::Load_XStringArray(false, 48);

			// 620
			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 620);
			Game::Load_XStringArray(false, 48);
		}

		// 812
		// 16 * 4 scriptstrings

		if (Zones::Version() >= 460)
		{
			for (int i = 0; i < 16; i++)
			{
				*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef * *>(varWeaponCompleteDef + 1028 + (i * 4));
				Game::Load_FxEffectDefHandle(false);
			}

			for (int i = 0; i < 16; i++)
			{
				*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef * *>(varWeaponCompleteDef + 1124 + (i * 4));
				Game::Load_FxEffectDefHandle(false);
			}

			for (int i = 0; i < 16; i++)
			{
				*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef * *>(varWeaponCompleteDef + 1188 + (i * 4));
				Game::Load_FxEffectDefHandle(false);
			}

			for (int i = 0; i < 16; i++)
			{
				*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef * *>(varWeaponCompleteDef + 1316 + (i * 4));
				Game::Load_FxEffectDefHandle(false);
			}

			for (int i = 0; i < 5; i++)
			{
				*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef * *>(varWeaponCompleteDef + 1444 + (i * 4));
				Game::Load_FxEffectDefHandle(false);
			}
		}
		else if (Zones::ZoneVersion >= 359)
		{
			// 972
			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(varWeaponCompleteDef + 908);
			Game::Load_FxEffectDefHandle(false);

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(varWeaponCompleteDef + 912);
			Game::Load_FxEffectDefHandle(false);
		}
		else
		{
			// 972
			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(varWeaponCompleteDef + 972);
			Game::Load_FxEffectDefHandle(false);

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(varWeaponCompleteDef + 976);
			Game::Load_FxEffectDefHandle(false);
		}

		if (Zones::Version() >= 460)
		{
			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 1464);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 1472);
			Game::Load_XString(false);
		}

		// 980
		if (Zones::ZoneVersion >= 359)
		{
			auto offset = (Zones::Version() >= 460) ? 1476 : 916;
			auto count = (Zones::Version() >= 461) ? 58 : (Zones::Version() >= 460) ? 57 : 52;
			
			// 53 soundalias name references; up to and including 1124
			for (int i = 0; i < count; ++i, offset += 4)
			{
				*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + offset);
				Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);
			}
		}
		else
		{
			// 50 soundalias name references; up to and including 1180
			for (int i = 0, offset = 980; i < 50; ++i, offset += 4)
			{
				*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + offset);
				Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);
			}

			if (Zones::ZoneVersion >= 318)
			{
				for (int i = 0, offset = 1184; i < 2; ++i, offset += 4)
				{
					*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + offset);
					Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);
				}

				varWeaponCompleteDef += 8; // to compensate for the 2 in between here
			}
		}

		if (Zones::Version() >= 460)
		{
			if (*reinterpret_cast<void**>(varWeaponCompleteDef + 1708))
			{
				if (*reinterpret_cast<DWORD*>(varWeaponCompleteDef + 1708) == -1)
				{
					*reinterpret_cast<void**>(varWeaponCompleteDef + 1708) = Game::DB_AllocStreamPos(3);
					*Game::varsnd_alias_list_name = *reinterpret_cast<Game::snd_alias_list_t * **>(varWeaponCompleteDef + 1708);

					Game::Load_snd_alias_list_nameArray(true, 31);
				}
				else
				{
					// full usability requires ConvertOffsetToPointer here
				}
			}

			if (*reinterpret_cast<void**>(varWeaponCompleteDef + 1712))
			{
				if (*reinterpret_cast<DWORD*>(varWeaponCompleteDef + 1712) == -1)
				{
					*reinterpret_cast<void**>(varWeaponCompleteDef + 1712) = Game::DB_AllocStreamPos(3);
					*Game::varsnd_alias_list_name = *reinterpret_cast<Game::snd_alias_list_t * **>(varWeaponCompleteDef + 1712);

					Game::Load_snd_alias_list_nameArray(true, 31);
				}
				else
				{
					// full usability requires ConvertOffsetToPointer here
				}
			}
		}
		else if (Zones::ZoneVersion >= 359)
		{
			if (*reinterpret_cast<void**>(varWeaponCompleteDef + 1128))
			{
				if (*reinterpret_cast<DWORD*>(varWeaponCompleteDef + 1128) == -1)
				{
					*reinterpret_cast<void**>(varWeaponCompleteDef + 1128) = Game::DB_AllocStreamPos(3);
					*Game::varsnd_alias_list_name = *reinterpret_cast<Game::snd_alias_list_t***>(varWeaponCompleteDef + 1128);

					Game::Load_snd_alias_list_nameArray(true, 31);
				}
				else
				{
					// full usability requires ConvertOffsetToPointer here
				}
			}

			if (*reinterpret_cast<void**>(varWeaponCompleteDef + 1132))
			{
				if (*reinterpret_cast<DWORD*>(varWeaponCompleteDef + 1132) == -1)
				{
					*reinterpret_cast<void**>(varWeaponCompleteDef + 1132) = Game::DB_AllocStreamPos(3);
					*Game::varsnd_alias_list_name = *reinterpret_cast<Game::snd_alias_list_t***>(varWeaponCompleteDef + 1132);

					Game::Load_snd_alias_list_nameArray(true, 31);
				}
				else
				{
					// full usability requires ConvertOffsetToPointer here
				}
			}
		}
		else
		{
			if (*reinterpret_cast<void**>(varWeaponCompleteDef + 1184))
			{
				if (*reinterpret_cast<DWORD*>(varWeaponCompleteDef + 1184) == -1)
				{
					*reinterpret_cast<void**>(varWeaponCompleteDef + 1184) = Game::DB_AllocStreamPos(3);
					*Game::varsnd_alias_list_name = *reinterpret_cast<Game::snd_alias_list_t***>(varWeaponCompleteDef + 1184);

					Game::Load_snd_alias_list_nameArray(true, 31);
				}
				else
				{
					// full usability requires ConvertOffsetToPointer here
				}
			}

			if (*reinterpret_cast<void**>(varWeaponCompleteDef + 1188))
			{
				if (*reinterpret_cast<DWORD*>(varWeaponCompleteDef + 1188) == -1)
				{
					*reinterpret_cast<void**>(varWeaponCompleteDef + 1188) = Game::DB_AllocStreamPos(3);
					*Game::varsnd_alias_list_name = *reinterpret_cast<Game::snd_alias_list_t***>(varWeaponCompleteDef + 1188);

					Game::Load_snd_alias_list_nameArray(true, 31);
				}
				else
				{
					// full usability requires ConvertOffsetToPointer here
				}
			}
		}

		if (Zones::Version() >= 460)
		{
			// 1192
			for (int offset = 1716; offset <= 1728; offset += 4)
			{
				*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef * *>(varWeaponCompleteDef + offset);
				Game::Load_FxEffectDefHandle(false);
			}
		}
		else if (Zones::ZoneVersion >= 359)
		{
			// 1192
			for (int offset = 1136; offset <= 1148; offset += 4)
			{
				*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(varWeaponCompleteDef + offset);
				Game::Load_FxEffectDefHandle(false);
			}
		}
		else
		{
			// 1192
			for (int offset = 1192; offset <= 1204; offset += 4)
			{
				*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(varWeaponCompleteDef + offset);
				Game::Load_FxEffectDefHandle(false);
			}
		}

		if (Zones::Version() >= 460)
		{
			// 1208
			static int matOffsets1[] = { 1732, 1736, 1952, 1956, 1960, 1964, 1968, 1972, 1980, 1988, 2000, 2004, 2008, 2012 };
			for (int i = 0; i < ARRAYSIZE(matOffsets1); ++i)
			{
				*Game::varMaterialHandle = reinterpret_cast<Game::Material * *>(varWeaponCompleteDef + matOffsets1[i]);
				Game::Load_MaterialHandle(false);
			}
		}
		else if (Zones::ZoneVersion >= 359)
		{
			// 1208
			static int matOffsets1[] = { 1152, 1156, 1372,1376,1380, 1384, 1388, 1392, 1400, 1408 };
			for (int i = 0; i < ARRAYSIZE(matOffsets1); ++i)
			{
				*Game::varMaterialHandle = reinterpret_cast<Game::Material**>(varWeaponCompleteDef + matOffsets1[i]);
				Game::Load_MaterialHandle(false);
			}
		}
		else
		{			// 1208
			static int matOffsets1[] = { 1208, 1212, 1428, 1432, 1436, 1440, 1444, 1448, 1456, 1464 };
			for (int i = 0; i < ARRAYSIZE(matOffsets1); ++i)
			{
				*Game::varMaterialHandle = reinterpret_cast<Game::Material**>(varWeaponCompleteDef + matOffsets1[i]);
				Game::Load_MaterialHandle(false);
			}
		}

		if (Zones::Version() >= 460)
		{
			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2024);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2032);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2048);
			Game::Load_XString(false);
		}
		else if (Zones::ZoneVersion >= 359)
		{
			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 1428);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 1436);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 1452);
			Game::Load_XString(false);
		}
		else
		{
			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 1484);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 1492);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 1508);
			Game::Load_XString(false);
		}

		if (Zones::Version() >= 460)
		{
			for (int offset = 2332; offset <= 2344; offset += 4)
			{
				*Game::varMaterialHandle = reinterpret_cast<Game::Material * *>(varWeaponCompleteDef + offset);
				Game::Load_MaterialHandle(false);
			}
		}
		else if (Zones::ZoneVersion >= 359)
		{
			for (int offset = 1716; offset <= 1728; offset += 4)
			{
				*Game::varMaterialHandle = reinterpret_cast<Game::Material**>(varWeaponCompleteDef + offset);
				Game::Load_MaterialHandle(false);
			}
		}
		else
		{
			for (int offset = 1764; offset <= 1776; offset += 4)
			{
				*Game::varMaterialHandle = reinterpret_cast<Game::Material**>(varWeaponCompleteDef + offset);
				Game::Load_MaterialHandle(false);
			}
		}

		if (Zones::Version() >= 460)
		{
			*Game::varPhysCollmapPtr = reinterpret_cast<Game::PhysCollmap * *>(varWeaponCompleteDef + 2544);
			Game::Load_PhysCollmapPtr(false);

			*Game::varPhysPresetPtr = reinterpret_cast<Game::PhysPreset * *>(varWeaponCompleteDef + 2548);
			Game::Load_PhysPresetPtr(false);
		}
		else if (Zones::ZoneVersion >= 359)
		{
			*Game::varPhysCollmapPtr = reinterpret_cast<Game::PhysCollmap**>(varWeaponCompleteDef + 1928);
			Game::Load_PhysCollmapPtr(false);

			*Game::varPhysPresetPtr = reinterpret_cast<Game::PhysPreset**>(varWeaponCompleteDef + 1932);
			Game::Load_PhysPresetPtr(false);
		}
		else
		{
			*Game::varPhysCollmapPtr = reinterpret_cast<Game::PhysCollmap**>(varWeaponCompleteDef + 1964);
			Game::Load_PhysCollmapPtr(false);
		}

		if (Zones::Version() >= 460)
		{
			*Game::varXModelPtr = reinterpret_cast<Game::XModel * *>(varWeaponCompleteDef + 2656);
			Game::Load_XModelPtr(false);
		}
		else if (Zones::ZoneVersion >= 359)
		{
			*Game::varXModelPtr = reinterpret_cast<Game::XModel**>(varWeaponCompleteDef + 2020);
			Game::Load_XModelPtr(false);
		}
		else
		{
			*Game::varXModelPtr = reinterpret_cast<Game::XModel**>(varWeaponCompleteDef + 2052);
			Game::Load_XModelPtr(false);
		}

		if (Zones::Version() >= 460)
		{
			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef * *>(varWeaponCompleteDef + 2664);
			Game::Load_FxEffectDefHandle(false);

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef * *>(varWeaponCompleteDef + 2668);
			Game::Load_FxEffectDefHandle(false);
		}
		else if (Zones::ZoneVersion >= 359)
		{
			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(varWeaponCompleteDef + 2028);
			Game::Load_FxEffectDefHandle(false);

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(varWeaponCompleteDef + 2032);
			Game::Load_FxEffectDefHandle(false);
		}
		else
		{
			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(varWeaponCompleteDef + 2060);
			Game::Load_FxEffectDefHandle(false);

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(varWeaponCompleteDef + 2064);
			Game::Load_FxEffectDefHandle(false);
		}

		if (Zones::Version() >= 460)
		{
			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t * *>(varWeaponCompleteDef + 2672);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t * *>(varWeaponCompleteDef + 2676);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);
		}
		else if (Zones::ZoneVersion >= 359)
		{
			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + 2036);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + 2040);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);
		}
		else
		{
			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + 2068);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + 2072);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);
		}

		if (Zones::Version() >= 460)
		{
			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef * *>(varWeaponCompleteDef + 2952);
			Game::Load_FxEffectDefHandle(false);

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef * *>(varWeaponCompleteDef + 2956);
			Game::Load_FxEffectDefHandle(false);

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef * *>(varWeaponCompleteDef + 2984);
			Game::Load_FxEffectDefHandle(false);
		}
		else if (Zones::ZoneVersion >= 359)
		{
			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(varWeaponCompleteDef + 2304);
			Game::Load_FxEffectDefHandle(false);

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(varWeaponCompleteDef + 2308);
			Game::Load_FxEffectDefHandle(false);

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(varWeaponCompleteDef + 2336);
			Game::Load_FxEffectDefHandle(false);
		}
		else
		{
			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(varWeaponCompleteDef + 2336);
			Game::Load_FxEffectDefHandle(false);

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(varWeaponCompleteDef + 2340);
			Game::Load_FxEffectDefHandle(false);

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(varWeaponCompleteDef + 2368); // 2376
			Game::Load_FxEffectDefHandle(false);
		}

		if (Zones::Version() >= 460)
		{
			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t * *>(varWeaponCompleteDef + 2988);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef * *>(varWeaponCompleteDef + 2992);
			Game::Load_FxEffectDefHandle(false);

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef * *>(varWeaponCompleteDef + 2996);
			Game::Load_FxEffectDefHandle(false);

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef * *>(varWeaponCompleteDef + 3000);
			Game::Load_FxEffectDefHandle(false);

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef * *>(varWeaponCompleteDef + 3004);
			Game::Load_FxEffectDefHandle(false);

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef * *>(varWeaponCompleteDef + 3008);
			Game::Load_FxEffectDefHandle(false);
			
			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3196);
			Game::Load_XString(false);
		}
		else if (Zones::ZoneVersion >= 359)
		{
			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + 2340);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2516);
			Game::Load_XString(false);
		}
		else
		{
			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + 2372); // 2380
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2548); // 2556
			Game::Load_XString(false);
		}

		if (Zones::Version() >= 460)
		{
			if (*reinterpret_cast<DWORD*>(varWeaponCompleteDef + 3204) == -1)
			{
				void* vec2 = Game::DB_AllocStreamPos(3);
				*reinterpret_cast<void**>(varWeaponCompleteDef + 3204) = vec2;

				Game::Load_Stream(true, vec2, 8 * *reinterpret_cast<short*>(varWeaponCompleteDef + 3776 + offsetShift));
			}

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3200);
			Game::Load_XString(false);

			if (*reinterpret_cast<DWORD*>(varWeaponCompleteDef + 3208) == -1)
			{
				void* vec2 = Game::DB_AllocStreamPos(3);
				*reinterpret_cast<void**>(varWeaponCompleteDef + 3208) = vec2;

				Game::Load_Stream(true, vec2, 8 * *reinterpret_cast<short*>(varWeaponCompleteDef + 3778 + offsetShift));
			}
		}
		else if (Zones::ZoneVersion >= 359)
		{
			if (*reinterpret_cast<DWORD*>(varWeaponCompleteDef + 2524) == -1)
			{
				void* vec2 = Game::DB_AllocStreamPos(3);
				*reinterpret_cast<void**>(varWeaponCompleteDef + 2524) = vec2;

				Game::Load_Stream(true, vec2, 8 * *reinterpret_cast<short*>(varWeaponCompleteDef + 3044));
			}

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2520);
			Game::Load_XString(false);

			if (*reinterpret_cast<DWORD*>(varWeaponCompleteDef + 2528) == -1)
			{
				void* vec2 = Game::DB_AllocStreamPos(3);
				*reinterpret_cast<void**>(varWeaponCompleteDef + 2528) = vec2;

				Game::Load_Stream(true, vec2, 8 * *reinterpret_cast<short*>(varWeaponCompleteDef + 3046));
			}
		}
		else
		{
			if (*reinterpret_cast<DWORD*>(varWeaponCompleteDef + 2556) == -1) // 2564
			{
				void* vec2 = Game::DB_AllocStreamPos(3);
				*reinterpret_cast<void**>(varWeaponCompleteDef + 2556) = vec2;

				Game::Load_Stream(true, vec2, 8 * *reinterpret_cast<short*>(varWeaponCompleteDef + ((Zones::ZoneVersion >= 318) ? 3076 : 3040)));
			}

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2552);
			Game::Load_XString(false);

			if (*reinterpret_cast<DWORD*>(varWeaponCompleteDef + 2560) == -1)
			{
				void* vec2 = Game::DB_AllocStreamPos(3);
				*reinterpret_cast<void**>(varWeaponCompleteDef + 2560) = vec2;

				Game::Load_Stream(true, vec2, 8 * *reinterpret_cast<short*>(varWeaponCompleteDef + ((Zones::ZoneVersion >= 318) ? 3078 : 3042)));
			}
		}

		if (Zones::Version() >= 460)
		{
			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3288 + offsetShift);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3292 + offsetShift);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3324 + offsetShift);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3328 + offsetShift);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3484 + offsetShift);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3488 + offsetShift);
			Game::Load_XString(false);
		}
		else if (Zones::ZoneVersion >= 359)
		{
			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2608);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2612);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2644);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2648);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2772);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2776);
			Game::Load_XString(false);
		}
		else
		{
			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2640);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2644);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2676);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2680);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2804);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2808);
			Game::Load_XString(false);
		}

		if (Zones::Version() >= 460)
		{
			*Game::varTracerDefPtr = reinterpret_cast<Game::TracerDef * *>(varWeaponCompleteDef + 3492 + offsetShift);
			Game::Load_TracerDefPtr(false);

			*Game::varTracerDefPtr = reinterpret_cast<Game::TracerDef * *>(varWeaponCompleteDef + 3496 + offsetShift);
			Game::Load_TracerDefPtr(false);

			*Game::varTracerDefPtr = reinterpret_cast<Game::TracerDef * *>(varWeaponCompleteDef + 3500 + offsetShift);
			Game::Load_TracerDefPtr(false);
			
			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t * *>(varWeaponCompleteDef + 3528 + offsetShift);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name); // 2848

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef * *>(varWeaponCompleteDef + 3532 + offsetShift);
			Game::Load_FxEffectDefHandle(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3536 + offsetShift);
			Game::Load_XString(false);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t * *>(varWeaponCompleteDef + 3552 + offsetShift);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t * *>(varWeaponCompleteDef + 3556 + offsetShift);
			Game::Load_snd_alias_list_nameArray(false, 4);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t * *>(varWeaponCompleteDef + 3572 + offsetShift);
			Game::Load_snd_alias_list_nameArray(false, 4);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t * *>(varWeaponCompleteDef + 3588 + offsetShift);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t * *>(varWeaponCompleteDef + 3592 + offsetShift);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);
		}
		else if (Zones::ZoneVersion >= 359)
		{
			*Game::varTracerDefPtr = reinterpret_cast<Game::TracerDef**>(varWeaponCompleteDef + 2780);
			Game::Load_TracerDefPtr(false);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + 2808);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name); // 2848

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(varWeaponCompleteDef + 2812);
			Game::Load_FxEffectDefHandle(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2816);
			Game::Load_XString(false);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + 2832);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + 2836);
			Game::Load_snd_alias_list_nameArray(false, 4);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + 2852);
			Game::Load_snd_alias_list_nameArray(false, 4);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + 2868);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + 2872);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);
		}
		else
		{
			*Game::varTracerDefPtr = reinterpret_cast<Game::TracerDef**>(varWeaponCompleteDef + 2812);
			Game::Load_TracerDefPtr(false);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + 2840);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name); // 2848

			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(varWeaponCompleteDef + 2844);
			Game::Load_FxEffectDefHandle(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2848);
			Game::Load_XString(false);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + 2864);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + 2868);
			Game::Load_snd_alias_list_nameArray(false, 4);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + 2884);
			Game::Load_snd_alias_list_nameArray(false, 4);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + 2900);
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);

			*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + 2904); // 2912
			Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);
		}

		if (Zones::Version() >= 460)
		{
			for (int i = 0, offset = 3660 + offsetShift; i < 6; ++i, offset += 4)
			{
				*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t * *>(varWeaponCompleteDef + offset);
				Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);
			}
		}
		else if (Zones::ZoneVersion >= 359)
		{
			for (int i = 0, offset = 2940; i < 6; ++i, offset += 4)
			{
				*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + offset);
				Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);
			}
		}
		else
		{
			if (Zones::ZoneVersion >= 318)
			{
				for (int i = 0, offset = 2972; i < 6; ++i, offset += 4)
				{
					*Game::varsnd_alias_list_name = reinterpret_cast<Game::snd_alias_list_t**>(varWeaponCompleteDef + offset);
					Game::Load_SndAliasCustom(*Game::varsnd_alias_list_name);
				}

				varWeaponCompleteDef += (6 * 4);
				varWeaponCompleteDef += 12;
			}
			else
			{

			}
		}

		if (Zones::Version() >= 460)
		{
			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3712 + offsetShift);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3728 + offsetShift);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3732 + offsetShift);
			Game::Load_XString(false);

			*Game::varMaterialHandle = reinterpret_cast<Game::Material * *>(varWeaponCompleteDef + 3740 + offsetShift);
			Game::Load_MaterialHandle(false);

			*Game::varMaterialHandle = reinterpret_cast<Game::Material * *>(varWeaponCompleteDef + 3744 + offsetShift);
			Game::Load_MaterialHandle(false);

			*Game::varMaterialHandle = reinterpret_cast<Game::Material * *>(varWeaponCompleteDef + 3748 + offsetShift);
			Game::Load_MaterialHandle(false);

			*Game::varMaterialHandle = reinterpret_cast<Game::Material * *>(varWeaponCompleteDef + 3752 + offsetShift);
			Game::Load_MaterialHandle(false);
		}
		else if (Zones::ZoneVersion >= 359)
		{
			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2988);
			Game::Load_XString(false);

			if (Zones::ZoneVersion >= 365)
			{
				varWeaponCompleteDef += 4;
			}

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3000);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3004);
			Game::Load_XString(false);

			*Game::varMaterialHandle = reinterpret_cast<Game::Material**>(varWeaponCompleteDef + 3012);
			Game::Load_MaterialHandle(false);

			*Game::varMaterialHandle = reinterpret_cast<Game::Material**>(varWeaponCompleteDef + 3016);
			Game::Load_MaterialHandle(false);

			*Game::varMaterialHandle = reinterpret_cast<Game::Material**>(varWeaponCompleteDef + 3020);
			Game::Load_MaterialHandle(false);
		}
		else
		{
			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2984);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 2996);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3000);
			Game::Load_XString(false);

			*Game::varMaterialHandle = reinterpret_cast<Game::Material**>(varWeaponCompleteDef + 3008);
			Game::Load_MaterialHandle(false);

			*Game::varMaterialHandle = reinterpret_cast<Game::Material**>(varWeaponCompleteDef + 3012);
			Game::Load_MaterialHandle(false);

			*Game::varMaterialHandle = reinterpret_cast<Game::Material**>(varWeaponCompleteDef + 3016);
			Game::Load_MaterialHandle(false);
		}

		if (Zones::Version() >= 460)
		{
			if (*reinterpret_cast<DWORD*>(varWeaponCompleteDef + 3780 + offsetShift) == -1)
			{
				void* vec2 = Game::DB_AllocStreamPos(3);
				*reinterpret_cast<void**>(varWeaponCompleteDef + 3780 + offsetShift) = vec2;

				Game::Load_Stream(true, vec2, 8 * *reinterpret_cast<short*>(varWeaponCompleteDef + 3776 + offsetShift));
			}

			if (*reinterpret_cast<DWORD*>(varWeaponCompleteDef + 3784 + offsetShift) == -1)
			{
				void* vec2 = Game::DB_AllocStreamPos(3);
				*reinterpret_cast<void**>(varWeaponCompleteDef + 3784 + offsetShift) = vec2;

				Game::Load_Stream(true, vec2, 8 * *reinterpret_cast<short*>(varWeaponCompleteDef + 3778 + offsetShift));
			}

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3876 + offsetShift);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3880 + offsetShift);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3884 + offsetShift);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 3996 + offsetShift);
			Game::Load_XString(false);

			*Game::varXString = reinterpret_cast<char**>(varWeaponCompleteDef + 4012 + offsetShift);
			Game::Load_XString(false);
		}
		else if (Zones::ZoneVersion >= 359)
		{
			if (*reinterpret_cast<DWORD*>(varWeaponCompleteDef + 3048) == -1)
			{
				void* vec2 = Game::DB_AllocStreamPos(3);
				*reinterpret_cast<void**>(varWeaponCompleteDef + 3048) = vec2;

				Game::Load_Stream(true, vec2, 8 * *reinterpret_cast<short*>(varWeaponCompleteDef + 3044));
			}

			if (*reinterpret_cast<DWORD*>(varWeaponCompleteDef + 3052) == -1)
			{
				void* vec2 = Game::DB_AllocStreamPos(3);
				*reinterpret_cast<void**>(varWeaponCompleteDef + 3052) = vec2;

				Game::Load_Stream(true, vec2, 8 * *reinterpret_cast<short*>(varWeaponCompleteDef + 3046));
			}
		}
		else
		{
			if (*reinterpret_cast<DWORD*>(varWeaponCompleteDef + 3044) == -1)
			{
				void* vec2 = Game::DB_AllocStreamPos(3);
				*reinterpret_cast<void**>(varWeaponCompleteDef + 3044) = vec2;

				Game::Load_Stream(true, vec2, 8 * *reinterpret_cast<short*>(varWeaponCompleteDef + 3040));
			}

			if (*reinterpret_cast<DWORD*>(varWeaponCompleteDef + 3048) == -1)
			{
				void* vec2 = Game::DB_AllocStreamPos(3);
				*reinterpret_cast<void**>(varWeaponCompleteDef + 3048) = vec2;

				Game::Load_Stream(true, vec2, 8 * *reinterpret_cast<short*>(varWeaponCompleteDef + 3042));
			}
		}

		Game::DB_PopStreamPos();
	}

// Code-analysis has a bug, the first memcpy makes it believe size of tempVar is 44 instead of 84
#pragma warning(push)
#pragma warning(disable: 6385)
	bool Zones::LoadGameWorldSp(bool atStreamStart, char* buffer, int size)
	{
		if (Zones::Version() >= VERSION_ALPHA2)
		{
			size = 84;
		}

		bool result = Game::Load_Stream(atStreamStart, buffer, size);

		if (Zones::Version() >= VERSION_ALPHA2)
		{
			char tempVar[84] = { 0 };
			std::memcpy(&tempVar[0], &buffer[0], 44);
			std::memcpy(&tempVar[56], &buffer[44], 28);
			std::memcpy(&tempVar[44], &buffer[72], 12);

			std::memcpy(buffer, tempVar, sizeof(tempVar));
		}

		return result;
	}
#pragma warning(pop)

	void Zones::LoadPathDataTail()
	{
		if (Zones::ZoneVersion >= VERSION_ALPHA2)
		{
			char* varPathData = reinterpret_cast<char*>(*Game::varPathData);

			if (*reinterpret_cast<char**>(varPathData + 56))
			{
				*reinterpret_cast<char**>(varPathData + 56) = Game::DB_AllocStreamPos(0);
				Game::Load_Stream(true, *reinterpret_cast<char**>(varPathData + 56), *reinterpret_cast<int*>(varPathData + 52));
			}

			if (*reinterpret_cast<char**>(varPathData + 64))
			{
				*reinterpret_cast<char**>(varPathData + 64) = Game::DB_AllocStreamPos(0);
				Game::Load_Stream(true, *reinterpret_cast<char**>(varPathData + 64), *reinterpret_cast<int*>(varPathData + 60));
			}

			if (*reinterpret_cast<char**>(varPathData + 76))
			{
				*reinterpret_cast<char**>(varPathData + 76) = Game::DB_AllocStreamPos(0);
				Game::Load_Stream(true, *reinterpret_cast<char**>(varPathData + 76), *reinterpret_cast<int*>(varPathData + 72));
			}
		}
	}

	bool Zones::Loadsnd_alias_tArray(bool atStreamStart, char* buffer, int len)
	{
		int count = 0;

		if (Zones::Version() >= VERSION_ALPHA2)
		{
			len /= 100;
			count = len;
			len *= 108;
		}

		bool result = Game::Load_Stream(atStreamStart, buffer, len);

		if (Zones::Version() >= VERSION_ALPHA2)
		{
			Utils::Memory::Allocator allocator;
			Game::snd_alias_t* tempSounds = allocator.allocateArray<Game::snd_alias_t>(count);

			for (int i = 0; i < count; ++i)
			{
				char* src = &buffer[i * 108];
				char* dest = reinterpret_cast<char*>(&tempSounds[i]);

				std::memcpy(dest + 0, src + 0, 60);
				std::memcpy(dest + 60, src + 68, 20);
				std::memcpy(dest + 80, src + 88, 20);

				AssetHandler::Relocate(src + 0, buffer + (i * 100) + 0, 60);
				AssetHandler::Relocate(src + 68, buffer + (i * 100) + 60, 20);
				AssetHandler::Relocate(src + 88, buffer + (i * 100) + 80, 20);
			}

			std::memcpy(buffer, tempSounds, sizeof(Game::snd_alias_t) * count);
		}

		return result;
	}

	bool Zones::LoadLoadedSound(bool atStreamStart, char* buffer, int size)
	{
		if (Zones::Version() >= VERSION_ALPHA2)
		{
			size = 48;
		}

		bool result = Game::Load_Stream(atStreamStart, buffer, size);

		if (Zones::Version() >= VERSION_ALPHA2)
		{
			std::memmove(buffer + 28, buffer + 32, 16);
			AssetHandler::Relocate(buffer + 32, buffer + 28, 16);
		}

		return result;
	}

// Code-analysis has a bug, the first memcpy makes it believe size of tempVar is 400 instead of 788
#pragma warning(push)
#pragma warning(disable: 6385)
	bool Zones::LoadVehicleDef(bool atStreamStart, char* buffer, int size)
	{
		if (Zones::Version() >= VERSION_ALPHA2)
		{
			size = 788;
		}

		bool result = Game::Load_Stream(atStreamStart, buffer, size);

		if (Zones::Version() >= VERSION_ALPHA2)
		{
			char tempVar[788] = { 0 };
			std::memcpy(&tempVar[0], &buffer[0], 400);
			std::memcpy(&tempVar[408], &buffer[400], 380);

			AssetHandler::Relocate(buffer + 400, buffer + 408, 388);

			std::memmove(buffer, tempVar, sizeof(tempVar));
		}

		return result;
	}
#pragma warning(pop)

	void Zones::LoadWeaponAttachStuff(DWORD* varWeaponAttachStuff, int count)
	{
		Game::Load_Stream(true, varWeaponAttachStuff, 12 * count);

		for (int i = 0; i < count; ++i)
		{
			if (varWeaponAttachStuff[1] < 16 || varWeaponAttachStuff[1] == 39)
			{
				if (varWeaponAttachStuff[2] == -1)
				{
					varWeaponAttachStuff[2] = reinterpret_cast<DWORD>(Game::DB_AllocStreamPos(0));
					*Game::varConstChar = reinterpret_cast<const char*>(varWeaponAttachStuff[2]);
					Game::Load_XStringCustom(Game::varConstChar);
				}
			}

			varWeaponAttachStuff += 3;
		}
	}

	bool Zones::LoadmenuDef_t(bool atStreamStart, char* buffer, int size)
	{
		if (Zones::ZoneVersion != 359 && Zones::ZoneVersion >= VERSION_ALPHA2) size += 4;

		bool result = Game::Load_Stream(atStreamStart, buffer, size);

		if (Zones::ZoneVersion >= VERSION_ALPHA2)
		{
			std::memmove(buffer + 168, buffer + 172, (Zones::ZoneVersion != 359 ? 232 : 228));
			AssetHandler::Relocate(buffer + 172, buffer + 168, (Zones::ZoneVersion != 359 ? 232 : 228));

			reinterpret_cast<Game::menuDef_t*>(buffer)->expressionData = nullptr;
		}

		return result;
	}

	void Zones::LoadWeaponAttach()
	{
		if (Zones::ZoneVersion < VERSION_ALPHA2)
		{
			return Utils::Hook::Call<void(bool)>(0x4F4160)(true);
		}

		// setup structures we use
		char* varWeaponAttach = *reinterpret_cast<char**>(0x112ADE0); // varAddonMapEnts

		// and do the stuff
		if (Zones::Version() >= 446)
		{
			Game::Load_Stream(true, varWeaponAttach, 20);

			Game::DB_PushStreamPos(3);

			*Game::varXString = reinterpret_cast<char**>(varWeaponAttach);
			Game::Load_XString(false);

			// load string array
			if (*reinterpret_cast<int**>(varWeaponAttach + 16) != nullptr)
			{
				if (*reinterpret_cast<int*>(varWeaponAttach + 16) == -1)
				{
					*reinterpret_cast<void**>(varWeaponAttach + 16) = Game::DB_AllocStreamPos(3);
					*Game::varXString = reinterpret_cast<char**>(varWeaponAttach + 16);
					Game::Load_XStringArray(true, *reinterpret_cast<int*>(varWeaponAttach + 12));
				}
				else
				{
					Utils::Hook::Call<void(int*)>(0x4A82B0)(reinterpret_cast<int*>(varWeaponAttach + 16));
				}
			}

			Game::DB_PopStreamPos();
		}
		else
		{
			Game::Load_Stream(true, varWeaponAttach, 12);

			Game::DB_PushStreamPos(3);

			*Game::varXString = reinterpret_cast<char**>(varWeaponAttach);
			Game::Load_XString(false);

			*reinterpret_cast<void**>(varWeaponAttach + 8) = Game::DB_AllocStreamPos(3);
			Zones::LoadWeaponAttachStuff(*reinterpret_cast<DWORD * *>(varWeaponAttach + 8), *reinterpret_cast<int*>(varWeaponAttach + 4));

			Game::DB_PopStreamPos();
		}
		
	}

	bool Zones::LoadMaterialShaderArgumentArray(bool atStreamStart, Game::MaterialShaderArgument* argument, int size)
	{
		// if (Zones::ZoneVersion >= 446 && currentAssetType == Game::XAssetType::ASSET_TYPE_FX) __debugbreak();
		bool result = Game::Load_Stream(atStreamStart, argument, size);

		Game::MaterialPass* curPass = *Game::varMaterialPass;
		int count = curPass->perPrimArgCount + curPass->perObjArgCount + curPass->stableArgCount;
		
		for (int i = 0; i < count && (Zones::ZoneVersion >= VERSION_ALPHA2); ++i)
		{
			Game::MaterialShaderArgument* arg = &argument[i];

			if (arg->type != D3DSHADER_PARAM_REGISTER_TYPE::D3DSPR_TEXTURE && arg->type != D3DSHADER_PARAM_REGISTER_TYPE::D3DSPR_ATTROUT)
			{
				continue;
			}

			if (Zones::Version() < 446)
			{
				// should be min 68 currently
				// >= 58 fixes foliage without bad side effects
				// >= 53 still has broken shadow mapping
				// >= 23 is still broken somehow
				if (arg->u.codeConst.index >= 58 && arg->u.codeConst.index <= 135) // >= 34 would be 31 in iw4 terms
				{
					arg->u.codeConst.index -= 3;

					if (Zones::Version() >= 359/* && arg->paramID <= 113*/)
					{
						arg->u.codeConst.index -= 7;

						if (arg->u.codeConst.index <= 53)
						{
							arg->u.codeConst.index += 1;
						}
					}
				}
				// >= 21 works fine for specular, but breaks trees
				// >= 4 is too low, breaks specular
				else if (arg->u.codeConst.index >= 11 && arg->u.codeConst.index < 58)
				{
					arg->u.codeConst.index -= 2;

					if (Zones::Version() >= 359)
					{
						if (arg->u.codeConst.index > 15 && arg->u.codeConst.index < 30)
						{
							arg->u.codeConst.index -= 1;

							if (arg->u.codeConst.index == 19)
							{
								arg->u.codeConst.index = 21;
							}
						}
						else if (arg->u.codeConst.index >= 50)
						{
							arg->u.codeConst.index += 6;
						}
					}
				}
			}
			else
			{
				if (arg->type == 3 || arg->type == 5)
				{
					// 446 is from a special client version that had lot of
					// unrelased/unfinished maps, is just enough for explore,
					// trees had issue with it
					if (Zones::Version() == 446)
					{
						static std::unordered_map<std::uint16_t, std::uint16_t> mapped_constants = {
							{ 33, 31 },
							{ 34, 32 },
							{ 36, 34 },
							{ 39, 37 },
							{ 40, 38 },
							{ 42, 40 },
							{ 43, 41 },
							{ 45, 43 },
							{ 62, 52 },
							{ 63, 53 },
							{ 199, 58 },
							{ 259, 86 },
							{ 263, 90 },
							{ 271, 98 },
							{ 279, 106 },
						};

						const auto itr = mapped_constants.find(arg->u.codeConst.index);
						if (itr != mapped_constants.end())
						{
							arg->u.codeConst.index = itr->second;
						}
					}
					else if (Zones::Version() == 461)
					{
						static std::unordered_map<std::uint16_t, std::uint16_t> mapped_constants = 
						{
							// mp_raid
							{ 33, 31 },
							{ 34, 32 },
							{ 36, 34 },
							{ 39, 37 },	
							{ 40, 38 },
							{ 42, 40 },
							{ 43, 41 },
							{ 45, 43 },
							{ 62, 52 },
							{ 63, 53 },
							{ 197, 58 },
							{ 202, 63 },
							{ 203, 64 },
							{ 261, 90 },
							{ 265, 94 },
							{ 269, 98 },
							{ 277, 106 },

							// mp_dome
							{ 38, 36 },
							{ 40, 38 },
							{ 118, 86 },
						};

						const auto itr = mapped_constants.find(arg->u.codeConst.index);
						if (itr != mapped_constants.end())
						{
							arg->u.codeConst.index = itr->second;
						}
						if (arg->u.codeConst.index == 257)
						{
							auto techsetName = (*reinterpret_cast<Game::MaterialTechniqueSet**>(0x112AE8C))->name;

							// dont know if this applies to 460 too, but I dont have 460 files to test
							if (!strncmp(techsetName, "wc_unlit_add", 12) ||
								!strncmp(techsetName, "wc_unlit_multiply", 17) )
							{
								// fixes glass and water
								arg->u.codeConst.index = 116;
							}
							else
							{
								// anything else
								arg->u.codeConst.index = 86;
							}
						}
						else
						{
							// copy-paste from 460
							if (arg->u.codeConst.index >= 259)
							{
								arg->u.codeConst.index -= 171;
							}
							else if (arg->u.codeConst.index >= 197)
							{
								arg->u.codeConst.index -= 139;
							}
						}
					}
					else if (Zones::Version() == 460 /*|| Zones::Version() == 446*/)		// 446 is no longer compatible, needs correct mappings
					{
						static std::unordered_map<std::uint16_t, std::uint16_t> mapped_constants = {
							{ 22, 21 },
							{ 33, 31 },
							{ 34, 32 },
							{ 36, 34 },
							{ 37, 35 },
							{ 38, 36 },
							{ 39, 37 },
							{ 40, 38 },
							{ 41, 39 },
							{ 42, 40 },
							{ 43, 41 },
							{ 44, 42 },
							{ 45, 43 },
							{ 62, 52 },
							{ 63, 53 },

							// these might need fixes?
							{ 197, 58 },
							{ 198, 59 },
							{ 202, 63 },
							{ 203, 64 },
							{ 207, 68 },
							{ 252, 81 },
							{ 253, 82 },

							// these seem fine
							{ 261, 90 },
							{ 265, 94 },
							{ 269, 98 },
							{ 272, 101 },
							{ 273, 102 },
							{ 274, 103 },
							{ 277, 106 },
						};

						const auto itr = mapped_constants.find(arg->u.codeConst.index);
						if (itr != mapped_constants.end())
						{
							arg->u.codeConst.index = itr->second;
						}
						else
						{
							if (arg->u.codeConst.index == 257)
							{
								if (FastFiles::Current() != "mp_conflict" && FastFiles::Current() != "mp_derail_sh" && FastFiles::Current() != "mp_overwatch_sh" && 
									FastFiles::Current() != "mp_con_spring" && FastFiles::Current() != "mp_resistance_sh" && FastFiles::Current() != "mp_lookout_sh")
								{
									const auto varMaterialTechniqueSet = *reinterpret_cast<Game::MaterialTechniqueSet * *>(0x112AE8C);
									if (varMaterialTechniqueSet->name && !strncmp(varMaterialTechniqueSet->name, "mc_", 3))
									{
										// fixes trees
										arg->u.codeConst.index = 86;
									}
									else
									{
										// fixes black spots in the maps
										arg->u.codeConst.index = 128;
									}
								}
								//else
								/*{
									arg->u.codeConst.index = 134;
								}*/
							}
							else if (arg->u.codeConst.index >= 259)
							{
								arg->u.codeConst.index -= 171;
							}
							else if (arg->u.codeConst.index >= 197)
							{
								arg->u.codeConst.index -= 139;
							}
						}
					}
				}
			}
		}

		return result;
	}

	bool Zones::LoadStructuredDataStructPropertyArray(bool atStreamStart, char* data, int size)
	{
		int count = 0;

		if (Zones::ZoneVersion >= VERSION_ALPHA2)
		{
			size /= 16;
			count = size;
			size *= 24;
		}

		bool result = Game::Load_Stream(atStreamStart, data, size);

		if (Zones::ZoneVersion >= VERSION_ALPHA2)
		{
			for (int i = 0; i < count; ++i)
			{
				std::memmove(data + (i * 16), data + (i * 24), 16);
				AssetHandler::Relocate(data + (i * 24), data + (i * 16), 16);
			}
		}

		return result;
	}

	bool Zones::LoadGfxImage(bool atStreamStart, char* buffer, int size)
	{
		if (Zones::ZoneVersion >= 332)
		{
			size = (Zones::ZoneVersion >= 359) ? 52 : 36;
		}

		bool result = Game::Load_Stream(atStreamStart, buffer, size);

		if (Zones::ZoneVersion >= 332)
		{
			AssetHandler::Relocate(buffer + (size - 4), buffer + 28, 4);

			if (Zones::Version() >= 359)
			{
				struct
				{
					Game::GfxImageLoadDef* texture;
					char mapType;
					char semantic;
					char category;
					char flags;
					int cardMemory;
					char pad[8]; // ?
					int dataLen1;
					int dataLen2;
					char pad2[4]; // ?
					short width;
					short height;
					short depth;
					char loaded;
					char pad3[5];
					Game::GfxImageLoadDef* storedTexture;
					char* name;
				} image359;

				AssertSize(image359, 52);

				// Copy to new struct
				std::memcpy(&image359, buffer, sizeof(image359));

				// Convert to old struct
				Game::GfxImage* image = reinterpret_cast<Game::GfxImage*>(buffer);
				image->mapType = image359.mapType;
				image->semantic = image359.semantic;
				image->category = image359.category;
				image->useSrgbReads = image359.flags;
				//image->cardMemory = image359.cardMemory;
				//image->dataLen1 = image359.dataLen1;
				//image->dataLen2 = image359.dataLen2;
				std::memcpy(image->picmip.platform, &image359.cardMemory, sizeof(int) * 3);
				image->height = image359.height;
				image->width = image359.width;
				image->depth = image359.depth;
				image->delayLoadPixels = image359.loaded;
				image->name = image359.name;

				FixImageCategory(image);

				// Used for later stuff
				(&image->delayLoadPixels)[1] = image359.pad3[1];
			}
			else
			{
				std::memcpy(buffer + 28, buffer + (size - 4), 4);

				Game::GfxImage* image = reinterpret_cast<Game::GfxImage*>(buffer);
				FixImageCategory(image);
			}
		}

		return result;
	}

	void Zones::FixImageCategory(Game::GfxImage* image) {
		// CODO makes use of additional enumerator values (9, 10, 11) that don't exist in IW4
		// We have to translate them. 9 is for Reflection probes,  11 is for Compass,  10 is for Lightmap
		switch (image->category)
		{
			case 9:
				image->category = Game::ImageCategory::IMG_CATEGORY_AUTO_GENERATED;
				break;
			case 10:
				image->category = Game::ImageCategory::IMG_CATEGORY_LIGHTMAP;
				break;
			case 11:
				image->category = Game::ImageCategory::IMG_CATEGORY_LOAD_FROM_FILE;
				break;
		}


		if (image->category > 7 || image->category < 0) {

#ifdef DEBUG
			if (IsDebuggerPresent()) __debugbreak();
#endif
		}
	}

	bool Zones::LoadXAsset(bool atStreamStart, char* buffer, int size)
	{
		int count = 0;

		if (Zones::ZoneVersion >= 334)
		{
			size /= 8;
			count = size;
			size *= 16;
		}

		bool result = Game::Load_Stream(atStreamStart, buffer, size);

		if (Zones::ZoneVersion >= 334)
		{
			for (int i = 0; i < count; ++i)
			{
				std::memmove(buffer + (i * 8), buffer + (i * 16), 4);
				std::memmove(buffer + (i * 8) + 4, buffer + (i * 16) + 8, 4);

				if (Zones::Version() >= 423)
				{
					// don't read assets that are unused by codol, for some retarded reason their header is written in the FF anyway
					if (*reinterpret_cast<int*>(buffer + (i * 8)) == Game::XAssetType::ASSET_TYPE_CLIPMAP_SP || 
						*reinterpret_cast<int*>(buffer + (i * 8)) == Game::XAssetType::ASSET_TYPE_GAMEWORLD_SP || 
						*reinterpret_cast<int*>(buffer + (i * 8)) == Game::XAssetType::ASSET_TYPE_GAMEWORLD_MP)
					{
						*reinterpret_cast<int*>(buffer + (i * 8)) = Game::XAssetType::ASSET_TYPE_UI_MAP;
						*reinterpret_cast<int**>(buffer + (i * 8) + 4) = nullptr;
					}
				}
				
				AssetHandler::Relocate(buffer + (i * 16), buffer + (i * 8) + 0, 4);
				AssetHandler::Relocate(buffer + (i * 16) + 8, buffer + (i * 8) + 4, 4);
			}
		}

		return result;
	}

	bool Zones::LoadMaterialTechnique(bool atStreamStart, char* buffer, int size)
	{
		// 359 and above adds an extra remapped techset ptr
		if (Zones::ZoneVersion >= 359) size += 4;
		// 446 amd above adds an additional technique
		if (Zones::ZoneVersion >= 446) size += 4;
		
		bool result = Game::Load_Stream(atStreamStart, buffer, size);

		if (Zones::ZoneVersion >= 359)
		{
			// This shouldn't make any difference.
			// The new entry is an additional remapped techset which is linked at runtime.
			// It's used when the 0x100 gameFlag in a material is set.
			// As MW2 flags are only 1 byte large, this won't be possible anyways
			int shiftTest = 4;

			std::memmove(buffer + 8 + shiftTest, buffer + 12 + shiftTest, (Zones::Version() >= 446) ? 200 : 196 - shiftTest);
			AssetHandler::Relocate(buffer + 12 + shiftTest, buffer + 8 + shiftTest, (Zones::Version() >= 446) ? 200 : 196 - shiftTest);
		}

		return result;
	}
	int Zones::LoadMaterialTechniqueArray(bool atStreamStart, int count)
	{
		if (Zones::Version() >= 446)
		{
			count += 1;
		}

		auto retval = Utils::Hook::Call<int(bool, int)>(0x497020)(atStreamStart, count);

		if (Zones::Version() >= 446)
		{
			auto lastTechnique = **reinterpret_cast<Game::MaterialTechnique * **>(0x112AEDC);
			auto varMaterialTechniqueSet = **reinterpret_cast<Game::MaterialTechniqueSet * **>(0x112B070);

			// patch last technique to match iw4
			varMaterialTechniqueSet->techniques[47] = lastTechnique;
		}

		return retval;
	}

	bool Zones::LoadMaterial(bool atStreamStart, char* buffer, int size)
	{
		// if (Zones::ZoneVersion >= 446 && currentAssetType == Game::ASSET_TYPE_XMODEL) __debugbreak();

		bool result = Game::Load_Stream(atStreamStart, buffer, (Zones::Version() >= 446) ? 104 : size);

		if (Zones::Version() >= 446)
		{
			char codol_material[104];
			memcpy(codol_material, buffer, 104);

			// move material data
			std::memmove(buffer, codol_material + 0x10, 4);
			std::memmove(buffer + 0x11, codol_material + 0x14, 6);
			std::memmove(buffer + 4, codol_material + 0x1A, 1);
			std::memmove(buffer + 5, codol_material + 0x1C, 3);
			std::memmove(buffer + 8, codol_material, 16);
			std::memmove(buffer + 0x18, codol_material + 0x20, 0x30);
			std::memmove(buffer + 0x48, codol_material + 0x51, 5);
			std::memmove(buffer + 0x50, codol_material + 0x58, 0x10);

			std::memset(buffer + 0x48 + 5, 0, 3);
			
			// relocate pointers
			AssetHandler::Relocate(buffer + 10, buffer, 4);
			AssetHandler::Relocate(buffer + 0x1B, buffer + 4, 4);
			AssetHandler::Relocate(buffer, buffer + 8, 16);
			AssetHandler::Relocate(buffer + 0x20, buffer + 0x18, 0x30);
			AssetHandler::Relocate(buffer + 0x51, buffer + 0x48, 5);
			AssetHandler::Relocate(buffer + 0x58, buffer + 0x50, 0x10);

			// fix statebit
			reinterpret_cast<Game::Material*>(buffer)->stateBitsEntry[47] = codol_material[0x50];
		}
		else if (Zones::ZoneVersion >= 359)
		{			
			struct material339_s
			{
				char drawSurfBegin[8]; // 4
				//int surfaceTypeBits;
				const char *name;
				char drawSurf[6];

				union
				{
					char gameFlags;
					short sGameFlags;
				};
				char sortKey;
				char textureAtlasRowCount;
				char textureAtlasColumnCount;
			} material359;

			static_assert(offsetof(material339_s, gameFlags) == 18, "");
			static_assert(offsetof(material339_s, sortKey) == 20, "");
			static_assert(offsetof(material339_s, textureAtlasColumnCount) == 22, "");

			static_assert(offsetof(Game::Material, stateBitsEntry) == 24, "");

			Game::Material* material = reinterpret_cast<Game::Material*>(buffer);
			memcpy(&material359, material, sizeof(material359));

			material->info.name = material359.name;
			material->info.sortKey = material359.sortKey;
			material->info.textureAtlasRowCount = material359.textureAtlasRowCount;
			material->info.textureAtlasColumnCount = material359.textureAtlasColumnCount;
			material->info.gameFlags = material359.gameFlags;

			// Probably wrong
			material->info.surfaceTypeBits = 0;//material359.surfaceTypeBits;

			// Pretty sure that's wrong
			// Actually, it's not
			// yes it was lol
			memcpy(&material->info.drawSurf.packed, material359.drawSurfBegin, 8);

			memcpy(&material->info.surfaceTypeBits, &material359.drawSurf[0], 6); // copies both surfaceTypeBits and hashIndex
			//material->drawSurf[8] = material359.drawSurf[0];
			//material->drawSurf[9] = material359.drawSurf[1];
			//material->drawSurf[10] = material359.drawSurf[2];
			//material->drawSurf[11] = material359.drawSurf[3];

			if (material359.sGameFlags & 0x100)
			{
				//OutputDebugStringA("");
			}
		}

		return result;
	}

	int gfxLightMapExtraCount = 0;
	int* gfxLightMapExtraPtr1 = nullptr;
	int* gfxLightMapExtraPtr2 = nullptr;
	
	bool Zones::LoadGfxWorld(bool atStreamStart, char* buffer, int size)
	{
		gfxLightMapExtraPtr1 = nullptr;
		gfxLightMapExtraPtr2 = nullptr;

		if (Zones::Version() >= 460) size += 984;
		else if (Zones::Version() >= 423) size += 980;
		else if (Zones::Version() >= 359) size += 968;
		
		bool result = Game::Load_Stream(atStreamStart, buffer, size);

		// fix 4 byte difference in FF ver 460+ this turns 460+ into 423
		if (Zones::Version() >= 460)
		{
			std::memmove(buffer + 0x34, buffer + 0x38, size - 0x38);
			AssetHandler::Relocate(buffer + 0x38, buffer + 0x34, size - 0x38);
		}
		
		// fix 12 byte difference in FF ver 423+, this turns 423+ into 359
		if (Zones::Version() >= 423)
		{
			// store extra pointers we just yeeted out of the structure
			gfxLightMapExtraCount = *reinterpret_cast<int*>(buffer + 0x50 + 0x10);
			gfxLightMapExtraPtr1 = *reinterpret_cast<int**>(buffer + 0x50 + 0x14);
			gfxLightMapExtraPtr2 = *reinterpret_cast<int**>(buffer + 0x50 + 0x18);

			// move gfxworld into 359 format
			std::memmove(buffer + 0x50 + 0x10, buffer + 0x50 + 0x1C, size - 0x6C);
			AssetHandler::Relocate(buffer + 0x50 + 0x1C, buffer + 0x50 + 0x10, size - 0x6C);
		}
				
		if (Zones::Version() >= 359)
		{
			int sunDiff = 8;	// Stuff that is part of the sunflare we would overwrite
			std::memmove(buffer + 348 + sunDiff, buffer + 1316 + sunDiff, 280 - sunDiff);
			AssetHandler::Relocate(buffer + 1316, buffer + 348, 280);
		}

		return result;
	}

	void Zones::Loadsunflare_t(bool atStreamStart)
	{
		Game::Load_MaterialHandle(atStreamStart);

		if (Zones::ZoneVersion >= 359)
		{
			char* varsunflare_t = *reinterpret_cast<char**>(0x112A848);

			*Game::varMaterialHandle = reinterpret_cast<Game::Material**>(varsunflare_t + 12);
			Game::Load_MaterialHandle(atStreamStart);

			*Game::varMaterialHandle = reinterpret_cast<Game::Material**>(varsunflare_t + 16);
			Game::Load_MaterialHandle(atStreamStart);

			std::memmove(varsunflare_t + 12, varsunflare_t + 20, 84);

			// Copy the remaining struct data we couldn't copy in LoadGfxWorld
			char* varGfxWorld = *reinterpret_cast<char**>(0x112A7F4);
			std::memmove(varGfxWorld + 348, varGfxWorld + 1316, 8);
		}
	}

	bool Zones::LoadStatement(bool atStreamStart, char* buffer, int size)
	{
		if (Zones::Version() >= 359) size -= 4;

		bool result = Game::Load_Stream(atStreamStart, buffer, size);

		if (Zones::Version() >= 359)
		{
			std::memmove(buffer + 12, buffer + 8, 12);
		}

		return result;
	}

	void Zones::LoadWindowImage(bool atStreamStart)
	{
		Game::Load_MaterialHandle(atStreamStart);

		if (Zones::Version() >= 360)
		{
			char** varGfxImagePtr = reinterpret_cast<char**>(0x112B4A0);
			char** varwindowDef_t = reinterpret_cast<char**>(0x112AF94);

			*varGfxImagePtr = *varwindowDef_t + 164;
			Game::Load_GfxImagePtr(atStreamStart);
		}
	}

	void Zones::LoadPhysPreset(bool atStreamStart, char* buffer, int size)
	{
		if (Zones::Version() >= VERSION_ALPHA2)
		{
			size = 68;
		}

		Game::Load_Stream(atStreamStart, buffer, size);
	}

	void Zones::LoadXModelSurfs(bool atStreamStart, char* buffer, int size)
	{
		if (Zones::Version() >= VERSION_ALPHA2)
		{
			size = 48;
		}

		Game::Load_Stream(atStreamStart, buffer, size);
	}

	void Zones::LoadImpactFx(bool atStreamStart, char* buffer, int size)
	{
		if (Zones::Version() >= 460) size = 0xB94;
		else if (Zones::Version() >= 446) size = 0xA64;
		else if (Zones::Version() >= VERSION_ALPHA2) size = 0x8C0;

		Game::Load_Stream(atStreamStart, buffer, size);

		if (Zones::Version() >= 460)
		{
			for (auto i = 0; i < Zones::ImpactFxArrayCount(); i++)
			{
				std::memmove(buffer + (i * 140), buffer + (i * 156), 140);
				AssetHandler::Relocate(buffer + (i * 156), buffer + (i * 140), 140);
			}
		}
	}

	int Zones::ImpactFxArrayCount()
	{
		if (Zones::Version() >= 446)
		{
			return 19;
		}
		
		if (Zones::Version() >= VERSION_ALPHA2)
		{
			return 16;
		}

		return 15;
	}

	__declspec(naked) void Zones::LoadImpactFxArray()
	{
		__asm
		{
			push edi
			pushad

			push edi
			call Zones::ImpactFxArrayCount
			pop edi

			mov [esp + 20h], eax

			popad
			pop edi

			push 4447E0h
			retn
		}
	}

	void Zones::LoadPathNodeArray(bool atStreamStart, char* buffer, int size)
	{
		if (Zones::Version() >= VERSION_ALPHA2)
		{
			size /= 136;
			size *= 148;
		}

		Game::Load_Stream(atStreamStart, buffer, size);
	}

	void Zones::LoadPathnodeConstantTail(bool atStreamStart, char* buffer, int size)
	{
		if (Zones::Version() >= VERSION_ALPHA2)
		{
			size /= 12;
			size *= 16;
		}

		Game::Load_Stream(atStreamStart, buffer, size);
	}

	void Zones::LoadExpressionSupportingDataPtr(bool atStreamStart)
	{
		if (Zones::Version() < 359)
		{
			Utils::Hook::Call<void(bool)>(0x4AF680)(atStreamStart);
		}
	}

	void Zones::SetVersion(int version)
	{
		AssetHandler::ClearRelocations();
		Zones::ZoneVersion = version;
	}

	__declspec(noinline) bool Zones::CheckGameMapSp(int type)
	{
		if (type == Game::XAssetType::ASSET_TYPE_GAMEWORLD_SP)
		{
			return true;
		}

		if (Zones::Version() >= VERSION_ALPHA2 && Zones::Version() < 423 && type == Game::XAssetType::ASSET_TYPE_GAMEWORLD_MP)
		{
			Maps::HandleAsSPMap();
			return true;
		}

		return false;
	}

	__declspec(naked) void Zones::GameMapSpPatchStub()
	{
		__asm
		{
			pushad

			push eax
			call Zones::CheckGameMapSp
			add esp, 4h

			test al, al
			jnz returnSafe

			popad
			push 4189AEh
			retn

		returnSafe:
			popad
			push 41899Dh
			retn
		}
	}

	int Zones::PathDataSize()
	{
		if (Zones::Version() >= VERSION_ALPHA2)
		{
			return 148;
		}

		return 136;
	}

	__declspec(naked) void Zones::LoadPathDataConstant()
	{
		__asm
		{
			push esi
			pushad

			call Zones::PathDataSize

			add [esp + 20h], eax

			popad
			pop esi

			push 4D6A4Dh
			retn
		}
	}
	
	__declspec(naked) void Zones::GetCurrentAssetTypeStub()
	{
		__asm pushad;
		previousAssetType = currentAssetType;
		__asm popad;

		__asm
		{
			// get asset type
			mov eax, ds:0x112aa54
			mov eax, [eax];
			
			// log asset type
			// mov previousAssetType, currentAssetType;
			mov currentAssetType, eax;

			// go back
			push 0x418847;
			retn;
		}
	}
	int Zones::LoadRandomFxGarbage(bool atStreamStart, char* buffer, int size)
	{
		int count = 0;
		if (Zones::Version() >= 446)
		{
			size /= 48;
			count = size;
			size *= 64;
		}

		const auto retval = Game::Load_Stream(atStreamStart, buffer, size);

		if (Zones::Version() >= 446)
		{
			for (auto i = 0; i < count; i++)
			{	
				std::memcpy(buffer + (48 * i) + 0, buffer + (64 * i) + 0, 24);
				std::memcpy(buffer + (48 * i) + 24, buffer + (64 * i) + 32, 24);
				
				AssetHandler::Relocate(buffer + (64 * i) + 0, buffer + (48 * i) + 0, 24);
				AssetHandler::Relocate(buffer + (64 * i) + 32, buffer + (48 * i) + 24, 24);
			}
		}

		return retval;
	}

	int currentGfxSurfaceIndex = 0;
	std::vector<std::pair<int, int*>> gfxSurfaceExtraData;
	
	int Zones::LoadGfxXSurfaceArray(bool atStreamStart, char* buffer, int size)
	{
		currentGfxSurfaceIndex = 0;
		gfxSurfaceExtraData.clear();

		int count = 0;
		
		if (Zones::Version() >= 423)
		{
			size /= 40;
			count = size;
			size *= 48;
		}

		const auto retval = Game::Load_Stream(atStreamStart, buffer, size);

		if (Zones::Version() >= 423)
		{
			// fix structure
			for (auto i = 0; i < count; i++)
			{
				auto read_count = *reinterpret_cast<int*>(buffer + (48 * i) + 40);
				auto read_ptr = *reinterpret_cast<int**>(buffer + (48 * i) + 44);

				// extra data stuff we need to load
				gfxSurfaceExtraData.push_back({
					read_count,
					read_ptr
				});

				// fix structure
				std::memmove(buffer + (40 * i), buffer + (48 * i), 40);
				AssetHandler::Relocate(buffer + (48 * i), buffer + (40 * i), 40);
			}
		}

		return retval;
	}

	int Zones::LoadGfxXSurfaceExtraData(bool atStreamStart)
	{
		const auto retval = Utils::Hook::Call<int(bool)>(0x4516B0)(atStreamStart);

		if (Zones::Version() >= 423)
		{
			const auto currentData = &gfxSurfaceExtraData[currentGfxSurfaceIndex];
			if (currentData->second)
			{
				currentData->second = reinterpret_cast<int*>(Game::DB_AllocStreamPos(0));
				Game::Load_Stream(true, currentData->second, currentData->first);
			}
			currentGfxSurfaceIndex++;
		}

		return retval;
	}

	int Zones::LoadGfxReflectionProbes(bool atStreamStart, char* buffer, int size)
	{
		int count = 0;
		
		if (Zones::Version() >= 446)
		{
			size /= 12;
			count = size;
			size *= 20;
		}

		auto retval = Game::Load_Stream(atStreamStart, buffer, size);

		if (Zones::Version() >= 446)
		{
			for (int i = 0; i < count; i++)
			{
				auto garbage = *reinterpret_cast<char**>(buffer + (20 * i) + 12);
				auto garbage_count = *reinterpret_cast<int*>(buffer + (20 * i) + 16);
				
				if (garbage != nullptr)
				{
					garbage = Game::DB_AllocStreamPos(3);
					Game::Load_Stream(true, garbage, 8 * garbage_count);

					for (int j = 0; j < garbage_count; j++)
					{
						auto garbage_2 = *reinterpret_cast<char**>(garbage + (8 * j) + 0);
						auto garbage_2_count = *reinterpret_cast<int*>(garbage + (8 * j) + 4);

						if (garbage_2)
						{
							garbage_2 = Game::DB_AllocStreamPos(1);
							Game::Load_Stream(true, garbage_2, 2 * garbage_2_count);
						}
					}
				}

				std::memmove(buffer + (12 * i), buffer + (20 * i), 12);
				AssetHandler::Relocate(buffer + (20 * i), buffer + (12 * i), 12);
			}
		}

		return retval;
	}

	void Zones::LoadGfxLightMapExtraData()
	{
		Game::DB_PopStreamPos();

		if (Zones::Version() >= 423)
		{
			if (gfxLightMapExtraPtr1)
			{
				gfxLightMapExtraPtr1 = reinterpret_cast<int*>(Game::DB_AllocStreamPos(3));
				Game::Load_Stream(true, gfxLightMapExtraPtr1, 12 * gfxLightMapExtraCount);
			}

			if (gfxLightMapExtraPtr2)
			{
				gfxLightMapExtraPtr2 = reinterpret_cast<int*>(Game::DB_AllocStreamPos(0));
				Game::Load_Stream(true, gfxLightMapExtraPtr2, gfxLightMapExtraCount);
			}
		}
	}

	__declspec(naked) void Zones::LoadXModelColSurfPtr()
	{
		static auto DB_ConvertOffsetToPointer_Address = 0x4A82B0;
		
		__asm
		{
			cmp dword ptr[eax], 0;
			je dontLoadAssetData;

			cmp dword ptr[eax], 0xFFFFFFFF;
			je loadAssetData;

			// check if FF is below 446, still load data in that case
			cmp Zones::ZoneVersion, 446;
			jl loadAssetData;

			// offset to pointer magic
			pushad;
			push eax;
			call DB_ConvertOffsetToPointer_Address;
			add esp, 4;
			popad;

		dontLoadAssetData:
			push 0x4C870E;
			retn;

		loadAssetData:
			push 0x4C86DD;
			retn;
		}
	}

	struct ClipInfo
	{
		int numCPlanes;
		Game::cplane_s* cPlanes;
		int numMaterials;
		Game::ClipMaterial* materials;
		int numCBrushSides;
		Game::cbrushside_t* cBrushSides;
		int numCBrushEdges;
		char* cBrushEdges;
		int numCLeafBrushNodes;
		Game::cLeafBrushNode_s* cLeafBrushNodes;			// cmodels use this?
		int numLeafBrushes;
		short* leafBrushes;
		unsigned short numBrushes;
		Game::cbrush_t* brushes;
		Game::Bounds* brushBounds;
		int* brushContents;
	};
	struct codolCmodel_t
	{
		Game::Bounds bounds;
		float radius;
		ClipInfo* infoPtr;
		Game::cLeaf_t leaf;
	};
	struct codolClipMap_t
	{
		char* name;
		bool isInUse;
		char pad1[3];
		ClipInfo info;
		ClipInfo* pInfo;
		int numStaticModels;
		Game::cStaticModel_s* staticModelList;
		int numCNodes;
		Game::cNode_t* cNodes;
		int numCLeaf;
		Game::cLeaf_t* cLeaf;
		int numVerts;
		float(*verts)[3];
		int numTriIndices;
		short* triIndices;
		char* triEdgeIsWalkable; //Size = ((triCount << 1) + triCount + 0x1F) >> 3 << 2
		int numCollisionBorders;
		Game::CollisionBorder* collisionBorders;
		int numCollisionPartitions;
		Game::CollisionPartition* collisionPartitions;
		int numCollisionAABBTrees;
		Game::CollisionAabbTree* collisionAABBTrees;
		int numCModels;
		codolCmodel_t* cModels;
		Game::MapEnts* mapEnts;
		Game::Stage* stages;
		unsigned char stageCount;
		char pad2[3];
		Game::MapTriggers trigger;
		short smodelNodeCount;
		Game::SModelAabbNode* smodelNodes;
		unsigned short dynEntCount[2];
		Game::DynEntityDef* dynEntDefList[2];
		Game::DynEntityPose* dynEntPoseList[2];
		Game::DynEntityClient* dynEntClientList[2];
		Game::DynEntityColl* dynEntCollList[2];
		char pad3[20];
		std::uint32_t isPlutoniumMap;
	};

	static Game::MapEnts codolMapEnts;
	static Game::MapEnts* codolMapEntsPtr;
	
	int Zones::LoadMapEnts(bool atStreamStart, Game::MapEnts* buffer, int size)
	{
		EntitiesVersion = Zones::Version();

		if (Zones::Version() >= 446)
		{
			size /= 44;
			size *= 36;

			memset(&codolMapEnts, 0, sizeof Game::MapEnts);

			buffer = &codolMapEnts;
			codolMapEntsPtr = &codolMapEnts;

			*reinterpret_cast<Game::MapEnts**>(0x112B3E8) = &codolMapEnts;		// varMapEnts
			*reinterpret_cast<Game::MapEnts***>(0x112B388) = &codolMapEntsPtr;	// varMapEntsPtr

			AssetHandler::Relocate(&codolMapEnts, buffer, size);
		}

		return Game::Load_Stream(atStreamStart, buffer, size);
	}
	
	ClipInfo* varClipInfoPtr;
	void Zones::Load_ClipInfo(bool atStreamStart)
	{
		AssertSize(ClipInfo, 64);
		AssertSize(Game::cplane_s, 20);
		AssertSize(Game::Bounds, 24);
		
		Game::Load_Stream(atStreamStart, varClipInfoPtr, sizeof ClipInfo);
		
		if (varClipInfoPtr->cPlanes)
		{
			if (varClipInfoPtr->cPlanes == reinterpret_cast<Game::cplane_s*>(0xFFFFFFFF))
			{
				varClipInfoPtr->cPlanes = reinterpret_cast<Game::cplane_s*>(Game::DB_AllocStreamPos(3));
				Game::Load_Stream(true, varClipInfoPtr->cPlanes, varClipInfoPtr->numCPlanes * sizeof(Game::cplane_s));
			}
			else
			{
				Game::DB_ConvertOffsetToPointer(&varClipInfoPtr->cPlanes);
			}
		}

		if (varClipInfoPtr->materials)
		{
			if (varClipInfoPtr->materials == reinterpret_cast<Game::ClipMaterial*>(0xFFFFFFFF))
			{
				varClipInfoPtr->materials = reinterpret_cast<Game::ClipMaterial*>(Game::DB_AllocStreamPos(3));
				*reinterpret_cast<Game::ClipMaterial**>(0x112A958) = varClipInfoPtr->materials;
				Utils::Hook::Call<void(bool, int)>(0x4895F0)(true, varClipInfoPtr->numMaterials);
			}
			else
			{
				Game::DB_ConvertOffsetToPointer(&varClipInfoPtr->materials);
			}
		}

		if (varClipInfoPtr->cBrushSides)
		{
			if (varClipInfoPtr->cBrushSides == reinterpret_cast<Game::cbrushside_t*>(0xFFFFFFFF))
			{
				varClipInfoPtr->cBrushSides = reinterpret_cast<Game::cbrushside_t*>(Game::DB_AllocStreamPos(3));
				*reinterpret_cast<Game::cbrushside_t**>(0x112B33C) = varClipInfoPtr->cBrushSides;
				Utils::Hook::Call<void(bool, int)>(0x420790)(true, varClipInfoPtr->numCBrushSides);
			}
			else
			{
				Game::DB_ConvertOffsetToPointer(&varClipInfoPtr->cBrushSides);
			}
		}

		if (varClipInfoPtr->cBrushEdges)
		{
			if (varClipInfoPtr->cBrushEdges == reinterpret_cast<char*>(0xFFFFFFFF))
			{
				varClipInfoPtr->cBrushEdges = reinterpret_cast<char*>(Game::DB_AllocStreamPos(0));
				Game::Load_Stream(true, varClipInfoPtr->cBrushEdges, varClipInfoPtr->numCBrushEdges);
			}
			else
			{
				Game::DB_ConvertOffsetToPointer(&varClipInfoPtr->cBrushEdges);
			}
		}

		if (varClipInfoPtr->cLeafBrushNodes)
		{
			if (varClipInfoPtr->cLeafBrushNodes == reinterpret_cast<Game::cLeafBrushNode_s*>(0xFFFFFFFF))
			{			
				varClipInfoPtr->cLeafBrushNodes = reinterpret_cast<Game::cLeafBrushNode_s*>(Game::DB_AllocStreamPos(3));
				*reinterpret_cast<Game::cLeafBrushNode_s**>(0x112B130) = varClipInfoPtr->cLeafBrushNodes;
				Utils::Hook::Call<void(bool, int)>(0x4C29D0)(true, varClipInfoPtr->numCLeafBrushNodes);
			}
			else
			{
				Game::DB_ConvertOffsetToPointer(&varClipInfoPtr->cLeafBrushNodes);
			}
		}

		if (varClipInfoPtr->leafBrushes)
		{
			if (varClipInfoPtr->leafBrushes == reinterpret_cast<short*>(0xFFFFFFFF))
			{
				varClipInfoPtr->leafBrushes = reinterpret_cast<short*>(Game::DB_AllocStreamPos(1));
				Game::Load_Stream(true, varClipInfoPtr->leafBrushes, varClipInfoPtr->numLeafBrushes * sizeof(short));
			}
			else
			{
				Game::DB_ConvertOffsetToPointer(&varClipInfoPtr->leafBrushes);
			}
		}

		AssertOffset(ClipInfo, numBrushes, 48);
		AssertOffset(ClipInfo, brushes, 52);
		AssertSize(Game::cbrush_t, 36);
		if (varClipInfoPtr->brushes)
		{
			if (varClipInfoPtr->brushes == reinterpret_cast<Game::cbrush_t*>(0xFFFFFFFF))
			{
				varClipInfoPtr->brushes = reinterpret_cast<Game::cbrush_t*>(Game::DB_AllocStreamPos(127));
				*reinterpret_cast<Game::cbrush_t**>(0x112B2B8) = varClipInfoPtr->brushes;
				Utils::Hook::Call<void(bool, unsigned int)>(0x4B4160)(true, varClipInfoPtr->numBrushes);
			}
			else
			{
				Game::DB_ConvertOffsetToPointer(&varClipInfoPtr->brushes);
			}
		}

		AssertOffset(ClipInfo, brushBounds, 56);
		if (varClipInfoPtr->brushBounds)
		{
			if (varClipInfoPtr->brushBounds == reinterpret_cast<Game::Bounds*>(0xFFFFFFFF))
			{
				varClipInfoPtr->brushBounds = reinterpret_cast<Game::Bounds*>(Game::DB_AllocStreamPos(127));
				Game::Load_Stream(true, varClipInfoPtr->brushBounds, varClipInfoPtr->numBrushes * sizeof(Game::Bounds));
			}
			else
			{
				Game::DB_ConvertOffsetToPointer(&varClipInfoPtr->brushBounds);
			}
		}

		AssertOffset(ClipInfo, brushContents, 60);
		if (varClipInfoPtr->brushContents)
		{
			if (varClipInfoPtr->brushContents == reinterpret_cast<int*>(0xFFFFFFFF))
			{
				varClipInfoPtr->brushContents = reinterpret_cast<int*>(Game::DB_AllocStreamPos(3));
				Game::Load_Stream(true, varClipInfoPtr->brushContents, 4 * varClipInfoPtr->numBrushes);
			}
			else
			{
				Game::DB_ConvertOffsetToPointer(&varClipInfoPtr->brushContents);
			}
		}
	}
	
	int Zones::LoadClipMap(bool atStreamStart)
	{
		if (Zones::Version() >= 446)
		{			
			AssertOffset(codolClipMap_t, pInfo, 72);
			
			AssertSize(Game::cStaticModel_s, 76);
			AssertOffset(codolClipMap_t, numStaticModels, 76);
			AssertOffset(codolClipMap_t, staticModelList, 80);

			auto varClipMap = *reinterpret_cast<codolClipMap_t**>(0x112A758);
			Game::Load_Stream(atStreamStart, varClipMap, 256);

			Game::DB_PushStreamPos(3);
			
			*Game::varXString = &varClipMap->name;
			Game::Load_XString(false);

			varClipInfoPtr = &varClipMap->info;
			Load_ClipInfo(false);

			Game::DB_PushStreamPos(0);
			
			varClipInfoPtr = varClipMap->pInfo;
			ClipInfo** assetPointer = nullptr;
			if (varClipMap->pInfo)
			{
				if (varClipMap->pInfo == reinterpret_cast<ClipInfo*>(0xFFFFFFFF) || 
					varClipMap->pInfo == reinterpret_cast<ClipInfo*>(0xFFFFFFFE))
				{
					const auto needsToAllocPointer = varClipMap->pInfo == reinterpret_cast<ClipInfo*>(0xFFFFFFFE);

					varClipMap->pInfo = reinterpret_cast<ClipInfo*>(Game::DB_AllocStreamPos(3));

					if (needsToAllocPointer)		// 0xFFFFFFFE
					{
						assetPointer = Game::DB_InsertPointer<ClipInfo>();
					}

					varClipInfoPtr = varClipMap->pInfo;
					Load_ClipInfo(true);

					// varClipMap->pInfo = &varClipMap->info;
					if (assetPointer)
					{
						*assetPointer = varClipMap->pInfo;
					}
				}
			}
			Game::DB_PopStreamPos();

			if (varClipMap->staticModelList)
			{
				varClipMap->staticModelList = reinterpret_cast<Game::cStaticModel_s*>(Game::DB_AllocStreamPos(3));
				*reinterpret_cast<Game::cStaticModel_s**>(0x112B0E4) = varClipMap->staticModelList;
				Utils::Hook::Call<void(bool, int)>(0x4B7440)(true, varClipMap->numStaticModels);
			}

			if (varClipMap->cNodes)
			{
				varClipMap->cNodes = reinterpret_cast<Game::cNode_t*>(Game::DB_AllocStreamPos(3));
				*reinterpret_cast<Game::cNode_t**>(0x112B1D0) = varClipMap->cNodes;
				Utils::Hook::Call<void(bool, int)>(0x4E65A0)(true, varClipMap->numCNodes);
			}

			if (varClipMap->cLeaf)
			{
				varClipMap->cLeaf = reinterpret_cast<Game::cLeaf_t*>(Game::DB_AllocStreamPos(3));
				Game::Load_Stream(true, varClipMap->cLeaf, sizeof(Game::cLeaf_t) * varClipMap->numCLeaf);
			}

			if (varClipMap->verts)
			{
				varClipMap->verts = reinterpret_cast<float(*)[3]>(Game::DB_AllocStreamPos(3));
				Game::Load_Stream(true, varClipMap->verts, 12 * varClipMap->numVerts);
			}

			if (varClipMap->triIndices)
			{
				varClipMap->triIndices = reinterpret_cast<short*>(Game::DB_AllocStreamPos(1));
				Game::Load_Stream(true, varClipMap->triIndices, sizeof(short) * varClipMap->numTriIndices * 3);
			}

			if (varClipMap->triEdgeIsWalkable)
			{
				varClipMap->triEdgeIsWalkable = static_cast<char*>(Game::DB_AllocStreamPos(0));
#pragma warning(push)
#pragma warning(disable: 4554)
				Game::Load_Stream(true, varClipMap->triEdgeIsWalkable, 0x1F + varClipMap->numTriIndices * 3 >> 3 & 0xFFFFFFFC);
#pragma warning(pop)
			}

			if (varClipMap->collisionBorders)
			{
				varClipMap->collisionBorders = reinterpret_cast<Game::CollisionBorder*>(Game::DB_AllocStreamPos(3));
				Game::Load_Stream(true, varClipMap->collisionBorders, varClipMap->numCollisionBorders * sizeof(Game::CollisionBorder));
			}

			if (varClipMap->collisionPartitions)
			{
				varClipMap->collisionPartitions = reinterpret_cast<Game::CollisionPartition*>(Game::DB_AllocStreamPos(3));
				*reinterpret_cast<Game::CollisionPartition**>(0x112AA38) = varClipMap->collisionPartitions;
				Utils::Hook::Call<void(bool, int)>(0x444AF0)(true, varClipMap->numCollisionPartitions);
			}

			AssertSize(Game::CollisionAabbTree, 32);
			if (varClipMap->collisionAABBTrees)
			{
				varClipMap->collisionAABBTrees = reinterpret_cast<Game::CollisionAabbTree*>(Game::DB_AllocStreamPos(15));
				Game::Load_Stream(true, varClipMap->collisionAABBTrees, varClipMap->numCollisionAABBTrees * sizeof(Game::CollisionAabbTree));
			}

			AssertSize(Game::cmodel_t, 68);
			AssertSize(codolCmodel_t, 72);
			AssertOffset(codolCmodel_t, infoPtr, 28);
			if (varClipMap->cModels)
			{
				varClipMap->cModels = reinterpret_cast<codolCmodel_t*>(Game::DB_AllocStreamPos(3));
				Game::Load_Stream(true, varClipMap->cModels, sizeof(codolCmodel_t) * varClipMap->numCModels);

				// read garbage iw5 data we don't need in iw4
				for (auto i = 0; i < varClipMap->numCModels; i++)
				{
					Game::DB_PushStreamPos(0);

					varClipInfoPtr = varClipMap->cModels[i].infoPtr;
					if (varClipInfoPtr)
					{
						if (varClipInfoPtr == reinterpret_cast<ClipInfo*>(0xFFFFFFFF) ||
							varClipInfoPtr == reinterpret_cast<ClipInfo*>(0xFFFFFFFE))
						{
							const auto needsToAllocPointer = varClipMap->pInfo == reinterpret_cast<ClipInfo*>(0xFFFFFFFE);
							ClipInfo** info = nullptr;

							varClipInfoPtr = reinterpret_cast<ClipInfo*>(Game::DB_AllocStreamPos(3));

							if (needsToAllocPointer)		// 0xFFFFFFFE
							{
								info = Game::DB_InsertPointer<ClipInfo>();
							}

							Load_ClipInfo(true);

							if (info)
							{
								*info = varClipInfoPtr;
							}
						}
					}
					
					Game::DB_PopStreamPos();
				}

				// fix cmodels
				std::vector<Game::cmodel_t> iw4Models;
				iw4Models.resize(varClipMap->numCModels);

				for (int i = 0; i < varClipMap->numCModels; i++)
				{
					std::memcpy(&iw4Models[i], &varClipMap->cModels[i], 28);
					std::memcpy(&iw4Models[i].leaf, &varClipMap->cModels[i].leaf, 40);
				}

				std::memcpy(varClipMap->cModels, iw4Models.data(), iw4Models.size() * sizeof Game::cmodel_t);
			}

			if (varClipMap->smodelNodes)
			{
				varClipMap->smodelNodes = reinterpret_cast<Game::SModelAabbNode*>(Game::DB_AllocStreamPos(3));
				Game::Load_Stream(true, varClipMap->smodelNodes, varClipMap->smodelNodeCount * sizeof(Game::SModelAabbNode));
			}

			// load mapents
			*reinterpret_cast<Game::MapEnts***>(0x112B388) = &varClipMap->mapEnts;
			Utils::Hook::Call<void(bool)>(0x5B9E10)(false);

			// load stages
			if (varClipMap->stages)
			{
				varClipMap->stages = (Game::Stage*)Game::DB_AllocStreamPos(3);
				*reinterpret_cast<Game::Stage**>(0x112A818) = varClipMap->stages;
				Utils::Hook::Call<void(bool, int)>(0x4AC760)(true, varClipMap->stageCount);

				// IW4 expects stages in mapents instaed of clipmap
				codolMapEnts.stageCount = varClipMap->stageCount;
				codolMapEnts.stages = varClipMap->stages;
			}

			// load map triggers
			*reinterpret_cast<Game::MapTriggers**>(0x112AB3C) = &varClipMap->trigger;
			Utils::Hook::Call<void(bool)>(0x43CBA0)(false);

			AssertOffset(codolClipMap_t, dynEntCount[0], 196);
			AssertOffset(codolClipMap_t, dynEntCount[1], 198);
			
			// dynamic entity shit
			for (int i = 0; i < 2; i++)
			{
				if (varClipMap->dynEntDefList[i])
				{
					varClipMap->dynEntDefList[i] = reinterpret_cast<Game::DynEntityDef*>(Game::DB_AllocStreamPos(3));
					*reinterpret_cast<Game::DynEntityDef**>(0x112AF3C) = varClipMap->dynEntDefList[i];
					Utils::Hook::Call<void(bool, unsigned int)>(0x47CE10)(true, varClipMap->dynEntCount[i]);
				}
			}

			Game::DB_PushStreamPos(2);
			
			for (int i = 0; i < 2; i++)
			{
				if (varClipMap->dynEntPoseList[i])
				{
					varClipMap->dynEntPoseList[i] = reinterpret_cast<Game::DynEntityPose*>(Game::DB_AllocStreamPos(3));
					Game::Load_Stream(true, varClipMap->dynEntPoseList[i], varClipMap->dynEntCount[i] * sizeof(Game::DynEntityPose));
				}
			}
			for (int i = 0; i < 2; i++)
			{
				if (varClipMap->dynEntClientList[i])
				{
					varClipMap->dynEntClientList[i] = reinterpret_cast<Game::DynEntityClient*>(Game::DB_AllocStreamPos(3));
					Game::Load_Stream(true, varClipMap->dynEntClientList[i], varClipMap->dynEntCount[i] * sizeof(Game::DynEntityClient));
				}
			}
			for (int i = 0; i < 2; i++)
			{
				if (varClipMap->dynEntCollList[i])
				{
					varClipMap->dynEntCollList[i] = reinterpret_cast<Game::DynEntityColl*>(Game::DB_AllocStreamPos(3));
					Game::Load_Stream(true, varClipMap->dynEntCollList[i], varClipMap->dynEntCount[i] * sizeof(Game::DynEntityColl));
				}
			}

			Game::DB_PopStreamPos();
			
			Game::DB_PopStreamPos();

			auto codolMap = new codolClipMap_t;
			memcpy(codolMap, varClipMap, sizeof codolClipMap_t);

			auto cancerMap = reinterpret_cast<codolClipMap_t*>(varClipMap);
			auto iw4Map = reinterpret_cast<Game::clipMap_t*>(varClipMap);
			
			memcpy(&iw4Map->planeCount, &codolMap->info.numCPlanes, 8);
			memcpy(&iw4Map->numStaticModels, &codolMap->numStaticModels, 8);
			memcpy(&iw4Map->numMaterials, &codolMap->info.numMaterials, 24);
			memcpy(&iw4Map->numNodes, &codolMap->numCNodes, 16);
			memcpy(&iw4Map->leafbrushNodesCount, &codolMap->info.numCLeafBrushNodes, 16);
			memcpy(&iw4Map->vertCount, &codolMap->numVerts, 52);
			memcpy(&iw4Map->numBrushes, &codolMap->info.numBrushes, 16);
			iw4Map->mapEnts = &codolMapEnts;
			memcpy(&iw4Map->smodelNodeCount, &codolMap->smodelNodeCount, 48);

			AssetHandler::Relocate(&cancerMap->info.numCPlanes, &iw4Map->planeCount, 8);
			AssetHandler::Relocate(&cancerMap->numStaticModels, &iw4Map->numStaticModels, 8);
			AssetHandler::Relocate(&cancerMap->info.numMaterials, &iw4Map->numMaterials, 24);
			AssetHandler::Relocate(&cancerMap->numCNodes, &iw4Map->numNodes, 16);
			AssetHandler::Relocate(&cancerMap->info.numCLeafBrushNodes, &iw4Map->leafbrushNodesCount, 16);
			AssetHandler::Relocate(&cancerMap->numVerts, &iw4Map->vertCount, 52);
			AssetHandler::Relocate(&cancerMap->info.numBrushes, &iw4Map->numBrushes, 16);
			AssetHandler::Relocate(&cancerMap->smodelNodeCount, &iw4Map->smodelNodeCount, 48);
			
			delete codolMap;
			
			return 1;
		}
		else
		{
			return Utils::Hook::Call<int(bool)>(0x46C390)(atStreamStart);
		}
	}

	static const unsigned int crcTable[] =
	{
		0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
		0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
		0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
		0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
		0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
		0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
		0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
		0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
		0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
		0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
		0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
		0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
		0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
		0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
		0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
		0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
		0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
		0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
		0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
		0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
		0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
		0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
		0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
		0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
		0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
		0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
		0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
		0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
		0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
		0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
		0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
		0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
		0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
		0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
		0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
		0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
		0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
		0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
		0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
		0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
		0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
		0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
		0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
	};
	uint32_t Zones::HashCRC32StringInt(const std::string& string, uint32_t initialCrc)
	{
		auto curPtr = reinterpret_cast<std::uint8_t*>(const_cast<char*>(string.data()));
		auto remaining = string.size();
		auto crc = ~initialCrc;

		for (; remaining--; ++curPtr)
		{
			crc = (crc >> 8) ^ crcTable[(crc ^ *curPtr) & 0xFF];
		}

		return (~crc);
	}

	std::unordered_map<int, Zones::FileData> Zones::fileDataMap;
	std::mutex Zones::fileDataMutex;
	
	__declspec(naked) int Zones::FS_FOpenFileReadForThreadOriginal(const char*, int*, int)
	{
		__asm
		{
			sub esp, 0x33C;
			
			push 0x643276;
			retn;
		}
	}
	int Zones::FS_FOpenFileReadForThreadHook(const char* file, int* filePointer, int thread)
	{
		const auto retval = FS_FOpenFileReadForThreadOriginal(file, filePointer, thread);

		if (file != nullptr && filePointer != nullptr && strlen(file) >= 4 && retval > 0)
		{
			std::string fileBuffer;
			fileBuffer.resize(retval);
			auto readSize = Game::FS_Read(&fileBuffer[0], retval, *filePointer);

			// check if file should be skipped
			auto skipFile = true;

			if (Utils::String::EndsWith(file, ".mp3") ||
				Utils::String::EndsWith(file, ".wav") ||
				(Utils::String::EndsWith(file, ".iwi") && readSize >= 3 && !Utils::String::StartsWith(fileBuffer, "IWi"))) {
				skipFile = false;
			}
			
			// if the header seems encrypted...
			if (fileBuffer.size() > 4 && readSize == retval && !skipFile)
			{
				auto packedSize = fileBuffer.size() - 4;
				auto unpackedSize = *reinterpret_cast<int*>(&fileBuffer[fileBuffer.size() - 4]);

				// calc encrypted buffer size
				auto encryptedBufferSize = fileBuffer.size();
				encryptedBufferSize -= 4;
				encryptedBufferSize += 16 - (encryptedBufferSize % 16);

				// prepare encryptedData buffer
				std::string encryptedData;
				encryptedData.resize(encryptedBufferSize);
				memcpy(&encryptedData[0], &fileBuffer[0], packedSize);

				// prepare decryptedData buffer
				std::string decryptedData;
				decryptedData.resize(encryptedBufferSize);

				register_cipher(&aes_desc);

				auto aes = find_cipher("aes");
		
				// attempt to decrypt the IWI
				symmetric_CTR ctr_state;
				memset(&ctr_state, 0, sizeof(symmetric_CTR));
				
				// decryption keys
				std::uint8_t aesKey[24] = { 0x15, 0x9a, 0x03, 0x25, 0xe0, 0x75, 0x2e, 0x80, 0xc6, 0xc0, 0x94, 0x2a, 0x50, 0x5c, 0x1c, 0x68, 0x8c, 0x17, 0xef, 0x53, 0x99, 0xf8, 0x68, 0x3c };
				std::uint32_t aesIV[4] = { 0x1010101, 0x1010101, 0x1010101, 0x1010101 };

				auto strippedFileName = std::filesystem::path(file).filename().string();
				auto nonce = HashCRC32StringInt(strippedFileName, strippedFileName.size());
				
				std::uint8_t iv[16];
				memset(iv, 0, sizeof iv);
				memcpy(iv, &nonce, 4);
				memcpy(iv + 4, &unpackedSize, 4);
				
				ctr_start(aes, reinterpret_cast<unsigned char*>(&aesIV[0]), &aesKey[0], sizeof aesKey, 0, CTR_COUNTER_BIG_ENDIAN, &ctr_state);

				// decrypt image
				auto readDataSize = 0u;
				while (readDataSize < packedSize)
				{
					auto left = (packedSize - readDataSize);
					auto blockSize = (left > 0x8000) ? 0x8000 : left;

					memcpy(iv + 8, &readDataSize, 4);
					memcpy(iv + 12, &blockSize, 4);

					ctr_setiv(iv, sizeof iv, &ctr_state);
					ctr_decrypt(reinterpret_cast<uint8_t*>(&encryptedData[readDataSize]), reinterpret_cast<uint8_t*>(&decryptedData[readDataSize]), blockSize, &ctr_state);

					readDataSize += blockSize;
				}
				
				ctr_done(&ctr_state);

				if (static_cast<std::uint8_t>(decryptedData[0]) == 0x78)
				{
					FileData data = {};
					data.readPos = 0;
					data.len = unpackedSize;
					data.fileContents.resize(unpackedSize);

					// decompress the buffer
					auto result = uncompress(reinterpret_cast<std::uint8_t*>(&data.fileContents[0]),
						reinterpret_cast<unsigned long*>(&data.len), reinterpret_cast<const uint8_t*>(&decryptedData[0]), packedSize);

					// insert file data
					if (result == Z_OK)
					{
						std::lock_guard<std::mutex> $(fileDataMutex);
						fileDataMap[*filePointer] = data;
						return unpackedSize;
					}
				}
			}

			// un-read data, file is apparently not encrypted
			Game::FS_Seek(*filePointer, 0, Game::FS_SEEK_SET);
		}
		
		return retval;
	}

	__declspec(naked) int Zones::FS_ReadOriginal(void*, size_t, int)
	{
		__asm
		{
			push ecx;
			mov eax, [esp + 0x10];

			push 0x4A04C5;
			retn;
		}
	}
	int Zones::FS_ReadHook(void* buffer, size_t size, int filePointer)
	{
		std::lock_guard<std::mutex> $(fileDataMutex);
		
		auto itr = fileDataMap.find(filePointer);
		if (itr != fileDataMap.end())
		{
			if (!itr->second.fileContents.empty())
			{
				const auto readSize = std::min(size, itr->second.fileContents.size() - itr->second.readPos);
				memcpy(buffer, &itr->second.fileContents[itr->second.readPos], readSize);
				itr->second.readPos += readSize;
				return readSize;
			}
		}

		return FS_ReadOriginal(buffer, size, filePointer);
	}
	__declspec(naked) void Zones::FS_FCloseFileOriginal(int)
	{
		__asm
		{
			mov eax, [esp + 4];
			push esi;

			push 0x462005;
			retn;
		}
	}
	void Zones::FS_FCloseFileHook(int filePointer)
	{
		std::lock_guard<std::mutex> $(fileDataMutex);
		
		FS_FCloseFileOriginal(filePointer);
		
		const auto itr = fileDataMap.find(filePointer);
		if (itr != fileDataMap.end())
		{
			fileDataMap.erase(itr);
		}
	}
	__declspec(naked) std::uint32_t Zones::FS_SeekOriginal(int, int, int)
	{
		__asm
		{
			push esi;
			mov esi, [esp + 8];

			push 0x4A63D5;
			retn;
		}
	}
	std::uint32_t Zones::FS_SeekHook(int fileHandle, int seekPosition, int seekOrigin)
	{
		std::lock_guard<std::mutex> $(fileDataMutex);

		const auto itr = fileDataMap.find(fileHandle);
		if (itr != fileDataMap.end())
		{
			if (seekOrigin == Game::FS_SEEK_SET)
			{
				itr->second.readPos = seekPosition;
			}
			else if (seekOrigin == Game::FS_SEEK_CUR)
			{
				itr->second.readPos += seekPosition;
			}
			else if (seekOrigin == Game::FS_SEEK_END)
			{
				itr->second.readPos = itr->second.fileContents.size() - seekPosition;
			}

			return itr->second.readPos;
		}
		else
		{
			return FS_SeekOriginal(fileHandle, seekPosition, seekOrigin);
		}
	}

	__declspec(naked) void Zones::LoadMapTriggersModelPointer()
	{
		static auto DB_ConvertOffsetToPointer_Address = 0x4A82B0;

		__asm
		{
			cmp dword ptr[edx + 4], 0;
			je dontLoadAssetData;

			cmp dword ptr[edx + 4], 0xFFFFFFFF;
			je loadAssetData;

			// check if FF is below 446, still load data in that case
			cmp Zones::ZoneVersion, 446;
			jl loadAssetData;

			// offset to pointer magic
			pushad;
			push eax;
			call DB_ConvertOffsetToPointer_Address;
			add esp, 4;
			popad;

		dontLoadAssetData:
			push 0x43CBF3;
			retn;

		loadAssetData:
			push 0x43CBC1;
			retn;
		}
	}
	__declspec(naked) void Zones::LoadMapTriggersHullPointer()
	{
		static auto DB_ConvertOffsetToPointer_Address = 0x4A82B0;

		__asm
		{
			cmp dword ptr[eax + 0Ch], 0;
			je dontLoadAssetData;

			cmp dword ptr[eax + 0Ch], 0xFFFFFFFF;
			je loadAssetData;

			// check if FF is below 446, still load data in that case
			cmp Zones::ZoneVersion, 446;
			jl loadAssetData;

			// offset to pointer magic
			pushad;
			push eax;
			call DB_ConvertOffsetToPointer_Address;
			add esp, 4;
			popad;

		dontLoadAssetData:
			push 0x43CC2E;
			retn;

		loadAssetData:
			push 0x43CBFE;
			retn;
		}
	}
	__declspec(naked) void Zones::LoadMapTriggersSlabPointer()
	{
		static auto DB_ConvertOffsetToPointer_Address = 0x4A82B0;

		__asm
		{
			cmp dword ptr[eax + 14h], 0;
			je dontLoadAssetData;

			cmp dword ptr[eax + 14h], 0xFFFFFFFF;
			je loadAssetData;

			// check if FF is below 446, still load data in that case
			cmp Zones::ZoneVersion, 446;
			jl loadAssetData;

			// offset to pointer magic
			pushad;
			push eax;
			call DB_ConvertOffsetToPointer_Address;
			add esp, 4;
			popad;

		dontLoadAssetData:
			push 0x43CC6D;
			retn;

		loadAssetData:
			push 0x43CC39;
			retn;
		}
	}

	void Zones::LoadFxWorldAsset(Game::FxWorld** asset)
	{
		Utils::Hook::Call<void(Game::FxWorld * *)>(0x4857F0)(asset);

		if (Zones::Version() >= 423 && asset && *asset)
		{
			// allocate glass data structures
			static Game::GameWorldMp glassMap;
			static Game::GameWorldMp* glassMapPtr;
			static Game::G_GlassData glassData;
			static std::vector<Game::G_GlassPiece> glassPieces;

			// clear previous glass data
			memset(&glassMap, 0, sizeof Game::GameWorldMp);
			memset(&glassData, 0, sizeof Game::G_GlassData);
			glassPieces.clear();
			
			// generate glassPieces array
			const auto pieceCount = (*asset)->glassSys.initPieceCount;
			if (pieceCount > 0)
			{
				glassPieces.resize(pieceCount);
				memset(&glassPieces[0], 0, sizeof(Game::G_GlassPiece) * pieceCount);

				// generate glassData array
				glassData.glassPieces = glassPieces.data();
				glassData.pieceCount = glassPieces.size();
			}
			else
			{
				// game seems to do an array lookup on the first index even if there's no glass pieces?
				static Game::G_GlassPiece emptyPiece;
				glassData.glassPieces = &emptyPiece;
			}

			// build glass asset
			glassMap.g_glassData = &glassData;
			glassMap.name = (*asset)->name;

			// set glass map ptr
			glassMapPtr = &glassMap;

			// add glass to DB
			Utils::Hook::Call<void(Game::GameWorldMp**)>(0x4A6240)(&glassMapPtr);
		}
	}

	void Zones::LoadXModelAsset(Game::XModel** asset)
	{
		if (Zones::Version() >= 446)
		{
			for (int i = 0; i < (*asset)->numLods; i++)
			{
				if ((*asset)->lodInfo[i].surfs == nullptr && Zones::Version() >= 446)
				{
					const auto name = (*asset)->name;
					const auto fx_model = Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_XMODEL, "void").model;
					memcpy(*asset, fx_model, sizeof Game::XModel);
					(*asset)->name = name;
					break;
				}
			}
		}

		return Utils::Hook::Call<void(Game::XModel * *)>(0x47A690)(asset);
	}

	// patch max file amount returned by Sys_ListFiles
	constexpr auto fileCountMultiplier = 8;
	constexpr auto maxFileCount = 8191 * fileCountMultiplier;

	int RelocateFileCount(std::uint32_t dwSize, int unk1, int unk2, int unk3)
	{
		dwSize *= fileCountMultiplier;
		return Utils::Hook::Call<int(std::uint32_t, int, int, int)>(0x430E90)(dwSize, unk1, unk2, unk3);
	}

	void Zones::LoadMaterialAsset(Game::Material** asset)
	{
		if (asset && *asset && Zones::Version() >= 446)
		{
			static std::vector<std::string> broken_materials = {
				"gfx_fxt_debris_wind_ash_z10",
				"gfx_fxt_smk_light_z3"
			};

			// replace broken materials with the default one as restricting them does not seem to work.
			const auto itr = std::find(broken_materials.begin(), broken_materials.end(), (*asset)->info.name);
			if (itr != broken_materials.end())
			{
				const auto name = (*asset)->info.name;
				const auto default_material = Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_MATERIAL, "$default").material;
				memcpy(*asset, default_material, sizeof Game::Material);
				(*asset)->info.name = name;
			}
		}

		return Utils::Hook::Call<void(Game::Material**)>(0x476750)(asset);
	}

	void Zones::LoadTracerDef(bool atStreamStart, Game::TracerDef* tracer, int size)
	{		
		if (Zones::Version() >= 460)
		{
			size = 116;
		}

		Game::Load_Stream(atStreamStart, tracer, size);
		*Game::varFxEffectDefHandle = nullptr;
		
		if (Zones::Version() >= 460)
		{
			*Game::varFxEffectDefHandle = reinterpret_cast<Game::FxEffectDef**>(tracer + 8);

			std::memmove(tracer + 8, tracer + 12, size - 12);
			AssetHandler::Relocate(tracer + 12, tracer + 8, size - 12);
		}
	}

	void Zones::LoadTracerDefFxEffect()
	{
		if (Zones::Version() >= 460)
		{
			Game::Load_FxEffectDefHandle(false);
		}
		
		Game::DB_PopStreamPos();
	}

	char* Zones::ParseVision_Stub(const char** data_p)
	{
		auto token = Game::Com_Parse(data_p);

		if (vision_replace_list.find(token) != vision_replace_list.end())
		{
			return vision_replace_list[token].data();
		}

		return token;
	}
	
	char* Zones::ParseShellShock_Stub(const char** data_p)
	{
		auto token = Game::Com_Parse(data_p);
		if (shellshock_replace_list.find(token) != shellshock_replace_list.end())
		{
			return shellshock_replace_list[token].data();
		}
		return token;
	}


	Zones::Zones()
	{
		Zones::ZoneVersion = 0;

		if (ZoneBuilder::IsEnabled()) {
			Command::Add("decryptImages", [](Command::Params*)
				{
					auto images = Game::Sys_ListFilesWrapper("iw4x/images", "iwi");
					Logger::Print("decrypting %u images...\n", images.size());

					for (auto& image : images)
					{
						char* buffer = nullptr;
						auto fileLength = Game::FS_ReadFile(Utils::String::VA("images/%s", image.data()), &buffer);

						if (fileLength && buffer)
						{
							if (!std::filesystem::exists("raw/images"))
							{
								std::filesystem::create_directories("raw/images");
							}

							if (!std::filesystem::exists(Utils::String::VA("raw/images/%s", image.data())))
							{
								const auto fp = fopen(Utils::String::VA("raw/images/%s", image.data()), "wb");
								if (fp)
								{
									fwrite(buffer, fileLength, 1, fp);
									fclose(fp);
								}
							}

							Game::FS_FreeFile(buffer);
						}
					}

					Logger::Print("decrypted %u images!\n", images.size());
				});

			Command::Add("decryptSounds", [](Command::Params*)
				{
					auto path = "iw4x/sound";
					std::vector<std::string> sounds{};
					std::error_code os_error;
					std::filesystem::recursive_directory_iterator walker(path, std::filesystem::directory_options::skip_permission_denied);
					for (auto i = std::filesystem::begin(walker); i != std::filesystem::end(walker); i = i.increment(os_error)) {
						if (!os_error) {
							if (!is_directory(i->path())) {
								auto pathStr = i->path().string();
								Utils::String::Replace(pathStr, Utils::String::VA("%s\\", path), "");
								sounds.push_back(pathStr);
							}
						}
					}

					Logger::Print("decrypting %u sounds...\n", sounds.size());

					for (auto& sound : sounds)
					{
						char* buffer = nullptr;
						auto fileLength = Game::FS_ReadFile(Utils::String::VA("sound/%s", sound.data()), &buffer);

						if (fileLength && buffer)
						{
							auto path = std::filesystem::path(sound.data());
							std::filesystem::create_directories("raw/sound" / path.parent_path());

							if (!std::filesystem::exists(Utils::String::VA("raw/sound/%s", sound.data())))
							{
								const auto fp = fopen(Utils::String::VA("raw/sound/%s", sound.data()), "wb");
								if (fp)
								{
									fwrite(buffer, fileLength, 1, fp);
									fclose(fp);
								}
							}

							Game::FS_FreeFile(buffer);
						}
					}

					Logger::Print("decrypted %u sounds!\n", sounds.size());
				});
		}

		// patch max filecount Sys_ListFiles can return
		Utils::Hook::Set<std::uint32_t>(0x45A66B, (maxFileCount + fileCountMultiplier) * 4);
		Utils::Hook::Set<std::uint32_t>(0x64AF78, maxFileCount);
		Utils::Hook::Set<std::uint32_t>(0x64B04F, maxFileCount);
		Utils::Hook::Set<std::uint32_t>(0x45A8CE, maxFileCount);
		Utils::Hook(0x45A806, RelocateFileCount, HOOK_CALL).install()->quick();
		Utils::Hook(0x45A6A0, RelocateFileCount, HOOK_CALL).install()->quick();
		
#ifndef DEBUG
		// Ignore missing soundaliases for now
		// TODO: Include them in the dependency zone!
		Utils::Hook::Nop(0x644207, 5);
#endif

		// Block Mark_pathnode_constant_t
		Utils::Hook::Set<BYTE>(0x4F74B0, 0xC3);

		// addon_map_ents asset type (we reuse it for weaponattach)
		Utils::Hook::Set<BYTE>(0x418B31, 0x72);

		// encrypted images hooks
		if (ZoneBuilder::IsEnabled()) {
			Utils::Hook(0x462000, Zones::FS_FCloseFileHook, HOOK_JUMP).install()->quick();
			Utils::Hook(0x4A04C0, Zones::FS_ReadHook, HOOK_JUMP).install()->quick();
			Utils::Hook(0x643270, Zones::FS_FOpenFileReadForThreadHook, HOOK_JUMP).install()->quick();
			Utils::Hook(0x4A63D0, Zones::FS_SeekHook, HOOK_JUMP).install()->quick();
		}

		// asset hooks
		Utils::Hook(0x47146D, Zones::LoadTracerDef, HOOK_CALL).install()->quick();
		Utils::Hook(0x4714A3, Zones::LoadTracerDefFxEffect, HOOK_JUMP).install()->quick();
		Utils::Hook(0x4039DE, Zones::LoadMaterialAsset, HOOK_CALL).install()->quick();
		Utils::Hook(0x4FCAEE, Zones::LoadXModelAsset, HOOK_CALL).install()->quick();
		Utils::Hook(0x5BA01E, Zones::LoadFxWorldAsset, HOOK_CALL).install()->quick();
		Utils::Hook(0x43CBBB, Zones::LoadMapTriggersModelPointer, HOOK_JUMP).install()->quick();
		Utils::Hook(0x43CBF8, Zones::LoadMapTriggersHullPointer, HOOK_JUMP).install()->quick();
		Utils::Hook(0x43CC33, Zones::LoadMapTriggersSlabPointer, HOOK_JUMP).install()->quick();
		Utils::Hook(0x47E1DD, Zones::LoadMapEnts, HOOK_CALL).install()->quick();
		Utils::Hook(0x4BF992, Zones::LoadClipMap, HOOK_CALL).install()->quick();
		Utils::Hook(0x4C86D8, Zones::LoadXModelColSurfPtr, HOOK_JUMP).install()->quick();
		Utils::Hook(0x44EB21, Zones::LoadGfxLightMapExtraData, HOOK_CALL).install()->quick();
		Utils::Hook(0x44EAD3, Zones::LoadGfxReflectionProbes, HOOK_CALL).install()->quick();
		Utils::Hook(0x4C08F8, Zones::LoadGfxXSurfaceExtraData, HOOK_CALL).install()->quick();
		Utils::Hook(0x4C08DC, Zones::LoadGfxXSurfaceArray, HOOK_CALL).install()->quick();
		Utils::Hook(0x45AE3D, Zones::LoadRandomFxGarbage, HOOK_CALL).install()->quick();
		Utils::Hook(0x495938, Zones::LoadFxElemDefArrayStub, HOOK_CALL).install()->quick();
		Utils::Hook(0x45ADA0, Zones::LoadFxElemDefStub, HOOK_CALL).install()->quick();
		Utils::Hook(0x4EA6FE, Zones::LoadXModelLodInfoStub, HOOK_CALL).install()->quick();
		Utils::Hook(0x410D90, Zones::LoadXModel, HOOK_CALL).install()->quick();
		Utils::Hook(0x4925C8, Zones::LoadXSurfaceArray, HOOK_CALL).install()->quick();
		Utils::Hook(0x4F4D0D, Zones::LoadGameWorldSp, HOOK_CALL).install()->quick();
		Utils::Hook(0x47CCD2, Zones::LoadWeaponCompleteDef, HOOK_CALL).install()->quick();
		Utils::Hook(0x483DA0, Zones::LoadVehicleDef, HOOK_CALL).install()->quick();
		Utils::Hook(0x4F0AC8, Zones::Loadsnd_alias_tArray, HOOK_CALL).install()->quick();
		Utils::Hook(0x403A5D, Zones::LoadLoadedSound, HOOK_CALL).install()->quick();
		Utils::Hook(0x463022, Zones::LoadWeaponAttach, HOOK_CALL).install()->quick();
		Utils::Hook(0x41A570, Zones::LoadmenuDef_t, HOOK_CALL).install()->quick();
		Utils::Hook(0x49591B, Zones::LoadFxEffectDef, HOOK_CALL).install()->quick();
		Utils::Hook(0x428F0A, Zones::LoadMaterialShaderArgumentArray, HOOK_CALL).install()->quick();
		Utils::Hook(0x4B1EB8, Zones::LoadStructuredDataStructPropertyArray, HOOK_CALL).install()->quick();
		Utils::Hook(0x49CE0D, Zones::LoadPhysPreset, HOOK_CALL).install()->quick();
		Utils::Hook(0x48E84D, Zones::LoadXModelSurfs, HOOK_CALL).install()->quick();
		Utils::Hook(0x4447C2, Zones::LoadImpactFx, HOOK_CALL).install()->quick();
		Utils::Hook(0x4447D0, Zones::LoadImpactFxArray, HOOK_JUMP).install()->quick();
		Utils::Hook(0x4D6A0B, Zones::LoadPathNodeArray, HOOK_CALL).install()->quick();
		Utils::Hook(0x4D6A47, Zones::LoadPathDataConstant, HOOK_JUMP).install()->quick();
		Utils::Hook(0x463D6E, Zones::LoadPathnodeConstantTail, HOOK_CALL).install()->quick();
		Utils::Hook(0x4471AD, Zones::LoadGfxImage, HOOK_CALL).install()->quick();
		Utils::Hook(0x41A590, Zones::LoadExpressionSupportingDataPtr, HOOK_CALL).install()->quick();
		Utils::Hook(0x459833, Zones::LoadExpressionSupportingDataPtr, HOOK_JUMP).install()->quick();
		Utils::Hook(0x5B9AA5, Zones::LoadXAsset, HOOK_CALL).install()->quick();
		Utils::Hook(0x461740, Zones::LoadMaterialTechniqueArray, HOOK_CALL).install()->quick();
		Utils::Hook(0x461710, Zones::LoadMaterialTechnique, HOOK_CALL).install()->quick();
		Utils::Hook(0x40330D, Zones::LoadMaterial, HOOK_CALL).install()->quick();
		Utils::Hook(0x4B8DC0, Zones::LoadGfxWorld, HOOK_CALL).install()->quick();
		Utils::Hook(0x4B8FF5, Zones::Loadsunflare_t, HOOK_CALL).install()->quick();
		Utils::Hook(0x418998, Zones::GameMapSpPatchStub, HOOK_JUMP).install()->quick();
		Utils::Hook(0x427A1B, Zones::LoadPathDataTail, HOOK_JUMP).install()->quick();

		Utils::Hook(0x4B4EA1, Zones::ParseShellShock_Stub, HOOK_CALL).install()->quick();
		Utils::Hook(0x4B4F0C, Zones::ParseShellShock_Stub, HOOK_CALL).install()->quick();
		
		Utils::Hook(0x4F4D3B, [] ()
		{
			if (Zones::ZoneVersion >= VERSION_ALPHA3)
			{
				ZeroMemory(*Game::varPathData, sizeof(Game::PathData));
			}
			else
			{
				// Load_PathData
				Utils::Hook::Call<void(int)>(0x4278A0)(false);
			}
		}, HOOK_CALL).install()->quick();

		// Change stream for images
		Utils::Hook(0x4D3225, [] ()
		{
			Game::DB_PushStreamPos((Zones::ZoneVersion >= 332) ? 3 : 0);
		}, HOOK_CALL).install()->quick();

		Utils::Hook(0x4597DD, Zones::LoadStatement, HOOK_CALL).install()->quick();
		Utils::Hook(0x471A39, Zones::LoadWindowImage, HOOK_JUMP).install()->quick();

		// Fix newer vision file
		Utils::Hook(0x59A849, ParseVision_Stub, HOOK_CALL).install()->quick();
		Utils::Hook(0x59A8AD, ParseVision_Stub, HOOK_CALL).install()->quick();

#ifdef DEBUG
		// Easy dirty disk debugging
		Utils::Hook::Set<WORD>(0x4CF7F0, 0xC3CC);
		// disable _invoke_watson to allow debugging
		Utils::Hook::Set<WORD>(0x6B9602,0xCCCC);
#endif
	}

	Zones::~Zones()
	{

	}
}
#pragma optimize( "", on ) 