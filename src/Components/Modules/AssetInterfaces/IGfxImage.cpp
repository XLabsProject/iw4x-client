#include "STDInclude.hpp"

#define IW4X_IMG_VERSION "0"

namespace Assets
{
	void IGfxImage::load(Game::XAssetHeader* header, const std::string& name, Components::ZoneBuilder::Zone* builder)
	{
		Game::GfxImage* image = Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_IMAGE, name.data()).image;
		if (image && name[0] != '*') return;

		image = builder->getAllocator()->allocate<Game::GfxImage>();
		if (!image)
		{
			Components::Logger::Error("Failed to allocate GfxImage structure!");
			return;
		}

		image->name = builder->getAllocator()->duplicateString(name);
		image->semantic = 2;

		const char* tempName = image->name;
		if (tempName[0] == '*') tempName++;

		Components::FileSystem::File imageFile(Utils::String::VA("images/%s.iw4xImage", tempName));
		if (imageFile.exists())
		{
			Utils::Stream::Reader reader(builder->getAllocator(), imageFile.getBuffer());

			__int64 magic = reader.read<__int64>();
			if (std::memcmp(&magic, "IW4xImg" IW4X_IMG_VERSION, 8))
			{
				Components::Logger::Error(0, "Reading image '%s' failed, header is invalid!", name.data());
			}

			image->mapType = reader.read<char>();
			image->semantic = reader.read<char>();
			image->category = reader.read<char>();

			int dataLength = reader.read<int>();
			image->cardMemory.platform[0] = dataLength;
			image->cardMemory.platform[1] = dataLength;

			Game::GfxImageLoadDefIW3 loadDef;
			image->texture.loadDef = reinterpret_cast<Game::GfxImageLoadDef*>(reader.readArray<char>(dataLength + 16));
			std::memcpy(&loadDef, image->texture.loadDef, sizeof(loadDef));

			image->texture.loadDef->levelCount = loadDef.levelCount;
			image->texture.loadDef->flags = loadDef.flags;
			image->texture.loadDef->format = loadDef.format;
			image->texture.loadDef->resourceSize = loadDef.resourceSize;
			ZeroMemory(image->texture.loadDef->pad, 3);

			if (image->texture.loadDef->resourceSize != dataLength)
			{
				Components::Logger::Error("Resource size doesn't match the data length (%s)!\n", name.data());
			}

			image->width = loadDef.dimensions[0];
			image->height = loadDef.dimensions[1];
			image->depth = loadDef.dimensions[2];

			image->delayLoadPixels = true;

			header->image = image;
		}
		else if (name[0] != '*')
		{
			char nameBuffer[MAX_PATH] = { 0 };
			Components::Materials::FormatImagePath(nameBuffer, sizeof(nameBuffer), 0, 0, name.data());
			Components::FileSystem::File iwi(nameBuffer);

			if (!iwi.exists())
			{
				Components::Logger::Error("Loading image '%s' failed!", iwi.getName().data());
				return;
			}

			auto iwiBuffer = iwi.getBuffer();

			const Game::GfxImageFileHeader* iwiHeader = reinterpret_cast<const Game::GfxImageFileHeader*>(iwiBuffer.data());

			if (std::memcmp(iwiHeader->tag, "IWi", 3) && iwiHeader->version == 8)
			{
				Components::Logger::Error("Image is not a valid IWi!");
				return;
			}

			image->mapType = Game::MAPTYPE_2D;
			image->cardMemory.platform[0] = iwiHeader->fileSizeForPicmip[0] - 32;
			image->cardMemory.platform[1] = iwiHeader->fileSizeForPicmip[0] - 32;

			image->texture.loadDef = builder->getAllocator()->allocate<Game::GfxImageLoadDef>();
			if (!image->texture.loadDef)
			{
				Components::Logger::Error("Failed to allocate GfxImageLoadDef structure!");
				return;
			}

			image->texture.loadDef->flags = iwiHeader->flags;
			image->texture.loadDef->levelCount = 0;

			image->width = iwiHeader->dimensions[0];
			image->height = iwiHeader->dimensions[1];
			image->depth = iwiHeader->dimensions[2];

			switch (iwiHeader->format)
			{
			case Game::IMG_FORMAT_BITMAP_RGBA:
			{
				image->texture.loadDef->format = 21;
				break;
			}

			case Game::IMG_FORMAT_BITMAP_RGB:
			{
				image->texture.loadDef->format = 20;
				break;
			}

			case Game::IMG_FORMAT_DXT1:
			{
				image->texture.loadDef->format = 0x31545844;
				break;
			}

			case Game::IMG_FORMAT_DXT3:
			{
				image->texture.loadDef->format = 0x33545844;
				break;
			}

			case Game::IMG_FORMAT_DXT5:
			{
				image->texture.loadDef->format = 0x35545844;
				break;
			}

			default:
			{
				break;
			}
			}

			header->image = image;
		}
	}

	void IGfxImage::dump(Game::XAssetHeader header)
	{
		auto image = header.image;

		if (!image) return;
		std::string name = image->name;

		if (image->texture.loadDef && image->texture.loadDef->resourceSize > 0)
		{
			if (name[0] == '*') name.erase(name.begin());

			Utils::Stream buffer;
			buffer.saveArray("IW4xImg" IW4X_IMG_VERSION, 8); // just stick version in the magic since we have an extra char

			buffer.saveObject(static_cast<unsigned char>(image->mapType));
			buffer.saveObject(image->semantic);
			buffer.saveObject(image->category);

			buffer.saveObject(image->texture.loadDef->resourceSize);
			buffer.save(image->texture.loadDef, 16 + image->texture.loadDef->resourceSize);

			Utils::IO::WriteFile(Utils::String::VA("dump/images/%s.iw4xImage", name.data()), buffer.toBuffer());
		}
		else
		{
			Components::FileSystem::File img(Utils::String::VA("images/%s.iwi", image->name));

			if (!img.exists())
			{
				// Ignore that
				if (Utils::String::StartsWith(image->name, "watersetup")) return;

				Components::Logger::Print("Image %s not found, mapping to normalmap!\n", name.data());
				img = Components::FileSystem::File("images/$identitynormalmap.iwi");
			}

			if (img.exists())
			{
				Game::GfxImageFileHeader fileHeader = *reinterpret_cast<const Game::GfxImageFileHeader*>(img.getBuffer().data());

				std::string buffer(reinterpret_cast<char*>(&fileHeader), sizeof Game::GfxImageFileHeader);
				buffer.append(img.getBuffer().data() + (sizeof Game::GfxImageFileHeader), img.getBuffer().size() - (sizeof Game::GfxImageFileHeader));

				Utils::IO::WriteFile(Utils::String::VA("dump/images/%s.iwi", image->name), buffer);
			}
			else
			{
				Components::Logger::Print("Unable to map to normalmap, this should not happen!\n");
			}
		}
	}

	void IGfxImage::save(Game::XAssetHeader header, Components::ZoneBuilder::Zone* builder)
	{
		AssertSize(Game::GfxImage, 32);

		Utils::Stream* buffer = builder->getBuffer();
		Game::GfxImage* asset = header.image;
		Game::GfxImage* dest = buffer->dest<Game::GfxImage>();
		buffer->save(asset);

		buffer->pushBlock(Game::XFILE_BLOCK_VIRTUAL);

		if (asset->name)
		{
			buffer->saveString(builder->getAssetName(this->getType(), asset->name));
			Utils::Stream::ClearPointer(&dest->name);
		}

		buffer->pushBlock(Game::XFILE_BLOCK_TEMP);

		if (asset->texture.loadDef)
		{
			buffer->align(Utils::Stream::ALIGN_4);

			Game::GfxImageLoadDef* destTexture = buffer->dest<Game::GfxImageLoadDef>();
			buffer->save(asset->texture.loadDef, 16, 1);

			builder->incrementExternalSize(asset->texture.loadDef->resourceSize);

			if (destTexture->resourceSize > 0)
			{
				buffer->save(asset->texture.loadDef->data, asset->texture.loadDef->resourceSize);
			}

			Utils::Stream::ClearPointer(&dest->texture.loadDef);
		}

		buffer->popBlock();
		buffer->popBlock();
	}
}
