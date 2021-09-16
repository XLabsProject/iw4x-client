#include "STDInclude.hpp"

namespace Components
{
	int Materials::ImageNameLength;
	Utils::Hook Materials::ImageVersionCheckHook;

	std::vector<Game::GfxImage*> Materials::ImageTable;
	std::vector<Game::Material*> Materials::MaterialTable;

	std::map<std::string, std::string> Materials::TechsetSwaps
	{
		{ "mc_l_hsm_b0c0s0_em", "mc_l_hsm_b0c0s0_custom_growing_ice_cracks" }, // Spotted once on italy_sh as a remapped techset, but not sure it's useful anymore
		{ "mc_l_sm_b0c0s0_em", "mc_l_sm_b0c0s0_custom_growing_ice_cracks" }, // Car windows in mp_italy_sh
		{ "mc_l_hsm_t0c0n0s0_fgcb", "mc_l_hsm_t0c0n0s0"},       //
		{ "mc_l_hsm_r0c0n0s0_fgcb", "mc_l_hsm_r0c0n0s0"},       // Various monkey king elements on mp_abandon_sh_mk
		{ "mc_l_hsm_r0c0n0_fgcb", "mc_l_hsm_r0c0n0"},           //
		{ "mc_l_hsm_t0c0_fgcb", "mc_l_hsm_t0c0"},  	            //
		{ "mc_l_hsm_b0c0n0s0p0_fgcb", "mc_l_hsm_b0c0n0s0p0"} // Ammo crate on mp_arkaden_sh
	};

	Game::Material* Materials::Create(const std::string& name, Game::GfxImage* image)
	{
		Game::Material* material = Utils::Memory::GetAllocator()->allocate<Game::Material>();
		Game::MaterialTextureDef* texture = Utils::Memory::GetAllocator()->allocate<Game::MaterialTextureDef>();

		material->textureCount = 1;
		material->textureTable = texture;

		material->info.name = Utils::Memory::GetAllocator()->duplicateString(name);
		material->info.sortKey = 0x22;
		material->info.textureAtlasColumnCount = 1;
		material->info.textureAtlasRowCount = 1;

		for (int i = 0; i < 48; ++i)
		{
			if (i != 4) material->stateBitsEntry[i] = -1;
		}

		material->stateFlags = 3;
		material->cameraRegion = 4;
		material->techniqueSet = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_TECHNIQUE_SET, "2d").techniqueSet;

		material->textureTable->nameHash = Game::R_HashString("colorMap");
		material->textureTable->nameStart = 'c';
		material->textureTable->nameEnd = 'p';
		material->textureTable->samplerState = -30;
		material->textureTable->u.image = image;

		Game::Material* cursor = Game::DB_FindXAssetHeader(Game::ASSET_TYPE_MATERIAL, "ui_cursor").material;
		if (cursor)
		{
			material->stateBitsTable = cursor->stateBitsTable;
			material->stateBitsCount = cursor->stateBitsCount;
		}

		Materials::MaterialTable.push_back(material);

