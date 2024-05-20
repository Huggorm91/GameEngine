#include "AssetManager.pch.h"
#include "NetworkComponent.h"
#include "Modelviewer/Core/Modelviewer.h"
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
		ModelViewer::Get().GetMessageHandler().
	}
	myRaisedFlags.reset();
}

void NetworkComponent::TransformHasChanged() const
{
	if (mySyncFlags[eSyncTransform])
	{
		myRaisedFlags[eTransformHasChanged];
	}
}

Json::Value NetworkComponent::ToJson() const
{
	Json::Value result = Component::ToJson();
	result["SyncFlags"] = mySyncFlags.to_ullong();
	return result;
}

inline std::string NetworkComponent::ToString() const
{
	return "NetworkComponent";
}

inline const NetworkComponent* NetworkComponent::GetTypePointer() const
{
	return this;
}
