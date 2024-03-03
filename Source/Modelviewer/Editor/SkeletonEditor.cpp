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
	myIsPlayingInReverse(false),
	myShouldRenderMesh(true),
	myAnimationTimer(0.f),
	myPlaybackMultiplier(1.f),
	myTargetFPS(60.f),
	myEditorTimeScale(0.f),
	myAnimation(std::make_shared<Animation>()),
	mySkeleton(nullptr),
	myMeshTexture(nullptr),
	myMesh(nullptr),
	mySkeletonOffset(nullptr),
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

	myMesh = &myModel.AddComponent<AnimatedMeshComponent>();
	myMesh->SetAnimation(myAnimation);
	mySkeletonOffset = &myModel.GetTransform();

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
		myMesh->Update();
		myAnimation->Update();
		myAnimationTimer += Crimson::Timer::GetDeltaTime();
		if (myAnimationTimer >= myAnimation->GetTargetFrameDelta())
		{
			myAnimationTimer -= myAnimation->GetTargetFrameDelta();

			DrawFrame();
		}
	}

	if (myShouldRenderMesh && mySkeleton)
	{
		myModel.Render();
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
	CreateSkeletonInspector();

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
		*myMesh = AssetManager::GetAsset<AnimatedMeshComponent>(mySkeleton->GetPath());
		
		myRootBone = &mySkeleton->GetBone(0);
		GenerateSkeletonDrawing();
	}

	if (aHideLines)
	{
		for (auto& [bone, line] : myLines)
		{
			line.SetActive(false);
		}
	}

	CheckSkeletonAnimationMatching();
	if (myHasMatchingBones)
	{
		myAnimation->Init(myBoneTransforms, mySkeleton);
		SetMeshAnimation();
		DrawFrame();
	}
}

