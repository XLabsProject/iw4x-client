#include "STDInclude.hpp"

#define IW4X_MODEL_VERSION 5

namespace Assets
{

	void IXModel::dumpXSurfaceCollisionTree(Game::XSurfaceCollisionTree* entry, Utils::Stream* buffer)
	{
		buffer->saveObject(*entry);

		if (entry->nodes)
		{
			buffer->saveArray(entry->nodes, entry->nodeCount);
		}

		if (entry->leafs)
		{
			buffer->saveArray(entry->leafs, entry->leafCount);
		}
	}

	void IXModel::dumpXSurface(Game::XSurface* surf, Utils::Stream* buffer)
	{
		if (surf->vertInfo.vertsBlend)
		{
			buffer->saveArray(surf->vertInfo.vertsBlend, surf->vertInfo.vertCount[0] + (surf->vertInfo.vertCount[1] * 3) + (surf->vertInfo.vertCount[2] * 5) + (surf->vertInfo.vertCount[3] * 7));
		}

		// Access vertex block
		if (surf->verts0)
		{
			buffer->saveArray(surf->verts0, surf->vertCount);
		}

		// Save_XRigidVertListArray
		if (surf->vertList)
		{
			buffer->saveArray(surf->vertList, surf->vertListCount);

			for (unsigned int i = 0; i < surf->vertListCount; ++i)
			{
				Game::XRigidVertList* rigidVertList = &surf->vertList[i];

				if (rigidVertList->collisionTree)
				{
					IXModel::dumpXSurfaceCollisionTree(rigidVertList->collisionTree, buffer);
				}
			}
		}

		// Access index block
		if (surf->triIndices)
		{
			buffer->saveArray(surf->triIndices, surf->triCount * 3);
		}
	}

	void IXModel::dumpXModelSurfs(Game::XModelSurfs* asset, Utils::Stream* buffer)
	{
		buffer->saveObject(*asset);

		if (asset->name)
		{
			buffer->saveString(asset->name);
		}

		if (asset->surfs)
		{
			buffer->saveArray(asset->surfs, asset->numsurfs);

			for (int i = 0; i < asset->numsurfs; ++i)
			{
				IXModel::dumpXSurface(&asset->surfs[i], buffer);
			}
		}
	}

