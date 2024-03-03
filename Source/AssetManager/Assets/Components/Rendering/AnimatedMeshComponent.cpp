#include "AssetManager.pch.h"
#include "AnimatedMeshComponent.h"
#include "AssetManager/AssetManager.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/GfxCmd_RenderMesh.h"
#include "GraphicsEngine/Commands/GfxCmd_RenderMeshShadow.h"
#include "Time/Timer.h"

AnimatedMeshComponent::AnimatedMeshComponent() :
	MeshComponent(ComponentType::AnimatedMesh),
	mySkeleton(nullptr),
	myAnimationState(AnimationState::Stopped)
{}

AnimatedMeshComponent::AnimatedMeshComponent(const TGA::FBX::Mesh& aMesh, std::vector<MeshElement>& anElementList, Skeleton* aSkeleton) :
	MeshComponent(aMesh, anElementList, ComponentType::AnimatedMesh),
	mySkeleton(aSkeleton),
	myAnimationState(AnimationState::Stopped)
{}

AnimatedMeshComponent::AnimatedMeshComponent(const AnimatedMeshComponent& aComponent) :
	MeshComponent(aComponent),
	myBoneTransformCache(aComponent.myBoneTransformCache),
	mySkeleton(aComponent.mySkeleton),
	myAnimation(aComponent.myAnimation ? aComponent.myAnimation->GetAsSharedPtr(): nullptr),
	myAnimationState(aComponent.myAnimationState)
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
	myAnimationState = aComponent.myAnimationState;
	return *this;
}

void AnimatedMeshComponent::Update()
{
	if (!myIsActive)
	{
		return;
	}


	if (myAnimationState != AnimationState::Stopped)
	{
		const bool isPlaying = myAnimation->Update();
		if (!isPlaying && myAnimationState == AnimationState::PlayOnce)
		{
			myAnimationState = AnimationState::Stopped;
		}
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

void AnimatedMeshComponent::Init(const Json::Value& aJson)
{
	MeshComponent::Init(aJson);
	myAnimation = std::make_shared<Animation>(AssetManager::GetAsset<Animation>(aJson["Animation"].asString()));
	myAnimation->Init(myBoneTransformCache, mySkeleton);
	//myAnimation->LoadFromJson(aJson["AnimationData"]);

	myAnimationState = static_cast<AnimationState>(aJson["AnimationState"].asInt());
	if (!mySkeleton || mySkeleton->GetPath() != aJson["Skeleton"].asString())
	{
		mySkeleton = AssetManager::GetAsset<Skeleton*>(aJson["Skeleton"].asString());
	}

	myAnimation->UpdateBoneCache(mySkeleton, myBoneTransformCache);
}

void AnimatedMeshComponent::Init(std::vector<MeshElement>& anElementList, const std::string& aName, Skeleton* aSkeleton)
{
	MeshComponent::Init(anElementList, aName);
	mySkeleton = aSkeleton;
}

void AnimatedMeshComponent::SetLooping(bool aIsLooping)
{
	myAnimation->SetIsLooping(aIsLooping);
	if (myAnimationState != AnimationState::Stopped)
	{
		StartAnimation();
	}
}

void AnimatedMeshComponent::ToogleLooping()
{
	myAnimation->ToogleLooping();
	if (myAnimationState != AnimationState::Stopped)
	{
		StartAnimation();
	}
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
	if (myAnimation->IsLooping())
	{
		myAnimationState = AnimationState::Looping;
	}
	else
	{
		myAnimationState = AnimationState::PlayOnce;
	}
}

void AnimatedMeshComponent::StopAnimation()
{
	myAnimationState = AnimationState::Stopped;
	myAnimation->StopAnimation();
	myAnimation->SetToFirstFrame();
	myAnimation->UpdateBoneCache(mySkeleton, myBoneTransformCache);
}

void AnimatedMeshComponent::PauseAnimation()
{
	myAnimationState = AnimationState::Stopped;
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
	ImGui::InputText("Animation", const_cast<std::string*>(&myAnimation->GetName()), ImGuiInputTextFlags_ReadOnly);
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
	aStream.write(reinterpret_cast<const char*>(&myAnimationState), sizeof(myAnimationState));
}

void AnimatedMeshComponent::Deserialize(std::istream& aStream)
{
	MeshComponent::Deserialize(aStream);
	std::string skeletonPath;
	std::getline(aStream, skeletonPath, '\0');
	//std::string animationPath;
	//std::getline(aStream, animationPath, '\0');
	aStream.read(reinterpret_cast<char*>(&myAnimationState), sizeof(myAnimationState));

	mySkeleton = AssetManager::GetAsset<Skeleton*>(skeletonPath);
	//myAnimation = AssetManager::GetAsset<Animation>(animationPath);
	//UpdateCache();
}

Json::Value AnimatedMeshComponent::ToJson() const
{
	Json::Value result = MeshComponent::ToJson();
	result["Animation"] = myAnimation->GetName();
	result["AnimationData"] = myAnimation->ToJson();
	result["AnimationState"] = static_cast<int>(myAnimationState);
	result["Skeleton"] = mySkeleton->GetPath();
	return result;
}

inline std::string AnimatedMeshComponent::ToString() const
{
	return "AnimatedMesh";
}

const AnimatedMeshComponent* AnimatedMeshComponent::GetTypePointer() const
{
	return this;
}
