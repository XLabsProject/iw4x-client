#pragma once

namespace Components
{
	class Materials : public Component
	{
	public:
		Materials();
		~Materials();

		static int FormatImagePath(char* buffer, size_t size, int, int, const char* image);

		static Game::Material* Create(const std::string& name, Game::GfxImage* image);
		static void Delete(Game::Material* material, bool deleteImage = false);

		static Game::GfxImage* CreateImage(const std::string& name, unsigned int width, unsigned int height, unsigned int depth, unsigned int flags, _D3DFORMAT format);
		static void DeleteImage(Game::GfxImage* image);

		static bool IsValid(Game::Material* material);

	private:
		static std::vector<Game::GfxImage*> ImageTable;
		static std::vector<Game::Material*> MaterialTable;
		static int ImageNameLength;
		static std::map<std::string, std::string> TechsetSwaps;

		static Utils::Hook ImageVersionCheckHook;
		static void ImageVersionCheck();

		static Game::Material* ResolveMaterial(const char* stringPtr);
		static void DrawMaterialStub();
		static void PostDrawMaterialStub();

		static int WriteDeathMessageIcon(char* string, int offset, Game::Material* material);
		static void DeathMessageStub();

		static int R_TextWidth_Hk(const char* text, int maxChars, Game::Font_s* font);

#ifdef DEBUG
		static void DumpImageCfg(int, const char*, const char* material);
		static void DumpImageCfgPath(int, const char*, const char* material);
#endif

		static int MaterialComparePrint(Game::Material* m1, Game::Material* m2);

		static void DeleteAll();

		static void Load_MaterialTechniqueSetAsset(Game::MaterialTechniqueSet** pptr);
	};
}
