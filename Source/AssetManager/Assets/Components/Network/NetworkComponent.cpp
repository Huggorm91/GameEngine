#include "AssetManager.pch.h"
#include "NetworkComponent.h"
#include "../../GameObject.h"
#ifndef NETWORK_SERVER
#include "GameplayEngine/Network/NetworkManager.h"
#else
#include "GameServer/GameServer.h"
#endif

NetworkComponent::NetworkComponent() : 
	Component(ComponentType::Network), 
	mySyncFrequency(0.f), 
	mySyncTimer(0.f)
{
}

NetworkComponent::NetworkComponent(const Json::Value& aJson) : 
	Component(aJson), 
	mySyncFrequency(aJson["SyncFrequency"].asFloat()), 
	mySyncTimer(0.f), 
	mySyncFlags(aJson["SyncFlags"].asLargestUInt())
{
}

void NetworkComponent::Update()
{
	if (!myIsActive)
	{
		return;
	}

	mySyncTimer += Crimson::Time::GetDeltaTime();
	if (mySyncTimer < mySyncFrequency)
	{
		return;
	}
	mySyncTimer = 0.f;

	if (myRaisedFlags[eTransformHasChanged])
	{
		Engine::GetNetworkManager().SendTransformChanged(*GetParentTransform(), myParent->GetUUID());
	}
	myRaisedFlags.reset();
}

void NetworkComponent::TransformHasChanged() const
{
	if (mySyncFlags[eSyncTransform])
	{
		const_cast<std::bitset<eStateCount>&>(myRaisedFlags)[eTransformHasChanged] = true;
	}
}

void NetworkComponent::SetSyncFrequency(float aTimeBetweenSyncs)
{
	mySyncFrequency = aTimeBetweenSyncs;
	mySyncTimer = 0.f;
}

void NetworkComponent::SyncTransform(bool aState)
{
	mySyncFlags[eSyncTransform] = aState;
	if (aState == false)
	{
		myRaisedFlags[eTransformHasChanged] = false;
	}
}

Json::Value NetworkComponent::ToJson() const
{
	Json::Value result = Component::ToJson();
	result["SyncFrequency"] = mySyncFrequency;
	result["SyncFlags"] = mySyncFlags.to_ullong();
	return result;
}
