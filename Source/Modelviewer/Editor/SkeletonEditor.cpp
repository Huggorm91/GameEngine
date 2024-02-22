#include "Modelviewer.pch.h"
#include "SkeletonEditor.h"
#include "AssetManager/Assets/Components/Camera/PerspectiveCameraComponent.h"
#include "AssetManager/Assets/Components/Camera/EditorCameraControllerComponent.h"
#include "AssetManager/Assets/ImguiTransform.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "Modelviewer.h"

SkeletonEditor::SkeletonEditor() :
	myIsActive(false),
	mySkeleton(nullptr),
	mySelectedBone(nullptr),
	myHoveredBone(nullptr),
	myRootBone(nullptr)
{}

void SkeletonEditor::Init(float aFoVDegree, float aNearPlane, float aFarPlane, float aCameraSpeed, float aMouseSensitivity)
{
	auto& camera = myCamera.AddComponent(PerspectiveCameraComponent(aFoVDegree, aNearPlane, aFarPlane));
	myCamera.AddComponent(EditorCameraControllerComponent(aCameraSpeed, aMouseSensitivity));
	camera.SetPosition({ 0.f, 100.f, 200.f });
	camera.SetRotation({ 0.f, 180.f, 0.f });

	myWindowSize = GraphicsEngine::Get().GetWindowSize();

	mySkeletonIcon = AssetManager::GetAsset<Texture>("Settings\\EditorAssets\\AnimatedModelIcon.dds");
	myAnimationIcon = AssetManager::GetAsset<Texture>("Settings\\EditorAssets\\AnimationIcon.dds");
}

void SkeletonEditor::Update()
{
	if (!myIsActive)
	{
		return;
	}
	myCamera.Update();

	// Create Editor workarea
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(myWindowSize.x, myWindowSize.y));
	const auto& color = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(color.x, color.y, color.z, 1.f));
	ImGui::Begin("Skeleton_MainWindow", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar);
	ImGui::PopStyleColor();

	CreateMenubar();

	ImGuiID dockspace_id = ImGui::GetID("Skeleton_MainDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_AutoHideTabBar);

	CreateViewport();
	CreateAssetBrowser();
	CreateSkeletonHeirarchy();

	ImGui::End();
}

void SkeletonEditor::SetSkeleton(Skeleton* aSkeleton, bool aHideLines)
{
	mySkeleton = aSkeleton;
	mySelectedBone = nullptr;
	myHoveredBone = nullptr;

	for (auto& [bone, line] : myLines)
	{
		line.Delete();
	}
	myLines.clear();

	if (mySkeleton)
	{
		myRootBone = &mySkeleton->GetBone(0);
		Crimson::Vector4f center = Crimson::Vector4f::NullPosition * myRootBone->myBindPoseInverse.GetInverse();
		Crimson::Vector3f extent = Crimson::Vector3f(2.f);
		myLines.emplace(myRootBone, GraphicsEngine::Get().GetLineDrawer().AddCube(center, extent, ColorManager::GetColor("White"), mySkeletonOffset.GetTransformMatrix()));

		for (auto& childIndex : myRootBone->myChildren)
		{
			CreateBoneLines(childIndex, center);
		}
	}

	if (aHideLines)
	{
		for (auto& [bone, line] : myLines)
		{
			line.SetActive(false);
		}
	}	
}

void SkeletonEditor::SetAnimation(Animation anAnimation)
{
	myAnimation = anAnimation;
}

void SkeletonEditor::SetCameraSpeed(float aSpeed)
{
	myCamera.GetComponent<EditorCameraControllerComponent>().SetSpeed(aSpeed);
}

void SkeletonEditor::SetMouseSensitivity(float aSensitivity)
{
	myCamera.GetComponent<EditorCameraControllerComponent>().SetMouseSensitivity(aSensitivity);
}

void SkeletonEditor::Activate()
{
	myIsActive = true;
	UpdateAvailableFiles();
	myCamera.SetActiveComponents(true);

	for (auto& [bone, line] : myLines)
	{
		line.SetActive(true);
	}
}

