#include "Modelviewer.pch.h"
#include "SkeletonEditor.h"
#include "AssetManager/Assets/Components/Camera/PerspectiveCameraComponent.h"
#include "AssetManager/Assets/Components/Camera/EditorCameraControllerComponent.h"
#include "AssetManager/Assets/ImguiTransform.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "Modelviewer.h"
#include "Time/Timer.h"

SkeletonEditor::SkeletonEditor() :
	myIsActive(false),
	myHasMatchingBones(false),
	myIsPlayingAnimation(false),
	myFrameIndex(1u),
	myAnimationTimer(0.f),
	myPlaybackMultiplier(1.f),
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

	myBoneColor = ColorManager::GetColor("White");
	mySelectedColor = ColorManager::GetColor("Red");
	myHoveredColor = ColorManager::GetColor("Green");

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
	if (myIsPlayingAnimation)
	{
		myAnimationTimer += Crimson::Timer::GetUnscaledDeltaTime() * myPlaybackMultiplier;
		if (myAnimationTimer >= myAnimation.GetFrameDelta())
		{
			myAnimationTimer -= myAnimation.GetFrameDelta();
			myAnimation.GetNextIndex(myFrameIndex);
			DrawFrame();
		}
	}

	// Create Editor workarea
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(myWindowSize.x, myWindowSize.y));
	const auto& color = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(color.x, color.y, color.z, 1.f));
	ImGui::Begin("Skeleton MainWindow", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar);
	ImGui::PopStyleColor();

	CreateMenubar();

	ImGuiID dockspace_id = ImGui::GetID("Skeleton MainDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_AutoHideTabBar);

	CreateViewport();
	CreateAssetBrowser();

	CreateAnimationInspector();
	CreateSkeletonHeirarchy();

	ImGui::End();
}

void SkeletonEditor::SetSkeleton(Skeleton* aSkeleton, bool aHideLines)
{
	mySkeleton = aSkeleton;
	mySelectedBone = nullptr;
	myHoveredBone = nullptr;

	ClearLines();

	if (mySkeleton)
	{
		myRootBone = &mySkeleton->GetBone(0);
		DrawSkeleton();
	}

	if (aHideLines)
	{
		for (auto& [bone, line] : myLines)
		{
			line.SetActive(false);
		}
	}

	CheckSkeletonAnimationMatching();
}