	void IXModel::dump(Game::XAssetHeader header)
	{
		auto asset = header.model;

		if (!asset) return;

		Utils::Stream buffer;
		buffer.saveArray("IW4xModl", 8);
		buffer.saveObject(IW4X_MODEL_VERSION);

		buffer.saveObject(*asset);

		if (asset->name)
		{
			buffer.saveString(asset->name);
		}

		if (asset->boneNames)
		{
			for (char i = 0; i < asset->numBones; ++i)
			{
				buffer.saveString(Game::SL_ConvertToString(asset->boneNames[i]));
			}
		}

		if (asset->parentList)
		{
			buffer.saveArray(asset->parentList, asset->numBones - asset->numRootBones);
		}

		if (asset->quats)
		{
			buffer.saveArray(asset->quats, (asset->numBones - asset->numRootBones) * 4);
		}

		if (asset->trans)
		{
			buffer.saveArray(asset->trans, (asset->numBones - asset->numRootBones) * 3);
		}

		if (asset->partClassification)
		{
			buffer.saveArray(asset->partClassification, asset->numBones);
		}

		if (asset->baseMat)
		{
			buffer.saveArray(asset->baseMat, asset->numBones);
		}

		if (asset->materialHandles)
		{
			buffer.saveArray(asset->materialHandles, asset->numsurfs);

			for (unsigned char i = 0; i < asset->numsurfs; ++i)
			{
				if (asset->materialHandles[i])
				{
					buffer.saveString(asset->materialHandles[i]->info.name);

					auto assetHeader = Game::XAssetHeader{};
					assetHeader.material = asset->materialHandles[i];
					Components::AssetHandler::Dump({ Game::XAssetType::ASSET_TYPE_MATERIAL, assetHeader });
				}
			}
		}

		// Save_XModelLodInfoArray
		{
			for (int i = 0; i < 4; ++i)
			{
				if (asset->lodInfo[i].modelSurfs)
				{
					IXModel::dumpXModelSurfs(asset->lodInfo[i].modelSurfs, &buffer);
				}
			}
		}

		// Save_XModelCollSurfArray
		if (asset->collSurfs)
		{
			buffer.saveArray(asset->collSurfs, asset->numCollSurfs);

			for (int i = 0; i < asset->numCollSurfs; ++i)
			{
				Game::XModelCollSurf_s* collSurf = &asset->collSurfs[i];

				if (collSurf->collTris)
				{
					buffer.saveArray(collSurf->collTris, collSurf->numCollTris);
				}
			}
		}

		if (asset->boneInfo)
		{
			buffer.saveArray(asset->boneInfo, asset->numBones);
		}

		if (asset->physPreset)
		{
			buffer.saveObject(*asset->physPreset);

			if (asset->physPreset->name)
			{
				buffer.saveString(asset->physPreset->name);
			}

			if (asset->physPreset->sndAliasPrefix)
			{
				buffer.saveString(asset->physPreset->sndAliasPrefix);
			}
		}

		if (asset->physCollmap)
		{
			Game::PhysCollmap* collmap = asset->physCollmap;
			buffer.saveObject(*collmap);

			if (collmap->name)
			{
				buffer.saveString(collmap->name);
			}

			if (collmap->geoms)
			{
				buffer.saveArray(collmap->geoms, collmap->count);

				for (unsigned int i = 0; i < collmap->count; ++i)
				{
					Game::PhysGeomInfo* geom = &collmap->geoms[i];

					if (geom->brushWrapper)
					{
						Game::BrushWrapper* brush = geom->brushWrapper;
						buffer.saveObject(*brush);
						{
							if (brush->brush.sides)
							{
								buffer.saveArray(brush->brush.sides, brush->brush.numsides);

								// Save_cbrushside_tArray
								for (unsigned short j = 0; j < brush->brush.numsides; ++j)
								{
									Game::cbrushside_t* side = &brush->brush.sides[j];

									if (side->plane)
									{
										buffer.saveObject(*side->plane);
									}
								}
							}

							if (brush->brush.baseAdjacentSide)
							{
								buffer.saveArray(brush->brush.baseAdjacentSide, brush->totalEdgeCount);
							}
						}

						// TODO: Add pointer support
						if (brush->planes)
						{
							buffer.saveArray(brush->planes, brush->brush.numsides);
						}
					}
				}
			}
		}

		Utils::IO::WriteFile(Utils::String::VA("%s/xmodel/%s.iw4xModel", "dump", asset->name), buffer.toBuffer());
	}


	void IXModel::loadXSurfaceCollisionTree(Game::XSurfaceCollisionTree* entry, Utils::Stream::Reader* reader)
	{
		if (entry->nodes)
		{
			entry->nodes = reader->readArray<Game::XSurfaceCollisionNode>(entry->nodeCount);
		}

		if (entry->leafs)
		{
			entry->leafs = reader->readArray<Game::XSurfaceCollisionLeaf>(entry->leafCount);
		}
	}

	void IXModel::loadXSurface(Game::XSurface* surf, Utils::Stream::Reader* reader, Components::ZoneBuilder::Zone* builder)
	{
		if (surf->vertInfo.vertsBlend)
		{
			surf->vertInfo.vertsBlend = reader->readArray<unsigned short>(surf->vertInfo.vertCount[0] + (surf->vertInfo.vertCount[1] * 3) + (surf->vertInfo.vertCount[2] * 5) + (surf->vertInfo.vertCount[3] * 7));
		}

		// Access vertex block
		if (surf->verts0)
		{
			surf->verts0 = reader->readArray<Game::GfxPackedVertex>(surf->vertCount);
		}

		// Save_XRigidVertListArray
		if (surf->vertList)
		{
			surf->vertList = reader->readArray<Game::XRigidVertList>(surf->vertListCount);

			for (unsigned int i = 0; i < surf->vertListCount; ++i)
			{
				Game::XRigidVertList* rigidVertList = &surf->vertList[i];

				if (rigidVertList->collisionTree)
				{
					rigidVertList->collisionTree = reader->readObject<Game::XSurfaceCollisionTree>();
					this->loadXSurfaceCollisionTree(rigidVertList->collisionTree, reader);
				}
			}
		}

		// Access index block
        if (surf->triIndices)
        {
            void* oldPtr = surf->triIndices;
            surf->triIndices = reader->readArray<unsigned short>(surf->triCount * 3);

            if (builder->getAllocator()->isPointerMapped(oldPtr))
            {
                surf->triIndices = builder->getAllocator()->getPointer<unsigned short>(oldPtr);
            }
            else
            {
                builder->getAllocator()->mapPointer(oldPtr, surf->triIndices);
            }
        }
	}

