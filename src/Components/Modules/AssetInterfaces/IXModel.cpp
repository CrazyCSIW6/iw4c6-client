#include <STDInclude.hpp>

#define IW4X_MODEL_VERSION 4

namespace Assets
{
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

	void IXModel::loadXSurface(Game::XSurface* surf, Utils::Stream::Reader* reader)
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
			surf->triIndices = reader->readArray<unsigned short>(surf->triCount * 3);
		}
	}

	void IXModel::loadXModelSurfs(Game::XModelSurfs* asset, Utils::Stream::Reader* reader)
	{
		if (asset->name)
		{
			asset->name = reader->readCString();
		}

		if (asset->surfaces)
		{
			asset->surfaces = reader->readArray<Game::XSurface>(asset->numSurfaces);

			for (int i = 0; i < asset->numSurfaces; ++i)
			{
				this->loadXSurface(&asset->surfaces[i], reader);
			}
		}
	}

	void IXModel::load(Game::XAssetHeader* header, std::string name, Components::ZoneBuilder::Zone* builder)
	{
		Components::FileSystem::File modelFile(Utils::String::VA("xmodel/%s.iw4xModel", name.data()));

		if (modelFile.exists())
		{
			Utils::Stream::Reader reader(builder->getAllocator(), modelFile.getBuffer());

			if (reader.read<__int64>() != *reinterpret_cast<__int64*>("IW4xModl"))
			{
				Components::Logger::Error(0, "Reading model '%s' failed, header is invalid!", name.data());
			}

			int version = reader.read<int>();
			if (version != IW4X_MODEL_VERSION)
			{
				Components::Logger::Error(0, "Reading model '%s' failed, expected version is %d, but it was %d!", name.data(), IW4X_MODEL_VERSION, version);
			}

			Game::XModel* asset = reader.readObject<Game::XModel>();

			if (asset->name)
			{
				asset->name = reader.readCString();
			}

			if (asset->boneNames)
			{
				asset->boneNames = builder->getAllocator()->allocateArray<unsigned short>(asset->numBones);

				for (char i = 0; i < asset->numBones; ++i)
				{
					asset->boneNames[i] = Game::SL_GetString(reader.readCString(), 0);
				}
			}

			if (asset->parentList)
			{
				asset->parentList = reader.readArray<char>(asset->numBones - asset->numRootBones);
			}

			if (asset->quats)
			{
				asset->quats = reader.readArray<short>((asset->numBones - asset->numRootBones) * 4);
			}

			if (asset->trans)
			{
				asset->trans = reader.readArray<float>((asset->numBones - asset->numRootBones) * 3);
			}

			if (asset->partClassification)
			{
				asset->partClassification = reader.readArray<char>(asset->numBones);
			}

			if (asset->baseMat)
			{
				asset->baseMat = reader.readArray<Game::DObjAnimMat>(asset->numBones);
			}

			if (asset->materialHandles)
			{
				asset->materialHandles = reader.readArray<Game::Material*>(asset->numsurfs);

				for (char i = 0; i < asset->numsurfs; ++i)
				{
					if (asset->materialHandles[i])
					{
						asset->materialHandles[i] = Components::AssetHandler::FindAssetForZone(Game::XAssetType::ASSET_TYPE_MATERIAL, reader.readString(), builder).material;
					}
				}
			}

			// Save_XModelLodInfoArray
			{
				for (int i = 0; i < 4; ++i)
				{
					if (asset->lodInfo[i].modelSurfs)
					{
						asset->lodInfo[i].modelSurfs = reader.readObject<Game::XModelSurfs>();
						this->loadXModelSurfs(asset->lodInfo[i].modelSurfs, &reader);
						Components::AssetHandler::StoreTemporaryAsset(Game::XAssetType::ASSET_TYPE_XMODELSURFS, { asset->lodInfo[i].modelSurfs });

						asset->lodInfo[i].surfs = asset->lodInfo[i].modelSurfs->surfaces;

						// Zero that for now, it breaks the models.
						// TODO: Figure out how that can be converted
						asset->lodInfo[i].smcBaseIndexPlusOne = 0;
						asset->lodInfo[i].smcSubIndexMask = 0;
						asset->lodInfo[i].smcBucket = 0;
					}
				}
			}

			// Save_XModelCollSurfArray
			if (asset->collSurfs)
			{
				asset->collSurfs = reader.readArray<Game::XModelCollSurf_s>(asset->numCollSurfs);

				for (int i = 0; i < asset->numCollSurfs; ++i)
				{
					Game::XModelCollSurf_s* collSurf = &asset->collSurfs[i];

					if (collSurf->collTris)
					{
						collSurf->collTris = reader.readArray<Game::XModelCollTri_s>(collSurf->numCollTris);
					}
				}
			}

			if (asset->boneInfo)
			{
				asset->boneInfo = reader.readArray<Game::XBoneInfo>(asset->numBones);
			}

			if (asset->physPreset)
			{
				asset->physPreset = reader.readObject<Game::PhysPreset>();

				if (asset->physPreset->name)
				{
					asset->physPreset->name = reader.readCString();
				}

				if (asset->physPreset->sndAliasPrefix)
				{
					asset->physPreset->sndAliasPrefix = reader.readCString();
				}

				// This is an experiment, ak74 fails though
				if (asset->name == "weapon_ak74u"s)
				{
					asset->physPreset = nullptr;
				}
				else
				{
					Components::AssetHandler::StoreTemporaryAsset(Game::XAssetType::ASSET_TYPE_PHYSPRESET, { asset->physPreset });
				}
			}

			if (asset->physCollmap)
			{
				// TODO
				asset->physCollmap = nullptr;
			}

			if (!reader.end())
			{
				Components::Logger::Error(0, "Reading model '%s' failed, remaining raw data found!", name.data());
			}

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
			for (char i = 0; i < asset->numsurfs; ++i)
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
				builder->loadAsset(Game::XAssetType::ASSET_TYPE_XMODELSURFS, asset->lodInfo[i].modelSurfs);
			}
		}

		if (asset->physPreset)
		{
			builder->loadAsset(Game::XAssetType::ASSET_TYPE_PHYSPRESET, asset->physPreset);
		}

		if (asset->physCollmap)
		{
			builder->loadAsset(Game::XAssetType::ASSET_TYPE_PHYS_COLLMAP, asset->physCollmap);
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

			for (char i = 0; i < asset->numsurfs; ++i)
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
					dest->lodInfo[i].modelSurfs = builder->saveSubAsset(Game::XAssetType::ASSET_TYPE_XMODELSURFS, asset->lodInfo[i].modelSurfs).surfaces;
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
			dest->physCollmap = builder->saveSubAsset(Game::XAssetType::ASSET_TYPE_PHYS_COLLMAP, asset->physCollmap).physCollmap;
		}

		buffer->popBlock();
	}
}
