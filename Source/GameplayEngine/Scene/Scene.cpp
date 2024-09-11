#include "GameplayEngine.pch.h"
#include "Scene.h"

Scene::Scene(const Json::Value& aJson) : name(aJson["SceneName"].asString())
{
	const auto& jsonObjects = aJson["GameObjects"];
	gameObjects.reserve(jsonObjects.size());

	std::unordered_map<UUIDv4::UUID, UUIDv4::UUID> childlist;
	for (auto& json : jsonObjects)
	{
		GameObject object = json;
		const std::string& parentID = GameObject::GetParentID(json);
		if (!parentID.empty())
		{
			childlist.emplace(object.GetUUID(), parentID);
		}
		gameObjects.emplace(object.GetUUID(), object);
	}

	for (auto& [childID, parentID] : childlist)
	{
		gameObjects.at(parentID).AddChild(&gameObjects.at(childID));
	}
}

void CopyObjects(Scene& aCopy, const Scene& anOriginal)
{
	aCopy.gameObjects.reserve(anOriginal.gameObjects.size());

	std::unordered_map<UUIDv4::UUID, UUIDv4::UUID> childlist;
	for (auto& [id, object] : anOriginal.gameObjects)
	{
		GameObject copy = object;
		copy.CopyUuidOf(object);
		aCopy.gameObjects.emplace(id, std::move(copy));

		if (object.HasParent())
		{
			childlist.emplace(id, object.GetParent()->GetUUID());
		}
	}

	for (auto& [childID, parentID] : childlist)
	{
		aCopy.gameObjects.at(parentID).AddChild(&aCopy.gameObjects.at(childID));
	}
}

Scene::Scene(const Scene& aScene) : name(aScene.name)
{
	CopyObjects(*this, aScene);	
}

Scene& Scene::operator=(const Scene& aScene)
{
	name = aScene.name;
	gameObjects.clear();
	CopyObjects(*this, aScene);
	return *this;
}

std::istream& operator>>(std::istream& aStream, Scene& aScene)
{
	// load
	Binary::eType type = Binary::eType::Unknown;
	aStream.read(reinterpret_cast<char*>(&type), sizeof(type));
	if (type != Binary::Scene)
	{
		throw std::runtime_error("SceneLoader::LoadBinaryScene: Invalid Binary::Type when loading scene.");
	}
	std::getline(aStream, aScene.name, '\0');

	unsigned gameobjectCount = 0;
	aStream.read(reinterpret_cast<char*>(&gameobjectCount), sizeof(gameobjectCount));

	std::unordered_map<UUIDv4::UUID, UUIDv4::UUID> childlist;
	for (unsigned i = 0; i < gameobjectCount; i++)
	{
		aStream.read(reinterpret_cast<char*>(&type), sizeof(type));
		if (type != Binary::GameObject)
		{
			throw std::runtime_error("SceneLoader::LoadBinaryScene: Invalid Binary::Type when loading GameObject.");
		}

		GameObject object(GameObject::nullUUID);
		const UUIDv4::UUID& parentID = object.Deserialize(aStream);
		if (parentID != GameObject::nullUUID)
		{
			childlist.emplace(object.GetUUID(), parentID);
		}
		aScene.gameObjects.emplace(object.GetUUID(), std::move(object));
	}

	for (auto& [childIndex, parentID] : childlist)
	{
		aScene.gameObjects.at(parentID).AddChild(&aScene.gameObjects.at(childIndex));
	}
	return aStream;
}

std::ostream& operator<<(std::ostream& aStream, const Scene& aScene)
{
	// save
	Binary::eType type = Binary::Scene;
	unsigned gameobjectCount = static_cast<unsigned>(aScene.gameObjects.size());

	aStream.write(reinterpret_cast<char*>(&type), sizeof(type));
	aStream.write(aScene.name.c_str(), aScene.name.size() + 1);
	aStream.write(reinterpret_cast<char*>(&gameobjectCount), sizeof(gameobjectCount));
	for (auto& [id, object] : aScene.gameObjects)
	{
		object.Serialize(aStream);
	}
	return aStream;
}