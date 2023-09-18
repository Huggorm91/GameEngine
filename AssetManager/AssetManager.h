#pragma once
#include "Managers/ModelManager.h"
#include "Managers/AnimationManager.h"
#include "Managers/MaterialManager.h"
#include "Managers/TextureManager.h"
#include "Managers/ShaderManager.h"
#include "Managers/PrefabManager.h"

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

	inline static void CreateAsset(const GameObject& anAsset, const std::string& anIdentifier){	myPrefabManager.CreatePrefab(anIdentifier, anAsset); }
	inline static void CreateAsset(const GameObject* anAsset, const std::string& anIdentifier){	myPrefabManager.CreatePrefab(anIdentifier, *anAsset); }

	inline static void CreateAsset(const Material& anAsset, const std::string& anIdentifier) { myMaterialManager.CreateMaterial(anAsset, anIdentifier); }
	inline static void CreateAsset(const Material* anAsset, const std::string& anIdentifier) { myMaterialManager.CreateMaterial(*anAsset, anIdentifier); }

	inline static void SaveAsset(const GameObject& anAsset, const std::string& aPath) { myPrefabManager.SavePrefab(aPath, anAsset); }
	inline static void SaveAsset(const GameObject* anAsset, const std::string& aPath) { myPrefabManager.SavePrefab(aPath , *anAsset); }

	inline static void SaveAsset(const Material& anAsset, const std::string& aPath) { myMaterialManager.SaveMaterial(&anAsset, aPath); }
	inline static void SaveAsset(const Material* anAsset, const std::string& aPath) { myMaterialManager.SaveMaterial(anAsset, aPath); }

	inline static Prefab GetPrefab(const std::string& anIdentifier) { return myPrefabManager.GetPrefab(anIdentifier); }

	inline static void CreatePrefab(const GameObject& anAsset, const std::string& anIdentifier){ myPrefabManager.CreatePrefab(anIdentifier, anAsset); }
	inline static void CreatePrefab(const GameObject* anAsset, const std::string& anIdentifier){ myPrefabManager.CreatePrefab(anIdentifier, *anAsset); }

	inline static void SavePrefab(const std::string& aPath) { myPrefabManager.SavePrefab(aPath); }
	inline static void SavePrefab(const GameObject& anAsset, const std::string& aPath) { myPrefabManager.SavePrefab(aPath, anAsset); }
	inline static void SaveAllPrefabs() { myPrefabManager.SaveAllPrefabs(); }

	inline static std::string GetModelExtension(){ return ModelManager::GetExtension(); }
	inline static std::string GetAnimationExtension(){ return AnimationManager::GetExtension(); }
	inline static std::string GetTextureExtension(){ return TextureManager::GetExtension(); }
	inline static std::string GetMaterialExtension(){ return MaterialManager::GetExtension(); }
	inline static std::string GetShaderExtension(){ return ShaderManager::GetExtension(); }
	inline static std::string GetPrefabExtension(){ return PrefabManager::GetExtension(); }

	inline static const std::string& GetModelPath(){ return myModelManager.GetPath(); }
	inline static const std::string& GetAnimationPath(){ return myAnimationManager.GetPath(); }
	inline static const std::string& GetTexturePath(){ return myTextureManager.GetPath(); }
	inline static const std::string& GetMaterialPath(){ return myMaterialManager.GetPath(); }
	inline static const std::string& GetShaderPath(){ return myShaderManager.GetPath(); }
	inline static const std::string& GetPrefabPath(){ return myPrefabManager.GetPath(); }

	// inline static const std::unordered_set<std::string>& GetAvailableModels(){ return myModelManager.GetPath(); }
	// inline static const std::unordered_set<std::string>& GetAvailableAnimations(){ return myAnimationManager.GetPath(); }
	// inline static const std::unordered_set<std::string>& GetAvailableTextures(){ return myTextureManager.GetPath(); }
	// inline static const std::unordered_set<std::string>& GetAvailableMaterials(){ return myMaterialManager.GetPath(); }
	// inline static const std::unordered_set<std::string>& GetAvailableShaders(){ return myShaderManager.GetPath(); }
	inline static const std::unordered_set<std::string>& GetAvailablePrefabs(){ return myPrefabManager.GetPrefablist(); }

private:
	friend class Prefab;

	static ModelManager myModelManager;
	static AnimationManager myAnimationManager;
	static TextureManager myTextureManager;
	static MaterialManager myMaterialManager;
	static ShaderManager myShaderManager;
	static PrefabManager myPrefabManager;
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
	if (GameObject* object = myModelManager.GetModel(anIdentifier); object != nullptr)
	{
		return *object;
	}
	return *myPrefabManager.GetTemplate(anIdentifier);
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