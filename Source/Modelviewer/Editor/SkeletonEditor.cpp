#include "Modelviewer.pch.h"
#include "SkeletonEditor.h"
#include "AssetManager/Assets/Components/Camera/PerspectiveCameraComponent.h"
#include "AssetManager/Assets/Components/Camera/EditorCameraControllerComponent.h"
#include "AssetManager/Assets/ImguiTransform.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "File/FileSelectors.h"
#include "Modelviewer.h"
#include "Time/Timer.h"
#include "Math/Sort.hpp"

SkeletonEditor::SkeletonEditor() :
	myIsActive(false),
	myHasMatchingBones(false),
	myIsPlayingInReverse(false),
	myShouldRenderMesh(true),
	myShouldRenderSkeleton(true),
	myIsEditingBlendSpace(false),
	myIsUsingRootMotion(false),
	myPlayCount(0),
	myAnimationTimer(0.f),
	myPlaybackMultiplier(1.f),
	myTargetFPS(60.f),
	myEditorTimeScale(0.f),
	mySkeleton(nullptr),
	myMeshTexture(nullptr),
	myMesh(nullptr),
	myBoneTransforms(nullptr),
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

	myMesh = &myModel.AddComponent<AnimationControllerComponent>();
	myMesh->SetTargetFPS(myTargetFPS);
	myMesh->SetRenderShadow(false);
	myBoneTransforms = const_cast<std::array<Crimson::Matrix4x4f, MAX_BONE_COUNT>*>(&myMesh->GetBoneTransforms());
	mySkeletonOffset = const_cast<Transform*>(&myModel.GetTransform());

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

	if (myPlayCount > 0)
	{
		myMesh->UpdateNoRender();
		myAnimationTimer += Crimson::Timer::GetDeltaTime();
		if (myAnimationTimer >= myMesh->myAnimationDelta)
		{
			myAnimationTimer = 0.f;
			DrawFrame();
		}
	}

	if (myShouldRenderMesh && mySkeleton)
	{
		myMesh->Render();
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
		myMesh->SetMesh(AssetManager::GetAsset<AnimatedMeshComponent>(mySkeleton->GetPath()));

		if (mySkeleton->GetBoneCount() > MAX_BONE_COUNT)
		{
			myShouldRenderMesh = false;
		}
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

	if (myMesh->HasAnimation())
	{
		CheckSkeletonAnimationMatching(myMesh->myAnimation);
		if (myHasMatchingBones || (myIsEditingBlendSpace && !myMesh->myAnimation->HasData()))
		{
			myMesh->SetSkeleton(AssetManager::GetAsset<Skeleton*>(mySkeleton->GetPath()));

			// This loop should not be needed but better safe than sorry
			for (auto& animation : myMesh->myAdditiveAnimations)
			{
				CheckSkeletonAnimationMatching(animation);
				if (!myHasMatchingBones)
				{
					break;
				}
			}
		}
		else
		{
			myMesh->SetSkeleton(nullptr);
		}
	}
	else
	{
		myMesh->SetSkeleton(AssetManager::GetAsset<Skeleton*>(mySkeleton->GetPath()));
	}

	if (myHasMatchingBones)
	{
		DrawFrame();
	}
}

void SkeletonEditor::SetAnimation(const std::shared_ptr<AnimationBase>& anAnimation)
{
	bool wasMatching = myHasMatchingBones;
	if (wasMatching)
	{
		ClearLines();
		GenerateSkeletonDrawing();
	}

	myIsEditingBlendSpace = false;

	if (!anAnimation)
	{
		myHasMatchingBones = false;
		myMesh->SetAnimation(anAnimation);
		return;
	}

	mySelectedAnimationPath = anAnimation->GetPath();

	CheckSkeletonAnimationMatching(anAnimation);
	if (myHasMatchingBones)
	{
		if (myIsUsingRootMotion)
		{
			anAnimation->EnableRootMotion(*mySkeletonOffset);
		}
		if (myPlayCount > 0)
		{
			myPlayCount = 1;
			anAnimation->StartAnimation();
		}

		myMesh->SetAnimation(anAnimation);
		if (!wasMatching)
		{
			myMesh->SetSkeleton(AssetManager::GetAsset<Skeleton*>(mySkeleton->GetPath()));
		}

		DrawFrame();
	}
	else
	{
		myMesh->ResetBoneCache();
		myMesh->myAnimation = anAnimation;
	}

	myMesh->myAnimation->SetTargetFPS(myTargetFPS);
	myMesh->myAnimation->SetIsLooping(true);
	myMesh->myAnimation->SetIsPlayingInReverse(myIsPlayingInReverse);
}

