#include "AssetManager.pch.h"
#include "ComponentType.h"
#include <JsonVector.hpp>
#include "../ComponentInclude.h"
#include "AssetManager/AssetManager.h"

void AddComponent(const Component* aComponent, GameObject& aParent)
{
	if (aComponent == nullptr)
	{
		AMLogger.Err("AddComponent: aComponent was nullptr! GameObject ID : " + std::to_string(aParent.GetID()));
		return;
	}

	switch (aComponent->GetType())
	{
	case ComponentType::Mesh:
	{
		const MeshComponent& component = *dynamic_cast<const MeshComponent*>(aComponent);
		aParent.AddComponent(component);
		break;
	}
	case ComponentType::AnimatedMesh:
	{
		const AnimatedMeshComponent& component = *dynamic_cast<const AnimatedMeshComponent*>(aComponent);
		aParent.AddComponent(component);
		break;
	}
	case ComponentType::Directionallight:
	{
		const DirectionallightComponent& component = *dynamic_cast<const DirectionallightComponent*>(aComponent);
		aParent.AddComponent(component);
		break;
	}
	case ComponentType::Pointlight:
	{
		const PointlightComponent& component = *dynamic_cast<const PointlightComponent*>(aComponent);
		aParent.AddComponent(component);
		break;
	}
	case ComponentType::Spotlight:
	{
		const SpotlightComponent& component = *dynamic_cast<const SpotlightComponent*>(aComponent);
		aParent.AddComponent(component);
		break;
	}
	case ComponentType::DebugDraw:
	{
		const DebugDrawComponent& component = *dynamic_cast<const DebugDrawComponent*>(aComponent);
		aParent.AddComponent(component);
		break;
	}
	case ComponentType::PerspectiveCamera:
	{
		const PerspectiveCameraComponent& component = *dynamic_cast<const PerspectiveCameraComponent*>(aComponent);
		aParent.AddComponent(component);
		break;
	}
	case ComponentType::Input:
	{
		const InputComponent& component = *dynamic_cast<const InputComponent*>(aComponent);
		aParent.AddComponent(component);
		break;
	}
	case ComponentType::Movement:
	{
		const MovementComponent& component = *dynamic_cast<const MovementComponent*>(aComponent);
		aParent.AddComponent(component);
		break;
	}
	default:
	{
		AMLogger.Err("AddComponent: Invalid component type! GameObject ID : " + std::to_string(aParent.GetID()));
	}
	}
}

void LoadComponent(const Json::Value& aJson, GameObject& aParent)
{
    ComponentType type = static_cast<ComponentType>(aJson["Type"].asInt());
	switch (type)
	{		
	case ComponentType::Mesh:
	{

		MeshComponent mesh = AssetManager::GetAsset<MeshComponent>(aJson["Path"].asString());
		mesh.Init(aJson);
		aParent.AddComponent(mesh);
		break;
	}
	case ComponentType::AnimatedMesh:
	{
		auto mesh = AssetManager::GetAsset<AnimatedMeshComponent>(aJson["Path"].asString());
		mesh.Init(aJson);
		aParent.AddComponent(mesh);
		break;
	}
	case ComponentType::Directionallight:
	{
		break;
	}
	case ComponentType::Pointlight:
	{
		break;
	}
	case ComponentType::Spotlight:
	{
		break;
	}
	case ComponentType::DebugDraw:
	{
		break;
	}
	case ComponentType::PerspectiveCamera:
	{
		break;
	}
	case ComponentType::Input:
	{
		break;
	}
	case ComponentType::Movement:
	{
		break;
	}
	default:
	{
		AMLogger.Err("LoadComponent: Invalid component type! GameObject ID : " + std::to_string(aParent.GetID()));
	}
	}
}

std::string ComponentTypeToString(const ComponentType aType)
{
	switch (aType)
	{
	case ComponentType::Unknown:
		return "Unknown";
	case ComponentType::Mesh:
		return "Mesh";
	case ComponentType::AnimatedMesh:
		return "AnimatedMesh";
	case ComponentType::Directionallight:
		return "Directionallight";
	case ComponentType::Pointlight:
		return "Pointlight";
	case ComponentType::Spotlight:
		return "Spotlight";
	case ComponentType::DebugDraw:
		return "DebugDraw";
	case ComponentType::PerspectiveCamera:
		return "PerspectiveCamera";
	case ComponentType::Input:
		return "Input";
	case ComponentType::Movement:
		return "Movement";
	default:
		return "Invalid";
	}
}

std::string ComponentTypeToString(const Json::Value& aJson)
{
	return ComponentTypeToString(static_cast<ComponentType>(aJson["Type"].asInt()));
}