void SkeletonEditor::SetAnimation(Animation anAnimation)
{
	if (myHasMatchingBones)
	{
		ClearLines();
		DrawSkeleton();
	}

	myAnimation = anAnimation;
	CheckSkeletonAnimationMatching();
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
	if (ImGui::Begin("Skeleton Viewport", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar))
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
	if (ImGui::Begin("Skeleton Heirarchy"))
	{
		if (mySkeleton == nullptr)
		{
			ImGui::End();
			return;
		}

		const std::string boneCount = "Bones: " + std::to_string(mySkeleton->GetBoneCount());
		ImGui::Text(boneCount.c_str());

		const std::string socketCount = "Sockets: " + std::to_string(mySkeleton->GetSocketCount());
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
			myLines.at(mySelectedBone).UpdateColor(myBoneColor);
			mySelectedBone = nullptr;
		}

		if (ImGui::IsItemHovered() && myHoveredBone)
		{
			myLines.at(myHoveredBone).UpdateColor(myBoneColor);
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
			myLines.at(myHoveredBone).UpdateColor(myBoneColor);
		}

		if (mySelectedBone == &aBone)
		{
			myHoveredBone = nullptr;
		}
		else
		{
			myHoveredBone = &aBone;
			myLines.at(myHoveredBone).UpdateColor(myHoveredColor);
		}
	}

	if (ImGui::IsItemClicked())
	{
		if (mySelectedBone)
		{
			myLines.at(mySelectedBone).UpdateColor(myBoneColor);
		}
		if (myHoveredBone == &aBone)
		{
			myHoveredBone = nullptr;
		}
		mySelectedBone = &aBone;
		myLines.at(mySelectedBone).UpdateColor(mySelectedColor);
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

void SkeletonEditor::CreateAnimationInspector()
{
	if (ImGui::Begin("Animation Inspector"))
	{
		if (!myAnimation.HasData())
		{
			ImGui::End();
			return;
		}

		const auto& animationData = myAnimation.GetData();
		ImGui::Text(animationData.myName.c_str());

		ImGui::Separator();

		const std::string frameCount = "Frames: " + std::to_string(animationData.myLength);
		ImGui::Text(frameCount.c_str());

		const std::string eventCount = "Events: " + std::to_string(animationData.myEventNames.size());
		ImGui::Text(eventCount.c_str());

		const std::string fps = "FPS: " + std::to_string(animationData.myFramesPerSecond);
		ImGui::Text(fps.c_str());

		const std::string duration = "Length: " + std::to_string(animationData.myDuration) + " seconds";
		ImGui::Text(duration.c_str());

		ImGui::Separator();

		if (myHasMatchingBones)
		{
			if(ImGui::Button("Start"))
			{
				myIsPlayingAnimation = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Stop"))
			{
				myIsPlayingAnimation = false;
				myAnimationTimer = 0.f;
				myFrameIndex = 1u;
			}
			ImGui::SameLine();
			if (ImGui::Button("Pause"))
			{
				myIsPlayingAnimation = false;
			}

			ImGui::DragFloat("Playback speed", &myPlaybackMultiplier, 0.05f, 0.f, 1000.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);

			int index = myFrameIndex;
			if (ImGui::SliderInt("Frame", &index, 0, myAnimation.GetLastFrameIndex(), "%d", ImGuiSliderFlags_AlwaysClamp))
			{
				myIsPlayingAnimation = false;
				myAnimationTimer = 0.f;
				myFrameIndex = index;
				DrawFrame();
			}
		}
		else
		{
			ImGui::Text("WARNING!");
			ImGui::Text("Animation does not match Skeleton!");
		}
	}
	ImGui::End();
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

void SkeletonEditor::DrawSkeleton()
{
	const Crimson::Vector4f& center = Crimson::Vector4f::NullPosition * myRootBone->myBindPoseInverse.GetInverse();
	const Crimson::Vector3f& extent = Crimson::Vector3f(2.f);
	myLines.emplace(myRootBone, GraphicsEngine::Get().GetLineDrawer().AddCube(center, extent, myBoneColor, mySkeletonOffset.GetTransformMatrix()));

	for (auto& childIndex : myRootBone->myChildren)
	{
		DrawSkeleton(childIndex, center);
	}
}

void SkeletonEditor::DrawSkeleton(unsigned anIndex, const Crimson::Vector4f& aParentPosition)
{
	const Bone& bone = mySkeleton->GetBone(anIndex);

	if (bone.myChildren.empty())
	{
		myLines.emplace(&bone, GraphicsEngine::Get().GetLineDrawer().AddCube(aParentPosition, Crimson::Vector3f(.5f), myBoneColor, mySkeletonOffset.GetTransformMatrix()));
	}
	else
	{
		auto position = Crimson::Vector4f::NullPosition * bone.myBindPoseInverse.GetInverse();
		myLines.emplace(&bone, GraphicsEngine::Get().GetLineDrawer().AddLine(aParentPosition, position, myBoneColor, mySkeletonOffset.GetTransformMatrix()));
		for (auto& childIndex : bone.myChildren)
		{
			DrawSkeleton(childIndex, position);
		}
	}	
}

void SkeletonEditor::DrawFrame()
{
	if (!myHasMatchingBones)
	{
		return;
	}

	const auto& frame = myAnimation.GetFrame(myFrameIndex);
	const Crimson::Vector4f& center = Crimson::Vector4f::NullPosition * frame.myGlobalTransforms.at(myRootBone->myName);
	auto* color = &myBoneColor;
	if (mySelectedBone == myRootBone)
	{
		color = &mySelectedColor;
	}
	else if (myHoveredBone == myRootBone)
	{
		color = &myHoveredColor;
	}
	GraphicsEngine::Get().GetLineDrawer().AddCube(center, Crimson::Vector3f(2.f), *color, mySkeletonOffset.GetTransformMatrix(), false, &myLines.at(myRootBone));

	for (auto& childIndex : myRootBone->myChildren)
	{
		DrawFrame(childIndex, center, frame);
	}
}

void SkeletonEditor::DrawFrame(unsigned anIndex, const Crimson::Vector4f& aParentPosition, const AnimationFrame& aFrame)
{
	const Bone& bone = mySkeleton->GetBone(anIndex);

	auto* color = &myBoneColor;
	if (mySelectedBone == &bone)
	{
		color = &mySelectedColor;
	}
	else if (myHoveredBone == &bone)
	{
		color = &myHoveredColor;
	}

	if (bone.myChildren.empty())
	{
		GraphicsEngine::Get().GetLineDrawer().AddCube(aParentPosition, Crimson::Vector3f(.5f), *color, mySkeletonOffset.GetTransformMatrix(), false, &myLines.at(&bone));
	}
	else
	{
		const auto& position = Crimson::Vector4f::NullPosition * aFrame.myGlobalTransforms.at(bone.myName);
		GraphicsEngine::Get().GetLineDrawer().AddLine(aParentPosition, position, *color, mySkeletonOffset.GetTransformMatrix(), false, &myLines.at(&bone));

		for (auto& childIndex : bone.myChildren)
		{
			DrawFrame(childIndex, position, aFrame);
		}
	}
}

void SkeletonEditor::CheckSkeletonAnimationMatching()
{
	myHasMatchingBones = false;
	myIsPlayingAnimation = false;
	myAnimationTimer = 0.f;
	myFrameIndex = 0u;

	if (mySkeleton == nullptr || !myAnimation.HasData())
	{
		return;
	}

	const auto& frame = myAnimation.GetFrame(0);
	if (mySkeleton->GetBoneCount() != frame.myLocalTransforms.size())
	{
		return;
	}

	for (auto& bone : mySkeleton->GetBones())
	{
		if (frame.myLocalTransforms.find(bone.myName) == frame.myLocalTransforms.end())
		{
			return;
		}
	}

	myHasMatchingBones = true;
}

void SkeletonEditor::ClearLines()
{
	for (auto& [bone, line] : myLines)
	{
		line.Delete();
	}
	myLines.clear();
}
