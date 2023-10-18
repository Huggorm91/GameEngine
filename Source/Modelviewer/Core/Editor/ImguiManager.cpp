#include "Modelviewer.pch.h"
#include "ImguiManager.h"
#include "../Modelviewer.h"
#include "../Commands/EditCmd_AddGameobject.h"
#include "../Commands/EditCmd_AddGameobjects.h"
#include "../Commands/EditCmd_RemoveGameobjects.h"
#include "../Commands/EditCmd_ChangeMultipleGameObjects.h"

#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetAmbientlight.h"
#include "GraphicsEngine/Commands/Light/LitCmd_SetShadowBias.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

#include "File\FileSelectors.h"
#include "Input/InputMapper.h"
#include "Time/Timer.h"

using namespace Crimson;

ImguiManager::ImguiManager() : myModelViewer(nullptr), myIsShowingNewObjectWindow(true), myIsShowingPrefabWindow(true), myOverwriteAppliedToAll(false), mySelectedPath(), myNewObject(std::make_shared<GameObject>()), mySelectedPrefabName(nullptr),
myImguiNameCounts(), mySelectedComponentType(ComponentType::Mesh), myDropfile(NULL), myDropFileCount(0), myDropFileSelection(0), myDropLocation(), myHasGottenDropfiles(false),
mySelectedObjects(), myDropfileAssettype(Assets::eAssetType::Unknown), myShouldOpenOverwritePopUp(false), myOverwriteFromPaths(), myOverwriteToPaths(), myImguiNameIndex(), myViewportAwaitsFile(false),
myMultiSelectionTransform(), myAvailableFiles(), myAssetPath("..\\Content\\"), myInternalAssetPath("Settings\\EditorAssets\\"), myAssetIcons(), myAssetBrowserIconSize(75), myHasAddedAssetFiles(false),
myAssetBrowserPath(), myShouldOpenPopUp(false), myActiveObjects(nullptr), myRefreshTimer(0.f)
{
	myNewObject->MarkAsPrefab();
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
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	}

	// Setup Platform/Renderer backends
	{
		ImGui_ImplWin32_Init(myModelViewer->myMainWindowHandle);
		ImGui_ImplDX11_Init(RHI::Device.Get(), RHI::Context.Get());
	}

	// Setup keybinds
	{
		auto& input = *InputMapper::GetInstance();
		input.Attach(this, eInputEvent::KeyDown, eKey::Del);
	}

	// Load AssetBrowser icons
	{
		myAssetIcons.emplace(Assets::eAssetType::Unknown, AssetManager::GetAsset<Texture>(myInternalAssetPath + "UnknownIcon.dds"));
		myAssetIcons.emplace(Assets::eAssetType::Model, AssetManager::GetAsset<Texture>(myInternalAssetPath + "ModelIcon.dds"));
		myAssetIcons.emplace(Assets::eAssetType::AnimatedModel, AssetManager::GetAsset<Texture>(myInternalAssetPath + "AnimatedModelIcon.dds"));
		myAssetIcons.emplace(Assets::eAssetType::Animation, AssetManager::GetAsset<Texture>(myInternalAssetPath + "AnimationIcon.dds"));
		myAssetIcons.emplace(Assets::eAssetType::Material, AssetManager::GetAsset<Texture>(myInternalAssetPath + "MaterialIcon.dds"));
		myAssetIcons.emplace(Assets::eAssetType::Prefab, AssetManager::GetAsset<Texture>(myInternalAssetPath + "PrefabIcon.dds"));
		myAssetIcons.emplace(Assets::eAssetType::Shader, AssetManager::GetAsset<Texture>(myInternalAssetPath + "ShaderIcon.dds"));
		myAssetIcons.emplace(Assets::eAssetType::Scene, AssetManager::GetAsset<Texture>(myInternalAssetPath + "SceneIcon.dds"));
		myAssetIcons.emplace(Assets::eAssetType::Folder, AssetManager::GetAsset<Texture>(myInternalAssetPath + "FolderIcon.dds"));
		myAssetIcons.emplace(Assets::eAssetType::Texture, AssetManager::GetAsset<Texture>(myInternalAssetPath + "TextureIcon.dds"));
		myAssetIcons.emplace(static_cast<Assets::eAssetType>(-1), AssetManager::GetAsset<Texture>(myInternalAssetPath + "StepOutIcon.dds"));
	}

	// Get available files and folders
	{
		RefreshAvailableFiles();
		SelectAssetBrowserPath(myAssetPath);
	}
}