void SkeletonEditor::SetAnimation(const std::shared_ptr<AnimationBase>& anAnimation)
{
	if (myHasMatchingBones)
	{
		ClearLines();
		GenerateSkeletonDrawing();
	}

	myAnimation = anAnimation;
	myAnimation->Init(myBoneTransforms, mySkeleton);
	myAnimation->SetTargetFPS(myTargetFPS);
	myAnimation->SetIsLooping(true);

	CheckSkeletonAnimationMatching();
	if (myHasMatchingBones)
	{
		SetMeshAnimation();
		DrawFrame();
	}
	else
	{
		myMesh->ResetBoneCache();
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

	myEditorTimeScale = Crimson::Timer::GetTimeScale();
	Crimson::Timer::SetTimeScale(myPlaybackMultiplier);

	GraphicsEngine::Get().SetLightMode(GraphicsEngine::LightMode::IgnoreLight);
	GraphicsEngine::Get().GetLineDrawer().SetUsingDepthBuffer(false);
	for (auto& [bone, line] : myLines)
	{
		line.SetActive(true);
	}
}

void SkeletonEditor::Deactivate()
{
	myIsActive = false;
	myCamera.SetActiveComponents(false);
	ModelViewer::Get().SetIsSceneActive(true);
	ModelViewer::Get().ActivateImGuiEditor();
	ModelViewer::Get().RestoreDebugSettings();

	Crimson::Timer::SetTimeScale(myEditorTimeScale);

	GraphicsEngine::Get().GetLineDrawer().SetUsingDepthBuffer(true);
	for (auto& [bone, line] : myLines)
	{
		line.SetActive(false);
	}
}

void SkeletonEditor::CreateMenubar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::MenuItem("Close"))
		{
			Deactivate();
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

void SkeletonEditor::CreateSkeletonInspector()
{
	if (ImGui::Begin("Skeleton Inspector"))
	{
		if (mySkeleton == nullptr)
		{
			ImGui::End();
			return;
		}

		ImGui::Checkbox("Show Mesh", &myShouldRenderMesh);
		if (myShouldRenderMesh)
		{
			auto color = myMesh->GetColor();
			if (ImGui::ColorEdit4("Mesh Color", &color.x))
			{
				myMesh->SetColor(color);
			}

			CreateTexturePicker();
		}

		ImGui::Separator();

		if (ImGui::ColorEdit4("Skeleton Color", &myBoneColor.x))
		{
			for (auto& [bone, line] : myLines)
			{
				if (bone == mySelectedBone || bone == myHoveredBone)
				{
					continue;
				}
				line.UpdateColor(myBoneColor);
			}
		}
		if (ImGui::ColorEdit4("Selected Bone Color", &mySelectedColor.x))
		{
			if (mySelectedBone)
			{
				myLines.at(mySelectedBone).UpdateColor(mySelectedColor);
			}
		}
		if (ImGui::ColorEdit4("Hovered Bope Color", &myHoveredColor.x))
		{
			if (myHoveredBone)
			{
				myLines.at(myHoveredBone).UpdateColor(myBoneColor);
			}
		}

		ImGui::Separator();

		const std::string boneCount = "Bones: " + std::to_string(mySkeleton->GetBoneCount());
		ImGui::Text(boneCount.c_str());

		const std::string socketCount = "Sockets: " + std::to_string(mySkeleton->GetSocketCount());
		ImGui::Text(socketCount.c_str());

		auto position = myModel.GetWorldPosition();
		if (ImGui::DragFloat3("Position", &position.x))
		{
			myModel.SetPosition(position);
			for (auto& [bone, line] : myLines)
			{
				line.UpdateTransform(mySkeletonOffset->GetTransformMatrix());
			}
		}

		auto rotation = myModel.GetTransform().GetRotation();
		if (ImGui::DragFloat3("Rotation", &rotation.x))
		{
			myModel.SetRotation(rotation);
			for (auto& [bone, line] : myLines)
			{
				line.UpdateTransform(mySkeletonOffset->GetTransformMatrix());
			}
		}

		const bool isOpen = ImGui::TreeNodeEx(mySkeleton->GetName().c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen);

		if (ImGui::IsItemClicked())
		{
			SelectBone(nullptr);
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
	const bool isOpen = ImGui::TreeNodeEx(aBone.name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | (mySelectedBone == &aBone ? ImGuiTreeNodeFlags_Selected : 0) | (aBone.children.empty() ? ImGuiTreeNodeFlags_Leaf : 0));

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
		if (myHoveredBone == &aBone)
		{
			myHoveredBone = nullptr;
		}
		SelectBone(&aBone);
	}

	if (isOpen)
	{
		for (auto& child : aBone.children)
		{
			CreateBoneList(mySkeleton->GetBone(child));
		}
		ImGui::TreePop();
	}
}

void SkeletonEditor::CreateTexturePicker()
{
	if (ImGui::BeginCombo("Mesh Texture", Crimson::ToString(myMeshTexture ? myMeshTexture->GetName() : L"").c_str(), ImGuiComboFlags_HeightLarge))
	{
		for (auto& path : AssetManager::GetAvailableTextures())
		{
			Texture* current = AssetManager::GetAsset<Texture*>(path);
			const bool isSelected = myMeshTexture == current;
			if (ImGui::Selectable(Crimson::ToString(current->GetName()).c_str(), isSelected))
			{
				myMeshTexture = current;
				myMesh->SetAlbedoTexture(myMeshTexture);
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}

			constexpr float imageSize = 30.f;
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - imageSize);
			ImGui::Image(current->GetSRV().Get(), ImVec2(imageSize, imageSize));
		}
		ImGui::EndCombo();
	}
}

void SkeletonEditor::CreateAnimationInspector()
{
	if (ImGui::Begin("Animation Inspector"))
	{
		if (!myAnimation->HasData())
		{
			ImGui::End();
			return;
		}

		if (auto animationPtr = std::dynamic_pointer_cast<Animation>(myAnimation))
		{
			const auto& animationData = animationPtr->GetData();
			ImGui::Text(animationData.name.c_str());

			ImGui::Separator();

			const std::string frameCount = "Frames: " + std::to_string(animationData.length);
			ImGui::Text(frameCount.c_str());

			const std::string eventCount = "Events: " + std::to_string(animationData.eventNames.size());
			ImGui::Text(eventCount.c_str());

			const std::string fps = "FPS: " + std::to_string(animationData.framesPerSecond);
			ImGui::Text(fps.c_str());

			const std::string duration = "Length: " + std::to_string(animationData.duration) + " seconds";
			ImGui::Text(duration.c_str());
		}
		else
		{
			// BlendSpace
		}

		ImGui::Separator();

		if (myHasMatchingBones)
		{
			if (ImGui::Button("Start"))
			{
				myIsPlayingAnimation = true;
				myMesh->StartAnimation();
			}
			ImGui::SameLine();
			if (ImGui::Button("Stop"))
			{
				myIsPlayingAnimation = false;
				myAnimationTimer = 0.f;
				myAnimation->SetToFirstFrame();
				myMesh->StopAnimation();
			}
			ImGui::SameLine();
			if (ImGui::Button("Pause"))
			{
				myIsPlayingAnimation = false;
				myMesh->PauseAnimation();
			}

			if (ImGui::Checkbox("Play in reverse", &myIsPlayingInReverse))
			{
				myMesh->SetPlayInReverse(myIsPlayingInReverse);
			}

			if (ImGui::DragFloat("Target FPS", &myTargetFPS, 1.f, 0.f, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp))
			{
				myMesh->SetTargetFPS(myTargetFPS);
			}

			if (ImGui::DragFloat("Playback speed", &myPlaybackMultiplier, 0.01f, 0.f, 1000.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
			{
				Crimson::Timer::SetTimeScale(myPlaybackMultiplier);
			}

			if (auto animationPtr = std::dynamic_pointer_cast<Animation>(myAnimation))
			{
				int index = animationPtr->GetCurrentFrameIndex();
				if (ImGui::SliderInt("Frame", &index, 0, animationPtr->GetLastFrameIndex(), "%d", ImGuiSliderFlags_AlwaysClamp))
				{
					myIsPlayingAnimation = false;
					myAnimationTimer = 0.f;
					animationPtr->SetFrameIndex(index);
					SetMeshAnimation();
					DrawFrame();
				}
			}
			else
			{
				// BlendSpace
			}
		}
		else
		{
			ImGui::Text("WARNING!");
			ImGui::Text("Animation does not match Skeleton!");
			ImGui::Spacing();
			ImGui::Text(myMissMatchMessage.c_str());
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
				if (mySelectedBone != nullptr)
				{
					SetAnimation(std::make_shared<AnimationLayer>(AssetManager::GetAsset<Animation>(file), GetBoneIndex(mySelectedBone)));
				}
				else
				{
					SetAnimation(std::make_shared<Animation>(AssetManager::GetAsset<Animation>(file)));
				}
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
		if (myAnimation->HasData())
		{
			isSelected = myAnimation->GetName() == aFile;
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

void SkeletonEditor::GenerateSkeletonDrawing()
{
	const Crimson::Vector4f& center = Crimson::Vector4f::NullPosition * myRootBone->bindPoseInverse.GetInverse();
	myLines.emplace(myRootBone, GraphicsEngine::Get().GetLineDrawer().AddCube(center, Crimson::Vector3f(2.f), myBoneColor, mySkeletonOffset->GetTransformMatrix()));

	for (auto& childIndex : myRootBone->children)
	{
		GenerateSkeletonDrawing(childIndex, center);
	}
}

void SkeletonEditor::GenerateSkeletonDrawing(unsigned anIndex, const Crimson::Vector4f& aParentPosition)
{
	const Bone& bone = mySkeleton->GetBone(anIndex);

	if (bone.children.empty())
	{
		myLines.emplace(&bone, GraphicsEngine::Get().GetLineDrawer().AddCube(aParentPosition, Crimson::Vector3f(.5f), myBoneColor, mySkeletonOffset->GetTransformMatrix()));
	}
	else
	{
		auto position = Crimson::Vector4f::NullPosition * bone.bindPoseInverse.GetInverse();
		myLines.emplace(&bone, GraphicsEngine::Get().GetLineDrawer().AddLine(aParentPosition, position, myBoneColor, mySkeletonOffset->GetTransformMatrix()));
		for (auto& childIndex : bone.children)
		{
			GenerateSkeletonDrawing(childIndex, position);
		}
	}
}

void SkeletonEditor::DrawSkeleton()
{
	const Crimson::Vector4f& center = Crimson::Vector4f::NullPosition * myRootBone->bindPoseInverse.GetInverse();
	auto* color = &myBoneColor;
	if (mySelectedBone == myRootBone)
	{
		color = &mySelectedColor;
	}
	else if (myHoveredBone == myRootBone)
	{
		color = &myHoveredColor;
	}
	GraphicsEngine::Get().GetLineDrawer().AddCube(center, Crimson::Vector3f(2.f), *color, mySkeletonOffset->GetTransformMatrix(), false, &myLines.at(myRootBone));

	for (auto& childIndex : myRootBone->children)
	{
		DrawSkeleton(childIndex, center);
	}
}

void SkeletonEditor::DrawSkeleton(unsigned anIndex, const Crimson::Vector4f& aParentPosition)
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

	if (bone.children.empty())
	{
		GraphicsEngine::Get().GetLineDrawer().AddCube(aParentPosition, Crimson::Vector3f(.5f), *color, mySkeletonOffset->GetTransformMatrix(), false, &myLines.at(&bone));
	}
	else
	{
		auto position = Crimson::Vector4f::NullPosition * bone.bindPoseInverse.GetInverse();
		GraphicsEngine::Get().GetLineDrawer().AddLine(aParentPosition, position, *color, mySkeletonOffset->GetTransformMatrix(), false, &myLines.at(&bone));
		for (auto& childIndex : bone.children)
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

	const AnimationFrame* frame = nullptr;
	if (auto animationPtr = std::dynamic_pointer_cast<Animation>(myAnimation))
	{
		frame = &animationPtr->GetCurrentFrame();
	}
	else
	{
		// BlendSpace
	}
	
	if (mySelectedBone && mySelectedBone != myRootBone)
	{
		auto& parentBone = mySkeleton->GetBone(mySelectedBone->parent);
		const Crimson::Vector4f& center = Crimson::Vector4f::NullPosition * parentBone.bindPoseInverse.GetInverse();

		DrawFrame(GetBoneIndex(mySelectedBone), center, parentBone.bindPoseInverse.GetInverse(), *frame);
	}
	else
	{
		const Crimson::Vector4f& center = Crimson::Vector4f::NullPosition * frame->globalTransformMatrices.at(myRootBone->name);
		auto* color = &myBoneColor;
		if (mySelectedBone == myRootBone)
		{
			color = &mySelectedColor;
		}
		else if (myHoveredBone == myRootBone)
		{
			color = &myHoveredColor;
		}
		GraphicsEngine::Get().GetLineDrawer().AddCube(center, Crimson::Vector3f(2.f), *color, mySkeletonOffset->GetTransformMatrix(), false, &myLines.at(myRootBone));

		for (auto& childIndex : myRootBone->children)
		{
			DrawFrame(childIndex, center, *frame);
		}
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

	if (bone.children.empty())
	{
		GraphicsEngine::Get().GetLineDrawer().AddCube(aParentPosition, Crimson::Vector3f(.5f), *color, mySkeletonOffset->GetTransformMatrix(), false, &myLines.at(&bone));
	}
	else
	{
		const auto& position = Crimson::Vector4f::NullPosition * aFrame.globalTransformMatrices.at(bone.name);
		GraphicsEngine::Get().GetLineDrawer().AddLine(aParentPosition, position, *color, mySkeletonOffset->GetTransformMatrix(), false, &myLines.at(&bone));

		for (auto& childIndex : bone.children)
		{
			DrawFrame(childIndex, position, aFrame);
		}
	}
}

void SkeletonEditor::DrawFrame(unsigned anIndex, const Crimson::Vector4f& aParentPosition, const Crimson::Matrix4x4f& aParentMatrix, const AnimationFrame& aFrame)
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

	if (bone.children.empty())
	{
		GraphicsEngine::Get().GetLineDrawer().AddCube(aParentPosition, Crimson::Vector3f(.5f), *color, mySkeletonOffset->GetTransformMatrix(), false, &myLines.at(&bone));
	}
	else
	{
		const auto& matrix = aFrame.localTransformMatrices.at(bone.name)* aParentMatrix;
		const auto& position = Crimson::Vector4f::NullPosition * matrix;
		GraphicsEngine::Get().GetLineDrawer().AddLine(aParentPosition, position, *color, mySkeletonOffset->GetTransformMatrix(), false, &myLines.at(&bone));

		for (auto& childIndex : bone.children)
		{
			DrawFrame(childIndex, position, matrix, aFrame);
		}
	}
}

void SkeletonEditor::CheckSkeletonAnimationMatching()
{
	myMissMatchMessage = "";
	myIsPlayingAnimation = false;
	myAnimationTimer = 0.f;

	myHasMatchingBones = myAnimation->IsValidSkeleton(mySkeleton, &myMissMatchMessage);
}

void SkeletonEditor::ClearLines()
{
	for (auto& [bone, line] : myLines)
	{
		line.Delete();
	}
	myLines.clear();
}

unsigned SkeletonEditor::GetBoneIndex(const Bone* aBone) const
{
	if (aBone->children.empty())
	{
		for (auto& childIndex : mySkeleton->GetBone(aBone->parent).children)
		{
			if (&mySkeleton->GetBone(childIndex) == aBone)
			{
				return childIndex;
			}
		}
	}
	else
	{
		return mySkeleton->GetBone(aBone->children.front()).parent;
	}

	return 0;
}

void SkeletonEditor::SetMeshAnimation()
{
	myMesh->ResetBoneCache();
	if (auto layerPtr = std::dynamic_pointer_cast<AnimationLayer>(myAnimation))
	{
		myMesh->SetAnimation(std::make_shared<AnimationLayer>(*layerPtr));
	}
	else if (auto animationPtr = std::dynamic_pointer_cast<Animation>(myAnimation))
	{
		myMesh->SetAnimation(std::make_shared<Animation>(*animationPtr));
	}
	else
	{
		//auto blendPtr = std::dynamic_pointer_cast<BlendSpace>(myAnimation);
		//myMesh->SetAnimation(std::make_shared<BlendSpace>(*blendPtr));
	}
}

void SkeletonEditor::SelectBone(const Bone* aBone)
{
	if (mySelectedBone)
	{
		myLines.at(mySelectedBone).UpdateColor(myBoneColor);
	}

	mySelectedBone = aBone;

	if (mySelectedBone)
	{
		myLines.at(mySelectedBone).UpdateColor(mySelectedColor);
	}	

	if (myIsPlayingAnimation)
	{
		if (auto animationPtr = std::dynamic_pointer_cast<Animation>(myAnimation))
		{
			if (mySelectedBone)
			{
				myAnimation = std::make_shared<AnimationLayer>(*animationPtr, GetBoneIndex(mySelectedBone));
			}
			else
			{
				myAnimation = std::make_shared<Animation>(*animationPtr);
			}
		}
		else
		{
			// BlendSpace
		}
		SetMeshAnimation();
		DrawSkeleton();
		DrawFrame();
	}
}