	void IXModel::loadXModelSurfs(Game::XModelSurfs* asset, Utils::Stream::Reader* reader, Components::ZoneBuilder::Zone* builder)
	{
		if (asset->name)
		{
			asset->name = reader->readCString();
		}

		if (asset->surfs)
		{
			asset->surfs = reader->readArray<Game::XSurface>(asset->numsurfs);

			for (int i = 0; i < asset->numsurfs; ++i)
			{
				this->loadXSurface(&asset->surfs[i], reader, builder);
			}
		}
	}

	void IXModel::load(Game::XAssetHeader* header, const std::string& name, Components::ZoneBuilder::Zone* builder)
	{
		Components::FileSystem::File modelFile(Utils::String::VA("xmodel/%s.iw4xModel", name.data()));

		if (!builder->isPrimaryAsset() && (Components::ZoneBuilder::MatchTechsetsDvar.get<bool>() || !modelFile.exists()))
		{
			header->model = Components::AssetHandler::FindOriginalAsset(this->getType(), name.data()).model;
			if (header->model) return;
		}


		if (modelFile.exists())
		{

			header->model = asset;
		}
	}

	void IXModel::mark(Game::XAssetHeader header, Components::ZoneBuilder::Zone* builder)
	{
		Game::XModel* asset = header.model;

		if (asset->boneNames)
		{
			for (char i = 0; i < asset->numBones; ++i)
			{
				builder->addScriptString(asset->boneNames[i]);
			}
		}

		if (asset->materialHandles)
		{
			for (unsigned char i = 0; i < asset->numsurfs; ++i)
			{
				if (asset->materialHandles[i])
				{
					builder->loadAsset(Game::XAssetType::ASSET_TYPE_MATERIAL, asset->materialHandles[i]);
				}
			}
		}

		for (int i = 0; i < 4; ++i)
		{
			if (asset->lodInfo[i].modelSurfs)
			{
				builder->loadAsset(Game::XAssetType::ASSET_TYPE_XMODEL_SURFS, asset->lodInfo[i].modelSurfs);
			}
		}

		if (asset->physPreset)
		{
			builder->loadAsset(Game::XAssetType::ASSET_TYPE_PHYSPRESET, asset->physPreset);
		}

		if (asset->physCollmap)
		{
			builder->loadAsset(Game::XAssetType::ASSET_TYPE_PHYSCOLLMAP, asset->physCollmap);
		}
	}

