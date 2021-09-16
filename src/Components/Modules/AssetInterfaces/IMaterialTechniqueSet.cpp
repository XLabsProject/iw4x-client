#include "STDInclude.hpp"

#define IW4X_TECHSET_VERSION "0"

namespace Assets
{
    void IMaterialTechniqueSet::load(Game::XAssetHeader* header, const std::string& name, Components::ZoneBuilder::Zone* builder)
    {
        if (!header->data) this->loadNative(header, name, builder); // Check if there is a native one
        if (!header->data) this->loadBinary(header, name, builder); // Check if we need to import a new one into the game
    }

	void IMaterialTechniqueSet::dump(Game::XAssetHeader header)
	{
        auto iw4Techset = header.techniqueSet;

        Utils::Stream buffer;
        buffer.saveArray("IW4xTSET", 8);
        buffer.saveObject(IW4X_TECHSET_VERSION);

        buffer.saveObject(*iw4Techset);

        if (iw4Techset->name)
        {
            buffer.saveString(iw4Techset->name);
        }

        for (int i = 0; i < 48; i++)
        {
            if (iw4Techset->techniques[i])
            {
                buffer.saveString(iw4Techset->techniques[i]->name);
                IMaterialTechniqueSet::dumpTechnique(iw4Techset->techniques[i]);
            }
        }

        Utils::IO::WriteFile(Utils::String::VA("dump/techsets/%s.iw4xTS", iw4Techset->name), buffer.toBuffer());
	}

    void IMaterialTechniqueSet::dumpVS(Game::MaterialVertexShader* vs)
    {
        if (!vs) return;
        Utils::Stream buffer;
        buffer.saveArray("IW4xVERT", 8);
        buffer.saveObject(IW4X_TECHSET_VERSION);

        buffer.saveObject(*vs);

        if (vs->name)
        {
            buffer.saveString(vs->name);
        }

        buffer.saveArray(vs->prog.loadDef.program, vs->prog.loadDef.programSize);

        Utils::IO::WriteFile(Utils::String::VA("dump/vs/%s.iw4xVS", vs->name), buffer.toBuffer());
    }

    void IMaterialTechniqueSet::dumpPS(Game::MaterialPixelShader* ps)
    {
        if (!ps) return;
        Utils::Stream buffer;
        buffer.saveArray("IW4xPIXL", 8);
        buffer.saveObject(IW4X_TECHSET_VERSION);

        buffer.saveObject(*ps);

        if (ps->name)
        {
            buffer.saveString(ps->name);
        }

        buffer.saveArray(ps->prog.loadDef.program, ps->prog.loadDef.programSize);

        Utils::IO::WriteFile(Utils::String::VA("dump/ps/%s.iw4xPS", ps->name), buffer.toBuffer());
    }

    void IMaterialTechniqueSet::dumpTechnique(Game::MaterialTechnique* tech)
    {
        AssertSize(Game::MaterialPass, 20);
        if (!tech) return;
        Utils::Stream buffer;
        buffer.saveArray("IW4xTECH", 8);
        buffer.saveObject(IW4X_TECHSET_VERSION);

        buffer.saveObject(tech->flags);
        buffer.saveObject(tech->passCount);

        buffer.saveArray(tech->passArray, tech->passCount);

        for (int i = 0; i < tech->passCount; i++)
        {
            Game::MaterialPass* pass = &tech->passArray[i];

            if (pass->vertexDecl)
            {
                std::string name = IMaterialTechniqueSet::dumpDecl(pass->vertexDecl);
                buffer.saveString(name.data());
            }

            if (pass->vertexShader)
            {
                buffer.saveString(pass->vertexShader->name);
                IMaterialTechniqueSet::dumpVS(pass->vertexShader);
            }

            if (pass->pixelShader)
            {
                buffer.saveString(pass->pixelShader->name);
                IMaterialTechniqueSet::dumpPS(pass->pixelShader);
            }

            buffer.saveArray(pass->args, pass->perPrimArgCount + pass->perObjArgCount + pass->stableArgCount);

#define MTL_ARG_LITERAL_PIXEL_CONST     0x0
#define MTL_ARG_LITERAL_VERTEX_CONST    0x1
#define MTL_ARG_CODE_VERTEX_CONST       0x3
#define MTL_ARG_CODE_PIXEL_CONST        0x5

            for (int k = 0; k < pass->perPrimArgCount + pass->perObjArgCount + pass->stableArgCount; ++k)
            {
                Game::MaterialShaderArgument* arg = &pass->args[k];
                if (arg->type == MTL_ARG_LITERAL_VERTEX_CONST || arg->type == MTL_ARG_LITERAL_PIXEL_CONST)
                {
                    buffer.saveArray(arg->u.literalConst, 4);
                }

                if (arg->type == MTL_ARG_CODE_VERTEX_CONST || arg->type == MTL_ARG_CODE_PIXEL_CONST)
                {
                    unsigned short val = arg->u.codeConst.index;
                    buffer.saveObject(val);
                    buffer.saveObject(arg->u.codeConst.firstRow);
                    buffer.saveObject(arg->u.codeConst.rowCount);
                }
            }
        }

        Utils::IO::WriteFile(Utils::String::VA("dump/techniques/%s.iw4xTech", tech->name), buffer.toBuffer());
    }

