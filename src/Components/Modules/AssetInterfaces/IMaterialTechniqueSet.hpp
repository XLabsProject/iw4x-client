#pragma once

namespace Assets
{
	class IMaterialTechniqueSet : public Components::AssetHandler::IAsset
	{
	public:
		virtual Game::XAssetType getType() override { return Game::XAssetType::ASSET_TYPE_TECHNIQUE_SET; };

		virtual void save(Game::XAssetHeader header, Components::ZoneBuilder::Zone* builder) override;
		virtual void mark(Game::XAssetHeader header, Components::ZoneBuilder::Zone* builder) override;
		virtual void load(Game::XAssetHeader* header, const std::string& name, Components::ZoneBuilder::Zone* builder) override;
		virtual void dump(Game::XAssetHeader header) override;

		void loadNative(Game::XAssetHeader* header, const std::string& name, Components::ZoneBuilder::Zone* builder);
		void loadBinary(Game::XAssetHeader* header, const std::string& name, Components::ZoneBuilder::Zone* builder);

		void loadBinaryTechnique(Game::MaterialTechnique** tech, const std::string& name, Components::ZoneBuilder::Zone* builder);

		void dumpTechnique(Game::MaterialTechnique* tech);
		void dumpVS(Game::MaterialVertexShader* vs);
		void dumpPS(Game::MaterialPixelShader* ps);

		std::string dumpDecl(Game::MaterialVertexDeclaration* decl);
	};
}
