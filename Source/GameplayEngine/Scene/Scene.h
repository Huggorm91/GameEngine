#pragma once
#include "AssetManager/Assets/GameObject.h"

struct Scene
{
	std::string name = "NewScene";
	std::unordered_map<UUIDv4::UUID, GameObject> gameObjects;

	Scene() = default;
	Scene(const Json::Value& aJson);
	Scene(const Scene& aScene);
	Scene(Scene&& aScene) = default;
	~Scene() = default;
	Scene& operator=(const Scene& aScene);
	Scene& operator=(Scene&& aScene) = default;
};

std::istream& operator>>(std::istream& aStream, Scene& aScene);
std::ostream& operator<<(std::ostream& aStream, const Scene& aScene);