    std::string IMaterialTechniqueSet::dumpDecl(Game::MaterialVertexDeclaration* decl)
    {
        if (!decl) return "";

        static int numDecls = 0;
        Utils::Memory::Allocator allocator;

        Game::MaterialVertexDeclaration* iw4Decl = allocator.allocate<Game::MaterialVertexDeclaration>();

        // TODO: figure out how to actually name these things
        iw4Decl->name = allocator.duplicateString(Utils::String::VA("iw4xDecl%d", numDecls++));
        iw4Decl->hasOptionalSource = decl->hasOptionalSource;
        iw4Decl->streamCount = decl->streamCount;

        memcpy(&iw4Decl->routing, &decl->routing, sizeof(Game::MaterialVertexStreamRouting));

        Utils::Stream buffer;
        buffer.saveArray("IW4xDECL", 8);
        buffer.saveObject(IW4X_TECHSET_VERSION);

        buffer.saveObject(*iw4Decl);

        if (iw4Decl->name)
        {
            buffer.saveString(iw4Decl->name);
        }

        Utils::IO::WriteFile(Utils::String::VA("dump/decl/%s.iw4xDECL", iw4Decl->name), buffer.toBuffer());

        return iw4Decl->name;
    }


    void IMaterialTechniqueSet::loadNative(Game::XAssetHeader* header, const std::string& name, Components::ZoneBuilder::Zone* /*builder*/)
    {
        header->techniqueSet = Components::AssetHandler::FindOriginalAsset(this->getType(), name.data()).techniqueSet;
    }

