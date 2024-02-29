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
	myAnimationTimer(0.f),
	myInterpolationTimer(0.f),
	myTargetFrameDelta(0.f),
	myAnimationState(AnimationState::Stopped),
	myIsLooping(false),
	myIsPlayingInReverse(false)
{}

AnimatedMeshComponent::AnimatedMeshComponent(const TGA::FBX::Mesh& aMesh, std::vector<MeshElement>& anElementList, Skeleton* aSkeleton) :
	MeshComponent(aMesh, anElementList, ComponentType::AnimatedMesh),
	mySkeleton(aSkeleton),
	myAnimationTimer(0.f),
	myInterpolationTimer(0.f),
	myTargetFrameDelta(0.f),
	myAnimationState(AnimationState::Stopped),
	myIsLooping(false),
	myIsPlayingInReverse(false)
{}

AnimatedMeshComponent::AnimatedMeshComponent(const AnimatedMeshComponent& aComponent) :
	MeshComponent(aComponent),
	myBoneTransformCache(aComponent.myBoneTransformCache),
	mySkeleton(aComponent.mySkeleton),
	myAnimation(aComponent.myAnimation),
	myAnimationTimer(aComponent.myAnimationTimer),
	myInterpolationTimer(aComponent.myInterpolationTimer),
	myTargetFrameDelta(aComponent.myTargetFrameDelta),
	myAnimationState(aComponent.myAnimationState),
	myIsLooping(aComponent.myIsLooping),
	myIsPlayingInReverse(aComponent.myIsPlayingInReverse)
{}

void AnimatedMeshComponent::Update()
{
	if (!myIsActive)
	{
		return;
	}

	if (myAnimationState != AnimationState::Stopped)
	{
		myAnimationTimer += Crimson::Timer::GetDeltaTime();
		const float frameDelta = myAnimation->GetFrameDelta();

		if (myAnimationTimer >= frameDelta)
		{
			myInterpolationTimer = 0.f;
			while (myAnimationTimer >= frameDelta)
			{
				myAnimationTimer -= frameDelta;
				const bool hasMoreFrames = myIsPlayingInReverse ? myAnimation->PreviousFrame() : myAnimation->NextFrame();
				if (!hasMoreFrames && myAnimationState == AnimationState::PlayOnce)
				{
					myAnimationTimer = 0.f;
					PauseAnimation();
					break;
				}
			}
			myAnimation->UpdateBoneCache(mySkeleton, myBoneTransformCache);
		}
		else if (myTargetFrameDelta > 0.f)
		{
			myInterpolationTimer += Crimson::Timer::GetDeltaTime();
			if (myInterpolationTimer >= myTargetFrameDelta)
			{
				myInterpolationTimer -= myTargetFrameDelta;
				myAnimation->UpdateBoneCache(mySkeleton, myBoneTransformCache, myAnimationTimer / frameDelta, myIsPlayingInReverse);
			}
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

void AnimatedMeshComponent::Init(const Json::Value& aJson)
{
	MeshComponent::Init(aJson);
	myAnimation = std::make_shared<Animation>(AssetManager::GetAsset<Animation>(aJson["Animation"].asString()));
	myAnimationTimer = aJson["AnimationTimer"].asFloat();
	myIsLooping = aJson["IsLooping"].asBool();
	myIsPlayingInReverse = aJson["IsPlayingInReverse"].asBool();
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
	myIsLooping = aIsLooping;
	if (myAnimationState != AnimationState::Stopped)
	{
		StartAnimation();
	}
}

void AnimatedMeshComponent::ToogleLooping()
{
	myIsLooping = !myIsLooping;
	if (myAnimationState != AnimationState::Stopped)
	{
		StartAnimation();
	}
}

bool AnimatedMeshComponent::IsLooping() const
{
	return myIsLooping;
}

void AnimatedMeshComponent::SetPlayInReverse(bool aShouldPlayInReverse)
{
	myIsPlayingInReverse = aShouldPlayInReverse;
}

void AnimatedMeshComponent::SetAnimation(const std::shared_ptr<AnimationBase>& anAnimation)
{
	myAnimationTimer = 0.f;
	myAnimation = anAnimation;
	myAnimation->UpdateBoneCache(mySkeleton, myBoneTransformCache);
}

void AnimatedMeshComponent::SetTargetFPS(float aFPS)
{
	myTargetFrameDelta = 1.f / aFPS;
}

void AnimatedMeshComponent::StartAnimation()
{
	if (myIsLooping)
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
	myAnimationTimer = 0.f;
	myAnimation->SetToFirstFrame();
	myAnimation->UpdateBoneCache(mySkeleton, myBoneTransformCache);
}

void AnimatedMeshComponent::PauseAnimation()
{
	myAnimationState = AnimationState::Stopped;
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
	if (ImGui::Checkbox("Loop", &myIsLooping) && myAnimationState != AnimationState::Stopped)
	{
		StartAnimation();
	}
	ImGui::Checkbox("Reverse", &myIsPlayingInReverse);

	ImGui::InputFloat("Timer", &myAnimationTimer, 0.f, 0.f, "%.4f", ImGuiInputTextFlags_ReadOnly);

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

	size_t size = sizeof(myAnimationTimer) + sizeof(myAnimationState) + sizeof(myIsLooping) + sizeof(myIsPlayingInReverse);
	aStream.write(reinterpret_cast<const char*>(&myAnimationTimer), size);
}

void AnimatedMeshComponent::Deserialize(std::istream& aStream)
{
	MeshComponent::Deserialize(aStream);
	std::string skeletonPath;
	std::getline(aStream, skeletonPath, '\0');
	//std::string animationPath;
	//std::getline(aStream, animationPath, '\0');

	size_t size = sizeof(myAnimationTimer) + sizeof(myAnimationState) + sizeof(myIsLooping) + sizeof(myIsPlayingInReverse);
	aStream.read(reinterpret_cast<char*>(&myAnimationTimer), size);

	mySkeleton = AssetManager::GetAsset<Skeleton*>(skeletonPath);
	//myAnimation = AssetManager::GetAsset<Animation>(animationPath);
	//UpdateCache();
}

Json::Value AnimatedMeshComponent::ToJson() const
{
	Json::Value result = MeshComponent::ToJson();
	result["Animation"] = myAnimation->GetName();
	result["AnimationTimer"] = myAnimationTimer;
	result["IsLooping"] = myIsLooping;
	result["IsPlayingInReverse"] = myIsPlayingInReverse;
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
