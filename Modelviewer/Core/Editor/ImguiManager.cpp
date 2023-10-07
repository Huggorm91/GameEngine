#include "ImguiManager.h"
#include "../Modelviewer.h"
#include "../Commands/EditCmd_AddGameobject.h"
#include "../Commands/EditCmd_RemoveGameobject.h"
#include "../Commands/EditCmd_RemoveMultipleGameobjects.h"
#include "../Commands/EditCmd_ChangeMultipleGameObjects.h"

#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetAmbientlight.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetShadowBias.h"

#include "AssetManager/AssetManager.h"
#include "File/DirectoryFunctions.h"

#include "ThirdParty/DearImGui/ImGui/imgui.h"
#include "ThirdParty/DearImGui/ImGui/imgui_stdlib.h"
#include "ThirdParty/DearImGui/ImGui/imgui_impl_win32.h"
#include "ThirdParty/DearImGui/ImGui/imgui_impl_dx11.h"

#include "Input/InputMapper.h"

using namespace Crimson;

ImguiManager::ImguiManager() : myModelViewer(nullptr), myIsShowingNewObjectWindow(true), myIsShowingPrefabWindow(true), myIsEditingPrefab(false), mySelectedPath(), myNewObject(), mySelectedPrefabName(nullptr), myImguiNameCounts(),
mySelectedComponentType(ComponentType::Mesh), myEditPrefab("Empty"), myDropfile(NULL), myDropFileCount(0), myDropFileSelection(0), myDropLocation(), myIsShowingDragFilePopUp(false), mySelectedObjects(), myDropfileAssettype(AssetTypes::eAssetType::Unknown),
myIsShowingOverwritePopUp(false), myHasClosedOverwritePopUp(false), myOverwriteFromPath(), myOverwriteToPath(), myImguiNameIndex(), myMultiSelectionTransform(), myAvailableFiles(), myAssetPath("Content\\")
{
	myNewObject.MarkAsPrefab();
}

void ImguiManager::Release()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImguiManager::Init()
{
	myModelViewer = &ModelViewer::Get();
	auto size = GraphicsEngine::Get().GetWindowSize();
	myWindowSize = { static_cast<float>(size.cx), static_cast<float>(size.cy) };

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(myModelViewer->myMainWindowHandle);
	ImGui_ImplDX11_Init(RHI::Device.Get(), RHI::Context.Get());

	// Setup keybinds
	auto& input = *InputMapper::GetInstance();
	input.Attach(this, eInputEvent::KeyDown, eKey::Del);

	// Get available files and folders
	RefreshAvailableFiles();
}

void ImguiManager::Update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Create Editor workarea
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(myWindowSize.x, myWindowSize.y));
	auto color = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(color.x, color.y, color.z, 1.f));
	ImGui::Begin("MainWindow", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);
	ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_AutoHideTabBar);
	ImGui::PopStyleColor();

	CreateViewport();

	CreatePreferenceWindow();

	CreateDropFilePopUp();
	CreateSceneContentWindow();
	CreateSelectedObjectWindow();

	CreateAssetBrowser();
	CreateContentList();

	CreatePrefabWindow();
	CreateNewObjectWindow();

	CreateOverwriteFilePopUp();
	ImGui::End();
}

void ImguiManager::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImguiManager::AddGameObject(GameObject* anObject)
{
	if (myImguiNameIndex.find(anObject) != myImguiNameIndex.end())
	{
		return;
	}

	if (auto iter = myImguiNameCounts.find(anObject->GetName()); iter != myImguiNameCounts.end())
	{
		std::string text = anObject->GetName() + " (" + std::to_string(iter->second++) + ")";
		myImguiNameIndex.emplace(anObject, text);
	}
	else
	{
		myImguiNameCounts.emplace(anObject->GetName(), 1);
		myImguiNameIndex.emplace(anObject, anObject->GetName());
	}
}

void ImguiManager::ChangeIndexName(GameObject* anObject, const std::string& aName)
{
	myImguiNameIndex.at(anObject) = aName;
}

const std::string& ImguiManager::GetIndexName(GameObject* anObject) const
{
	return myImguiNameIndex.at(anObject);
}