    void IMaterialTechniqueSet::loadBinaryTechnique(Game::MaterialTechnique** tech, const std::string& name, Components::ZoneBuilder::Zone* builder)
    {
        AssertSize(Game::MaterialPass, 20);

        Components::FileSystem::File techFile(Utils::String::VA("techniques/%s.iw4xTech", name.data()));
        if (!techFile.exists()) {
            *tech = nullptr;
            Components::Logger::Print("Warning: Missing technique '%s'\n", name.data());
            return;
        }

        Utils::Stream::Reader reader(builder->getAllocator(), techFile.getBuffer());

        char* magic = reader.readArray<char>(8);
        if (std::memcmp(magic, "IW4xTECH", 8))
        {
            Components::Logger::Error(0, "Reading technique '%s' failed, header is invalid!", name.data());
        }

        std::string version;
        version.push_back(reader.read<char>());
        if (version != IW4X_TECHSET_VERSION)
        {
            Components::Logger::Error("Reading technique '%s' failed, expected version is %d, but it was %d!", name.data(), atoi(IW4X_TECHSET_VERSION), atoi(version.data()));
        }

        unsigned short flags = reader.read<unsigned short>();
        unsigned short passCount = reader.read<unsigned short>();

        Game::MaterialTechnique* asset = (Game::MaterialTechnique*)builder->getAllocator()->allocateArray<unsigned char>(sizeof(Game::MaterialTechnique) + (sizeof(Game::MaterialPass) * (passCount - 1)));

        asset->name = builder->getAllocator()->duplicateString(name);
        asset->flags = flags;
        asset->passCount = passCount;

        Game::MaterialPass* passes = reader.readArray<Game::MaterialPass>(passCount);
        std::memcpy(asset->passArray, passes, sizeof(Game::MaterialPass) * passCount);

        for (unsigned short i = 0; i < asset->passCount; i++)
        { 
            Game::MaterialPass* pass = &asset->passArray[i];

            if (pass->vertexDecl)
            {
                const char* declName = reader.readCString();
                pass->vertexDecl = Components::AssetHandler::FindAssetForZone(Game::XAssetType::ASSET_TYPE_VERTEXDECL, declName, builder).vertexDecl;
            }

            if (pass->vertexShader)
            {
                const char* vsName = reader.readCString();
                pass->vertexShader = Components::AssetHandler::FindAssetForZone(Game::XAssetType::ASSET_TYPE_VERTEXSHADER, vsName, builder).vertexShader;

            }

            if (pass->pixelShader)
            {
                const char* psName = reader.readCString();
                pass->pixelShader = Components::AssetHandler::FindAssetForZone(Game::XAssetType::ASSET_TYPE_PIXELSHADER, psName, builder).pixelShader;
            }

            pass->args = reader.readArray<Game::MaterialShaderArgument>(pass->perPrimArgCount + pass->perObjArgCount + pass->stableArgCount);

            for (int j = 0; j < pass->perPrimArgCount + pass->perObjArgCount + pass->stableArgCount; j++)
            {
                if (pass->args[j].type == 1 || pass->args[j].type == 7)
                {
                    pass->args[j].u.literalConst = reader.readArray<float>(4);
                }

                if (pass->args[j].type == 3 || pass->args[j].type == 5)
                {
                    pass->args[j].u.codeConst.index = *reader.readObject<unsigned short>();
                    pass->args[j].u.codeConst.firstRow = *reader.readObject<unsigned char>();
                    pass->args[j].u.codeConst.rowCount = *reader.readObject<unsigned char>();
                }
            }
        }

        *tech = asset;
    }

    void IMaterialTechniqueSet::loadBinary(Game::XAssetHeader* header, const std::string& name, Components::ZoneBuilder::Zone* builder)
    {
        Components::FileSystem::File tsFile(Utils::String::VA("techsets/%s.iw4xTS", name.data()));
        if (!tsFile.exists()) return;

        Utils::Stream::Reader reader(builder->getAllocator(), tsFile.getBuffer());

        char* magic = reader.readArray<char>(8);
        if (std::memcmp(magic, "IW4xTSET", 8))
        {
            Components::Logger::Error(0, "Reading techset '%s' failed, header is invalid!", name.data());
        }

        std::string version;
        version.push_back(reader.read<char>());
        if (version != IW4X_TECHSET_VERSION)
        {
            Components::Logger::Error("Reading techset '%s' failed, expected version is %d, but it was %d!", name.data(), atoi(IW4X_TECHSET_VERSION), atoi(version.data()));
        }

        Game::MaterialTechniqueSet* asset = reader.readObject<Game::MaterialTechniqueSet>();

        if (asset->name)
        {
            asset->name = reader.readCString();
        }

        for (int i = 0; i < 48; i++)
        {
            if (asset->techniques[i])
            {
                const char* techName = reader.readCString();
                this->loadBinaryTechnique(&asset->techniques[i], techName, builder);
            }
        }


        header->techniqueSet = asset;
    }

	void IMaterialTechniqueSet::mark(Game::XAssetHeader header, Components::ZoneBuilder::Zone* builder)
	{ 
		Game::MaterialTechniqueSet* asset = header.techniqueSet;

		for (int i = 0; i < ARRAYSIZE(Game::MaterialTechniqueSet::techniques); ++i)
		{
			Game::MaterialTechnique* technique = asset->techniques[i];

			if (!technique) continue;

			for (short j = 0; j < technique->passCount; ++j)
			{
				Game::MaterialPass* pass = &technique->passArray[j];

				if (pass->vertexDecl)
				{
					builder->loadAsset(Game::XAssetType::ASSET_TYPE_VERTEXDECL, pass->vertexDecl);
				}

				if (pass->vertexShader)
				{
					builder->loadAsset(Game::XAssetType::ASSET_TYPE_VERTEXSHADER, pass->vertexShader);
				}

				if (pass->pixelShader)
				{
					builder->loadAsset(Game::XAssetType::ASSET_TYPE_PIXELSHADER, pass->pixelShader);
				}
			}
		}
	}

