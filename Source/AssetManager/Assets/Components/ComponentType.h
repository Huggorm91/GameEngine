#pragma once
namespace Json{ class Value; }
class GameObject;
class Component;

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
	Count
};

Component* AddComponent(const Component* aComponent, GameObject& aParent);
Component* AddComponent(const ComponentType aType, GameObject& aParent);
void LoadComponent(const Json::Value& aJson, GameObject& aParent);
void LoadComponent(std::istream& aStream, GameObject& aParent);

// Mainly used by editor to list all types without creating instances of them.
std::string ComponentTypeToString(const ComponentType aType);
//std::string ComponentTypeToString(const Json::Value& aJson);