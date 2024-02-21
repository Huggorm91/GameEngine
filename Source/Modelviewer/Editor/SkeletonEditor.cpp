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
	myCamera.SetPosition({ 0.f, 100.f, -200.f });
	myCamera.AddComponent(PerspectiveCameraComponent(aFoVDegree, aNearPlane, aFarPlane));
	myCamera.AddComponent(EditorCameraControllerComponent(aCameraSpeed, aMouseSensitivity));

	myWindowSize = GraphicsEngine::Get().GetWindowSize();

	myAssetIcon = AssetManager::GetAsset<Texture>("Settings\\EditorAssets\\AnimatedModelIcon.dds");
	SetSkeleton(AssetManager::GetAsset<Skeleton*>("SK_C_TGA_Bro.fbx"));
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
	CreateSkeletonHeirarchy();
	CreateAssetBrowser();

	ImGui::End();
}

void SkeletonEditor::SetSkeleton(Skeleton* aSkeleton)
{
	mySkeleton = aSkeleton;

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

	for (auto& [bone, line] : myLines)
	{
		line.SetActive(false);
	}
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

}

void SkeletonEditor::UpdateAvailableFiles()
{
	myAvailableFiles.clear();
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
					myAvailableFiles.emplace(file);
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
