#include "STDInclude.hpp"

namespace Assets
{
	void Isnd_alias_list_t::load(Game::XAssetHeader* header, const std::string& name, Components::ZoneBuilder::Zone* builder)
	{
		auto handler = iw4oa::API::getHandlerForType(static_cast<uint8_t>(this->getType()));

		Components::FileSystem::File aliasFile(Utils::String::VA(handler->getSerializedFilePath(name.c_str())));

		iw4oa::MemoryManager* manager = builder->getAllocator()->allocate<iw4oa::MemoryManager>();

		const auto lambda = [builder](uint8_t type, const char* assetName){
			auto find = Components::AssetHandler::FindAssetForZone(
				static_cast<Game::XAssetType>(type),
				assetName,
				builder
			);
			return *reinterpret_cast<iw4oa::Game::XAssetHeader*>(&find);
		};

		try {
			Game::snd_alias_list_t* aliasList = reinterpret_cast<Game::snd_alias_list_t*>(handler->deserialize(name.c_str(), aliasFile.exists() ? aliasFile.getBuffer() : std::string(), *manager, lambda));

			header->sound = aliasList;
		}
		catch (iw4oa::IAssetHandler::MissingFileException e) {
			Components::Logger::Print("Asset %s could not be found on disk, will try to fetch the asset from the pool instead.\n", name.c_str());
			header->sound = Components::AssetHandler::FindOriginalAsset(this->getType(), name.c_str()).sound;
		}
	}

	void Isnd_alias_list_t::mark(Game::XAssetHeader header, Components::ZoneBuilder::Zone* builder)
	{
		Game::snd_alias_list_t* asset = header.sound;

		for (unsigned int i = 0; i < asset->count; ++i)
		{
			Game::snd_alias_t* alias = &asset->head[i];

			if (alias->soundFile && alias->soundFile->type == Game::snd_alias_type_t::SAT_LOADED)
			{
				builder->loadAsset(Game::XAssetType::ASSET_TYPE_LOADED_SOUND, alias->soundFile->u.loadSnd);
			}

			if (alias->volumeFalloffCurve)
			{
				if (!builder->loadAsset(Game::XAssetType::ASSET_TYPE_SOUND_CURVE, alias->volumeFalloffCurve))
				{
					// (Should never happen, but just in case)
					alias->volumeFalloffCurve->filename = "$default";
					builder->loadAsset(Game::XAssetType::ASSET_TYPE_SOUND_CURVE, alias->volumeFalloffCurve);
				}
			}
		}
	}

	void Isnd_alias_list_t::dump(Game::XAssetHeader header)
	{
		auto ents = header.sound;

		if (ents->count > 32)
		{
			// This is probably garbage data
			return;
		}

		if (ents->count == 0) {
			// Very weird but it happens, notably on mp_crash_snow
			// Soundaliases with a zero-count list crash iw4 so we skip them
			// They should be empty anyway
			return;
		}

		for (size_t i = 0; i < ents->count; i++)
		{
			auto alias = ents->head[i];

			std::string soundFile("");
			if (alias.soundFile)
			{
				switch (alias.soundFile->type)
				{
					// LOADED
				case Game::snd_alias_type_t::SAT_LOADED:
					// Save the LoadedSound subasset
					soundFile = alias.soundFile->u.loadSnd->name;
					auto ldSndHeader = Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_LOADED_SOUND, soundFile.c_str());
					
					Components::AssetHandler::Dump({ Game::XAssetType::ASSET_TYPE_LOADED_SOUND, ldSndHeader });
					break;

					// STREAMED
				case Game::snd_alias_type_t::SAT_STREAMED:
				{
					auto streamedSoundFmt = "dump/sound/%s";
					soundFile = alias.soundFile->u.streamSnd.filename.info.raw.name;

					if (alias.soundFile->u.streamSnd.filename.info.raw.dir)
					{
						soundFile = Utils::String::VA("%s/%s", alias.soundFile->u.streamSnd.filename.info.raw.dir, soundFile.c_str());
					}

					auto fullPath = Utils::String::VA(streamedSoundFmt, soundFile.c_str());
					auto destinationDirectory = Utils::String::VA(streamedSoundFmt, alias.soundFile->u.streamSnd.filename.info.raw.dir);
					auto internalPath = Utils::String::VA("sound/%s", soundFile.c_str());

					Utils::IO::CreateDir(destinationDirectory);
					std::ofstream destination(fullPath, std::ios::binary);

					int handle;
					Game::FS_FOpenFileRead(internalPath, &handle);

					if (handle != 0)
					{
						char buffer[1024];
						int bytesRead;

						while ((bytesRead = Game::FS_Read(buffer, sizeof(buffer), handle)) > 0)
						{
							destination.write(buffer, bytesRead);
						}

						destination.flush();
						destination.close();

						Game::FS_FCloseFile(handle);
					}
					break;
				}

				// I DON'T KNOW :(
				default:
					Components::Logger::Print("Error dumping sound alias %s: unknown format %d\n", alias.aliasName, alias.soundFile->type);
					return;
				}
			}
		}

