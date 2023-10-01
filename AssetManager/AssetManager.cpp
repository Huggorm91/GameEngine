#include "AssetManager.pch.h"
#include "AssetManager.h"
#include <Importer.h>
#include "DirectoryFunctions.h"

ModelManager AssetManager::myModelManager;
AnimationManager AssetManager::myAnimationManager;
TextureManager AssetManager::myTextureManager;
ShaderManager AssetManager::myShaderManager;
MaterialManager AssetManager::myMaterialManager;
PrefabManager AssetManager::myPrefabManager;

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

void AssetManager::RestartImporter()
{
	TGA::FBX::Importer::InitImporter();
}

void AssetManager::ReleaseImporter()
{
	TGA::FBX::Importer::UninitImporter();
}
