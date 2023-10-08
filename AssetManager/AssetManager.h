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
	static void PreLoadAssets();

	// Call after releasing importer in order to initialize the Importer again
	static void RestartImporter();
	// Uninitializes the Importer in order to free resources
	static void ReleaseImporter();

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
	inline static Material GetAsset(const std::string& anIdentifier, const std::string& aVertexShader, const std::string& aPixelShader, const std::string& anAlbedo = "", const std::string& aNormal = "") { return *myMaterialManager.CreateMaterial(anIdentifier, myShaderManager.GetShader(aVertexShader, myIsLoggingErrors), myShaderManager.GetShader(aPixelShader, myIsLoggingErrors), anAlbedo.empty() ? nullptr : myTextureManager.GetTexture(anAlbedo, myIsLoggingErrors), aNormal.empty() ? nullptr : myTextureManager.GetTexture(aNormal, myIsLoggingErrors)); }

	inline static void CreateAsset(const GameObject& anAsset, const std::string& anIdentifier){	myPrefabManager.CreatePrefab(anIdentifier, anAsset); }
	inline static void CreateAsset(const GameObject* anAsset, const std::string& anIdentifier){	myPrefabManager.CreatePrefab(anIdentifier, *anAsset); }

	inline static void CreateAsset(const Material& anAsset, const std::string& anIdentifier) { myMaterialManager.CreateMaterial(anAsset, anIdentifier); }
	inline static void CreateAsset(const Material* anAsset, const std::string& anIdentifier) { myMaterialManager.CreateMaterial(*anAsset, anIdentifier); }

	inline static void SaveAsset(const GameObject& anAsset, const std::string& aPath) { myPrefabManager.SavePrefab(aPath, anAsset); }
	inline static void SaveAsset(const GameObject* anAsset, const std::string& aPath) { myPrefabManager.SavePrefab(aPath , *anAsset); }

	inline static void SaveAsset(const Material& anAsset, const std::string& aPath) { myMaterialManager.SaveMaterial(&anAsset, aPath); }
	inline static void SaveAsset(const Material* anAsset, const std::string& aPath) { myMaterialManager.SaveMaterial(anAsset, aPath); }

	inline static Prefab GetPrefab(const std::string& anIdentifier) { return myPrefabManager.GetPrefab(anIdentifier, myIsLoggingErrors); }

	inline static void CreatePrefab(const GameObject& anAsset, const std::string& anIdentifier){ myPrefabManager.CreatePrefab(anIdentifier, anAsset); }
	inline static void CreatePrefab(const GameObject* anAsset, const std::string& anIdentifier){ myPrefabManager.CreatePrefab(anIdentifier, *anAsset); }

	inline static void SavePrefab(const std::string& aPath) { myPrefabManager.SavePrefab(aPath); }
	inline static void SavePrefab(const GameObject& anAsset, const std::string& aPath) { myPrefabManager.SavePrefab(aPath, anAsset); }
	inline static void SaveAllPrefabs() { myPrefabManager.SaveAllPrefabs(); }

	inline static const char* GetModelExtension(){ return ModelManager::GetExtension(); }
	inline static const char* GetAnimationExtension(){ return AnimationManager::GetExtension(); }
	inline static const char* GetTextureExtension(){ return TextureManager::GetExtension(); }
	inline static const char* GetMaterialExtension(){ return MaterialManager::GetExtension(); }
	inline static const char* GetShaderExtension(){ return ShaderManager::GetExtension(); }
	inline static const char* GetPrefabExtension(){ return PrefabManager::GetExtension(); }

	inline static const char* GetModelPath(){ return ModelManager::GetPath(); }
	inline static const char* GetAnimationPath(){ return AnimationManager::GetPath(); }
	inline static const char* GetTexturePath(){ return TextureManager::GetPath(); }
	inline static const char* GetMaterialPath(){ return MaterialManager::GetPath(); }
	inline static const char* GetShaderPath(){ return ShaderManager::GetPath(); }
	inline static const char* GetPrefabPath(){ return PrefabManager::GetPath(); }

	inline static const wchar_t* GetModelExtensionW(){ return ModelManager::GetExtensionW(); }
	inline static const wchar_t* GetAnimationExtensionW(){ return AnimationManager::GetExtensionW(); }
	inline static const wchar_t* GetTextureExtensionW(){ return TextureManager::GetExtensionW(); }
	inline static const wchar_t* GetMaterialExtensionW(){ return MaterialManager::GetExtensionW(); }
	inline static const wchar_t* GetShaderExtensionW(){ return ShaderManager::GetExtensionW(); }
	inline static const wchar_t* GetPrefabExtensionW(){ return PrefabManager::GetExtensionW(); }

	inline static const wchar_t* GetModelPathW(){ return ModelManager::GetPathW(); }
	inline static const wchar_t* GetAnimationPathW(){ return AnimationManager::GetPathW(); }
	inline static const wchar_t* GetTexturePathW(){ return TextureManager::GetPathW(); }
	inline static const wchar_t* GetMaterialPathW(){ return MaterialManager::GetPathW(); }
	inline static const wchar_t* GetShaderPathW(){ return ShaderManager::GetPathW(); }
	inline static const wchar_t* GetPrefabPathW(){ return PrefabManager::GetPathW(); }

	// inline static const std::unordered_set<std::string>& GetAvailableModels(){ return myModelManager.GetModellist(); }
	// inline static const std::unordered_set<std::string>& GetAvailableAnimations(){ return myAnimationManager.GetAnimationlist(); }
	inline static const std::unordered_set<std::string>& GetAvailableTextures(){ return myTextureManager.GetTexturelist(); }
	// inline static const std::unordered_set<std::string>& GetAvailableMaterials(){ return myMaterialManager.GetMateriallist(); }
	// inline static const std::unordered_set<std::string>& GetAvailableShaders(){ return myShaderManager.GetShaderlist(); }
	inline static const std::unordered_set<std::string>& GetAvailablePrefabs(){ return myPrefabManager.GetPrefablist(); }

	inline static void SetLogErrors(bool aState){ myIsLoggingErrors = aState; }