void ImguiManager::ReceiveEvent(eInputEvent, eKey aKey)
{
	switch (aKey)
	{
	case eKey::Del:
	{
		if (mySelectedObjects.size() == 1)
		{
			myModelViewer->AddCommand(std::make_shared<EditCmd_RemoveGameObject>(*mySelectedObjects.begin()));
		}
		else if (mySelectedObjects.size() > 1)
		{
			myModelViewer->AddCommand(std::make_shared<EditCmd_RemoveMultipleGameObjects>());
		}
		mySelectedObjects.clear();
		break;
	}
	default:
		break;
	}
}

void ImguiManager::ReceiveEvent(eInputAction, float)
{
}

void ImguiManager::SetDropFile(HDROP aHandle)
{
	myIsShowingDragFilePopUp = true;
	myDropfile = aHandle;
	myDropFileSelection = 0;
	myDropFileCount = DragQueryFileA(aHandle, 0xFFFFFFFF, NULL, 0);
	POINT position;
	if (DragQueryPoint(aHandle, &position))
	{
		myDropLocation = position;
	}
	else
	{
		myDropLocation = Vector2i::Null;
	}

	if (auto typeList = AssetTypes::GetPossibleTypes(GetFileExtension(GetDropFilePath(myDropFileSelection))); typeList.size() == 1)
	{
		myDropfileAssettype = typeList.back();
	}
	else
	{
		myDropfileAssettype = AssetTypes::eAssetType::Unknown;
	}
}

void ImguiManager::RefreshAvailableFiles()
{
	myAvailableFiles.clear();
	myAvailableFiles = Crimson::GetAllFilepathsInDirectory(myAssetPath);
	auto folders = Crimson::GetAllFoldersInDirectory(myAssetPath);
	for (auto& folder : folders)
	{
		myAvailableFiles.emplace(folder);
	}
}

std::string ImguiManager::GetDropFilePath(unsigned anIndex)
{
	LPSTR fileName = new char[1024];
	unsigned charCount = DragQueryFileA(myDropfile, anIndex, fileName, 1024);
	std::string result(fileName, charCount);
	delete[] fileName;
	return result;
}

void ImguiManager::ReleaseDropFile()
{
	DragFinish(myDropfile);
	myDropfile = NULL;
	myDropFileCount = 0;
	myDropFileSelection = 0;
	myDropfileAssettype = AssetTypes::eAssetType::Unknown;
}

bool ImguiManager::NextDropFile()
{
	if (++myDropFileSelection >= myDropFileCount)
	{
		ReleaseDropFile();
		return false;
	}
	else
	{
		if (auto typeList = AssetTypes::GetPossibleTypes(GetFileExtension(GetDropFilePath(myDropFileSelection))); typeList.size() == 1)
		{
			myDropfileAssettype = typeList.back();
		}
		else
		{
			myDropfileAssettype = AssetTypes::eAssetType::Unknown;
		}
		return true;
	}
}

bool ImguiManager::IsLastDropFile()
{
	return myDropFileCount == 0 || myDropFileSelection == myDropFileCount - 1;
}

bool ImguiManager::IsSelected(const std::shared_ptr<GameObject>& anObject)
{
	for (auto& selection : mySelectedObjects)
	{
		if (selection == anObject)
		{
			return true;
		}
	}
	return false;
}

void ImguiManager::CreateViewport()
{
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar))
	{
		const Crimson::Vector2f aspectRatio = { myWindowSize.y / myWindowSize.x, myWindowSize.x / myWindowSize.y };

		auto size = ImGui::GetContentRegionAvail();

		const Crimson::Vector2f regionSize = { size.x, size.y };
		const Crimson::Vector2f regionRatio = { size.y / size.x, size.x / size.y };

		if (regionRatio.x < aspectRatio.x)
		{
			size.x = size.y / aspectRatio.x;
		}
		if (regionRatio.y < aspectRatio.y)
		{
			size.y = size.x / aspectRatio.y;
		}

		const Crimson::Vector2f off = (regionSize - Crimson::Vector2f{ size.x, size.y }) * 0.5f;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off.x);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + off.y);

		ImGui::Image(GraphicsEngine::Get().GetBackBufferCopy()->GetSRV().Get(), size);
	}
	ImGui::End();
	//ImGui::PopStyleVar();
}