void SkeletonEditor::CreateMenubar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::MenuItem("Close"))
		{
			myIsActive = false;
			myCamera.SetActiveComponents(false);
			ModelViewer::Get().SetIsSceneActive(true);
			ModelViewer::Get().ActivateImGuiEditor();

			for (auto& [bone, line] : myLines)
			{
				line.SetActive(false);
			}
		}
		ImGui::EndMenuBar();
	}
}

void SkeletonEditor::CreateViewport()
{
	if (ImGui::Begin("Skeleton_Viewport", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar))
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
}

void SkeletonEditor::CreateSkeletonHeirarchy()
{
	if (ImGui::Begin("Skeleton_Heirarchy"))
	{
		if (mySkeleton == nullptr)
		{
			ImGui::End();
			return;
		}

		std::string boneCount = "Bones: " + std::to_string(mySkeleton->GetBoneCount());
		ImGui::Text(boneCount.c_str());

		std::string socketCount = "Sockets: " + std::to_string(mySkeleton->GetSocketCount());
		ImGui::Text(socketCount.c_str());

		auto position = mySkeletonOffset.GetPosition();
		if (ImGui::DragFloat3("Position", &position.x))
		{
			mySkeletonOffset.SetPosition(position);
			for (auto& [bone, line] : myLines)
			{
				line.UpdateTransform(mySkeletonOffset.GetTransformMatrix());
			}
		}

		auto rotation = mySkeletonOffset.GetRotation();
		if (ImGui::DragFloat3("Rotation", &rotation.x))
		{
			mySkeletonOffset.SetRotation(rotation);
			for (auto& [bone, line] : myLines)
			{
				line.UpdateTransform(mySkeletonOffset.GetTransformMatrix());
			}
		}

		const bool isOpen = ImGui::TreeNodeEx(mySkeleton->GetName().c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen);

		if (ImGui::IsItemClicked())
		{
			mySelectedBone = nullptr;
		}

		if (ImGui::IsItemHovered() && myHoveredBone)
		{
			myLines.at(myHoveredBone).UpdateColor(ColorManager::GetColor("White"));
			myHoveredBone = nullptr;
		}

		if (isOpen && myRootBone)
		{
			CreateBoneList(*myRootBone);
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void SkeletonEditor::CreateBoneList(const Bone& aBone)
{
	const bool isOpen = ImGui::TreeNodeEx(aBone.myName.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | (mySelectedBone == &aBone ? ImGuiTreeNodeFlags_Selected : 0) | (aBone.myChildren.empty() ? ImGuiTreeNodeFlags_Leaf : 0));

	if (ImGui::IsItemHovered() && myHoveredBone != &aBone)
	{
		if (myHoveredBone)
		{
			myLines.at(myHoveredBone).UpdateColor(ColorManager::GetColor("White"));
		}

		if (mySelectedBone == &aBone)
		{
			myHoveredBone = nullptr;
		}
		else
		{
			myHoveredBone = &aBone;
			myLines.at(myHoveredBone).UpdateColor(ColorManager::GetColor("Green"));
		}
	}

	if (ImGui::IsItemClicked())
	{
		if (mySelectedBone)
		{
			myLines.at(mySelectedBone).UpdateColor(ColorManager::GetColor("White"));
		}
		if (myHoveredBone == &aBone)
		{
			myHoveredBone = nullptr;
		}
		mySelectedBone = &aBone;
		myLines.at(mySelectedBone).UpdateColor(ColorManager::GetColor("Red"));
	}

	if (isOpen)
	{
		for (auto& child : aBone.myChildren)
		{
			CreateBoneList(mySkeleton->GetBone(child));
		}
		ImGui::TreePop();
	}
}

void SkeletonEditor::CreateAssetBrowser()
{
	if (ImGui::Begin("Skeletons and Animations"))
	{
		const float spacing = 10.f;
		const float iconSize = 75.f;
		const float groupsize = iconSize + spacing + (ImGui::GetStyle().ChildBorderSize * 2.f);
		const float contentsize = ImGui::GetWindowSize().x - (ImGui::GetStyle().WindowPadding.x * 2.f);
		unsigned count = 1;

		for (auto& file : myAvailableSkeletons)
		{
			if (CreateFileButton(file, iconSize, false))
			{
				SetSkeleton(AssetManager::GetAsset<Skeleton*>(file), false);
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

		for (auto& file : myAvailableAnimations)
		{
			if (CreateFileButton(file, iconSize, true))
			{
				SetAnimation(AssetManager::GetAsset<Animation>(file));
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
	}
	ImGui::End();
}

bool SkeletonEditor::CreateFileButton(const std::string& aFile, float anIconSize, bool anIsAnimation)
{
	bool isSelected = false;

	if (anIsAnimation)
	{
		if (myAnimation.HasData())
		{
			isSelected = myAnimation.GetPath() == aFile;
		}		
	}
	else
	{
		if (mySkeleton)
		{
			isSelected = mySkeleton->GetPath() == aFile;
		}		
	}

	if (isSelected)
	{
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->ChannelsSplit(2);
		draw_list->ChannelsSetCurrent(1);
	}

	ImGui::BeginGroup();
	if (anIsAnimation)
	{
		ImGui::Image(myAnimationIcon.GetSRV().Get(), ImVec2(anIconSize, anIconSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
	}
	else
	{
		ImGui::Image(mySkeletonIcon.GetSRV().Get(), ImVec2(anIconSize, anIconSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
	}
	ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + anIconSize);
	ImGui::Text(Crimson::GetFileName(aFile).c_str());
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

	if (ImGui::IsItemClicked())
	{
		return true;
	}
	return false;
}

void SkeletonEditor::UpdateAvailableFiles()
{
	myAvailableSkeletons.clear();
	myAvailableAnimations.clear();

	{ // Sort out skeletons
		auto files = Crimson::GetAllFilepathsInDirectory(AssetManager::GetModelPath(), true);
		for (auto& file : files)
		{
			auto potentialTypes = Assets::GetPossibleTypes(Crimson::GetFileExtension(file));
			for (auto& type : potentialTypes)
			{
				if (type == Assets::eAssetType::AnimatedModel || type == Assets::eAssetType::Model || type == Assets::eAssetType::Animation)
				{
					if (Assets::GetModelType(file) == Assets::eAssetType::AnimatedModel)
					{
						myAvailableSkeletons.emplace(file);
					}
					break;
				}
			}
		}
	}

	{ // Sort out animations
		auto files = Crimson::GetAllFilepathsInDirectory(AssetManager::GetAnimationPath(), true);
		for (auto& file : files)
		{
			auto potentialTypes = Assets::GetPossibleTypes(Crimson::GetFileExtension(file));
			for (auto& type : potentialTypes)
			{
				if (type == Assets::eAssetType::AnimatedModel || type == Assets::eAssetType::Model || type == Assets::eAssetType::Animation)
				{
					if (Assets::GetModelType(file) == Assets::eAssetType::Animation)
					{
						myAvailableAnimations.emplace(file);
					}
					break;
				}
			}
		}
	}
}

void SkeletonEditor::CreateBoneLines(unsigned anIndex, const Crimson::Vector4f& aParentPosition)
{
	const Bone& bone = mySkeleton->GetBone(anIndex);
	if (bone.myChildren.empty())
	{
		myLines.emplace(&bone, GraphicsEngine::Get().GetLineDrawer().AddCube(aParentPosition, Crimson::Vector3f(.5f), ColorManager::GetColor("White"), mySkeletonOffset.GetTransformMatrix()));
	}
	else
	{
		auto position = Crimson::Vector4f::NullPosition * bone.myBindPoseInverse.GetInverse();
		myLines.emplace(&bone, GraphicsEngine::Get().GetLineDrawer().AddLine(aParentPosition, position, ColorManager::GetColor("White"), mySkeletonOffset.GetTransformMatrix()));
		for (auto& childIndex : bone.myChildren)
		{
			CreateBoneLines(childIndex, position);
		}
	}	
}
