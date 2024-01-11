#include "AssetManager.pch.h"
#include "ComponentType.h"
#include "Json/JsonVector.hpp"
#include "../ComponentInclude.h"
#include "AssetManager/AssetManager.h"

void AddComponent(const Component* aComponent, GameObject& aParent)
{
	if (aComponent == nullptr)
	{
		AMLogger.Err("AddComponent: aComponent was nullptr! GameObject ID : " + aParent.GetIDString());
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
	case ComponentType::ParticleEmitter:
	{
		const ParticleEmitterComponent& component = *dynamic_cast<const ParticleEmitterComponent*>(aComponent);
		aParent.AddComponent(component);
		break;
	}
	default:
	{
		AMLogger.Err("AddComponent: Invalid component type! GameObject ID : " + aParent.GetIDString());
	}
	}
}

void AddComponent(const ComponentType aType, GameObject& aParent)
{
	switch (aType)
	{
	case ComponentType::Mesh:
	{
		aParent.AddComponent<MeshComponent>();
		break;
	}
	case ComponentType::AnimatedMesh:
	{
		aParent.AddComponent<AnimatedMeshComponent>();
		break;
	}
	case ComponentType::Directionallight:
	{
		aParent.AddComponent<DirectionallightComponent>();
		break;
	}
	case ComponentType::Pointlight:
	{
		aParent.AddComponent<PointlightComponent>();
		break;
	}
	case ComponentType::Spotlight:
	{
		aParent.AddComponent<SpotlightComponent>();
		break;
	}
	case ComponentType::DebugDraw:
	{
		aParent.AddComponent<DebugDrawComponent>();
		break;
	}
	case ComponentType::PerspectiveCamera:
	{
		aParent.AddComponent<PerspectiveCameraComponent>();
		break;
	}
	case ComponentType::ParticleEmitter:
	{
		aParent.AddComponent<ParticleEmitterComponent>();
		break;
	}
	default:
	{
		AMLogger.Err("AddComponent: Invalid component type! GameObject ID : " + aParent.GetIDString());
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
		auto& mesh = aParent.AddComponent(AssetManager::GetAsset<MeshComponent>(aJson["Path"].asString()));
		mesh.Init(aJson);		
		break;
	}
	case ComponentType::AnimatedMesh:
	{
		auto& mesh = aParent.AddComponent(AssetManager::GetAsset<AnimatedMeshComponent>(aJson["Path"].asString()));
		mesh.Init(aJson);		
		break;
	}
	case ComponentType::Directionallight:
	{
		aParent.AddComponent(DirectionallightComponent(aJson));
		break;
	}
	case ComponentType::Pointlight:
	{
		aParent.AddComponent(PointlightComponent(aJson));
		break;
	}
	case ComponentType::Spotlight:
	{
		aParent.AddComponent(SpotlightComponent(aJson));
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
	case ComponentType::ParticleEmitter:
	{
		aParent.AddComponent(ParticleEmitterComponent(aJson));
		break;
	}
	default:
	{
		AMLogger.Err("LoadComponent: Invalid component type! GameObject ID : " + aParent.GetIDString());
	}
	}
}

void LoadComponent(std::istream& aStream, GameObject& aParent)
{
	ComponentType type;
	aStream.read(reinterpret_cast<char*>(&type), sizeof(type));

	switch (type)
	{
	case ComponentType::Mesh:
	{
		auto& mesh = aParent.AddComponent<MeshComponent>();
		mesh.Deserialize(aStream);
		break;
	}
	case ComponentType::AnimatedMesh:
	{
		auto& mesh = aParent.AddComponent<AnimatedMeshComponent>();
		mesh.Deserialize(aStream);
		break;
	}
	case ComponentType::Directionallight:
	{
		auto& light = aParent.AddComponent<DirectionallightComponent>();
		light.Deserialize(aStream);
		break;
	}
	case ComponentType::Pointlight:
	{
		auto& light = aParent.AddComponent<PointlightComponent>();
		light.Deserialize(aStream);
		break;
	}
	case ComponentType::Spotlight:
	{
		auto& light = aParent.AddComponent<SpotlightComponent>();
		light.Deserialize(aStream);
		break;
	}
	case ComponentType::DebugDraw:
	{
		auto& debug = aParent.AddComponent<DebugDrawComponent>();
		debug.Deserialize(aStream);
		break;
	}
	case ComponentType::PerspectiveCamera:
	{
		auto& camera = aParent.AddComponent<PerspectiveCameraComponent>();
		camera.Deserialize(aStream);
		break;
	}
	case ComponentType::ParticleEmitter:
	{
		auto& emitter = aParent.AddComponent<ParticleEmitterComponent>();
		emitter.Deserialize(aStream);
		break;
	}
	case ComponentType::Count:
		break;
	default:
		AMLogger.Err("BinaryLoadComponent: Invalid component type! GameObject ID : " + aParent.GetIDString());
		break;
	}
}

std::string ComponentTypeToString(const ComponentType aType)
{
	switch (aType)
	{
	case ComponentType::Mesh:
	{
		return "Mesh";
	}
	case ComponentType::AnimatedMesh:
	{
		return "AnimatedMesh";
	}
	case ComponentType::Directionallight:
	{
		return "Directionallight";
	}
	case ComponentType::Pointlight:
	{
		return "Pointlight";
	}
	case ComponentType::Spotlight:
	{
		return "Spotlight";
	}
	case ComponentType::DebugDraw:
	{
		return "DebugDraw";
	}
	case ComponentType::PerspectiveCamera:
	{
		return "PerspectiveCamera";
	}
	case ComponentType::ParticleEmitter:
	{
		return "ParticleEmitter";
	}
	default:
	{
		AMLogger.Err("ComponentTypeToString: Invalid component type!");
		return "Invalid";
	}
	}
}
