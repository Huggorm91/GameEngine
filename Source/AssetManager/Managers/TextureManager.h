#pragma once
#include "GraphicsEngine/Rendering/Texture.h"
#include <unordered_map>
#include <unordered_set>

class TextureManager
{
public:
	TextureManager() = default;
	~TextureManager() = default;

	void Init();
	void LoadAllTextures();

	Texture* GetTexture(const std::string& aPath);
	const std::unordered_set<std::string>& GetTexturelist() const;

	static inline const char* GetExtension(){ return ".dds"; }
	static inline const char* GetPath(){ return "..\\Content\\Textures\\"; }
	static inline const wchar_t* GetExtensionW(){ return L".dds"; }
	static inline const wchar_t* GetPathW(){ return L"..\\Content\\Textures\\"; }

private:
	std::unordered_map<std::string, Texture> myTextures;
	std::unordered_set<std::string> myLoadedTextures;
	std::unordered_set<std::string> myFilePaths;

	Texture* LoadTexture(const std::string& aPath);
};