void ImguiManager::Update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	myRefreshTimer += Timer::GetDeltaTime();
	if (myRefreshTimer >= 60.f)
	{
		myRefreshTimer = 0.f;
		RefreshAvailableFiles();
		SelectAssetBrowserPath(myAssetBrowserPath);
	}

	// Create Editor workarea
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(myWindowSize.x, myWindowSize.y));
	const auto& color = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(color.x, color.y, color.z, 1.f));
	ImGui::Begin("MainWindow", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar);
	ImGui::PopStyleColor();

	CreateMenubar();

	ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_AutoHideTabBar);

	CreateViewport();

	CreatePreferenceWindow();

	CreateSceneContentWindow();
	CreateSelectedObjectWindow();

	CreateAssetBrowser();
	CreateContentList();

	CreatePrefabWindow();
	CreateNewObjectWindow();

	CreatePopUp();
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
	if (myImguiNameIndex.find(anObject->GetID()) != myImguiNameIndex.end())
	{
		return;
	}

	if (auto iter = myImguiNameCounts.find(anObject->GetName()); iter != myImguiNameCounts.end())
	{
		std::string text = anObject->GetName() + " (" + std::to_string(iter->second++) + ")";
		myImguiNameIndex.emplace(anObject->GetID(), text);
	}
	else
	{
		myImguiNameCounts.emplace(anObject->GetName(), 1);
		myImguiNameIndex.emplace(anObject->GetID(), anObject->GetName());
	}
}

void ImguiManager::ChangeIndexName(GameObject* anObject, const std::string& aName)
{
	myImguiNameIndex.at(anObject->GetID()) = aName;
}

const std::string& ImguiManager::GetIndexName(GameObject* anObject) const
{
	return myImguiNameIndex.at(anObject->GetID());
}

void ImguiManager::Reset()
{
	mySelectedObjects.clear();
	myActiveObjects = nullptr;
	myImguiNameCounts.clear();
	myImguiNameIndex.clear();
}

