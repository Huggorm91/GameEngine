#include "AssetManager.pch.h"
#include "SceneManager.h"

void SceneManager::Init()
{
	UpdateFilePaths();
}

void SceneManager::UpdateFilePaths()
{
	myFilePaths = Crimson::GetAllFilepathsInDirectory(GetPath(), GetExtension(), true);
	auto binaries = Crimson::GetAllFilepathsInDirectory(GetPath(), GetBinaryExtension(), true);
	for (auto& binary : binaries)
	{
		myFilePaths.emplace(binary);
	}
}

const std::unordered_set<std::string>& SceneManager::GetScenelist()
{
	return myFilePaths;
}

Scene SceneManager::GetScene(const std::string& aPath, bool aShouldLogErrors)
{
	auto path = ValidatePath(aPath);
	if (path.empty())
	{
		if (aShouldLogErrors)
		{
			AMLogger.Warn("SceneManager: Could not load Scene from path: " + aPath);
		}
		return Scene();
	}

	if (Crimson::HasValidExtension(path, GetBinaryExtension()))
	{
		return LoadBinaryScene(path, aShouldLogErrors);
	}
	return LoadScene(path, aShouldLogErrors);
}

EditorScene SceneManager::GetEditorScene(const std::string& aPath, bool aShouldLogErrors)
{
	auto path = ValidatePath(aPath);
	if (path.empty())
	{
		if (aShouldLogErrors)
		{
			AMLogger.Warn("SceneManager: Could not load EditorScene from path: " + aPath);
		}
		return EditorScene();
	}

	if (Crimson::HasValidExtension(path, GetBinaryExtension()))
	{
		return LoadBinaryEditorScene(path, aShouldLogErrors);
	}
	return LoadEditorScene(path, aShouldLogErrors);
}

void SceneManager::SaveScene(const std::string& aPath, const Scene& aScene, bool anAsBinary)
{
	if (anAsBinary)
	{
		auto path = ValidateSavePath(aPath, GetBinaryExtension());
		if (path.empty())
		{
			AMLogger.Warn("SceneManager: Could not save Scene to path: " + aPath);
			return;
		}

		SaveSceneToBinary(path, aScene);
	}
	else
	{
		auto path = ValidateSavePath(aPath, GetExtension());
		if (path.empty())
		{
			AMLogger.Warn("SceneManager: Could not save Scene to path: " + aPath);
			return;
		}

		SaveSceneToFile(path, aScene);
	}
}

void SceneManager::SaveScene(const std::string& aPath, const EditorScene& aScene, bool anAsBinary)
{
	if (anAsBinary)
	{
		auto path = ValidateSavePath(aPath, GetBinaryExtension());
		if (path.empty())
		{
			AMLogger.Warn("SceneManager: Could not save Scene to path: " + aPath);
			return;
		}

		SaveSceneToBinary(path, aScene);
	}
	else
	{
		auto path = ValidateSavePath(aPath, GetExtension());
		if (path.empty())
		{
			AMLogger.Warn("SceneManager: Could not save Scene to path: " + aPath);
			return;
		}

		SaveSceneToFile(path, aScene);
	}
}

std::string SceneManager::ValidatePath(const std::string& aPath) const
{
	if (Crimson::HasValidExtension(aPath, std::vector<std::string>{ GetBinaryExtension(), GetExtension() }))
	{
		return Crimson::GetValidPath(aPath, GetPath());
	}

	std::string result = aPath + GetBinaryExtension();
	if (result = Crimson::GetValidPath(result, GetPath()); result.empty())
	{
		result = aPath + GetExtension();
		result = Crimson::GetValidPath(result, GetPath());
	}
	return result;
}

std::string SceneManager::ValidateSavePath(const std::string& aPath, const std::string& anExtension) const
{
	std::string result = aPath;
	if (!Crimson::HasValidExtension(result, anExtension))
	{
		result += anExtension;
	}
	return Crimson::CreateValidPath(result, GetPath());
}

Scene SceneManager::LoadScene(const std::string& aPath, bool aShouldLogErrors)
{
	Json::Value json;
	std::fstream fileStream(aPath, std::ios::in);
	if (fileStream)
	{
		fileStream >> json;
	}
	else
	{
		if (aShouldLogErrors)
		{
			AMLogger.Err("Could not open file at: " + aPath);
		}
		fileStream.close();
		return Scene();
	}
	fileStream.close();

	return json;
}