		auto handler = iw4oa::API::getHandlerForType(static_cast<uint8_t>(this->getType()));
		handler->serialize(reinterpret_cast<void*>(ents), "dump");
	}

	void Isnd_alias_list_t::save(Game::XAssetHeader header, Components::ZoneBuilder::Zone* builder)
	{
		AssertSize(Game::snd_alias_list_t, 12);

		Utils::Stream* buffer = builder->getBuffer();
		Game::snd_alias_list_t* asset = header.sound;
		Game::snd_alias_list_t* dest = buffer->dest<Game::snd_alias_list_t>();
		buffer->save(asset);

		buffer->pushBlock(Game::XFILE_BLOCK_VIRTUAL);

		if (asset->aliasName)
		{
			buffer->saveString(builder->getAssetName(this->getType(), asset->aliasName));
			Utils::Stream::ClearPointer(&dest->aliasName);
		}

		if (asset->head)
		{
			if (builder->hasPointer(asset->head))
			{
				dest->head = builder->getPointer(asset->head);
			}
			else
			{
				AssertSize(Game::snd_alias_t, 100);

				buffer->align(Utils::Stream::ALIGN_4);
				builder->storePointer(asset->head);

				Game::snd_alias_t* destHead = buffer->dest<Game::snd_alias_t>();
				buffer->saveArray(asset->head, asset->count);

				for (unsigned int i = 0; i < asset->count; ++i)
				{
					Game::snd_alias_t* destAlias = &destHead[i];
					Game::snd_alias_t* alias = &asset->head[i];

					if (alias->aliasName)
					{
						buffer->saveString(alias->aliasName);
						Utils::Stream::ClearPointer(&destAlias->aliasName);
					}

					if (alias->subtitle)
					{
						buffer->saveString(alias->subtitle);
						Utils::Stream::ClearPointer(&destAlias->subtitle);
					}

					if (alias->secondaryAliasName)
					{
						buffer->saveString(alias->secondaryAliasName);
						Utils::Stream::ClearPointer(&destAlias->secondaryAliasName);
					}

					if (alias->chainAliasName)
					{
						buffer->saveString(alias->chainAliasName);
						Utils::Stream::ClearPointer(&destAlias->chainAliasName);
					}

					if (alias->mixerGroup)
					{
						buffer->saveString(alias->mixerGroup);
						Utils::Stream::ClearPointer(&destAlias->mixerGroup);
					}

					if (alias->soundFile)
					{
						if (builder->hasPointer(alias->soundFile))
						{
							destAlias->soundFile = builder->getPointer(alias->soundFile);
						}
						else
						{
							AssertSize(Game::snd_alias_t, 100);

							buffer->align(Utils::Stream::ALIGN_4);
							builder->storePointer(alias->soundFile);

							Game::SoundFile* destSoundFile = buffer->dest<Game::SoundFile>();
							buffer->save(alias->soundFile);

							// Save_SoundFileRef
							{
								if (alias->soundFile->type == Game::snd_alias_type_t::SAT_LOADED)
								{
									destSoundFile->u.loadSnd = builder->saveSubAsset(Game::XAssetType::ASSET_TYPE_LOADED_SOUND, alias->soundFile->u.loadSnd).loadSnd;
								}
								else
								{
									// Save_StreamedSound
									{
										if (alias->soundFile->u.streamSnd.filename.info.raw.dir)
										{
											buffer->saveString(alias->soundFile->u.streamSnd.filename.info.raw.dir);
											Utils::Stream::ClearPointer(&destSoundFile->u.streamSnd.filename.info.raw.dir);
										}

										if (alias->soundFile->u.streamSnd.filename.info.raw.name)
										{
											buffer->saveString(alias->soundFile->u.streamSnd.filename.info.raw.name);
											Utils::Stream::ClearPointer(&destSoundFile->u.streamSnd.filename.info.raw.name);
										}
									}
								}
							}

							Utils::Stream::ClearPointer(&destAlias->soundFile);
						}
					}

					if (alias->volumeFalloffCurve)
					{
						destAlias->volumeFalloffCurve = builder->saveSubAsset(Game::XAssetType::ASSET_TYPE_SOUND_CURVE, alias->volumeFalloffCurve).sndCurve;
					}

					if (alias->speakerMap)
					{
						if (builder->hasPointer(alias->speakerMap))
						{
							destAlias->speakerMap = builder->getPointer(alias->speakerMap);
						}
						else
						{
							AssertSize(Game::SpeakerMap, 408);

							buffer->align(Utils::Stream::ALIGN_4);
							builder->storePointer(alias->speakerMap);

							Game::SpeakerMap* destSoundFile = buffer->dest<Game::SpeakerMap>();
							buffer->save(alias->speakerMap);

							if (alias->speakerMap->name)
							{
								buffer->saveString(alias->speakerMap->name);
								Utils::Stream::ClearPointer(&destSoundFile->name);
							}

							Utils::Stream::ClearPointer(&destAlias->speakerMap);
						}
					}
				}

				Utils::Stream::ClearPointer(&dest->head);
			}
		}

		buffer->popBlock();
	}


	Isnd_alias_list_t::Isnd_alias_list_t() : IAsset()
	{

		Components::Command::Add("dumpSound", [this](Components::Command::Params* params)
			{
				if (params->length() < 2) return;
				dump(Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_SOUND, params->get(1)));
			});
	}
}