void ImguiManager::ReceiveEvent(eInputEvent, eKey aKey)
{
	switch (aKey)
	{
	case eKey::Del:
	{
		if (mySelectedObjects.size() > 0)
		{
			myModelViewer->AddCommand(std::make_shared<EditCmd_RemoveGameObjects>());
			mySelectedObjects.clear();
		}
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
	myHasGottenDropfiles = true;
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

	if (auto typeList = Assets::GetPossibleTypes(GetFileExtension(GetDropFilePath(myDropFileSelection))); typeList.size() == 1)
	{
		myDropfileAssettype = typeList.back();
	}
	else
	{
		myDropfileAssettype = Assets::eAssetType::Unknown;
	}
}

void ImguiManager::ReleaseDropFile()
{
	DragFinish(myDropfile);
	myDropfile = NULL;
	myDropFileCount = 0;
	myDropFileSelection = 0;
	myDropfileAssettype = Assets::eAssetType::Unknown;
	myHasGottenDropfiles = false;
}

void ImguiManager::RefreshAvailableFiles()
{
	myAvailableFiles.clear();
	auto files = GetAllFilepathsInDirectory(myAssetPath, true);
	for (auto& file : files)
	{
		auto potentialTypes = Assets::GetPossibleTypes(GetFileExtension(file));
		if (potentialTypes.size() == 1)
		{
			myAvailableFiles.emplace(file, potentialTypes.back());
		}
		else
		{
			bool isHandled = false;
			for (auto& type : potentialTypes)
			{
				switch (type)
				{
				case Assets::eAssetType::Unknown:
				case Assets::eAssetType::Texture:
					break;
				case Assets::eAssetType::Model:
				case Assets::eAssetType::AnimatedModel:
				case Assets::eAssetType::Animation:
				{
					AssetManager::SetLogErrors(false);
					if (AssetManager::GetAsset<Animation>(file).HasData())
					{
						myAvailableFiles.emplace(file, Assets::eAssetType::Animation);
						isHandled = true;
					}
					else
					{
						if (AssetManager::GetAsset<GameObject>(file).HasComponent<AnimatedMeshComponent>())
						{
							myAvailableFiles.emplace(file, Assets::eAssetType::AnimatedModel);
							isHandled = true;
						}
						else
						{
							myAvailableFiles.emplace(file, Assets::eAssetType::Model);
							isHandled = true;
						}
					}
					AssetManager::SetLogErrors(true);
					break;
				}
				case Assets::eAssetType::Material:
				case Assets::eAssetType::Prefab:
				case Assets::eAssetType::Shader:
				case Assets::eAssetType::Scene:
				case Assets::eAssetType::Folder:
					break;
				default:
					myModelViewer->myLogger.Warn("ImguiManager::RefreshAvailableFiles: Invalid AssetType!");
					break;
				}

				if (isHandled)
				{
					break;
				}
			}
		}
	}

	auto folders = GetAllFoldersInDirectory(myAssetPath, true);
	for (auto& folder : folders)
	{
		myAvailableFiles.emplace(folder, Assets::eAssetType::Folder);
	}
}

void ImguiManager::SetActiveObjects(std::unordered_map<unsigned, std::shared_ptr<GameObject>>* aList)
{
	myActiveObjects = aList;

	std::unordered_set<GameObject*> newSelection;
	for (auto& current : mySelectedObjects)
	{
		for (auto& [id, object] : *myActiveObjects)
		{
			if (current->GetID() == id)
			{
				newSelection.emplace(object.get());
			}
		}
	}
	mySelectedObjects = newSelection;
	myMultiSelectionTransform = Transform();
}

std::string ImguiManager::GetDropFilePath(unsigned anIndex)
{
	LPSTR fileName = new char[1024];
	unsigned charCount = DragQueryFileA(myDropfile, anIndex, fileName, 1024);
	std::string result(fileName, charCount);
	delete[] fileName;
	return result;
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
		if (auto typeList = Assets::GetPossibleTypes(GetFileExtension(GetDropFilePath(myDropFileSelection))); typeList.size() == 1)
		{
			myDropfileAssettype = typeList.back();
		}
		else
		{
			myDropfileAssettype = Assets::eAssetType::Unknown;
		}
		return true;
	}
}

bool ImguiManager::IsLastDropFile()
{
	return myDropFileCount == 0 || myDropFileSelection == myDropFileCount - 1;
}

void ImguiManager::CopyAllDropFiles(const std::string& aTargetFolder)
{
	do
	{
		std::string source = GetDropFilePath(myDropFileSelection);
		std::string target = aTargetFolder;
		if (target.back() != '\\' && target.back() != '/')
		{
			target += "\\";
		}
		target += GetFileName(source);

		if (CopyFileA(source.c_str(), target.c_str(), TRUE) == 0)
		{
			if (FileExists(target))
			{
				myOverwriteFromPaths.emplace_back(source);
				myOverwriteToPaths.emplace_back(target);
				myShouldOpenOverwritePopUp = true;
			}
			else
			{
				myModelViewer->GetLogger().Err("ImGui Manager: Failed to copy file to path: " + target);
			}
		}
		else
		{
			myHasAddedAssetFiles = true;
			myLatestAddedFile = target;
		}
	} while (NextDropFile());
}

void ImguiManager::AddToSelection(GameObject* anObject)
{
	myMultiSelectionTransform = Transform();
	mySelectedObjects.emplace(anObject);
}

bool ImguiManager::IsSelected(const std::shared_ptr<GameObject>& anObject)
{
	for (auto& selection : mySelectedObjects)
	{
		if (selection == anObject.get())
		{
			return true;
		}
	}
	return false;
}

void ImguiManager::CreateMenubar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene"))
			{
				myModelViewer->LoadScene("");
			}
			if (ImGui::MenuItem("Open Scene"))
			{
				std::wstring extensions = L"*" + std::wstring(AssetManager::GetSceneExtensionW()) + L";*";
				extensions += std::wstring(AssetManager::GetSceneBinaryExtensionW()) + L";";
				std::string path;
				if (Crimson::ShowOpenFileSelector(path, { L"Scenes", extensions }, ToWString(GetAbsolutePath(AssetManager::GetScenePath()))))
				{
					myModelViewer->LoadScene(path);
				}
			}
			if (ImGui::MenuItem("Save Scene"))
			{
				std::wstring extension = std::wstring(AssetManager::GetSceneExtensionW());
				std::wstring filename = ToWString(AddExtensionIfMissing(myModelViewer->myScene.Name, AssetManager::GetSceneExtension(), true));
				std::string path;
				if (Crimson::ShowSaveFileSelector(path, filename, extension.substr(1), {L"Scene", L"*" + extension + L";"}, ToWString(GetAbsolutePath(AssetManager::GetScenePath()))))
				{
					myModelViewer->SaveScene(path, false);
				}
			}
			if (ImGui::MenuItem("Save As Binary"))
			{
				std::wstring extension = std::wstring(AssetManager::GetSceneBinaryExtensionW());
				std::wstring filename = ToWString(AddExtensionIfMissing(myModelViewer->myScene.Name, AssetManager::GetSceneBinaryExtension(), true));
				std::string path;
				if (Crimson::ShowSaveFileSelector(path, filename, extension.substr(1), { L"Scene", L"*" + extension + L";" }, ToWString(GetAbsolutePath(AssetManager::GetScenePath()))))
				{
					myModelViewer->SaveScene(path, true);
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void ImguiManager::CreateViewport()
{
	if (myModelViewer->myIsInPlayMode)
	{
		const auto& color = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.f, color.y, color.z, 1.f));
	}
	if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar))
	{
		if (myHasGottenDropfiles && ImGui::IsWindowHovered())
		{
			myHasGottenDropfiles = false;

			if (myDropFileCount == 1 &&
				Assets::IsType(Assets::eAssetType::Texture, GetDropFilePath(myDropFileSelection)) &&
				!mySelectedObjects.empty())
			{
				CopyAllDropFiles(AssetManager::GetTexturePath());
				myViewportAwaitsFile = true;
			}
			else
			{
				myShouldOpenPopUp = true;
				myPopUpMessage = "Can only drop a single Texture-file in Viewport,\nand must have at least 1 selected GameObject!";
			}
		}

		if (myViewportAwaitsFile && myHasAddedAssetFiles)
		{
			myViewportAwaitsFile = false;
			Texture* texture = AssetManager::GetAsset<Texture*>(myLatestAddedFile);
			for (auto& object : mySelectedObjects)
			{
				for (auto& component : object->GetComponents<MeshComponent>())
				{
					component->SetAlbedoTexture(texture);
				}
				for (auto& component : object->GetComponents<AnimatedMeshComponent>())
				{
					component->SetAlbedoTexture(texture);
				}
			}
		}
		const Vector2f aspectRatio = { myWindowSize.y / myWindowSize.x, myWindowSize.x / myWindowSize.y };

		auto size = ImGui::GetContentRegionAvail();

		const Vector2f regionSize = { size.x, size.y };
		const Vector2f regionRatio = { size.y / size.x, size.x / size.y };

		if (regionRatio.x < aspectRatio.x)
		{
			size.x = size.y / aspectRatio.x;
		}
		if (regionRatio.y < aspectRatio.y)
		{
			size.y = size.x / aspectRatio.y;
		}

		const Vector2f off = (regionSize - Vector2f{ size.x, size.y }) * 0.5f;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off.x);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + off.y);

		ImGui::Image(GraphicsEngine::Get().GetBackBufferCopy()->GetSRV().Get(), size);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Dragged_Texture"))
			{
				IM_ASSERT(payload->DataSize == sizeof(Texture*));
				Texture* texture = *static_cast<Texture**>(payload->Data);
				for (auto& object : mySelectedObjects)
				{
					for (auto& component : object->GetComponents<MeshComponent>())
					{
						component->SetAlbedoTexture(texture);
					}
					for (auto& component : object->GetComponents<AnimatedMeshComponent>())
					{
						component->SetAlbedoTexture(texture);
					}
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
	ImGui::End();
	if (myModelViewer->myIsInPlayMode)
	{
		ImGui::PopStyleColor();
	}
}

void ImguiManager::CreateAssetBrowser()
{
	if (ImGui::Begin("Asset Browser", nullptr, ImGuiWindowFlags_NoTitleBar))
	{
		if (myHasGottenDropfiles && myDropFileCount > 0 && ImGui::IsWindowHovered())
		{
			myHasGottenDropfiles = false;
			std::string target = myAssetBrowserPath;
			if (IsFile(target))
			{
				target = GetContainingFolder(target);
			}

			CopyAllDropFiles(target);
		}

		ImGui::Text(GetPathWithoutDots(myAssetBrowserPath).c_str());
		ImGui::Separator();

		const float spacing = 10.f;
		const float groupsize = myAssetBrowserIconSize + spacing + (ImGui::GetStyle().ChildBorderSize * 2.f);
		const float contentsize = ImGui::GetWindowSize().x - (ImGui::GetStyle().WindowPadding.x * 2.f);
		unsigned count = 1;

		if (myAssetBrowserPath != myAssetPath)
		{
			std::string parentFolder = GetContainingFolder(myAssetBrowserPath);
			if (IsFile(myAssetBrowserPath))
			{
				parentFolder.pop_back();
				parentFolder = GetContainingFolder(parentFolder);
			}
			ImGui::BeginGroup();
			ImGui::Image(myAssetIcons.at(static_cast<Assets::eAssetType>(-1)).GetSRV().Get(), ImVec2(myAssetBrowserIconSize, myAssetBrowserIconSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + myAssetBrowserIconSize);
			ImGui::Text(GetPathWithoutDots(parentFolder).c_str());
			ImGui::PopTextWrapPos();
			ImGui::EndGroup();

			if (ImGui::IsItemClicked())
			{
				if (!parentFolder.empty() && parentFolder.back() == '\\' && parentFolder != myAssetPath)
				{
					parentFolder.pop_back();
				}
				SelectAssetBrowserPath(parentFolder);
			}

			ImGui::SameLine(0.f, spacing);
			++count;
		}


		bool hasClicked = false;
		for (auto& file : myAssetBrowserFiles)
		{
			if (AssetBrowserButton(file))
			{
				myAssetBrowserPath = file;
				hasClicked = true;
			}

			const float next = groupsize + (groupsize * count);
			if (next <= contentsize)
			{
				ImGui::SameLine(0.f, spacing);
				++count;
			}
			else
			{
				count = 1;
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
	const bool isSelected = myAssetBrowserPath == aPath;
	bool hasClicked = false;
	Texture* texture = nullptr;
	Assets::eAssetType type = myAvailableFiles[aPath];
	if (type == Assets::eAssetType::Texture)
	{
		texture = AssetManager::GetAsset<Texture*>(aPath);
		if (texture->GetArraySize() != 1)
		{
			texture = &myAssetIcons.at(type);
		}
	}
	else
	{
		texture = &myAssetIcons.at(type);
	}

	if (isSelected)
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->ChannelsSplit(2);
		draw_list->ChannelsSetCurrent(1);
	}

	ImGui::BeginGroup();
	ImGui::Image(texture->GetSRV().Get(), ImVec2(myAssetBrowserIconSize, myAssetBrowserIconSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
	ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + myAssetBrowserIconSize);
	ImGui::Text(GetFileName(aPath).c_str());
	ImGui::PopTextWrapPos();
	ImGui::EndGroup();

	if (isSelected)
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->ChannelsSetCurrent(0);
		ImVec2 p_min = ImGui::GetItemRectMin();
		ImVec2 p_max = ImGui::GetItemRectMax();
		draw_list->AddRectFilled(p_min, p_max, IM_COL32(255, 255, 255, 127));
		draw_list->ChannelsMerge();
	}

	switch (type)
	{
	case Assets::eAssetType::Unknown:
		break;
	case Assets::eAssetType::Texture:
	{
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ImGui::SetDragDropPayload("Dragged_Texture", &texture, sizeof(Texture*));
			ImGui::Image(texture->GetSRV().Get(), ImVec2(50.f, 50.f));
			ImGui::Text("Will be set as Albedo-texture for selected object if dropped in viewport");
			ImGui::EndDragDropSource();
			return true;
		}
		break;
	}
	case Assets::eAssetType::Model:
		break;
	case Assets::eAssetType::AnimatedModel:
		break;
	case Assets::eAssetType::Animation:
		break;
	case Assets::eAssetType::Material:
		break;
	case Assets::eAssetType::Prefab:
		break;
	case Assets::eAssetType::Shader:
		break;
	case Assets::eAssetType::Scene:
		break;
	case Assets::eAssetType::Folder:
		break;
	default:
		break;
	}

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
		auto content = GetFoldersInDirectory(myAssetPath, true);
		for (auto& folder : content)
		{
			ContentListButton(folder);
		}
	}
	ImGui::End();
}

void ImguiManager::ContentListButton(const std::string& aPath)
{
	auto content = GetFoldersInDirectory(aPath, true);
	const bool isOpen = ImGui::TreeNodeEx(GetFileName(aPath).c_str(), ImGuiTreeNodeFlags_OpenOnArrow | (myContentListPath == aPath ? ImGuiTreeNodeFlags_Selected : 0) | (content.empty() ? ImGuiTreeNodeFlags_Leaf : 0));

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
	if (!FileExists(aPath))
	{
		if (myAvailableFiles.find(aPath) != myAvailableFiles.end())
		{
			RefreshAvailableFiles();
		}
		myAssetBrowserPath = GetContainingFolder(aPath);
	}
	else
	{
		myAssetBrowserPath = aPath;
	}

	if (IsFolder(myAssetBrowserPath))
	{
		myContentListPath = myAssetBrowserPath;
		auto folders = GetFoldersInDirectory(myAssetBrowserPath, true);
		auto files = GetFilepathsInDirectory(myAssetBrowserPath, true);
		myAssetBrowserFiles = folders;
		for (auto& file : files)
		{
			myAssetBrowserFiles.emplace(file);
		}
	}
}

std::string ImguiManager::GetPathWithoutDots(const std::string& aPath)
{
	if (aPath.size() > 3 && aPath.front() == '.')
	{
		return aPath.substr(3);
	}
	return aPath;
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
	}
	ImGui::End();
}

void ImguiManager::CreatePopUp()
{
	if (myHasGottenDropfiles)
	{
		myHasGottenDropfiles = false;
		myPopUpMessage = "Invalid droplocation!\nDrop files on Assetbrowser to add them to the selected folder!";
		ImGui::OpenPopup("PopUp");
	}

	if (myShouldOpenPopUp)
	{
		myShouldOpenPopUp = false;
		ImGui::OpenPopup("PopUp");
	}

	if (ImGui::BeginPopupModal("PopUp", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(myPopUpMessage.c_str());

		ImVec2 buttonSize(100.f, 0.f);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - buttonSize.x);
		if (ImGui::Button("Close", buttonSize))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void ImguiManager::CreateSelectedObjectWindow()
{
	if (ImGui::Begin("Selected GameObject"))
	{
		if (mySelectedObjects.size() == 1)
		{
			GameObject* selectedObject = *mySelectedObjects.begin();
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
				myModelViewer->AddCommand(std::make_shared<EditCmd_RemoveGameObjects>());
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
		const bool isOpen = ImGui::TreeNodeEx(myModelViewer->myScene.Name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen);

		if (ImGui::BeginDragDropTarget())
		{
			DropSceneContent(nullptr);
			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsItemClicked())
		{
			mySelectedObjects.clear();
		}

		if (isOpen)
		{
			for (auto& [id, object] : *myActiveObjects)
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

	const bool isOpen = ImGui::TreeNodeEx(myImguiNameIndex.at(anObject->GetID()).c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | (IsSelected(anObject) ? ImGuiTreeNodeFlags_Selected : 0) | (anObject->HasChild() ? 0 : ImGuiTreeNodeFlags_Leaf));

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
		AddToSelection(anObject.get());
	}

	if (ImGui::BeginDragDropSource())
	{
		const unsigned id = anObject->GetID();
		ImGui::SetDragDropPayload("Dragged_SceneObject", &id, sizeof(unsigned));
		if (!IsSelected(anObject))
		{
			mySelectedObjects.clear();
			AddToSelection(anObject.get());
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
				*myNewObject = AssetManager::GetPrefab(*mySelectedPrefabName);
				myNewObject->MarkAsPrefab();
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
			if (ShowOpenFileSelector(mySelectedPath, { L"Prefab files", std::wstring(L"*") + AssetManager::GetPrefabExtensionW() }, ToWString(GetAbsolutePath(AssetManager::GetPrefabPath())), L"Select Prefab"))
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
			*myNewObject = AssetManager::GetPrefab("Default");
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
			AddComponent(mySelectedComponentType, *myNewObject);
		}

		ImGui::Separator();
		myNewObject->CreateImGuiWindowContent("New Object");
		ImGui::Separator();

		if (ImGui::Button("Add To Scene"))
		{
			myModelViewer->AddCommand(std::make_shared<EditCmd_AddGameObject>(std::make_shared<GameObject>(*myNewObject)));
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
				AssetManager::SavePrefab(*myNewObject, name);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Save"))
			{
				AssetManager::SavePrefab(*myNewObject, name);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void ImguiManager::CreateOverwriteFilePopUp()
{
	if (myHasAddedAssetFiles)
	{
		RefreshAvailableFiles();
		SelectAssetBrowserPath(myAssetBrowserPath);
		myHasAddedAssetFiles = false;
	}

	if (myShouldOpenOverwritePopUp)
	{
		if (myOverwriteFromPaths.size() != myOverwriteToPaths.size())
		{
			ModelViewer::GetLogger().Warn("ImGui Manager: Amount of source files does not match target files!");
			myOverwriteFromPaths.clear();
			myOverwriteToPaths.clear();
		}
		else
		{
			ImGui::OpenPopup("OverwriteFile");
		}
		myShouldOpenOverwritePopUp = false;
	}

	if (ImGui::BeginPopupModal("OverwriteFile"))
	{
		const std::string* source = &myOverwriteFromPaths.back();
		const std::string* target = &myOverwriteToPaths.back();

		ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x);
		ImGui::Text("From:");
		ImGui::Text(source->c_str());
		ImGui::Separator();
		ImGui::Text("To:");
		ImGui::Text(target->c_str());
		ImGui::PopTextWrapPos();

		ImGui::Checkbox("Apply to all", &myOverwriteAppliedToAll);

		if (ImGui::Button("Ignore"))
		{
			if (myOverwriteAppliedToAll)
			{
				myOverwriteFromPaths.clear();
				myOverwriteToPaths.clear();
			}
			else
			{
				myOverwriteFromPaths.pop_back();
				myOverwriteToPaths.pop_back();
			}
		}
		ImGui::SameLine();

		if (ImGui::Button("Overwrite"))
		{
			auto overwriteFile = [this](const std::string& aSource, const std::string& aTarget){
				if (CopyFileA(aSource.c_str(), aTarget.c_str(), FALSE) == 0)
				{
					ModelViewer::GetLogger().Err("ImGui Manager: Failed to create a copy of file \"" + GetFileName(aTarget) + "\" in folder: " + GetContainingFolder(aTarget));
				}
				else
				{
					myLatestAddedFile = aTarget;
				}
			};

			if (myOverwriteAppliedToAll)
			{
				for (auto sourceIter = myOverwriteFromPaths.rbegin(), targetIter = myOverwriteToPaths.rbegin(); sourceIter != myOverwriteFromPaths.rend(); sourceIter++, targetIter++)
				{
					overwriteFile(*sourceIter, *targetIter);
				}
				myOverwriteFromPaths.clear();
				myOverwriteToPaths.clear();
			}
			else
			{
				overwriteFile(*source, *target);
				myOverwriteFromPaths.pop_back();
				myOverwriteToPaths.pop_back();
			}
			myHasAddedAssetFiles = true;
		}
		ImGui::SameLine();

		if (ImGui::Button("Create Copy"))
		{
			auto copyFile = [this](const std::string& aSource, const std::string& aTarget){
				std::string path = GetContainingFolder(aTarget);
				std::string filename = GetFileNameWithoutExtension(aTarget);
				std::string extension = GetFileExtension(aTarget);
				int i = 1;

				while (CopyFileA(aSource.c_str(), (path + filename + "(" + std::to_string(i) + ")" + extension).c_str(), TRUE) == 0)
				{
					if (i++ == 32)
					{
						ModelViewer::GetLogger().Err("ImGui Manager: Failed to create a copy of file \"" + filename + extension + "\" in folder: " + path);
						break;
					}
				}
				if (i < 33)
				{
					myLatestAddedFile = path + filename + "(" + std::to_string(i) + ")" + extension;
				}
			};

			if (myOverwriteAppliedToAll)
			{
				for (auto sourceIter = myOverwriteFromPaths.rbegin(), targetIter = myOverwriteToPaths.rbegin(); sourceIter != myOverwriteFromPaths.rend(); sourceIter++, targetIter++)
				{
					copyFile(*sourceIter, *targetIter);
				}
				myOverwriteFromPaths.clear();
				myOverwriteToPaths.clear();
			}
			else
			{
				copyFile(*source, *target);
				myOverwriteFromPaths.pop_back();
				myOverwriteToPaths.pop_back();
			}
			myHasAddedAssetFiles = true;
		}

		if (myOverwriteFromPaths.empty())
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}