void ImguiManager::CreateAssetBrowser()
{
	if (ImGui::Begin("Asset Browser", nullptr, ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::Text(myAssetBrowserPath.c_str());
		ImGui::Separator();
		bool hasClicked = false;
		for (auto file : myAssetBrowserFiles)
		{
			if (AssetBrowserButton(file))
			{
				myAssetBrowserPath = file;
				hasClicked = true;
			}
		}

		if (hasClicked)
		{
			SelectAssetBrowserPath(myAssetBrowserPath);
		}
	}
	ImGui::End();
}

bool ImguiManager::AssetBrowserButton(const std::string& aPath)
{
	bool hasClicked = false;
	ImGui::Text(aPath.c_str());
	if (ImGui::IsItemClicked())
	{
		hasClicked = true;
	}
	return hasClicked;
}

void ImguiManager::CreateContentList()
{
	if (ImGui::Begin("Content", nullptr, ImGuiWindowFlags_NoTitleBar))
	{
		auto content = GetFoldersInDirectory(myAssetPath);
		for (auto& folder : content)
		{
			ContentListButton(folder);
		}
	}
	ImGui::End();	
}

void ImguiManager::ContentListButton(const std::string& aPath)
{
	auto content = GetFoldersInDirectory(aPath);
	const bool isOpen = ImGui::TreeNodeEx(Crimson::GetFileName(aPath).c_str(), ImGuiTreeNodeFlags_OpenOnArrow | (content.empty() ? ImGuiTreeNodeFlags_Leaf : 0));

	if (ImGui::IsItemClicked())
	{
		SelectAssetBrowserPath(aPath);
	}

	if (isOpen)
	{
		for (auto& folder : content)
		{
			ContentListButton(folder);
		}
		ImGui::TreePop();
	}
}

void ImguiManager::SelectAssetBrowserPath(const std::string& aPath)
{
	myAssetBrowserPath = aPath;
	if (Crimson::IsFolder(aPath))
	{
		auto folders = GetFoldersInDirectory(aPath);
		auto files = GetFilepathsInDirectory(aPath);
		myAssetBrowserFiles = folders;
		for (auto& file : files)
		{
			myAssetBrowserFiles.emplace(file);
		}
	}	
}

void ImguiManager::CreatePreferenceWindow()
{
	ApplicationState& applicationState = myModelViewer->myApplicationState;
	if (ImGui::Begin("Preferences"))
	{
		ImGui::SeparatorText("Launch Settings");
		ImGui::Checkbox("Start Maximized", &applicationState.StartMaximized);
		ImGui::DragInt2("Window Size", &applicationState.WindowSize.x, 1.f, 0, INT_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::InputText("Window Title", &applicationState.WindowTitle);

		ImGui::SeparatorText("Camera Settings");
		if (ImGui::DragFloat("Movement Speed", &applicationState.CameraSpeed))
		{
			myModelViewer->myCamera.SetMovementSpeed(applicationState.CameraSpeed);
		}
		if (ImGui::DragFloat("Rotation Speed", &applicationState.CameraRotationSpeed))
		{
			myModelViewer->myCamera.SetRotationSpeed(applicationState.CameraRotationSpeed);
		}
		if (ImGui::DragFloat("Mouse Sensitivity", &applicationState.CameraMouseSensitivity))
		{
			myModelViewer->myCamera.SetMouseSensitivity(applicationState.CameraMouseSensitivity);
		}

		ImGui::SeparatorText("Scene Settings");
		if (ImGui::DragFloat("Ambientlight Intensity", &applicationState.AmbientIntensity, 0.01f, 0.f, INFINITY, "%.3f", ImGuiSliderFlags_AlwaysClamp))
		{
			GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_SetAmbientlight>(nullptr, applicationState.AmbientIntensity));
		}
		if (ImGui::DragFloat("Shadow Bias", &applicationState.ShadowBias, 0.0001f, 0.f, 1.f, "%.4f", ImGuiSliderFlags_AlwaysClamp))
		{
			GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_SetShadowBias>(applicationState.ShadowBias));
		}

		ImGui::SeparatorText("");
		if (ImGui::Button("Save Preferences"))
		{
			myModelViewer->SaveState();
		}
		ImGui::SameLine();
		if (ImGui::Button("Save Scene"))
		{
			// Add file selector
			myModelViewer->SaveScene("Default");
		}
	}
	ImGui::End();
}

