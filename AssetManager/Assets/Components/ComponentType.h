#pragma once
#include <fstream>

namespace Json{ class Value; }

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

class GameObject;
class Component;

void AddComponent(const Component* aComponent, GameObject& aParent);
void AddComponent(const ComponentType aType, GameObject& aParent);
void LoadComponent(const Json::Value& aJson, GameObject& aParent);
void LoadComponent(std::istream& aStream, GameObject& aParent);

// Mainly used by editor to list all types without creating instances of them.
std::string ComponentTypeToString(const ComponentType aType);
//std::string ComponentTypeToString(const Json::Value& aJson);