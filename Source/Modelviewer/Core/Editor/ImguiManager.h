#pragma once
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <shellapi.h>
#include "Input/InputObserver.hpp"
#include "AssetTypes.h"
#include <unordered_set>

class Texture;
class ModelViewer;
class GameObject;

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
	void Reset();

	void ReceiveEvent(Crimson::eInputEvent, Crimson::eKey) override;
	void ReceiveEvent(Crimson::eInputAction, float) override;

	void SetDropFile(HDROP aHandle);
	void ReleaseDropFile();

	void RefreshAvailableFiles();

	void SetActiveObjects(std::unordered_map<GameObjectID, std::shared_ptr<GameObject>>* aList);

private:
	friend class EditCommand;

	bool myIsShowingPrefabWindow;
	bool myIsShowingNewObjectWindow;

	bool myShouldOpenOverwritePopUp;
	bool myOverwriteAppliedToAll;

	bool myHasAddedAssetFiles;

	bool myViewportAwaitsFile;
	bool myShouldOpenPopUp;
	bool myHasGottenDropfiles;

	Assets::eAssetType myDropfileAssettype;
	ComponentType mySelectedComponentType;

	unsigned myDropFileCount;
	unsigned myDropFileSelection;

	float myRefreshTimer;
	float myAssetBrowserIconSize;

	HDROP myDropfile;
	ModelViewer* myModelViewer;
	const std::string* mySelectedPrefabName;
	std::unordered_map<GameObjectID, std::shared_ptr<GameObject>>* myActiveObjects;

	Crimson::Vector2i myDropLocation;
	Crimson::Vector2f myViewportSize;
	Crimson::Vector2f myWindowSize;

	const std::string myAssetPath;
	const std::string myInternalAssetPath;

	std::string myPopUpMessage;
	std::string myLatestAddedFile;

	std::string mySelectedPath;
	std::string myAssetBrowserPath;
	std::string myContentListPath;

	Transform myMultiSelectionTransform;
	
	std::shared_ptr<GameObject> myNewObject;
	std::unordered_set<GameObject*> mySelectedObjects;

	std::vector<std::string> myOverwriteFromPaths;
	std::vector<std::string> myOverwriteToPaths;

	std::unordered_set<std::string> myAssetBrowserFiles;

	std::unordered_map<std::string, Assets::eAssetType> myAvailableFiles;
	std::unordered_map<Assets::eAssetType, Texture> myAssetIcons;
	std::unordered_map<std::string, unsigned> myImguiNameCounts;
	std::unordered_map<GameObjectID, std::string> myImguiNameIndex;

	std::string GetDropFilePath(unsigned anIndex);
	// Returns true if another file exists
	bool NextDropFile();
	bool IsLastDropFile();
	void CopyAllDropFiles(const std::string& aTargetFolder);

	void AddToSelection(GameObject* anObject);
	bool IsSelected(const std::shared_ptr<GameObject>& anObject);

	void CreateMenubar();
	void CreateViewport();

	void CreateAssetBrowser();
	bool AssetBrowserButton(const std::string& aPath);
	void CreateContentList();
	void ContentListButton(const std::string& aPath);
	void SelectAssetBrowserPath(const std::string& aPath);
	std::string GetPathWithoutDots(const std::string& aPath);

	void CreatePreferenceWindow();
	void CreatePopUp();

	void CreateSelectedObjectWindow();
	void CreateSceneContentWindow();
	void SceneContentButton(const std::shared_ptr<GameObject>& anObject);
	void DropSceneContent(GameObject* aParent);

	void CreatePrefabWindow();
	void CreateNewObjectWindow();

	void CreateOverwriteFilePopUp();

	template<typename T>
	bool CreateDragSource(const std::string& aPayloadName, T* aPayload, const std::string& aTooltip = "");
};

template<typename T>
inline bool ImguiManager::CreateDragSource(const std::string& aPayloadName, T* aPayload, const std::string& aTooltip)
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
	{
		ImGui::SetDragDropPayload(aPayloadName.c_str(), aPayload, sizeof(T));
		if (!aTooltip.empty())
		{
			ImGui::Text(aTooltip.c_str());
		}
		ImGui::EndDragDropSource();
		return true;
	}
	return false;
}
