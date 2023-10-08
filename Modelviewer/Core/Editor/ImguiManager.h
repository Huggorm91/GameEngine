#pragma once
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <shellapi.h>
#include "Input/InputObserver.hpp"
#include "AssetManager/Assets/Prefab.h"
#include "AssetTypes.h"
#include <unordered_set>

class Texture;
class ModelViewer;

class ImguiManager : public Crimson::InputObserver
{
public:
	ImguiManager();
	~ImguiManager() = default;

	void Release();

	void Init();
	void Update();
	void Render();

	void AddGameObject(GameObject* anObject);
	void ChangeIndexName(GameObject* anObject, const std::string& aName);
	const std::string& GetIndexName(GameObject* anObject) const;

	void ReceiveEvent(Crimson::eInputEvent, Crimson::eKey) override;
	void ReceiveEvent(Crimson::eInputAction, float) override;

	void SetDropFile(HDROP aHandle);

	void RefreshAvailableFiles();

private:
	friend class EditCommand;

	bool myIsEditingPrefab;
	bool myIsShowingPrefabWindow;
	bool myIsShowingNewObjectWindow;
	bool myIsShowingDragFilePopUp;
	bool myIsShowingOverwritePopUp;
	bool myHasClosedOverwritePopUp;

	Assets::eAssetType myDropfileAssettype;
	ComponentType mySelectedComponentType;

	unsigned myDropFileCount;
	unsigned myDropFileSelection;

	float myAssetBrowserIconSize;

	HDROP myDropfile;
	ModelViewer* myModelViewer;
	const std::string* mySelectedPrefabName;

	Crimson::Vector2i myDropLocation;
	Crimson::Vector2f myViewportSize;
	Crimson::Vector2f myWindowSize;

	std::string myAssetPath;
	std::string myBasicAssetPath;

	std::string myOverwriteFromPath;
	std::string myOverwriteToPath;
	std::string mySelectedPath;
	std::string myAssetBrowserPath;
	std::string myContentListPath;

	Transform myMultiSelectionTransform;

	Prefab myEditPrefab;
	GameObject myNewObject;
	std::unordered_set<std::shared_ptr<GameObject>> mySelectedObjects;

	std::unordered_set<std::string> myAssetBrowserFiles;

	std::unordered_map<std::string, Assets::eAssetType> myAvailableFiles;
	std::unordered_map<Assets::eAssetType, Texture*> myAssetIcons;
	std::unordered_map<std::string, unsigned> myImguiNameCounts;
	std::unordered_map<GameObject*, std::string> myImguiNameIndex;

	std::string GetDropFilePath(unsigned anIndex);
	void ReleaseDropFile();
	// Returns true if another file exists
	bool NextDropFile();
	bool IsLastDropFile();

	bool IsSelected(const std::shared_ptr<GameObject>& anObject);

	void CreateViewport();

	void CreateAssetBrowser();
	bool AssetBrowserButton(const std::string& aPath);
	void CreateContentList();
	void ContentListButton(const std::string& aPath);
	void SelectAssetBrowserPath(const std::string& aPath);

	void CreatePreferenceWindow();
	void CreateDropFilePopUp();

	void CreateSelectedObjectWindow();
	void CreateSceneContentWindow();
	void SceneContentButton(const std::shared_ptr<GameObject>& anObject);
	void DropSceneContent(GameObject* aParent);

	void CreatePrefabWindow();
	void CreateNewObjectWindow();

	void CreateOverwriteFilePopUp();
};
