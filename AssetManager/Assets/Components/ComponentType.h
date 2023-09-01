#pragma once
#include <string>

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
	Input,
	Movement
};

class GameObject;
class Component;

void AddComponent(const Component* aComponent, GameObject& aParent);
void LoadComponent(const Json::Value& aJson, GameObject& aParent);
std::string ComponentTypeToString(const ComponentType aType);
std::string ComponentTypeToString(const Json::Value& aJson);