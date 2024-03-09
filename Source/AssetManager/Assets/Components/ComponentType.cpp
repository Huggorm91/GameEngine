#include "AssetManager.pch.h"
#include "ComponentType.h"
#include "Json/JsonVector.hpp"
#include "../ComponentInclude.h"
#include "AssetManager/AssetManager.h"

Component* AddComponent(const Component* aComponent, GameObject& aParent)
{
	if (aComponent == nullptr)
	{
		AMLogger.Err("AddComponent: aComponent was nullptr! GameObject ID : " + std::to_string(aParent.GetID()));
		return nullptr;
	}

	switch (aComponent->GetType())
	{
	case ComponentType::Mesh:
	{
		const MeshComponent& component = *dynamic_cast<const MeshComponent*>(aComponent);
		return &aParent.AddComponent(component);
	}
	case ComponentType::AnimatedMesh:
	{
		const AnimatedMeshComponent& component = *dynamic_cast<const AnimatedMeshComponent*>(aComponent);
		return &aParent.AddComponent(component);
	}
	case ComponentType::Directionallight:
	{
		const DirectionallightComponent& component = *dynamic_cast<const DirectionallightComponent*>(aComponent);
		return &aParent.AddComponent(component);
	}
	case ComponentType::Pointlight:
	{
		const PointlightComponent& component = *dynamic_cast<const PointlightComponent*>(aComponent);
		return &aParent.AddComponent(component);
	}
	case ComponentType::Spotlight:
	{
		const SpotlightComponent& component = *dynamic_cast<const SpotlightComponent*>(aComponent);
		return &aParent.AddComponent(component);
	}
	case ComponentType::DebugDraw:
	{
		const DebugDrawComponent& component = *dynamic_cast<const DebugDrawComponent*>(aComponent);
		return &aParent.AddComponent(component);
	}
	case ComponentType::PerspectiveCamera:
	{
		const PerspectiveCameraComponent& component = *dynamic_cast<const PerspectiveCameraComponent*>(aComponent);
		return &aParent.AddComponent(component);
	}
	case ComponentType::ParticleEmitter:
	{
		const ParticleEmitterComponent& component = *dynamic_cast<const ParticleEmitterComponent*>(aComponent);
		return &aParent.AddComponent(component);
	}
	case ComponentType::EditorCameraController:
	{
		const EditorCameraControllerComponent& component = *dynamic_cast<const EditorCameraControllerComponent*>(aComponent); 
		return &aParent.AddComponent(component);
	}
	case ComponentType::AnimationController:
	{
		const AnimationControllerComponent& component = *dynamic_cast<const AnimationControllerComponent*>(aComponent); 
		return &aParent.AddComponent(component);
	}
	default:
	{
		AMLogger.Err("AddComponent: Invalid component type! GameObject ID : " + std::to_string(aParent.GetID()));
	}
	}
	return nullptr;
}

Component* AddComponent(const ComponentType aType, GameObject& aParent)
{
	switch (aType)
	{
	case ComponentType::Mesh:
	{
		return &aParent.AddComponent<MeshComponent>();
	}
	case ComponentType::AnimatedMesh:
	{
		return &aParent.AddComponent<AnimatedMeshComponent>();
	}
	case ComponentType::Directionallight:
	{
		return &aParent.AddComponent<DirectionallightComponent>();
	}
	case ComponentType::Pointlight:
	{
		return &aParent.AddComponent<PointlightComponent>();
	}
	case ComponentType::Spotlight:
	{
		return &aParent.AddComponent<SpotlightComponent>();
	}
	case ComponentType::DebugDraw:
	{
		return &aParent.AddComponent<DebugDrawComponent>();
	}
	case ComponentType::PerspectiveCamera:
	{
		return &aParent.AddComponent<PerspectiveCameraComponent>();
	}
	case ComponentType::ParticleEmitter:
	{
		return &aParent.AddComponent<ParticleEmitterComponent>();
	}
	case ComponentType::EditorCameraController:
	{
		return &aParent.AddComponent<EditorCameraControllerComponent>();
	}
	case ComponentType::AnimationController:
	{
		return &aParent.AddComponent<AnimationControllerComponent>();
	}
	default:
	{
		AMLogger.Err("AddComponent: Invalid component type! GameObject ID : " + std::to_string(aParent.GetID()));
	}
	}
	return nullptr;
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
		aParent.AddComponent(PerspectiveCameraComponent(aJson));
		break;
	}
	case ComponentType::ParticleEmitter:
	{
		aParent.AddComponent(ParticleEmitterComponent(aJson));
		break;
	}
	case ComponentType::EditorCameraController:
	{
		break;
	}
	case ComponentType::AnimationController:
	{
		aParent.AddComponent(AnimationControllerComponent(aJson));
		break;
	}
	default:
	{
		AMLogger.Err("LoadComponent: Invalid component type! GameObject ID : " + std::to_string(aParent.GetID()));
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
	case ComponentType::EditorCameraController:
	{
		break;
	}
	case ComponentType::AnimationController:
	{
		auto& controller = aParent.AddComponent<AnimationControllerComponent>();
		controller.Deserialize(aStream);
		break;
	}
	case ComponentType::Count:
		break;
	default:
		AMLogger.Err("BinaryLoadComponent: Invalid component type! GameObject ID : " + std::to_string(aParent.GetID()));
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
	case ComponentType::EditorCameraController:
	{
		return "EditorCameraController";
	}
	case ComponentType::AnimationController:
	{
		return "AnimationController";
	}
	default:
	{
		AMLogger.Err("ComponentTypeToString: Invalid component type!");
		return "Invalid";
	}
	}
}