void ImguiManager::CreateDropFilePopUp()
{
	if (myIsShowingDragFilePopUp)
	{
		ImGui::OpenPopup("DropFile");
		myIsShowingDragFilePopUp = false;
	}

	if (ImGui::BeginPopupModal("DropFile"))
	{
		std::string copyPath;
		const std::string& filePath = GetDropFilePath(myDropFileSelection);
		ImGui::Text("Source:");
		ImGui::Text(filePath.c_str());
		auto typeList = AssetTypes::GetPossibleTypes(GetFileExtension(filePath));

		if (typeList.size() > 1)
		{
			if (ImGui::BeginCombo(" ", AssetTypes::GetAssetTypeName(myDropfileAssettype).c_str()))
			{
				for (auto& type : typeList)
				{
					const bool isSelected = myDropfileAssettype == type;
					if (ImGui::Selectable(AssetTypes::GetAssetTypeName(type).c_str(), isSelected))
					{
						myDropfileAssettype = type;
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}
		copyPath = AssetTypes::GetAssetPath(myDropfileAssettype) + GetFileName(filePath);

		ImGui::Text("Target:");
		ImGui::Text(copyPath.c_str());

		ImGui::Separator();
		if (ImGui::Button("Close"))
		{
			ReleaseDropFile();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Add"))
		{
			if (CopyFileA(filePath.c_str(), copyPath.c_str(), TRUE) == 0)
			{
				if (FileExists(copyPath))
				{
					myOverwriteFromPath = filePath;
					myOverwriteToPath = copyPath;
					ImGui::CloseCurrentPopup();
					myIsShowingOverwritePopUp = true;
				}
				else
				{
					myModelViewer->GetLogger().Err("ImGui Manager: Failed to copy file to path: " + copyPath);
				}
			}
			else
			{
				if (!NextDropFile())
				{
					ImGui::CloseCurrentPopup();
				}
			}
		}
		ImGui::EndPopup();
	}
	else if (myHasClosedOverwritePopUp && myDropfile) // Select next file after Overwrite Popup finnished
	{
		if (NextDropFile())
		{
			myIsShowingDragFilePopUp = true;
		}
	}
}

void ImguiManager::CreateSelectedObjectWindow()
{
	if (ImGui::Begin("Selected GameObject"))
	{
		if (mySelectedObjects.size() == 1)
		{
			std::shared_ptr<GameObject> selectedObject = *mySelectedObjects.begin();
			selectedObject->CreateImGuiWindowContent("Selected GameObject");

			ImGui::Separator();
			if (ImGui::Button("Save As Prefab"))
			{
				ImGui::OpenPopup("Select Prefab Name");
			}
			if (ImGui::BeginPopupModal("Select Prefab Name"))
			{
				static std::string name;
				if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					AssetManager::SavePrefab(*selectedObject, name);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("Close"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Save"))
				{
					AssetManager::SavePrefab(*selectedObject, name);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::SameLine(0.f, 50.f);
			if (ImGui::Button("Delete"))
			{
				myModelViewer->AddCommand(std::make_shared<EditCmd_RemoveGameObject>(*mySelectedObjects.begin()));
				mySelectedObjects.clear();
			}
		}
		else if (mySelectedObjects.size() > 1)
		{
			myMultiSelectionTransform.CreateMultipleSelectionImGuiComponents("Selected GameObject");
		}
	}
	ImGui::End();
}

void ImguiManager::CreateSceneContentWindow()
{
	if (ImGui::Begin("Scene"))
	{
		const bool isOpen = ImGui::TreeNodeEx(myModelViewer->myLoadedScene.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen);

		if (ImGui::BeginDragDropTarget())
		{
			DropSceneContent(nullptr);
			ImGui::EndDragDropTarget();
		}

		if (isOpen)
		{
			for (auto& [id, object] : myModelViewer->myGameObjects)
			{
				if (object->HasParent())
				{
					continue;
				}

				SceneContentButton(object);
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void ImguiManager::SceneContentButton(const std::shared_ptr<GameObject>& anObject)
{
	using namespace Crimson;

	const bool isOpen = ImGui::TreeNodeEx(myImguiNameIndex.at(anObject.get()).c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | (IsSelected(anObject) ? ImGuiTreeNodeFlags_Selected : 0) | (anObject->HasChild() ? 0 : ImGuiTreeNodeFlags_Leaf));

	if (ImGui::BeginDragDropTarget())
	{
		DropSceneContent(anObject.get());
		ImGui::EndDragDropTarget();
	}
	else if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		if (!InputMapper::GetInstance()->GetKeyDownOrHeld(eKey::Ctrl))
		{
			mySelectedObjects.clear();
		}
		mySelectedObjects.emplace(anObject);
	}

	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("Dragged_SceneObject", &anObject->GetIDRef(), sizeof(unsigned));
		if (!IsSelected(anObject))
		{
			mySelectedObjects.clear();
			mySelectedObjects.emplace(anObject);
		}
		ImGui::EndDragDropSource();
	}

	if (isOpen)
	{
		for (auto& child : anObject->GetChildren())
		{
			SceneContentButton(myModelViewer->GetGameObject(child->GetID()));
		}
		ImGui::TreePop();
	}
}

void ImguiManager::DropSceneContent(GameObject* aParent)
{
	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Dragged_SceneObject"))
	{
		//IM_ASSERT(payload->DataSize == sizeof(unsigned));
		//unsigned id = *static_cast<unsigned*>(payload->Data);
		if (aParent)
		{
			auto parent = myModelViewer->GetGameObject(aParent->GetID());
			for (auto& object : mySelectedObjects)
			{
				parent->AddChild(object);
			}
		}
		else
		{
			for (auto& object : mySelectedObjects)
			{
				object->RemoveParent();
			}
		}
	}
}

void ImguiManager::CreatePrefabWindow()
{
	if (ImGui::Begin("Prefab", &myIsShowingPrefabWindow))
	{
		if (ImGui::BeginCombo(" ", mySelectedPrefabName ? mySelectedPrefabName->c_str() : ""))
		{
			for (auto& path : AssetManager::GetAvailablePrefabs())
			{
				const bool isSelected = (mySelectedPrefabName == &path);
				if (ImGui::Selectable(path.c_str(), isSelected))
				{
					mySelectedPrefabName = &path;
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (mySelectedPrefabName)
		{
			if (ImGui::Button("Set as new object"))
			{
				myNewObject = AssetManager::GetPrefab(*mySelectedPrefabName);
				myNewObject.MarkAsPrefab();
				if (!myIsShowingNewObjectWindow)
				{
					myIsShowingNewObjectWindow = true;
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Add to Scene"))
			{
				myModelViewer->AddCommand(std::make_shared<EditCmd_AddGameObject>(AssetManager::GetPrefab(*mySelectedPrefabName)));
			}
		}

		ImGui::Separator();

		if (ImGui::Button("Select Prefab"))
		{
			if (ShowFileSelector(mySelectedPath, { L"Prefab files", std::wstring(L"*") + AssetManager::GetPrefabExtensionW() }, ToWString(GetFullPath(AssetManager::GetPrefabPath())), L"Select Prefab"))
			{
				myModelViewer->myLogger.Log("Selected File!");
			}
		}
		ImGui::Text(GetRelativePath(mySelectedPath).c_str());
	}
	ImGui::End();
}

void ImguiManager::CreateNewObjectWindow()
{
	if (ImGui::Begin("New Object", &myIsShowingNewObjectWindow))
	{
		if (ImGui::Button("Reset to Default"))
		{
			myNewObject = AssetManager::GetPrefab("Default");
		}
		if (ImGui::BeginCombo(" ", ComponentTypeToString(mySelectedComponentType).c_str()))
		{
			int* selectedType = (int*)&mySelectedComponentType;
			for (int i = 1; i < static_cast<int>(ComponentType::Count); i++)
			{
				const bool isSelected = (*selectedType == i);
				if (ImGui::Selectable(ComponentTypeToString(static_cast<ComponentType>(i)).c_str(), isSelected))
				{
					*selectedType = i;
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();

		if (ImGui::Button("Add Component"))
		{
			AddComponent(mySelectedComponentType, myNewObject);
		}

		ImGui::Separator();
		myNewObject.CreateImGuiWindowContent("New Object");
		ImGui::Separator();

		if (ImGui::Button("Add To Scene"))
		{
			myModelViewer->AddCommand(std::make_shared<EditCmd_AddGameObject>(std::make_shared<GameObject>(myNewObject)));
		}
		ImGui::SameLine();
		if (ImGui::Button("Save As Prefab"))
		{
			ImGui::OpenPopup("Select Prefab Name");
		}
		if (ImGui::BeginPopupModal("Select Prefab Name"))
		{
			static std::string name;
			if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				AssetManager::SavePrefab(myNewObject, name);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Save"))
			{
				AssetManager::SavePrefab(myNewObject, name);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void ImguiManager::CreateOverwriteFilePopUp()
{
	static auto closeOverwritePopUp = [&]{
		myOverwriteFromPath = "";
		myOverwriteToPath = "";
		myHasClosedOverwritePopUp = true;
		ImGui::CloseCurrentPopup();
	};

	// Leaves myOverwriteFromPath & myOverwriteToPath with their latest paths for error handling and debugging
	static auto errorCloseOverwritePopUp = [&]{
		myHasClosedOverwritePopUp = true;
		ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	};

	if (myHasClosedOverwritePopUp)
	{
		myHasClosedOverwritePopUp = false;
	}

	if (myIsShowingOverwritePopUp)
	{
		ImGui::OpenPopup("OverwriteFile");
		myIsShowingOverwritePopUp = false;
	}

	if (ImGui::BeginPopupModal("OverwriteFile"))
	{
		ImGui::Text("From:");
		ImGui::Text(myOverwriteFromPath.c_str());
		ImGui::Separator();
		ImGui::Text("To:");
		ImGui::Text(myOverwriteToPath.c_str());

		if (ImGui::Button("Close"))
		{
			closeOverwritePopUp();
		}
		ImGui::SameLine();

		if (ImGui::Button("Overwrite"))
		{
			if (CopyFileA(myOverwriteFromPath.c_str(), myOverwriteToPath.c_str(), FALSE) == 0)
			{
				std::string filename = RemoveFileExtension(GetFileName(myOverwriteToPath));
				std::string extension = GetFileExtension(myOverwriteToPath);
				ModelViewer::GetLogger().Err("ImGui Manager: Failed to create a copy of file \"" + filename + extension + "\" in folder: " + GetContainingFolder(myOverwriteToPath));
				errorCloseOverwritePopUp();
				return;
			}

			closeOverwritePopUp();
		}
		ImGui::SameLine();

		if (ImGui::Button("Create Copy"))
		{
			std::string path = GetContainingFolder(myOverwriteToPath);
			std::string filename = RemoveFileExtension(GetFileName(myOverwriteToPath));
			std::string extension = GetFileExtension(myOverwriteToPath);
			int i = 1;

			while (CopyFileA(myOverwriteFromPath.c_str(), (path + filename + "(" + std::to_string(i) + ")" + extension).c_str(), TRUE) == 0)
			{
				i++;
				if (i == 32)
				{
					ModelViewer::GetLogger().Err("ImGui Manager: Failed to create a copy of file \"" + filename + extension + "\" in folder: " + GetContainingFolder(myOverwriteToPath));
					errorCloseOverwritePopUp();
					return;
				}
			}

			closeOverwritePopUp();
		}
		ImGui::EndPopup();
	}
}
