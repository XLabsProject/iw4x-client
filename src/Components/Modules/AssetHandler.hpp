#pragma once

namespace Components
{
	class AssetHandler : public Component
	{
	public:
		class IAsset
		{
		public:
			virtual ~IAsset() {};
			virtual Game::XAssetType getType() { return Game::XAssetType::ASSET_TYPE_INVALID; };
			virtual void mark(Game::XAssetHeader /*header*/, ZoneBuilder::Zone* /*builder*/) { /*ErrorTypeNotSupported(this);*/ };
			virtual void save(Game::XAssetHeader /*header*/, ZoneBuilder::Zone* /*builder*/) { /*ErrorTypeNotSupported(this);*/ };
			virtual void dump(Game::XAssetHeader /*header*/) { /*ErrorTypeNotSupported(this);*/ };
			virtual void load(Game::XAssetHeader* /*header*/, const std::string& /*name*/, ZoneBuilder::Zone* /*builder*/) { /*ErrorTypeNotSupported(this);*/ };
		};

		typedef Game::XAssetHeader(Callback)(Game::XAssetType type, const std::string& name);
		typedef void(RestrictCallback)(Game::XAssetType type, Game::XAssetHeader asset, const std::string& name, bool* restrict);

		AssetHandler();
		~AssetHandler();

		static void OnFind(Game::XAssetType type, Utils::Slot<Callback> callback);
		static void OnLoad(Utils::Slot<RestrictCallback> callback);

		static void ClearRelocations();
		static void Relocate(void* start, void* to, DWORD size = 4);

		static void ZoneSave(Game::XAsset asset, ZoneBuilder::Zone* builder);
		static void ZoneMark(Game::XAsset asset, ZoneBuilder::Zone* builder);
		static void Dump(Game::XAsset asset);

		static Game::XAssetHeader FindOriginalAsset(Game::XAssetType type, const char* filename);
		static Game::XAssetHeader FindAssetForZone(Game::XAssetType type, const std::string& filename, ZoneBuilder::Zone* builder, bool isSubAsset = true);

		static void ClearTemporaryAssets();
		static void StoreTemporaryAsset(Game::XAssetType type, Game::XAssetHeader asset);

		static void ResetBypassState();

        static void ExposeTemporaryAssets(bool expose);

		static void OffsetToAlias(Utils::Stream::Offset* offset);
		
	private:
		static thread_local int BypassState;
        static bool ShouldSearchTempAssets;

		static std::map<std::string, Game::XAssetHeader> TemporaryAssets[Game::XAssetType::ASSET_TYPE_COUNT];

		static std::map<Game::XAssetType, IAsset*> AssetInterfaces;
		static std::map<Game::XAssetType, Utils::Slot<Callback>> TypeCallbacks;
		static Utils::Signal<RestrictCallback> RestrictSignal;

		static std::map<void*, void*> Relocations;

		static std::vector<std::pair<Game::XAssetType, std::string>> EmptyAssets;

		static void RegisterInterface(IAsset* iAsset);

		static Game::XAssetHeader FindAsset(Game::XAssetType type, const char* filename);
        static Game::XAssetHeader FindTemporaryAsset(Game::XAssetType type, const char* filename);
		static bool IsAssetEligible(Game::XAssetType type, Game::XAssetHeader* asset);
		static void FindAssetStub();
		static void AddAssetStub();

		static void StoreEmptyAsset(Game::XAssetType type, const char* name);
		static void StoreEmptyAssetStub();

		static void ModifyAsset(Game::XAssetType type, Game::XAssetHeader asset, const std::string& name);

		static int HasThreadBypass();
		static void SetBypassState(bool value);

		static void MissingAssetError(int severity, const char* format, const char* type, const char* name);

		void reallocateEntryPool();
	};
}

#include "AssetInterfaces/IFont_s.hpp"
#include "AssetInterfaces/IWeapon.hpp"
#include "AssetInterfaces/IXModel.hpp"
#include "AssetInterfaces/IFxWorld.hpp"
#include "AssetInterfaces/IMapEnts.hpp"
#include "AssetInterfaces/IRawFile.hpp"
#include "AssetInterfaces/IComWorld.hpp"
#include "AssetInterfaces/IGfxImage.hpp"
#include "AssetInterfaces/IGfxWorld.hpp"
#include "AssetInterfaces/IMaterial.hpp"
#include "AssetInterfaces/ISndCurve.hpp"
#include "AssetInterfaces/IMenuList.hpp"
#include "AssetInterfaces/IclipMap_t.hpp"
#include "AssetInterfaces/ImenuDef_t.hpp"
#include "AssetInterfaces/ITracerDef.hpp"
#include "AssetInterfaces/IPhysPreset.hpp"
#include "AssetInterfaces/IXAnimParts.hpp"
#include "AssetInterfaces/IFxEffectDef.hpp"
#include "AssetInterfaces/IGameWorldMp.hpp"
#include "AssetInterfaces/IGameWorldSp.hpp"
#include "AssetInterfaces/IGfxLightDef.hpp"
#include "AssetInterfaces/ILoadedSound.hpp"
#include "AssetInterfaces/IPhysCollmap.hpp"
#include "AssetInterfaces/IStringTable.hpp"
#include "AssetInterfaces/IXModelSurfs.hpp"
#include "AssetInterfaces/ILocalizeEntry.hpp"
#include "AssetInterfaces/Isnd_alias_list_t.hpp"
#include "AssetInterfaces/IMaterialPixelShader.hpp"
#include "AssetInterfaces/IMaterialTechniqueSet.hpp"
#include "AssetInterfaces/IMaterialVertexShader.hpp"
#include "AssetInterfaces/IStructuredDataDefSet.hpp"
#include "AssetInterfaces/IMaterialVertexDeclaration.hpp"