	void IXModel::save(Game::XAssetHeader header, Components::ZoneBuilder::Zone* builder)
	{
		AssertSize(Game::XModel, 304);

		Utils::Stream* buffer = builder->getBuffer();
		Game::XModel* asset = header.model;
		Game::XModel* dest = buffer->dest<Game::XModel>();
		buffer->save(asset);

		buffer->pushBlock(Game::XFILE_BLOCK_VIRTUAL);

		if (asset->name)
		{
			buffer->saveString(builder->getAssetName(this->getType(), asset->name));
			Utils::Stream::ClearPointer(&dest->name);
		}

		if (asset->boneNames)
		{
			buffer->align(Utils::Stream::ALIGN_2);

			unsigned short* destBoneNames = buffer->dest<unsigned short>();
			buffer->saveArray(asset->boneNames, asset->numBones);

			for (char i = 0; i < asset->numBones; ++i)
			{
				builder->mapScriptString(&destBoneNames[i]);
			}

			Utils::Stream::ClearPointer(&dest->boneNames);
		}

		if (asset->parentList)
		{
			buffer->save(asset->parentList, asset->numBones - asset->numRootBones);
			Utils::Stream::ClearPointer(&dest->parentList);
		}

		if (asset->quats)
		{
			buffer->align(Utils::Stream::ALIGN_2);
			buffer->saveArray(asset->quats, (asset->numBones - asset->numRootBones) * 4);
			Utils::Stream::ClearPointer(&dest->quats);
		}

		if (asset->trans)
		{
			buffer->align(Utils::Stream::ALIGN_4);
			buffer->saveArray(asset->trans, (asset->numBones - asset->numRootBones) * 3);
			Utils::Stream::ClearPointer(&dest->trans);
		}

		if (asset->partClassification)
		{
			buffer->save(asset->partClassification, asset->numBones);
			Utils::Stream::ClearPointer(&dest->partClassification);
		}

		if (asset->baseMat)
		{
			AssertSize(Game::DObjAnimMat, 32);

			buffer->align(Utils::Stream::ALIGN_4);
			buffer->saveArray(asset->baseMat, asset->numBones);
			Utils::Stream::ClearPointer(&dest->baseMat);
		}

		if (asset->materialHandles)
		{
			buffer->align(Utils::Stream::ALIGN_4);

			Game::Material** destMaterials = buffer->dest<Game::Material*>();
			buffer->saveArray(asset->materialHandles, asset->numsurfs);

			for (unsigned char i = 0; i < asset->numsurfs; ++i)
			{
				if (asset->materialHandles[i])
				{
					destMaterials[i] = builder->saveSubAsset(Game::XAssetType::ASSET_TYPE_MATERIAL, asset->materialHandles[i]).material;
				}
			}

			Utils::Stream::ClearPointer(&dest->materialHandles);
		}

		// Save_XModelLodInfoArray
		{
			AssertSize(Game::XModelLodInfo, 44);

			for (int i = 0; i < 4; ++i)
			{
				if (asset->lodInfo[i].modelSurfs)
				{
					dest->lodInfo[i].modelSurfs = builder->saveSubAsset(Game::XAssetType::ASSET_TYPE_XMODEL_SURFS, asset->lodInfo[i].modelSurfs).modelSurfs;
				}
			}
		}

		// Save_XModelCollSurfArray
		if (asset->collSurfs)
		{
			AssertSize(Game::XModelCollSurf_s, 44);

			buffer->align(Utils::Stream::ALIGN_4);

			Game::XModelCollSurf_s* destColSurfs = buffer->dest<Game::XModelCollSurf_s>();
			buffer->saveArray(asset->collSurfs, asset->numCollSurfs);

			for (int i = 0; i < asset->numCollSurfs; ++i)
			{
				Game::XModelCollSurf_s* destCollSurf = &destColSurfs[i];
				Game::XModelCollSurf_s* collSurf = &asset->collSurfs[i];

				if (collSurf->collTris)
				{
					buffer->align(Utils::Stream::ALIGN_4);

					buffer->save(collSurf->collTris, 48, collSurf->numCollTris);
					Utils::Stream::ClearPointer(&destCollSurf->collTris);
				}
			}

			Utils::Stream::ClearPointer(&dest->collSurfs);
		}

		if (asset->boneInfo)
		{
			AssertSize(Game::XBoneInfo, 28);

			buffer->align(Utils::Stream::ALIGN_4);

			buffer->saveArray(asset->boneInfo, asset->numBones);
			Utils::Stream::ClearPointer(&dest->boneInfo);
		}

		if (asset->physPreset)
		{
			dest->physPreset = builder->saveSubAsset(Game::XAssetType::ASSET_TYPE_PHYSPRESET, asset->physPreset).physPreset;
		}

		if (asset->physCollmap)
		{
			dest->physCollmap = builder->saveSubAsset(Game::XAssetType::ASSET_TYPE_PHYSCOLLMAP, asset->physCollmap).physCollmap;
		}

		buffer->popBlock();
	}

	IXModel::IXModel() : Components::AssetHandler::IAsset()
	{
		Components::Command::Add("dumpXModel", [this](Components::Command::Params* params)
		{
			if (params->length() < 2) return;
			auto mdlName = params->get(1);
			IXModel::dump(Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_XMODEL, mdlName).model);
		});
	}
}
