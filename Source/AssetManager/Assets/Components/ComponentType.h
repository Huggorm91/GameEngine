#pragma once

enum class ComponentType
{
	Unknown,
	Mesh,
	AnimatedMesh,
	Directionallight,
	Pointlight,
	Spotlight,
	DebugDraw,
	PerspectiveCamera,
	ParticleEmitter,
	EditorCameraController,
	AnimationController,
	BoxCollider,
	SphereCollider,
	RayCollider,
	CapsuleCollider,
	Count
};

namespace Json{ class Value; }
class GameObject;
class Component;

Component* AddComponent(const ComponentType aType, GameObject& aParent);
void LoadComponent(const Json::Value& aJson, GameObject& aParent);
void LoadComponent(std::istream& aStream, GameObject& aParent);

// Mainly used by editor to list all types without creating instances of them.
std::string ComponentTypeToString(const ComponentType aType);