void SkeletonEditor::AddAnimation(const std::shared_ptr<AnimationBase>& anAnimation)
{
	if (myIsEditingBlendSpace || anAnimation == nullptr || !myHasMatchingBones)
	{
		return;
	}

	CheckSkeletonAnimationMatching(anAnimation);

	if (myHasMatchingBones)
	{
		anAnimation->SetTargetFPS(myTargetFPS);
		anAnimation->SetIsLooping(true);
		anAnimation->SetIsPlayingInReverse(myIsPlayingInReverse);
		if (myPlayCount > 0)
		{
			++myPlayCount;
			anAnimation->StartAnimation();
		}
		myMesh->AddAnimation(anAnimation);

		DrawFrame();
	}
	else
	{
		myMesh->myAdditiveAnimations.emplace_back(anAnimation);
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

	auto& engine = GraphicsEngine::Get();
	engine.SetDebugMode(GraphicsEngine::DebugMode::Default);
	engine.SetLightMode(GraphicsEngine::LightMode::IgnoreLight);
	engine.SetRenderMode(GraphicsEngine::RenderMode::Mesh);
	engine.GetLineDrawer().SetUsingDepthBuffer(false);
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

	if (myIsEditingBlendSpace)
	{
		myIsEditingBlendSpace = false;
		myMesh->SetAnimation(nullptr);
	}
}

void SkeletonEditor::CreateMenubar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Blend Space"))
		{
			if (ImGui::MenuItem("Create New"))
			{
				BlendSpace newBlendSpace;
				newBlendSpace.myName = "NewBlendSpace";
				newBlendSpace.SetIsLooping(true);
				myMesh->SetAnimation(std::make_shared<BlendSpace>(newBlendSpace));
				myIsEditingBlendSpace = true;
				myHasMatchingBones = true;
				if (mySkeleton)
				{
					DrawFrame();
				}
			}
			if (ImGui::MenuItem("Save"))
			{
				SaveBlendSpace();
			}
			ImGui::EndMenu();
		}
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

		if (ImGui::Checkbox("Show Skeleton", &myShouldRenderSkeleton))
		{
			for (auto& [bone, line] : myLines)
			{
				line.SetActive(myShouldRenderSkeleton);
			}
		}
		ImGui::SameLine();

		const bool disableRender = mySkeleton->GetBoneCount() > MAX_BONE_COUNT;

		if (disableRender)
		{
			ImGui::BeginDisabled();
		}

		ImGui::Checkbox("Show Mesh", &myShouldRenderMesh);

		if (disableRender)
		{
			ImGui::EndDisabled();
		}


		auto color = myMesh->GetColor();
		if (ImGui::ColorEdit4("Mesh Color", &color.x))
		{
			myMesh->SetColor(color);
		}

		CreateTexturePicker();

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
		if (ImGui::ColorEdit4("Hovered Bone Color", &myHoveredColor.x))
		{
			if (myHoveredBone)
			{
				myLines.at(myHoveredBone).UpdateColor(myBoneColor);
			}
		}

		ImGui::Separator();

		const std::string boneCount = "Bones: " + std::to_string(mySkeleton->GetBoneCount());
		ImGui::Text(boneCount.c_str());

		if (mySkeleton->GetBoneCount() > MAX_BONE_COUNT)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 0.f, 0.f, 1.f));
			const std::string warning = "WARNING! Too many bones! Maximum supported amount is: " + std::to_string(MAX_BONE_COUNT);
			ImGui::Text(warning.c_str());
			ImGui::PopStyleColor();
		}

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

		auto rotation = myModel.GetTransform().GetRotationDegree();
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
		const unsigned boneIndex = mySkeleton->GetBoneIndex(aBone.name);
		SetBone(boneIndex, myMesh->myAnimation);
		for (auto& animation : myMesh->myAdditiveAnimations)
		{
			SetBone(boneIndex, animation);
		}
		myMesh->UpdateBoneCache();
		DrawSkeleton();
		DrawFrame();
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

		if (!myMesh->HasAnimation())
		{
			ImGui::End();
			return;
		}

		if (ImGui::DragFloat("Target FPS", &myTargetFPS, 1.f, 0.f, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp))
		{
			myMesh->SetTargetFPS(myTargetFPS);
		}

		if (ImGui::DragFloat("Playback speed", &myPlaybackMultiplier, 0.01f, 0.f, 1000.f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
		{
			Crimson::Timer::SetTimeScale(myPlaybackMultiplier);
		}

		if (ImGui::Checkbox("Root Motion", &myIsUsingRootMotion))
		{
			if (myIsUsingRootMotion)
			{
				myMesh->myAnimation->EnableRootMotion(*mySkeletonOffset);
			}
			else
			{
				myMesh->myAnimation->DisableRootMotion();
			}
		}

		ImGui::Separator();

		{ // Buttons
			const bool disabledAdd = myIsEditingBlendSpace || mySelectedAnimationPath.empty() || Crimson::GetFileExtension(mySelectedAnimationPath) != AssetManager::GetAnimationExtension();

			if (disabledAdd)
			{
				ImGui::BeginDisabled();
			}

			if (ImGui::Button("Add Animation"))
			{
				if (myMesh->myAnimation)
				{
					AddAnimation(mySelectedAnimationPath);
				}
				else
				{
					SetAnimation(mySelectedAnimationPath);
				}
			}

			if (disabledAdd)
			{
				ImGui::EndDisabled();
			}

			ImGui::SameLine();

			if (ImGui::Button("Clear"))
			{
				SetAnimation(std::shared_ptr<AnimationBase>());
				myPlayCount = 0;
				myIsEditingBlendSpace = false;
				ImGui::End();
				return;
			}
		}

		{ // Primary Animation
			if (ImGui::CollapsingHeader("Primary Animation", ImGuiTreeNodeFlags_DefaultOpen))
			{
				switch (myMesh->myAnimation->myType)
				{
				case AnimationBase::AnimationType::Animation:
				{
					auto animationPtr = std::dynamic_pointer_cast<Animation>(myMesh->myAnimation);
					CreateAnimationSection(animationPtr.get());
					break;
				}
				case AnimationBase::AnimationType::AnimationLayer:
				{
					auto layerPtr = std::dynamic_pointer_cast<AnimationLayer>(myMesh->myAnimation);
					CreateAnimationLayerSection(layerPtr.get());
					break;
				}
				case AnimationBase::AnimationType::BlendSpace:
				{
					auto blendPtr = std::dynamic_pointer_cast<BlendSpace>(myMesh->myAnimation);
					CreateBlendSpaceSection(blendPtr.get());
					break;
				}
				default:
					break;
				}

				if (mySkeleton)
				{
					int boneIndex = myMesh->myAnimation->GetStartBoneIndex();
					ImGui::Text(mySkeleton->GetBone(boneIndex).namespaceName.c_str());
					if (ImGui::DragInt("Bone Index", &boneIndex, 1.f, 0, mySkeleton->GetBoneCount() - 1, "%d", ImGuiSliderFlags_AlwaysClamp))
					{
						SetBone(boneIndex, myMesh->myAnimation);
						myMesh->UpdateBoneCache();
						SelectBone(&mySkeleton->GetBone(boneIndex));
						DrawFrame();
					}
				}
			}
		}

		{ // Additive Animations
			size_t index = 0;
			for (auto& animation : myMesh->myAdditiveAnimations)
			{
				if (!myHasMatchingBones && index + 1 == myMesh->myAdditiveAnimations.size())
				{
					PrintAnimationMissmatchError();
					ImGui::End();
					return;
				}

				const std::string& text = "Additive Animation " + std::to_string(index + 1);
				if (ImGui::CollapsingHeader(text.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::PushID(static_cast<int>(index));
					if (ImGui::Button("Remove"))
					{
						myMesh->RemoveAnimation(index);
						ImGui::PopID();
						ImGui::End();
						return;
					}

					switch (animation->myType)
					{
					case AnimationBase::AnimationType::Animation:
					{
						auto animationPtr = std::dynamic_pointer_cast<Animation>(animation);
						CreateAnimationSection(animationPtr.get());
						break;
					}
					case AnimationBase::AnimationType::AnimationLayer:
					{
						auto layerPtr = std::dynamic_pointer_cast<AnimationLayer>(animation);
						CreateAnimationLayerSection(layerPtr.get());
						break;
					}
					case AnimationBase::AnimationType::BlendSpace:
					{
						auto blendPtr = std::dynamic_pointer_cast<BlendSpace>(animation);
						CreateBlendSpaceSection(blendPtr.get());
						break;
					}
					default:
						break;
					}

					if (mySkeleton)
					{
						int boneIndex = animation->GetStartBoneIndex();
						ImGui::Text(mySkeleton->GetBone(boneIndex).namespaceName.c_str());
						if (ImGui::DragInt("Bone Index", &boneIndex, 1.f, 0, mySkeleton->GetBoneCount() - 1, "%d", ImGuiSliderFlags_AlwaysClamp))
						{
							SetBone(boneIndex, animation);
							myMesh->UpdateBoneCache();
							SelectBone(&mySkeleton->GetBone(boneIndex));
							DrawFrame();
						}
					}

					ImGui::PopID();
				}

				++index;
			}
		}
	}
	ImGui::End();
}

void SkeletonEditor::CreateAnimationSection(Animation* anAnimation)
{
	if (myMesh->myAdditiveAnimations.empty() && !myHasMatchingBones)
	{
		PrintAnimationMissmatchError();
		return;
	}
	CreateAnimationInfo(anAnimation);

	ImGui::Separator();

	CreateAnimationControls(anAnimation);
	ImGui::Separator();
	CreateAnimationParameters(anAnimation);
}

void SkeletonEditor::CreateAnimationLayerSection(AnimationLayer* anAnimation)
{
	if (myMesh->myAdditiveAnimations.empty() && !myHasMatchingBones)
	{
		PrintAnimationMissmatchError();
		return;
	}
	CreateAnimationInfo(anAnimation);

	ImGui::Separator();

	CreateAnimationControls(anAnimation);
	ImGui::Separator();
	CreateAnimationParameters(anAnimation);
}

void SkeletonEditor::CreateBlendSpaceSection(BlendSpace* aBlendSpace)
{
	ImGui::InputText("Name", &aBlendSpace->myName);

	if (!aBlendSpace->myAnimations.empty())
	{
		Animation* animation = const_cast<Animation*>(aBlendSpace->myLongestAnimation);
		if (animation == nullptr)
		{
			animation = aBlendSpace->myAnimations.back().animation.get();
		}

		if (!aBlendSpace->myHasMatchingFPS)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 0.f, 0.f, 1.f));
			ImGui::Text("Warning! \nThe animations in this blend space does not have matching FPS. \nMake sure to set a target FPS for it to work properly!");
			ImGui::PopStyleColor();
		}
		CreateAnimationInfo(animation);
		ImGui::DragFloat("Test Blend Value", &aBlendSpace->myBlendValue);
	}

	ImGui::Separator();

	{
		const bool isDisabled = mySelectedAnimationPath.empty() || Crimson::GetFileExtension(mySelectedAnimationPath) != AssetManager::GetAnimationExtension();
		if (isDisabled)
		{
			ImGui::BeginDisabled();
		}

		if (ImGui::Button("Add Blend Animation"))
		{
			if (aBlendSpace->myBoneIndex == 0u)
			{
				aBlendSpace->AddAnimation(AssetManager::GetAsset<Animation>(mySelectedAnimationPath), 0.f);
			}
			else
			{
				aBlendSpace->AddAnimation(AssetManager::GetAsset<Animation>(mySelectedAnimationPath), aBlendSpace->myBoneIndex, 0.f);
			}
			CheckSkeletonAnimationMatching(std::make_shared<Animation>(AssetManager::GetAsset<Animation>(mySelectedAnimationPath)));
			if (myHasMatchingBones)
			{
				DrawFrame();
			}
		}

		if (isDisabled)
		{
			ImGui::EndDisabled();
		}
	}

	if (myMesh->myAdditiveAnimations.empty() && !myHasMatchingBones)
	{
		PrintAnimationMissmatchError();
		return;
	}

	CreateAnimationControls(aBlendSpace);

	ImGui::Separator();

	int index = 0;
	for (auto& data : aBlendSpace->myAnimations)
	{
		ImGui::PushID(index);
		ImGui::Text(data.animation->GetPath().c_str());
		CreateAnimationParameters(data.animation.get());
		if (ImGui::InputFloat("Blend Value", &data.blendValue, 0.f, 0.f, "%.1f", ImGuiInputTextFlags_EnterReturnsTrue))
		{
			Crimson::QuickSort(aBlendSpace->myAnimations);
			ImGui::PopID();
			break;
		}
		ImGui::Dummy(ImGui::GetItemRectSize());
		ImGui::PopID();
		index++;
	}
}

