#include "AssetManager.pch.h"
#include "ComponentType.h"
#include "Json/JsonVector.hpp"
#include "ComponentInclude.h"
#include "AssetManager/AssetManager.h"

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
	case ComponentType::BoxCollider:
	{
		return &aParent.AddComponent<BoxColliderComponent>();
	}
	case ComponentType::SphereCollider:
	{
		return &aParent.AddComponent<SphereColliderComponent>();
	}
	case ComponentType::RayCollider:
	{
		return &aParent.AddComponent<RayColliderComponent>();
	}
	case ComponentType::CapsuleCollider:
	{
		return &aParent.AddComponent<CapsuleColliderComponent>();
	}
	case ComponentType::Network: 
	{ 
		return &aParent.AddComponent<NetworkComponent>();
	}
	default:
	{
		AMLogger.Err("AddComponent: Invalid component type! GameObject ID : " + std::to_string(aParent.GetID()) + "\tComponent type: " + std::to_string(static_cast<int>(aType)));
		break;
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
		aParent.AddComponent(MeshComponent(aJson));	
		break;
	}
	case ComponentType::AnimatedMesh:
	{
		aParent.AddComponent(AnimatedMeshComponent(aJson));	
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
	case ComponentType::BoxCollider:
	{
		aParent.AddComponent(BoxColliderComponent(aJson));
		break;
	}
	case ComponentType::SphereCollider:
	{
		aParent.AddComponent(SphereColliderComponent(aJson));
		break;
	}
	case ComponentType::RayCollider:
	{
		aParent.AddComponent(RayColliderComponent(aJson));
		break;
	}
	case ComponentType::CapsuleCollider:
	{
		aParent.AddComponent(CapsuleColliderComponent(aJson));
		break;
	}
	case ComponentType::Network: 
	{ 
		aParent.AddComponent(NetworkComponent(aJson));
		break; 
	}
	default:
	{
		AMLogger.Err("LoadComponent: Invalid component type! GameObject ID : " + std::to_string(aParent.GetID()) + "\tComponent type: " + std::to_string(static_cast<int>(type)));
		break;
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
	case ComponentType::BoxCollider:
	{
		auto& collider = aParent.AddComponent<BoxColliderComponent>();
		collider.Deserialize(aStream);
		break;
	}
	case ComponentType::SphereCollider:
	{
		auto& collider = aParent.AddComponent<SphereColliderComponent>();
		collider.Deserialize(aStream);
		break;
	}
	case ComponentType::RayCollider:
	{
		auto& collider = aParent.AddComponent<RayColliderComponent>();
		collider.Deserialize(aStream);
		break;
	}
	case ComponentType::CapsuleCollider:
	{
		auto& collider = aParent.AddComponent<CapsuleColliderComponent>();
		collider.Deserialize(aStream);
		break;
	}
	case ComponentType::Network: 
	{ 
		auto& network = aParent.AddComponent<NetworkComponent>();
		network.Deserialize(aStream);
		break; 
	}
	case ComponentType::Count:
		break;
	default:
		AMLogger.Err("BinaryLoadComponent: Invalid component type! GameObject ID : " + std::to_string(aParent.GetID()) + "\tComponent type: " + std::to_string(static_cast<int>(type)));
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
	case ComponentType::BoxCollider:
	{
		return "BoxCollider";
	}
	case ComponentType::SphereCollider:
	{
		return "SphereCollider";
	}
	case ComponentType::RayCollider:
	{
		return "RayCollider";
	}
	case ComponentType::CapsuleCollider:
	{
		return "CapsuleCollider";
	}
	case ComponentType::Network: 
	{
		return "Network";
	}
	default:
	{
		AMLogger.Err("ComponentTypeToString: Invalid component type: " + std::to_string(static_cast<int>(aType)));
		return "Invalid";
	}
	}
}
