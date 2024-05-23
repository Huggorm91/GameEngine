#include "AssetManager.pch.h"
#include "NetworkComponent.h"
#include "Editor/Modelviewer.h"
#include "NetworkClient/MessageHandler.h"

NetworkComponent::NetworkComponent() : Component(ComponentType::Network)
{
}

NetworkComponent::NetworkComponent(const Json::Value& aJson) : Component(aJson), mySyncFlags(aJson["SyncFlags"].asLargestUInt())
{
}

void NetworkComponent::Update()
{
	if (myRaisedFlags[eTransformHasChanged])
	{
		ModelViewer::Get().GetMessageHandler().SendTransformChanged(*GetParentTransform(), myParent->GetID());
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

Json::Value NetworkComponent::ToJson() const
{
	Json::Value result = Component::ToJson();
	result["SyncFlags"] = mySyncFlags.to_ullong();
	return result;
}