	void IMaterialTechniqueSet::save(Game::XAssetHeader header, Components::ZoneBuilder::Zone* builder)
	{
		AssertSize(Game::MaterialTechniqueSet, 204);

		Utils::Stream* buffer = builder->getBuffer();
		Game::MaterialTechniqueSet* asset = header.techniqueSet;
		Game::MaterialTechniqueSet* dest = buffer->dest<Game::MaterialTechniqueSet>();
		buffer->save(asset);

		buffer->pushBlock(Game::XFILE_BLOCK_VIRTUAL);

		if (asset->name)
		{
			buffer->saveString(builder->getAssetName(this->getType(), asset->name));
			Utils::Stream::ClearPointer(&dest->name);
		}

		// Save_MaterialTechniquePtrArray
		static_assert(ARRAYSIZE(Game::MaterialTechniqueSet::techniques) == 48, "Techniques array invalid!");

		for (int i = 0; i < ARRAYSIZE(Game::MaterialTechniqueSet::techniques); ++i)
		{
			Game::MaterialTechnique* technique = asset->techniques[i];

			if (technique)
			{
				if (builder->hasPointer(technique))
				{
					dest->techniques[i] = builder->getPointer(technique);
				}
				else
				{
					// Size-check is obsolete, as the structure is dynamic
					buffer->align(Utils::Stream::ALIGN_4);
					builder->storePointer(technique);

					Game::MaterialTechnique* destTechnique = buffer->dest<Game::MaterialTechnique>();
					buffer->save(technique, 8);

					// Save_MaterialPassArray
					Game::MaterialPass* destPasses = buffer->dest<Game::MaterialPass>();
					buffer->saveArray(technique->passArray, technique->passCount);

					for (short j = 0; j < technique->passCount; ++j)
					{
						AssertSize(Game::MaterialPass, 20);

						Game::MaterialPass* destPass = &destPasses[j];
						Game::MaterialPass* pass = &technique->passArray[j];

						if (pass->vertexDecl)
						{
							destPass->vertexDecl = builder->saveSubAsset(Game::XAssetType::ASSET_TYPE_VERTEXDECL, pass->vertexDecl).vertexDecl;
						}

						if (pass->vertexShader)
						{
							destPass->vertexShader = builder->saveSubAsset(Game::XAssetType::ASSET_TYPE_VERTEXSHADER, pass->vertexShader).vertexShader;
						}

						if (pass->pixelShader)
						{
							destPass->pixelShader = builder->saveSubAsset(Game::XAssetType::ASSET_TYPE_PIXELSHADER, pass->pixelShader).pixelShader;
						}

						if (pass->args)
						{
							buffer->align(Utils::Stream::ALIGN_4);
							Game::MaterialShaderArgument* destArgs = buffer->dest<Game::MaterialShaderArgument>();
							buffer->saveArray(pass->args, pass->perPrimArgCount + pass->perObjArgCount + pass->stableArgCount);

							for (int k = 0; k < pass->perPrimArgCount + pass->perObjArgCount + pass->stableArgCount; ++k)
							{
								Game::MaterialShaderArgument* arg = &pass->args[k];
								Game::MaterialShaderArgument* destArg = &destArgs[k];

								if (arg->type == 1 || arg->type == 7)
								{
									if (builder->hasPointer(arg->u.literalConst))
									{
										destArg->u.literalConst = builder->getPointer(arg->u.literalConst);
									}
									else
									{
										buffer->align(Utils::Stream::ALIGN_4);
										builder->storePointer(arg->u.literalConst);

										buffer->saveArray(arg->u.literalConst, 4);
										Utils::Stream::ClearPointer(&destArg->u.literalConst);
									}
								}
							}

							Utils::Stream::ClearPointer(&destPass->args);
						}
					}

					if (technique->name)
					{
						buffer->saveString(technique->name);
						Utils::Stream::ClearPointer(&destTechnique->name);
					}

					Utils::Stream::ClearPointer(&dest->techniques[i]);
				}
			}
		}

		buffer->popBlock();
	}
}
