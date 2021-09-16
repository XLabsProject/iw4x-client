#pragma once

namespace Assets
{
	class IGfxWorld : public Components::AssetHandler::IAsset
	{
	public:
		IGfxWorld();

		virtual Game::XAssetType getType() override { return Game::XAssetType::ASSET_TYPE_GFXWORLD; };

		virtual void save(Game::XAssetHeader header, Components::ZoneBuilder::Zone* builder) override;
		virtual void mark(Game::XAssetHeader header, Components::ZoneBuilder::Zone* builder) override;
		virtual void load(Game::XAssetHeader* header, const std::string& name, Components::ZoneBuilder::Zone* builder) override;
	private:
		void saveGfxWorldDpvsPlanes(Game::GfxWorld* world, Game::GfxWorldDpvsPlanes* asset, Game::GfxWorldDpvsPlanes* dest, Components::ZoneBuilder::Zone* builder);
		void saveGfxWorldDraw(Game::GfxWorldDraw* asset, Game::GfxWorldDraw* dest, Components::ZoneBuilder::Zone* builder);
		void saveGfxLightGrid(Game::GfxLightGrid* asset, Game::GfxLightGrid* dest, Components::ZoneBuilder::Zone* builder);
		void savesunflare_t(Game::sunflare_t* asset, Game::sunflare_t* dest, Components::ZoneBuilder::Zone* builder);
		void saveGfxWorldDpvsStatic(Game::GfxWorld* world, Game::GfxWorldDpvsStatic* asset, Game::GfxWorldDpvsStatic* dest, int planeCount, Components::ZoneBuilder::Zone* builder);
		void saveGfxWorldDpvsDynamic(Game::GfxWorldDpvsDynamic* asset, Game::GfxWorldDpvsDynamic* dest, int cellCount, Components::ZoneBuilder::Zone* builder);

		void loadGfxWorldDraw(Game::GfxWorldDraw* asset, Components::ZoneBuilder::Zone* builder, Utils::Stream::Reader* reader);
		void loadGfxWorldDpvsStatic(Game::GfxWorld* world, Game::GfxWorldDpvsStatic* asset, Components::ZoneBuilder::Zone* builder, Utils::Stream::Reader* reader);
	};
}