		return material;
	}

	void Materials::Delete(Game::Material* material, bool deleteImage)
	{
		if (!material) return;

		if (deleteImage)
		{
			for (char i = 0; i < material->textureCount; ++i)
			{
				Materials::DeleteImage(material->textureTable[i].u.image);
			}
		}

		Utils::Memory::GetAllocator()->free(material->textureTable);
		Utils::Memory::GetAllocator()->free(material->info.name);
		Utils::Memory::GetAllocator()->free(material);

		auto mat = std::find(Materials::MaterialTable.begin(), Materials::MaterialTable.end(), material);
		if (mat != Materials::MaterialTable.end())
		{
			Materials::MaterialTable.erase(mat);
		}
	}

	Game::GfxImage* Materials::CreateImage(const std::string& name, unsigned int width, unsigned int height, unsigned int depth, unsigned int flags, _D3DFORMAT format)
	{
		Game::GfxImage* image = Utils::Memory::GetAllocator()->allocate<Game::GfxImage>();
		image->name = Utils::Memory::GetAllocator()->duplicateString(name);

		Game::Image_Setup(image, width, height, depth, flags, format);

		Materials::ImageTable.push_back(image);

		return image;
	}

	void Materials::DeleteImage(Game::GfxImage* image)
	{
		if (!image) return;

		Game::Image_Release(image);

		Utils::Memory::GetAllocator()->free(image->name);
		Utils::Memory::GetAllocator()->free(image);

		auto img = std::find(Materials::ImageTable.begin(), Materials::ImageTable.end(), image);
		if (img != Materials::ImageTable.end())
		{
			Materials::ImageTable.erase(img);
		}
	}

	void Materials::DeleteAll()
	{
		std::vector<Game::Material*> materials;
		Utils::Merge(&materials, Materials::MaterialTable);
		Materials::MaterialTable.clear();

		for (auto& material : materials)
		{
			Materials::Delete(material);
		}

		std::vector<Game::GfxImage*> images;
		Utils::Merge(&images, Materials::ImageTable);
		Materials::ImageTable.clear();

		for (auto& image : images)
		{
			Materials::DeleteImage(image);
		}
	}

	bool Materials::IsValid(Game::Material* material)
	{
		if (!material || !material->textureCount || !material->textureTable) return false;

		for (char i = 0; i < material->textureCount; ++i)
		{
			if (!material->textureTable[i].u.image || !material->textureTable[i].u.image->texture.map)
			{
				return false;
			}
		}

		return true;
	}

	__declspec(naked) void Materials::ImageVersionCheck()
	{
		__asm
		{
			cmp eax, 9
			je returnSafely

			jmp Materials::ImageVersionCheckHook.original

		returnSafely:
			mov al, 1
			add esp, 18h
			retn
		}
	}

	Game::Material* Materials::ResolveMaterial(const char* stringPtr)
	{
		const char* imagePtr = stringPtr + 4;
		unsigned int length = static_cast<unsigned int>(stringPtr[3] & 0xFF);

		if (strlen(imagePtr) >= length)
		{
			Materials::ImageNameLength = 4 + length;
			std::string image(imagePtr, length);

			auto* material = Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_MATERIAL, image.data()).material;

			if(material == nullptr || material->techniqueSet == nullptr || material->techniqueSet->name == nullptr || strcmp(material->techniqueSet->name, "2d") != 0)
				return Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_MATERIAL, "default").material;

			return material;
		}

		Materials::ImageNameLength = 4;
		return Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_MATERIAL, "default").material;
	}

	__declspec(naked) void Materials::PostDrawMaterialStub()
	{
		__asm
		{
			mov eax, Materials::ImageNameLength
			add [esp + 30h], eax

			mov eax, 5358FFh
			jmp eax
		}
	}

	__declspec(naked) void Materials::DrawMaterialStub()
	{
		__asm
		{
			push eax
			pushad

			push ecx
			call Materials::ResolveMaterial
			add esp, 4h

			mov [esp + 20h], eax

			// Make all material text icons have white tint
			mov eax,[esp + 0x50]
			or eax,0x00FFFFFF
			mov [esp + 0x50],eax

			popad
			pop eax

			push 5310F0h
			retn
		}
	}

	int Materials::WriteDeathMessageIcon(char* string, int offset, Game::Material* material)
	{
		if (!material)
		{
			material = Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_MATERIAL, "default").material;
		}

		int length = strlen(material->info.name);
		string[offset++] = static_cast<char>(length);

		strncpy_s(string + offset, 1024 - offset, material->info.name, length);

		return offset + length;
	}

	__declspec(naked) void Materials::DeathMessageStub()
	{
		__asm
		{
			push eax
			pushad

			push edx // Material
			push eax // offset
			push ecx // String

			call Materials::WriteDeathMessageIcon
			add esp, 0Ch

			mov [esp + 20h], eax
			popad
			pop eax

			add esp, 8h
			retn
		}
	}

	int Materials::FormatImagePath(char* buffer, size_t size, int, int, const char* image)
	{
#if 0
		if (Utils::String::StartsWith(image, "preview_"))
		{
			std::string newImage = image;
			Utils::String::Replace(newImage, "preview_", "loadscreen_");

			if (FileSystem::FileReader(fmt::sprintf("images/%s.iwi", newImage.data())).exists())
			{
				image = Utils::String::VA("%s", newImage.data());
			}
		}
#endif

		return _snprintf_s(buffer, size, size, "images/%s.iwi", image);
	}

	int Materials::MaterialComparePrint(Game::Material* m1, Game::Material* m2)
	{
		//__debugbreak();
		return Utils::Hook::Call<int(Game::Material*, Game::Material*)>(0x5235B0)(m1, m2);
	}

#ifdef DEBUG
	void Materials::DumpImageCfg(int, const char*, const char* material)
	{
		Materials::DumpImageCfgPath(0, nullptr, Utils::String::VA("images/%s.iwi", material));
	}

	void Materials::DumpImageCfgPath(int, const char*, const char* material)
	{
		FILE* fp = nullptr;
		if (!fopen_s(&fp, "dump.cfg", "a") && fp)
		{
			fprintf(fp, "dumpraw %s\n", material);
			fclose(fp);
		}
	}