Scene SceneManager::LoadBinaryScene(const std::string& aPath, bool aShouldLogErrors)
{
	Scene result;
	std::fstream fileStream(aPath, std::ios::in | std::ios::binary);
	if (fileStream)
	{
		try
		{
			fileStream >> result;
		}
		catch (const std::exception& e)
		{
			AMLogger.Err(e.what() + std::string(" Path: ") + aPath);
			fileStream.close();
			return result;
		}
	}
	else
	{
		if (aShouldLogErrors)
		{
			AMLogger.Err("Could not open file at: " + aPath);
		}
		fileStream.close();
		return result;
	}
	fileStream.close();

	return result;
}

EditorScene SceneManager::LoadEditorScene(const std::string& aPath, bool aShouldLogErrors)
{
	Json::Value json;
	std::fstream fileStream(aPath, std::ios::in);
	if (fileStream)
	{
		fileStream >> json;
	}
	else
	{
		if (aShouldLogErrors)
		{
			AMLogger.Err("Could not open file at: " + aPath);
		}
		fileStream.close();
		return EditorScene();
	}
	fileStream.close();

	EditorScene result = json;
	result.path = aPath;
	return result;
}

EditorScene SceneManager::LoadBinaryEditorScene(const std::string& aPath, bool aShouldLogErrors)
{
	EditorScene result;
	std::fstream fileStream(aPath, std::ios::in | std::ios::binary);
	if (fileStream)
	{
		try
		{
			fileStream >> result;
		}
		catch (const std::exception& e)
		{
			AMLogger.Err(e.what() + std::string(" Path: ") + aPath);
			fileStream.close();
			return result;
		}		
	}
	else
	{
		if (aShouldLogErrors)
		{
			AMLogger.Err("Could not open file at: " + aPath);
		}
		fileStream.close();
		return result;
	}
	fileStream.close();

	result.path = aPath;
	return result;
}

void SceneManager::SaveSceneToFile(const std::string& aPath, const Scene& aScene)
{
	std::fstream fileStream(aPath, std::ios::out | std::ios::trunc);
	if (fileStream)
	{
		Json::Value json;
		json.setComment("// " + aScene.name, Json::commentBefore);
		json["SceneName"] = aScene.name;
		json["GameObjects"] = Json::arrayValue;

		int i = 0;
		for (auto& [id, object] : aScene.gameObjects)
		{
			json["GameObjects"][i] = object.ToJson();
			json["GameObjects"][i].setComment("// " + object.GetName() + " | ID: " + id.str(), Json::commentBefore);
			i++;
		}

		Json::StreamWriterBuilder builder;
		builder["commentStyle"] = "All";
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		writer->write(json, &fileStream);
		fileStream.flush();
	}
	else
	{
		AMLogger.Err("Could not open file at: " + aPath);
	}
	fileStream.close();
}

void SceneManager::SaveSceneToBinary(const std::string& aPath, const Scene& aScene)
{
	std::fstream fileStream(aPath, std::ios::out | std::ios::trunc | std::ios::binary);
	if (fileStream)
	{
		fileStream << aScene;
	}
	else
	{
		AMLogger.Err("Could not open file at: " + aPath);
	}
	fileStream.close();
}

void SceneManager::SaveSceneToFile(const std::string& aPath, const EditorScene& aScene)
{
	std::fstream fileStream(aPath, std::ios::out | std::ios::trunc);
	if (fileStream)
	{
		Json::Value json;
		json.setComment("// " + aScene.name, Json::commentBefore);
		json["SceneName"] = aScene.name;
		json["GameObjects"] = Json::arrayValue;

		int i = 0;
		for (auto& [id, object] : aScene.gameObjects)
		{
			json["GameObjects"][i] = object->ToJson();
			json["GameObjects"][i].setComment("// " + object->GetName() + " | ID: " + id.str(), Json::commentBefore);
			i++;
		}

		Json::StreamWriterBuilder builder;
		builder["commentStyle"] = "All";
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		writer->write(json, &fileStream);
		fileStream.flush();
	}
	else
	{
		AMLogger.Err("Could not open file at: " + aPath);
	}
	fileStream.close();
}

void SceneManager::SaveSceneToBinary(const std::string& aPath, const EditorScene& aScene)
{
	std::fstream fileStream(aPath, std::ios::out | std::ios::trunc | std::ios::binary);
	if (fileStream)
	{
		fileStream << aScene;
	}
	else
	{
		AMLogger.Err("Could not open file at: " + aPath);
	}
	fileStream.close();
}

