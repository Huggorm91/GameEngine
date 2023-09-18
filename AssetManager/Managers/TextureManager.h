#pragma once
#include "GraphicsEngine/Rendering/Texture.h"
#include <unordered_map>
#include <unordered_set>

class TextureManager
{
public:
	TextureManager() = default;
	TextureManager(const std::string& aPath);
	~TextureManager() = default;

	void Init();

	Texture* GetTexture(const std::string& aPath);

	inline const std::string& GetPath(){ return myPath; }
	static inline std::string GetExtension(){ return ".dds"; }

private:
	std::unordered_map<std::string, Texture> myTextures;
	std::unordered_set<std::string> myFilePaths;
	const std::string myPath = "Content\\Textures\\";

	Texture* LoadTexture(const std::string& aPath);
};