void SkeletonEditor::CreateAnimationInfo(Animation* anAnimation)
{
	const auto& animationData = anAnimation->GetData();
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

void SkeletonEditor::CreateAnimationControls(AnimationBase* anAnimation)
{
	bool isReversing = anAnimation->IsPlayingInReverse();
	if (ImGui::Checkbox("Play in reverse", &isReversing))
	{
		anAnimation->SetIsPlayingInReverse(isReversing);
	}

	if (ImGui::Button("Start"))
	{
		if (!anAnimation->IsPlaying())
		{
			++myPlayCount;
		}

		anAnimation->StartAnimation();
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		if (anAnimation->IsPlaying())
		{
			--myPlayCount;
		}
		myAnimationTimer = 0.f;
		anAnimation->SetToFirstFrame();
		anAnimation->StopAnimation();
		myMesh->UpdateBoneCache();
		DrawFrame();
	}
	ImGui::SameLine();
	if (ImGui::Button("Pause"))
	{
		if (anAnimation->IsPlaying())
		{
			--myPlayCount;
		}
		anAnimation->StopAnimation();
	}
}

void SkeletonEditor::CreateAnimationParameters(Animation* anAnimation)
{
	int index = anAnimation->GetCurrentFrameIndex();
	if (ImGui::SliderInt("Frame", &index, 0, anAnimation->GetLastFrameIndex(), "%d", ImGuiSliderFlags_AlwaysClamp))
	{
		myPlayCount = 0;
		myMesh->PauseAnimation();
		myAnimationTimer = 0.f;
		anAnimation->SetFrameIndex(index);
		myMesh->UpdateBoneCache();
		DrawFrame();
	}
}

void SkeletonEditor::PrintAnimationMissmatchError()
{
	ImGui::Text("WARNING!");
	ImGui::Text("Animation does not match Skeleton!");
	ImGui::Spacing();
	ImGui::Text(myMissMatchMessage.c_str());
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
			CreateFileButton(file, iconSize, false);
			if (ImGui::IsItemClicked())
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
			CreateFileButton(file, iconSize, true);
			if (ImGui::IsItemClicked())
			{
				if (mySelectedAnimationPath != file)
				{
					mySelectedAnimationPath = file;
				}
				else
				{
					SetAnimation(file);
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

void SkeletonEditor::CreateFileButton(const std::string& aFile, float anIconSize, bool anIsAnimation)
{
	bool isSelected = false;

	if (anIsAnimation)
	{
		isSelected = mySelectedAnimationPath == aFile;
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
}

void SkeletonEditor::SetAnimation(const std::string& aPath)
{
	if (Crimson::GetFileExtension(aPath) == AssetManager::GetBlendSpaceExtension())
	{
		SetAnimation(std::make_shared<BlendSpace>(AssetManager::GetAsset<BlendSpace>(aPath)));
	}
	else
	{
		if (mySelectedBone != nullptr)
		{
			SetAnimation(std::make_shared<AnimationLayer>(AssetManager::GetAsset<Animation>(aPath), GetBoneIndex(mySelectedBone)));
		}
		else
		{
			SetAnimation(std::make_shared<Animation>(AssetManager::GetAsset<Animation>(aPath)));
		}
	}
}

void SkeletonEditor::AddAnimation(const std::string& aPath)
{
	if (Crimson::GetFileExtension(aPath) == AssetManager::GetBlendSpaceExtension())
	{
		AddAnimation(std::make_shared<BlendSpace>(AssetManager::GetAsset<BlendSpace>(aPath)));
	}
	else
	{
		if (mySelectedBone != nullptr)
		{
			AddAnimation(std::make_shared<AnimationLayer>(AssetManager::GetAsset<Animation>(aPath), GetBoneIndex(mySelectedBone)));
		}
		else
		{
			AddAnimation(std::make_shared<Animation>(AssetManager::GetAsset<Animation>(aPath)));
		}
	}
}

void SkeletonEditor::UpdateAvailableFiles()
{
	myAvailableSkeletons.clear();
	myAvailableAnimations.clear();

	AssetManager::SetLogErrors(false);

	{ // Sort out skeletons
		auto files = Crimson::GetAllFilepathsInDirectory(AssetManager::GetModelPath(), true);
		for (auto& file : files)
		{
			auto potentialTypes = Assets::GetPossibleTypes(Crimson::GetFileExtension(file));
			for (auto& type : potentialTypes)
			{
				if (type == Assets::eAssetType::AnimatedModel || type == Assets::eAssetType::Model || type == Assets::eAssetType::Animation)
				{
					if (AssetManager::GetAsset<GameObject>(file).HasComponent<AnimatedMeshComponent>())
					{
						myAvailableSkeletons.emplace(file);
					}
					break;
				}
			}
		}
	}

	{ // Sort out animations
		auto& files = AssetManager::GetAvailableAnimations();
		for (auto& file : files)
		{
			auto potentialTypes = Assets::GetPossibleTypes(Crimson::GetFileExtension(file));
			if (potentialTypes.size() == 1 && potentialTypes.back() == Assets::eAssetType::Animation)
			{
				myAvailableAnimations.emplace(file);
				continue;
			}
			for (auto& type : potentialTypes)
			{
				if (type == Assets::eAssetType::AnimatedModel || type == Assets::eAssetType::Model || type == Assets::eAssetType::Animation)
				{
					if (AssetManager::GetAsset<Animation>(file).HasData())
					{
						myAvailableAnimations.emplace(file);
					}
					break;
				}
			}
		}
	}
	AssetManager::SetLogErrors(true);
}

void SkeletonEditor::GenerateSkeletonDrawing()
{
	if (!myShouldRenderSkeleton)
	{
		return;
	}

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
	if (!myShouldRenderSkeleton)
	{
		return;
	}

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
	if (!myHasMatchingBones || !myShouldRenderSkeleton)
	{
		return;
	}

	if (mySelectedBone && mySelectedBone != myRootBone)
	{
		auto& parentBone = mySkeleton->GetBone(mySelectedBone->parent);
		const Crimson::Vector4f& center = Crimson::Vector4f::NullPosition * parentBone.bindPoseInverse.GetInverse();

		DrawFrame(GetBoneIndex(mySelectedBone), center);
	}
	else
	{
		const Crimson::Vector4f& center = Crimson::Vector4f::NullPosition * myBoneTransforms->at(0);
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
			DrawFrame(childIndex, center);
		}
	}
}

void SkeletonEditor::DrawFrame(unsigned anIndex, const Crimson::Vector4f& aParentPosition)
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
		const auto& position = Crimson::Vector4f::NullPosition * (bone.bindPoseInverse.GetInverse() * myBoneTransforms->at(anIndex));
		GraphicsEngine::Get().GetLineDrawer().AddLine(aParentPosition, position, *color, mySkeletonOffset->GetTransformMatrix(), false, &myLines.at(&bone));

		for (auto& childIndex : bone.children)
		{
			DrawFrame(childIndex, position);
		}
	}
}

void SkeletonEditor::CheckSkeletonAnimationMatching(const std::shared_ptr<AnimationBase>& anAnimation)
{
	myMissMatchMessage = "";
	myAnimationTimer = 0.f;

	if (!mySkeleton)
	{
		myHasMatchingBones = false;
		myMissMatchMessage = "No Skeleton selected";
		return;
	}

	myHasMatchingBones = anAnimation->IsValidSkeleton(mySkeleton, &myMissMatchMessage);

	if (mySkeleton->GetBoneCount() > MAX_BONE_COUNT)
	{
		myHasMatchingBones = false;
		myMissMatchMessage = "Animation has too many bones! Max amount is " + std::to_string(MAX_BONE_COUNT);
	}

	if (!myHasMatchingBones)
	{
		myPlayCount = 0;
		if (myMesh->HasAnimation())
		{
			myMesh->StopAnimation();
		}
	}
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
}

void SkeletonEditor::SetBone(unsigned anIndex, std::shared_ptr<AnimationBase>& outAnimation)
{
	if (auto animationPtr = std::dynamic_pointer_cast<Animation>(outAnimation))
	{
		if (anIndex > 0u)
		{
			outAnimation = std::make_shared<AnimationLayer>(*animationPtr, anIndex);
		}
		else
		{
			if (auto layerPtr = std::dynamic_pointer_cast<AnimationLayer>(outAnimation))
			{
				outAnimation = std::make_shared<Animation>(*layerPtr);
			}
			else
			{
				outAnimation = std::make_shared<Animation>(*animationPtr);
			}
		}
	}
	else
	{
		auto blendPtr = std::dynamic_pointer_cast<BlendSpace>(outAnimation);
		blendPtr->SetBoneIndex(anIndex);
	}
}

void SkeletonEditor::LoadBlendSpace()
{
	std::string path;
	if (Crimson::ShowOpenFileSelector(path, { L"Blend Spaces",  L"*" + Crimson::ToWString(AssetManager::GetBlendSpaceExtension()) + L";" }, Crimson::ToWString(Crimson::GetAbsolutePath(AssetManager::GetAnimationPath()))))
	{
		SetAnimation(std::make_shared<BlendSpace>(AssetManager::GetAsset<BlendSpace>(path)));
	}
}

void SkeletonEditor::SaveBlendSpace()
{
	if (myMesh->myAnimation && myMesh->myAnimation->GetType() == AnimationBase::AnimationType::BlendSpace)
	{
		std::shared_ptr<BlendSpace> blendspace = std::dynamic_pointer_cast<BlendSpace>(myMesh->myAnimation);
		std::wstring extension = Crimson::ToWString(AssetManager::GetBlendSpaceExtension());
		std::wstring filename = Crimson::ToWString(Crimson::AddExtensionIfMissing(blendspace->GetName(), AssetManager::GetBlendSpaceExtension(), true));
		std::string path;
		if (Crimson::ShowSaveFileSelector(path, filename, extension.substr(1), { L"Blend Space", L"*" + extension + L";" }, Crimson::ToWString(Crimson::GetAbsolutePath(AssetManager::GetAnimationPath()))))
		{
			blendspace->myPath = path;
			AssetManager::SaveAsset(*blendspace, path);
		}
	}
}
