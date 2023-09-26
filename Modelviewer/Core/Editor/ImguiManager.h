#pragma once
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <shellapi.h>
#include <AbstractClasses/InputObserver.hpp>
#include "AssetManager/Assets/Prefab.h"
#include "AssetTypes.h"

class ModelViewer;

class ImguiManager : public CommonUtilities::InputObserver
{
public:
	ImguiManager();
	~ImguiManager() = default;

	void Release();

	void Init();
	void Update();
	void Render();

	void ReceiveEvent(CommonUtilities::eInputEvent, CommonUtilities::eKey) override;
	void ReceiveEvent(CommonUtilities::eInputAction, float) override;

	void SetDropFile(HDROP aHandle);

private:
	friend class EditCommand;

	bool myIsEditingPrefab;
	bool myIsShowingPrefabWindow;
	bool myIsShowingNewObjectWindow;
	bool myIsShowingDragFilePopUp;
	bool myIsShowingOverwritePopUp;
	bool myHasClosedOverwritePopUp;

	AssetTypes::eAssetType myDropfileAssettype;
	ComponentType mySelectedComponentType;

	unsigned myDropFileCount;
	unsigned myDropFileSelection;

	HDROP myDropfile;
	ModelViewer* myModelViewer;
	const std::string* mySelectedPrefabName;

	std::weak_ptr<GameObject> mySelectedObject;

	CommonUtilities::Vector2i myDropLocation;

	std::string myOverwriteFromPath;
	std::string myOverwriteToPath;
	std::string mySelectedPath;

	Prefab myEditPrefab;
	GameObject myNewObject;

	std::unordered_map<std::string, unsigned> myImguiNameCounts;

	std::string GetDropFilePath(unsigned anIndex);
	void ReleaseDropFile();
	// Returns true if another file exists
	bool NextDropFile();
	bool IsLastDropFile();

	void CreatePreferenceWindow();
	void CreateDropFilePopUp();

	void CreateSelectedObjectWindow();
	void CreateSceneContentWindow();

	void CreatePrefabWindow();
	void CreateNewObjectWindow();

	void CreateOverwriteFilePopUp();
};
