#include "AssetManager.pch.h"
#include "AssetManager.h"
#include <Importer.h>
#include "File/DirectoryFunctions.h"

bool AssetManager::myIsLoggingErrors = true;
ModelManager AssetManager::myModelManager;
AnimationManager AssetManager::myAnimationManager;
TextureManager AssetManager::myTextureManager;
ShaderManager AssetManager::myShaderManager;
MaterialManager AssetManager::myMaterialManager;
PrefabManager AssetManager::myPrefabManager;
SceneManager AssetManager::mySceneManager;

void AssetManager::Init()
{
	AMLogger = Logger::Create("AssetManager");

#ifdef _DEBUG
	try
	{
#endif
		AMLogger.SetPrintToVSOutput(false);

		TGA::FBX::Importer::InitImporter();
		myModelManager.Init();
		myAnimationManager.Init();
		myTextureManager.Init();
		myShaderManager.Init();
		myMaterialManager.Init();
		myPrefabManager.Init();
		mySceneManager.Init();

#ifdef _DEBUG
	}
	catch (const std::exception& e)
	{
		AMLogger.LogException(e);
		exit(-1);
	}
#endif	
}

void AssetManager::GeneratePrimitives()
{
	myModelManager.GeneratePrimitives();
}

void AssetManager::PreLoadAssets()
{
	myTextureManager.LoadAllTextures();
}

void AssetManager::UpdateFilepaths()
{
	myModelManager.UpdateFilePaths();
	myAnimationManager.UpdateFilePaths();
	myTextureManager.UpdateFilePaths();
	myShaderManager.UpdateFilePaths();
	myMaterialManager.UpdateFilePaths();
	//myPrefabManager.UpdateFilePaths();
	mySceneManager.UpdateFilePaths();
}

void AssetManager::RestartImporter()
{
	TGA::FBX::Importer::InitImporter();
}

void AssetManager::ReleaseImporter()
{
	TGA::FBX::Importer::UninitImporter();
}
