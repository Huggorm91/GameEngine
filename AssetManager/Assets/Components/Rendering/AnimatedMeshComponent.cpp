#include "AssetManager.pch.h"
#include "AnimatedMeshComponent.h"
#include "AssetManager/AssetManager.h"
#include "GraphicsEngine/GraphicsEngine.h"
#include "GraphicsEngine/Commands/GfxCmd_RenderMesh.h"
#include "GraphicsEngine/Commands/GfxCmd_RenderMeshShadow.h"
#include <Timer.h>

AnimatedMeshComponent::AnimatedMeshComponent() : MeshComponent(ComponentType::AnimatedMesh), myBoneTransformCache(), mySkeleton(nullptr), myAnimation(), myAnimationTimer(), myCurrentFrame(1),
myAnimationState(AnimationState::Stopped)
{
}

AnimatedMeshComponent::AnimatedMeshComponent(const TGA::FBX::Mesh& aMesh, std::vector<MeshElement>& anElementList, const std::string* aPath, Skeleton* aSkeleton) : MeshComponent(aMesh, anElementList, aPath, ComponentType::AnimatedMesh), myBoneTransformCache(),
mySkeleton(aSkeleton), myAnimation(), myAnimationTimer(), myCurrentFrame(1), myAnimationState(AnimationState::Stopped)
{
}

AnimatedMeshComponent::AnimatedMeshComponent(const AnimatedMeshComponent& aComponent) : MeshComponent(aComponent), myBoneTransformCache(aComponent.myBoneTransformCache),
mySkeleton(aComponent.mySkeleton), myAnimation(aComponent.myAnimation), myAnimationTimer(aComponent.myAnimationTimer), myCurrentFrame(aComponent.myCurrentFrame),
myAnimationState(aComponent.myAnimationState)
{
}

void AnimatedMeshComponent::Update()
{
	if (!myIsActive)
	{
		return;
	}

	if (myAnimationState != AnimationState::Stopped)
	{
		myAnimationTimer += CommonUtilities::Timer::GetDeltaTime();
		const float frameDelta = myAnimation.GetFrameDelta();

		// Gives choppy animations at low FPS, skips frames
		/*while (myAnimationTimer >= frameDelta)
		{
			myAnimationTimer -= frameDelta;
			if (myAnimation.GetNextIndex(myCurrentFrame) && myAnimationState == AnimationState::PlayOnce)
			{
				StopAnimation();
				break;
			}
			else if(myAnimationTimer >= frameDelta)
			{
				UpdateCache();
			}
		}*/

		// Gives slow animations at low FPS, does not skip frames
		if (myAnimationTimer >= frameDelta)
		{
			if (myAnimation.GetNextIndex(myCurrentFrame) && myAnimationState == AnimationState::PlayOnce)
			{
				StopAnimation();
			}
			else
			{
				myAnimationTimer -= frameDelta;
				UpdateCache();
			}
		}
	}

	if (myRenderShadow)
	{
		GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_RenderMeshShadow>(*this));
	}
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_RenderMesh>(*this));
}

void AnimatedMeshComponent::Init(const Json::Value& aJson)
{
	MeshComponent::Init(aJson);
	myAnimation = AssetManager::GetAsset<Animation>(aJson["Animation"].asString());
	myAnimationTimer = aJson["AnimationTimer"].asFloat();
	myCurrentFrame = aJson["CurrentFrame"].asUInt();
	myAnimationState = static_cast<AnimationState>(aJson["AnimationState"].asInt());
	if (!mySkeleton || mySkeleton->GetPath() != aJson["Skeleton"].asString())
	{
		mySkeleton = AssetManager::GetAsset<Skeleton*>(aJson["Skeleton"].asString());
	}

	UpdateCache();
}

void AnimatedMeshComponent::Init(std::vector<MeshElement>& anElementList, const std::string& aName, const std::string* aPath, Skeleton* aSkeleton)
{
	MeshComponent::Init(anElementList, aName, aPath);
	mySkeleton = aSkeleton;
	ComponentPointersInvalidated();
}

void AnimatedMeshComponent::SetAnimation(const Animation& anAnimation)
{
	myAnimation = anAnimation;
	UpdateCache();
}

void AnimatedMeshComponent::StartAnimation(bool aIsLooping)
{
	if (aIsLooping)
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
	myCurrentFrame = 1;
	myAnimationTimer = 0.f;
	UpdateCache();
}

void AnimatedMeshComponent::PauseAnimation()
{
	myAnimationState = AnimationState::Stopped;
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

const std::array<CommonUtilities::Matrix4x4f, 128>& AnimatedMeshComponent::GetBoneTransforms() const
{
	return myBoneTransformCache;
}

Json::Value AnimatedMeshComponent::ToJson() const
{
	Json::Value result = MeshComponent::ToJson();
	result["Animation"] = myAnimation.GetName();
	result["AnimationTimer"] = myAnimationTimer;
	result["CurrentFrame"] = myCurrentFrame;
	result["AnimationState"] = static_cast<int>(myAnimationState);
	result["Skeleton"] = mySkeleton->GetPath();
	return result;
}

const AnimatedMeshComponent* AnimatedMeshComponent::GetTypePointer() const
{
	return this;
}

void AnimatedMeshComponent::UpdateCache()
{
	assert(mySkeleton != nullptr && "AnimatedMeshComponent is not Initialized!");
	assert(myAnimation.HasData() && "Animation has no data!");
	UpdateHeirarchy(0, CommonUtilities::Matrix4x4f::Null);
}

void AnimatedMeshComponent::UpdateHeirarchy(unsigned int anIndex, const CommonUtilities::Matrix4x4f& aParentMatrix)
{
	auto& bone = mySkeleton->GetBones()[anIndex];
	auto& frame = myAnimation.GetFrame(myCurrentFrame);
	CommonUtilities::Matrix4x4f matrix = GetLocalTransform(bone, frame) * aParentMatrix;
	myBoneTransformCache[anIndex] = bone.myBindPoseInverse * matrix;
	for (auto& childIndex : bone.myChildren)
	{
		UpdateHeirarchy(childIndex, matrix);
	}
}

const CommonUtilities::Matrix4x4f& AnimatedMeshComponent::GetLocalTransform(const Bone& aBone, const AnimationFrame& aFrame) const
{
	if (auto iter = aFrame.myLocalTransforms.find(aBone.myNamespaceName); iter != aFrame.myLocalTransforms.end())
	{
		return iter->second;
	}
	else
	{
		return aFrame.myLocalTransforms.at(aBone.myName);
	}
}
