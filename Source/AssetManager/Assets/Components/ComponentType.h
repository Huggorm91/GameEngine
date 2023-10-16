#pragma once
#include <string>

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
	Count
};

void AddComponent(const Component* aComponent, GameObject& aParent);
void AddComponent(const ComponentType aType, GameObject& aParent);
void LoadComponent(const Json::Value& aJson, GameObject& aParent);

// Mainly used by editor to list all types without creating instances of them.
std::string ComponentTypeToString(const ComponentType aType);
//std::string ComponentTypeToString(const Json::Value& aJson);