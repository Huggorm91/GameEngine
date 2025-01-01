#include "AssetManager.pch.h"
#include "AssignmentComponent.h"
#include "GameplayEngine/PostMaster/PostMaster.h"
#include "AssetManager/Assets/GameObject.h"
#include "AssetManager/Assets/Components/Collision/ColliderComponent.h"
#ifdef NETWORK_SERVER
#include "GameServer/GameServer.h"
#include "GameplayEngine/Container/Grid.h"
#endif // NETWORK_SERVER

AssignmentComponent::AssignmentComponent() : Component(ComponentType::Assignment), myLifeTime(0.f)
{
}

AssignmentComponent::AssignmentComponent(const Crimson::Vector3f& aDirection) : Component(ComponentType::Assignment), myLifeTime(0.f), myDirection(aDirection.GetNormalizedNoAssert())
{
}

AssignmentComponent::AssignmentComponent(const Json::Value& aJson) : Component(aJson), myLifeTime(0.f)
{
}

void AssignmentComponent::Update()
{
	float deltaTime = Crimson::Time::GetDeltaTime();
	myLifeTime += deltaTime;
	if (myLifeTime >= 10.f)
	{
		Engine::GetPostMaster().SendInstantMessage({ Crimson::eMessageType::GameObject_Died, GetParentID() });
		return;
	}

	const auto& newPos = myParent->GetWorldPosition() + myDirection * (100.f * deltaTime);
	myParent->SetPosition(newPos);

#ifdef NETWORK_SERVER
	auto& position = myParent->GetWorldPosition();
	for (auto& [id, object] : Engine::GetNetworkManager().GetClientObjects())
	{
		float syncFrequency = 1.f;

		int distance = Engine::GetGrid().GetTileDistance(object.GetWorldPosition(), position);
		if (distance == 0)
		{
			constexpr float frequency = 1.f / 60.f;
			syncFrequency = frequency;
		}
		else if (distance == 1)
		{
			constexpr float frequency = 1.f / 10.f;
			syncFrequency = frequency;
		}
		else if (distance == 2)
		{
			constexpr float frequency = 1.f / 2.f;
			syncFrequency = frequency;
		}

		if (auto iter = mySyncTimers.find(id); iter != mySyncTimers.end())
		{
			float& timer = iter->second;
			timer += Crimson::Time::GetDeltaTime();
			if (timer >= syncFrequency)
			{
				Engine::GetNetworkManager().SendTransformChanged(*GetParentTransform(), syncFrequency, myParent->GetUUID(), id);
				timer = 0.f;
			}
		}
		else
		{
			mySyncTimers.emplace(id, 0.f);
		}
	}
#endif // NETWORK_SERVER
}

void AssignmentComponent::OnCollisionEnter(CollisionLayer::Layer aLayer, ColliderComponent* collider)
{
	if (aLayer == CollisionLayer::Layer::NetworkCulling)
	{
		Engine::GetPostMaster().SendInstantMessage({ Crimson::eMessageType::GameObject_Enable, std::pair(GetParentID(), collider->GetParentID()) });
	}
	else
	{
		Engine::GetPostMaster().SendInstantMessage({ Crimson::eMessageType::GameObject_Died, GetParentID() });
	}
}

void AssignmentComponent::OnCollisionExit(CollisionLayer::Layer aLayer, ColliderComponent* collider)
{
	if (aLayer == CollisionLayer::Layer::NetworkCulling)
	{
		Engine::GetPostMaster().SendInstantMessage({ Crimson::eMessageType::GameObject_Disable, std::pair(GetParentID(), collider->GetParentID()) });
	}
}
