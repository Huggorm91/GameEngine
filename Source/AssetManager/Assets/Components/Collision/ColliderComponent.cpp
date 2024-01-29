#include "AssetManager.pch.h"
#include "ColliderComponent.h"
#include "Engine\Engine.h"
#include "Managers\CollisionManager.h"
#include "ImGui\imgui.h"

ColliderComponent::ColliderComponent(ComponentType aType):
	Component(aType),
	myIsTrigger(false),
	myIsColliding(false),
	myHasChanged(false),
	myLayer(eCollisionLayer_Default)
{
}

ColliderComponent::ColliderComponent(const Json::Value& aJson) :
	Component(aJson),
	myIsTrigger(aJson["IsTrigger"].asBool()),
	myIsColliding(false),
	myHasChanged(true),
	myLayersToCollideWith(aJson["CollidingLayers"].asUInt64()),
	myLayer(static_cast<eCollisionLayer>(aJson["Layer"].asUInt()))
{
}

ColliderComponent::~ColliderComponent()
{
	Engine::GetCollisionManager().RemoveCollider(this);
}

void ColliderComponent::Update()
{
	if (myHasChanged)
	{
		UpdateWorldPosition();
	}
	if (IsValid())
	{
		Engine::GetCollisionManager().AddCollider(this);
	}
}

Json::Value ColliderComponent::ToJson() const
{
	auto result = Component::ToJson();
	result["IsTrigger"] = myIsTrigger;
	result["CollidingLayers"] = static_cast<unsigned>(myLayersToCollideWith.to_ulong());
	result["Layer"] = myLayer;
	return result;
}

inline std::string ColliderComponent::ToString() const
{
	return "ColliderComponent";
}

inline const ColliderComponent* ColliderComponent::GetTypePointer() const
{
    return this;
}

eCollisionLayer ColliderComponent::GetLayer() const
{
	return myLayer;
}

void ColliderComponent::SetLayer(eCollisionLayer aLayer)
{
	myLayer = aLayer;
}

void ColliderComponent::SetCollidingLayer(eCollisionLayer aLayer, bool aShouldCollideWith)
{
	myLayersToCollideWith[aLayer] = aShouldCollideWith;
}

void ColliderComponent::SetCollidingLayers(std::bitset<eCollisionLayer_Count> aLayerList)
{
	myLayersToCollideWith = aLayerList;
}

bool ColliderComponent::CollidesWithLayer(eCollisionLayer aLayer) const
{
	return myLayersToCollideWith[aLayer];
}

void ColliderComponent::SetIsTrigger(bool aState)
{
	myIsTrigger = aState;
}

bool ColliderComponent::IsTrigger() const
{
	return myIsTrigger;
}

bool ColliderComponent::IsColliding() const
{
	return myIsColliding;
}

void ColliderComponent::TransformHasChanged() const
{
	const_cast<ColliderComponent&>(*this).UpdateWorldPosition();
}

void ColliderComponent::CreateImGuiComponents(const std::string& aWindowName)
{
	Component::CreateImGuiComponents(aWindowName);

	ImGui::Checkbox("Trigger", &myIsTrigger);

	if (ImGui::BeginCombo("Layer", globalLayerNames[myLayer].c_str(), ImGuiComboFlags_HeightLarge))
	{
		for (unsigned index = 0; index < globalLayerNames.size(); index++)
		{
			const bool isSelected = index == myLayer;
			if (ImGui::Selectable(globalLayerNames[index].c_str(), isSelected))
			{
				myLayer = static_cast<eCollisionLayer>(index);
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Layers To Collide With", nullptr, ImGuiComboFlags_HeightLarge))
	{
		for (unsigned index = 0; index < globalLayerNames.size(); index++)
		{
			const bool isSelected = myLayersToCollideWith[index];
			if (ImGui::Selectable(globalLayerNames[index].c_str(), isSelected))
			{
				myLayersToCollideWith[index] = !myLayersToCollideWith[index];
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

void ColliderComponent::Serialize(std::ostream& aStream) const
{
	Component::Serialize(aStream);
	unsigned long layers = myLayersToCollideWith.to_ulong();
	aStream.write(reinterpret_cast<const char*>(&myIsTrigger), sizeof(myIsTrigger));
	aStream.write(reinterpret_cast<const char*>(&layers), sizeof(layers));
	aStream.write(reinterpret_cast<const char*>(&myLayer), sizeof(myLayer));
}

void ColliderComponent::Deserialize(std::istream& aStream)
{
	Component::Deserialize(aStream);
	unsigned long layers = 0u;
	aStream.read(reinterpret_cast<char*>(&myIsTrigger), sizeof(myIsTrigger));
	aStream.read(reinterpret_cast<char*>(&layers), sizeof(layers));
	aStream.read(reinterpret_cast<char*>(&myLayer), sizeof(myLayer));
	myLayersToCollideWith = layers;
}