private:
	friend class Prefab;

	static bool myIsLoggingErrors;
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
	if (GameObject* object = myModelManager.GetModel(anIdentifier, myIsLoggingErrors); object != nullptr)
	{
		return *object;
	}
	return *myPrefabManager.GetTemplate(anIdentifier, myIsLoggingErrors);
}

template<>
inline MeshComponent AssetManager::GetAsset(const std::string& anIdentifier)
{
	return *myModelManager.GetMesh(anIdentifier, myIsLoggingErrors);
}

template<>
inline AnimatedMeshComponent AssetManager::GetAsset(const std::string& anIdentifier)
{
	return *myModelManager.GetAnimatedMesh(anIdentifier, myIsLoggingErrors);
}

template<>
inline Animation AssetManager::GetAsset(const std::string& anIdentifier)
{
	if (Animation* animation = myAnimationManager.GetAnimation(anIdentifier, myIsLoggingErrors); animation != nullptr)
	{
		return *animation;
	}
	return Animation();
}

template<>
inline Skeleton& AssetManager::GetAsset(const std::string& anIdentifier)
{
	return *myModelManager.GetSkeleton(anIdentifier, myIsLoggingErrors);
}

template<>
inline Skeleton* AssetManager::GetAsset(const std::string& anIdentifier)
{
	return myModelManager.GetSkeleton(anIdentifier, myIsLoggingErrors);
}

template<>
inline Texture& AssetManager::GetAsset(const std::string& anIdentifier)
{
	return *myTextureManager.GetTexture(anIdentifier, myIsLoggingErrors);
}

template<>
inline Texture* AssetManager::GetAsset(const std::string& anIdentifier)
{
	return myTextureManager.GetTexture(anIdentifier, myIsLoggingErrors);
}

template<>
inline Shader& AssetManager::GetAsset(const std::string& anIdentifier)
{
	return *myShaderManager.GetShader(anIdentifier, myIsLoggingErrors);
}

template<>
inline Shader* AssetManager::GetAsset(const std::string& anIdentifier)
{
	return myShaderManager.GetShader(anIdentifier, myIsLoggingErrors);
}

template<>
inline Material AssetManager::GetAsset(const std::string& anIdentifier)
{
	return *myMaterialManager.GetMaterial(anIdentifier, myIsLoggingErrors);
}