inline Scene::Scene(const Json::Value& aJson) : name(aJson["SceneName"].asString())
{
	std::unordered_map<UUIDv4::UUID, UUIDv4::UUID> childlist;
	for (auto& json : aJson["GameObjects"])
	{
		GameObject object = json;
		std::string parentID = GameObject::GetParentID(json);
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

inline EditorScene::EditorScene(const Json::Value& aJson): name(aJson["SceneName"].asString()), path()
{
	std::unordered_map<UUIDv4::UUID, UUIDv4::UUID> childlist;
	for (auto& json : aJson["GameObjects"])
	{
		std::shared_ptr<GameObject> object = std::make_shared<GameObject>(json);
		std::string parentID = GameObject::GetParentID(json);
		if (!parentID.empty())
		{
			childlist.emplace(object->GetUUID(), parentID);
		}
		gameObjects.emplace(object->GetUUID(), object);
	}

	for (auto& [childID, parentID] : childlist)
	{
		gameObjects.at(parentID)->AddChild(gameObjects.at(childID).get());
	}
}

std::istream& operator>>(std::istream& aStream, Scene& aScene)
{
	// load
	Binary::eType type;
	aStream.read(reinterpret_cast<char*>(&type), sizeof(type));
	if (type != Binary::Scene)
	{
		throw std::runtime_error("SceneManager::LoadBinaryScene: Invalid Binary::Type when loading scene.");
	}
	std::getline(aStream, aScene.name, '\0');

	unsigned gameobjectCount = 0;
	aStream.read(reinterpret_cast<char*>(&gameobjectCount), sizeof(gameobjectCount));

	std::unordered_map<UUIDv4::UUID, UUIDv4::UUID> childlist;
	{
		UUIDv4::UUID parentID = GameObject::nullUUID;
		for (unsigned i = 0; i < gameobjectCount; i++)
		{
			aStream.read(reinterpret_cast<char*>(&type), sizeof(type));
			if (type != Binary::GameObject)
			{
				throw std::runtime_error("SceneManager::LoadBinaryScene: Invalid Binary::Type when loading GameObject.");
			}
			GameObject object;
			parentID = object.Deserialize(aStream);
			if (parentID != GameObject::nullUUID)
			{
				childlist.emplace(object.GetUUID(), parentID);
			}
			aScene.gameObjects.emplace(object.GetUUID(), std::move(object));
		}
	}

	for (auto& [childID, parentID] : childlist)
	{
		aScene.gameObjects.at(parentID).AddChild(&aScene.gameObjects.at(childID));
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
	for (auto& object : aScene.gameObjects)
	{
		object.second.Serialize(aStream);
	}
	return aStream;
}

std::istream& operator>>(std::istream& aStream, EditorScene& aScene)
{
	// load
	Binary::eType type;
	aStream.read(reinterpret_cast<char*>(&type), sizeof(type));
	if (type != Binary::Scene)
	{
		throw std::runtime_error("SceneManager::LoadBinaryScene: Invalid Binary::Type when loading scene.");
	}
	std::getline(aStream, aScene.name, '\0');

	unsigned gameobjectCount = 0;
	aStream.read(reinterpret_cast<char*>(&gameobjectCount), sizeof(gameobjectCount));

	std::unordered_map<UUIDv4::UUID, UUIDv4::UUID> childlist;
	{
		UUIDv4::UUID parentID = GameObject::nullUUID;
		for (unsigned i = 0; i < gameobjectCount; i++)
		{
			aStream.read(reinterpret_cast<char*>(&type), sizeof(type));
			if (type != Binary::GameObject)
			{
				throw std::runtime_error("SceneManager::LoadBinaryScene: Invalid Binary::Type when loading GameObject.");
			}

			std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
			parentID = object->Deserialize(aStream);
			if (parentID != GameObject::nullUUID)
			{
				childlist.emplace(object->GetUUID(), parentID);
			}
			aScene.gameObjects.emplace(object->GetUUID(), object);
		}
	}

	for (auto& [childID, parentID] : childlist)
	{
		aScene.gameObjects.at(parentID)->AddChild(aScene.gameObjects.at(childID).get());
	}
	return aStream;
}

std::ostream& operator<<(std::ostream& aStream, const EditorScene& aScene)
{
	// save
	Binary::eType type = Binary::Scene;
	unsigned gameobjectCount = static_cast<unsigned>(aScene.gameObjects.size());

	aStream.write(reinterpret_cast<char*>(&type), sizeof(type));
	aStream.write(aScene.name.c_str(), aScene.name.size() + 1);
	aStream.write(reinterpret_cast<char*>(&gameobjectCount), sizeof(gameobjectCount));
	for (auto& object : aScene.gameObjects)
	{
		object.second->Serialize(aStream);
	}
	return aStream;
}
