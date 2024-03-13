#include "AssetManager.pch.h"
#include "AnimatedMeshComponent.h"
#include "AssetManager/AssetManager.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/GfxCmd_RenderMesh.h"
#include "GraphicsEngine/Commands/GfxCmd_RenderMeshShadow.h"
#include "Time/Timer.h"

AnimatedMeshComponent::AnimatedMeshComponent() :
	MeshComponent(ComponentType::AnimatedMesh),
	mySkeleton(nullptr)
{}

AnimatedMeshComponent::AnimatedMeshComponent(const Json::Value& aJson) :
	MeshComponent(aJson),
	myAnimation(aJson["Animation"].isNull() ? nullptr : LoadAnimationFromJson(aJson["Animation"].asString(), aJson["AnimationData"])),
	mySkeleton(aJson["Skeleton"].isNull() ? nullptr : AssetManager::GetAsset<Skeleton*>(aJson["Skeleton"].asString()))
{}

AnimatedMeshComponent::AnimatedMeshComponent(const TGA::FBX::Mesh& aMesh, std::vector<MeshElement>& anElementList, Skeleton* aSkeleton) :
	MeshComponent(aMesh, anElementList, ComponentType::AnimatedMesh),
	mySkeleton(aSkeleton)
{}

AnimatedMeshComponent::AnimatedMeshComponent(const AnimatedMeshComponent& aComponent) :
	MeshComponent(aComponent),
	myBoneTransformCache(aComponent.myBoneTransformCache),
	mySkeleton(aComponent.mySkeleton),
	myAnimation(aComponent.myAnimation ? aComponent.myAnimation->GetAsSharedPtr() : nullptr)
{}

AnimatedMeshComponent& AnimatedMeshComponent::operator=(const AnimatedMeshComponent& aComponent)
{
	MeshComponent::operator=(aComponent);
	myBoneTransformCache = aComponent.myBoneTransformCache;
	mySkeleton = aComponent.mySkeleton;
	if (aComponent.myAnimation)
	{
		myAnimation = aComponent.myAnimation->GetAsSharedPtr();
	}
	return *this;
}

void AnimatedMeshComponent::Update()
{
	if (!myIsActive || !myAnimation)
	{
		return;
	}

	if (myAnimation->IsPlaying())
	{
		myAnimation->Update();
	}

	Render();
}

void AnimatedMeshComponent::Render()
{
	if (!myIsActive)
	{
		return;
	}

	if (myRenderShadow)
	{
		GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_RenderMeshShadow>(*this));
	}
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_RenderMesh>(*this, myIsDeferred));
}

void AnimatedMeshComponent::Init(GameObject* aParent)
{
	MeshComponent::Init(aParent);
	if (myAnimation)
	{
		myAnimation->Init(myBoneTransformCache, mySkeleton);
	}
}

void AnimatedMeshComponent::Init(std::vector<MeshElement>& anElementList, const std::string& aName, Skeleton* aSkeleton)
{
	MeshComponent::Init(anElementList, aName);
	mySkeleton = aSkeleton;
}

void AnimatedMeshComponent::SetLooping(bool aIsLooping)
{
	myAnimation->SetIsLooping(aIsLooping);
}

void AnimatedMeshComponent::ToogleLooping()
{
	myAnimation->ToogleLooping();
}

bool AnimatedMeshComponent::IsLooping() const
{
	return myAnimation->IsLooping();
}

void AnimatedMeshComponent::SetPlayInReverse(bool aShouldPlayInReverse)
{
	myAnimation->SetIsPlayingInReverse(aShouldPlayInReverse);
}

void AnimatedMeshComponent::SetAnimation(const std::shared_ptr<AnimationBase>& anAnimation)
{
	myAnimation = anAnimation;
	myAnimation->Init(myBoneTransformCache, mySkeleton);
}

void AnimatedMeshComponent::SetTargetFPS(float aFPS)
{
	myAnimation->SetTargetFPS(aFPS);
}

void AnimatedMeshComponent::StartAnimation()
{
	myAnimation->StartAnimation();
}

void AnimatedMeshComponent::StopAnimation()
{
	myAnimation->StopAnimation();
	myAnimation->SetToFirstFrame();
	myAnimation->UpdateBoneCache(mySkeleton, myBoneTransformCache);
}

void AnimatedMeshComponent::PauseAnimation()
{
	myAnimation->StopAnimation();
}

void AnimatedMeshComponent::ResetBoneCache()
{
	myBoneTransformCache.fill(Crimson::Matrix4x4f::Identity);
}

bool AnimatedMeshComponent::HasSkeleton() const
{
	return mySkeleton != nullptr;
}

void AnimatedMeshComponent::SetSkeleton(Skeleton* aSkeleton)
{
	mySkeleton = aSkeleton;
}

const Skeleton& AnimatedMeshComponent::GetSkeleton() const
{
	return *mySkeleton;
}

const std::array<Crimson::Matrix4x4f, MAX_BONE_COUNT>& AnimatedMeshComponent::GetBoneTransforms() const
{
	return myBoneTransformCache;
}

void AnimatedMeshComponent::CreateImGuiComponents(const std::string& aWindowName)
{
	MeshComponent::CreateImGuiComponents(aWindowName);
	ImGui::InputText("Animation", const_cast<std::string*>(&myAnimation->GetPath()), ImGuiInputTextFlags_ReadOnly);
	/*if (ImGui::Checkbox("Loop", &myIsLooping) && myAnimationState != AnimationState::Stopped)
	{
		StartAnimation();
	}
	ImGui::Checkbox("Reverse", &myIsPlayingInReverse);

	ImGui::InputFloat("Timer", &myAnimationTimer, 0.f, 0.f, "%.4f", ImGuiInputTextFlags_ReadOnly);*/

	if (ImGui::Button("Start"))
	{
		StartAnimation();
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		StopAnimation();
	}
	ImGui::SameLine();
	if (ImGui::Button("Pause"))
	{
		PauseAnimation();
	}
}

void AnimatedMeshComponent::Serialize(std::ostream& aStream) const
{
	MeshComponent::Serialize(aStream);
	size_t skeletonSize = mySkeleton ? mySkeleton->GetPath().size() + 1 : 1;
	aStream.write(mySkeleton ? mySkeleton->GetPath().c_str() : "\0", skeletonSize);
	//size_t animationSize = myAnimation.IsValid() ? myAnimation.GetPath().size() + 1 : 1;
	//aStream.write(myAnimation.IsValid() ? myAnimation.GetPath().c_str() : "\0", animationSize);
}

void AnimatedMeshComponent::Deserialize(std::istream& aStream)
{
	MeshComponent::Deserialize(aStream);
	std::string skeletonPath;
	std::getline(aStream, skeletonPath, '\0');
	//std::string animationPath;
	//std::getline(aStream, animationPath, '\0');

	mySkeleton = AssetManager::GetAsset<Skeleton*>(skeletonPath);
	//myAnimation = AssetManager::GetAsset<Animation>(animationPath);
	//UpdateCache();
}

Json::Value AnimatedMeshComponent::ToJson() const
{
	Json::Value result = MeshComponent::ToJson();
	result["Animation"] = myAnimation->GetPath();
	result["AnimationData"] = myAnimation->ToJson();
	result["Skeleton"] = mySkeleton->GetPath();
	return result;
}