#endif
	void Materials::Load_MaterialTechniqueSetAsset(Game::MaterialTechniqueSet** pptr)
	{
		if (Zones::GetEntitiesZoneVersion() >= VERSION_LATEST_CODO){
			auto charName = (*pptr)->name;
			if (charName[0] == ',') charName = &charName[1];
			std::string name(charName);
			auto entry = TechsetSwaps.find(name);

			if (entry != TechsetSwaps.end())
			{
				const char* newTechset = entry->second.c_str();

				Components::Logger::Print("Swapped techset %s for %s at runtime\n", name.c_str(), newTechset);

				*pptr = Game::DB_FindXAssetEntry(Game::XAssetType::ASSET_TYPE_TECHNIQUE_SET, newTechset)->asset.header.techniqueSet;
				return;
			}
		}
		
		// Passthrough
		return Utils::Hook::Call<void(Game::MaterialTechniqueSet**)>(0x4BFA80)(pptr);
	}


	int Materials::R_TextWidth_Hk(const char* text, int maxChars, Game::Font_s* font)
	{
		auto lineWidth = 0;
		auto maxWidth = 0;

		if (maxChars <= 0)
			maxChars = 0x7FFFFFFF;

		if (text == nullptr)
			return 0;

		auto count = 0;
		while (text && *text && count < maxChars)
		{
			const auto letter = Game::SEH_ReadCharFromString(&text, nullptr);
			if (letter == '\r' || letter == '\n')
			{
				lineWidth = 0;
			}
			else
			{
				if (letter == '^' && text)
				{
					if (*text >= '0' && *text <= Colors::LastColorIndex)
					{
						text++;
						continue;
					}

					if (*text >= '\x01' && *text <= '\x02' && text[1] != '\0' && text[2] != '\0' && text[3] != '\0')
					{
						const auto width = text[1];
						const auto materialNameLength = text[3];

						// This is how the game calculates width and height. Probably some 1 byte floating point number.
						auto v9 = font->pixelHeight * (width - 16) + 16;
						auto w = ((((v9 >> 24) & 0x1F) + v9) >> 5);

						lineWidth += w;

						text += 4;
						for (auto currentLength = 0; currentLength < materialNameLength && *text; currentLength++)
							text++;
						continue;
					}
				}
				
				lineWidth += R_GetCharacterGlyph(font, letter)->dx;
				if (lineWidth > maxWidth)
					maxWidth = lineWidth;
				count++;
			}
		}

		return maxWidth;
	}

	Materials::Materials()
	{
		Materials::ImageNameLength = 7;

		// Allow codo images
		Materials::ImageVersionCheckHook.initialize(0x53A456, Materials::ImageVersionCheck, HOOK_CALL)->install();

		// Fix material pointer exploit
		// Also make all material text icons have white tint
		Utils::Hook(0x534E0C, Materials::DrawMaterialStub, HOOK_CALL).install()->quick();

		// Increment string pointer accordingly
		Utils::Hook(0x5358FA, Materials::PostDrawMaterialStub, HOOK_JUMP).install()->quick();

		// Adapt death message to IW5 material format
		Utils::Hook(0x5A30D9, Materials::DeathMessageStub, HOOK_JUMP).install()->quick();

		// Resolve preview images to loadscreens
		Utils::Hook(0x53AC19, Materials::FormatImagePath, HOOK_CALL).install()->quick();

		// Intercept techset finding (DB_FindXAssetHeader) so we can swap them at runtime (although we should keep it to a minimum)
		Utils::Hook(0x4FF10E, Materials::Load_MaterialTechniqueSetAsset, HOOK_CALL).install()->quick();

		// Debug material comparison
		Utils::Hook::Set<void*>(0x523894, Materials::MaterialComparePrint);

		// Consider material text icons when calculating text width
		Utils::Hook(0x5056C0, Materials::R_TextWidth_Hk, HOOK_JUMP).install()->quick();

#ifdef DEBUG
		if (Flags::HasFlag("dump"))
		{
			Utils::Hook(0x51F5AC, Materials::DumpImageCfg, HOOK_CALL).install()->quick();
			Utils::Hook(0x51F4C4, Materials::DumpImageCfg, HOOK_CALL).install()->quick();
			Utils::Hook(0x53AC62, Materials::DumpImageCfgPath, HOOK_CALL).install()->quick();
		}
		else
		{
			// Ignore missing images
			Utils::Hook::Nop(0x51F5AC, 5);
			Utils::Hook::Nop(0x51F4C4, 5);
		}
#endif

		Renderer::OnDeviceRecoveryBegin([]()
		{
			for (auto& image : Materials::ImageTable)
			{
				Game::Image_Release(image);
				image->texture.map = nullptr;
			}
		});

		/*Renderer::OnDeviceRecoveryEnd([]()
		{
			for (auto& image : Materials::ImageTable)
			{
				Utils::Hook::Call<void(void*)>(0x51F7B0)(image);
			}
		});*/
	}

	Materials::~Materials()
	{
		Materials::DeleteAll();

		Materials::ImageVersionCheckHook.uninstall();
	}
}
