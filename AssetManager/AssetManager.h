#pragma once
#include "Managers/ModelManager.h"
#include "Managers/AnimationManager.h"
#include "Managers/MaterialManager.h"
#include "Managers/TextureManager.h"
#include "Managers/ShaderManager.h"

class AssetManager
{
public:
	AssetManager() = default;
	~AssetManager() = default;

	static void Init();
	static void GeneratePrimitives();

	template<class T> static T GetAsset(const std::string& anIdentifier);

	inline static GameObject GetAsset(Primitives anIdentifier) { return *myModelManager.GetModel(anIdentifier); }
	template<> static GameObject GetAsset(const std::string& anIdentifier);

	template<> static MeshComponent GetAsset(const std::string& anIdentifier);
	template<> static AnimatedMeshComponent GetAsset(const std::string& anIdentifier);

	template<> static Animation GetAsset(const std::string& anIdentifier);

	template<> static Skeleton& GetAsset(const std::string& anIdentifier);
	template<> static Skeleton* GetAsset(const std::string& anIdentifier);

	template<> static Texture& GetAsset(const std::string& anIdentifier);
	template<> static Texture* GetAsset(const std::string& anIdentifier);

	template<> static Shader& GetAsset(const std::string& anIdentifier);
	template<> static Shader* GetAsset(const std::string& anIdentifier);

	template<> static Material GetAsset(const std::string& anIdentifier);
	inline static Material GetAsset(const std::string& anIdentifier, Shader* aVertexShader, Shader* aPixelShader, Texture* anAlbedo = nullptr, Texture* aNormal = nullptr) { return *myMaterialManager.CreateMaterial(anIdentifier, aVertexShader, aPixelShader, anAlbedo, aNormal); }
	inline static Material GetAsset(const std::string& anIdentifier, const std::string& aVertexShader, const std::string& aPixelShader, const std::string& anAlbedo = "", const std::string& aNormal = "") { return *myMaterialManager.CreateMaterial(anIdentifier, myShaderManager.GetShader(aVertexShader), myShaderManager.GetShader(aPixelShader), anAlbedo.empty() ? nullptr : myTextureManager.GetTexture(anAlbedo), aNormal.empty() ? nullptr : myTextureManager.GetTexture(aNormal)); }

	inline static void CreateAsset(const GameObject& anAsset, const std::string& anIdentifier);
	inline static void CreateAsset(const GameObject* anAsset, const std::string& anIdentifier);

	inline static void CreateAsset(const Material& anAsset, const std::string& anIdentifier) { myMaterialManager.CreateMaterial(anAsset, anIdentifier); }
	inline static void CreateAsset(const Material* anAsset, const std::string& anIdentifier) { myMaterialManager.CreateMaterial(*anAsset, anIdentifier); }

	inline static void SaveAsset(const Material& anAsset, const std::string& aPath) { myMaterialManager.SaveMaterial(&anAsset, aPath); }
	inline static void SaveAsset(const Material* anAsset, const std::string& aPath) { myMaterialManager.SaveMaterial(anAsset, aPath); }

private:
	static ModelManager myModelManager;
	static AnimationManager myAnimationManager;
	static TextureManager myTextureManager;
	static MaterialManager myMaterialManager;
	static ShaderManager myShaderManager;
};

/*******************************************************************************************************************************************************/

template<class T>
inline T AssetManager::GetAsset(const std::string& anIdentifier)
{
	return nullptr;
}

template<>
inline GameObject AssetManager::GetAsset(const std::string& anIdentifier)
{
	return *myModelManager.GetModel(anIdentifier);
}

template<>
inline MeshComponent AssetManager::GetAsset(const std::string& anIdentifier)
{
	return *myModelManager.GetMesh(anIdentifier);
}

template<>
inline AnimatedMeshComponent AssetManager::GetAsset(const std::string& anIdentifier)
{
	return *myModelManager.GetAnimatedMesh(anIdentifier);
}

template<>
inline Animation AssetManager::GetAsset(const std::string& anIdentifier)
{
	return *myAnimationManager.GetAnimation(anIdentifier);
}

template<>
inline Skeleton& AssetManager::GetAsset(const std::string& anIdentifier)
{
	return *myModelManager.GetSkeleton(anIdentifier);
}

template<>
inline Skeleton* AssetManager::GetAsset(const std::string& anIdentifier)
{
	return myModelManager.GetSkeleton(anIdentifier);
}

template<>
inline Texture& AssetManager::GetAsset(const std::string& anIdentifier)
{
	return *myTextureManager.GetTexture(anIdentifier);
}

template<>
inline Texture* AssetManager::GetAsset(const std::string& anIdentifier)
{
	return myTextureManager.GetTexture(anIdentifier);
}

template<>
inline Shader& AssetManager::GetAsset(const std::string& anIdentifier)
{
	return *myShaderManager.GetShader(anIdentifier);;
}

template<>
inline Shader* AssetManager::GetAsset(const std::string& anIdentifier)
{
	return myShaderManager.GetShader(anIdentifier);
}

template<>
inline Material AssetManager::GetAsset(const std::string& anIdentifier)
{
	return *myMaterialManager.GetMaterial(anIdentifier);
}