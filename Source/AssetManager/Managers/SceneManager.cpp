#include "AssetManager.pch.h"
#include "SceneManager.h"

void SceneManager::Init()
{
	myFilePaths = Crimson::GetAllFilepathsInDirectory(GetPath(), GetExtension());
	auto binaries = Crimson::GetAllFilepathsInDirectory(GetPath(), GetBinaryExtension());
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
	result.Path = aPath;
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

	result.Path = aPath;
	return result;
}

void SceneManager::SaveSceneToFile(const std::string& aPath, const Scene& aScene)
{
	std::fstream fileStream(aPath, std::ios::out | std::ios::trunc);
	if (fileStream)
	{
		Json::Value json;
		json.setComment("// " + aScene.Name, Json::commentBefore);
		json["SceneName"] = aScene.Name;
		json["GameObjectIDCount"] = aScene.GameObjectIDCount;
		json["GameObjects"] = Json::arrayValue;

		int i = 0;
		for (auto& [id, object] : aScene.GameObjects)
		{
			json["GameObjects"][i] = object.ToJson();
			json["GameObjects"][i].setComment("// GameObject ID: " + std::to_string(static_cast<int>(id)), Json::commentBefore);
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
		json.setComment("// " + aScene.Name, Json::commentBefore);
		json["SceneName"] = aScene.Name;
		json["GameObjectIDCount"] = aScene.GameObjectIDCount;
		json["GameObjects"] = Json::arrayValue;

		int i = 0;
		for (auto& [id, object] : aScene.GameObjects)
		{
			json["GameObjects"][i] = object->ToJson();
			json["GameObjects"][i].setComment("// GameObject ID: " + std::to_string(static_cast<int>(id)), Json::commentBefore);
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

inline Scene::Scene(const Json::Value& aJson) : GameObjectIDCount(aJson["GameObjectIDCount"].asUInt()), Name(aJson["SceneName"].asString()), GameObjects()
{
	std::unordered_map<GameObjectID, GameObjectID> childlist;
	for (auto& json : aJson["GameObjects"])
	{
		GameObject object = json;
		GameObjectID parentID = GameObject::GetParentID(json);
		if (parentID != GameObjectID::Invalid)
		{
			childlist.emplace(object.GetID(), parentID);
		}
		GameObjects.emplace(object.GetID(), object);
	}

	for (auto& [childID, parentID] : childlist)
	{
		GameObjects.at(parentID).AddChild(&GameObjects.at(childID));
	}
}

inline EditorScene::EditorScene(const Json::Value& aJson): GameObjectIDCount(aJson["GameObjectIDCount"].asUInt()), Name(aJson["SceneName"].asString()), Path(), GameObjects()
{
	std::unordered_map<GameObjectID, GameObjectID> childlist;
	for (auto& json : aJson["GameObjects"])
	{
		std::shared_ptr<GameObject> object = std::make_shared<GameObject>(json);
		GameObjectID parentID = GameObject::GetParentID(json);
		if (parentID != GameObjectID::Invalid)
		{
			childlist.emplace(object->GetID(), parentID);
		}
		GameObjects.emplace(object->GetID(), object);
	}

	for (auto& [childID, parentID] : childlist)
	{
		GameObjects.at(parentID)->AddChild(GameObjects.at(childID).get());
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
	std::getline(aStream, aScene.Name, '\0');
	aStream.read(reinterpret_cast<char*>(&aScene.GameObjectIDCount), sizeof(aScene.GameObjectIDCount));

	unsigned gameobjectCount = 0;
	aStream.read(reinterpret_cast<char*>(&gameobjectCount), sizeof(gameobjectCount));

	std::unordered_map<GameObjectID, GameObjectID> childlist;
	{
		GameObjectID parentID = GameObjectID::Invalid;
		for (unsigned i = 0; i < gameobjectCount; i++)
		{
			aStream.read(reinterpret_cast<char*>(&type), sizeof(type));
			if (type != Binary::GameObject)
			{
				throw std::runtime_error("SceneManager::LoadBinaryScene: Invalid Binary::Type when loading GameObject.");
			}
			GameObject object;
			parentID = object.Deserialize(aStream);
			if (parentID != GameObjectID::Invalid)
			{
				childlist.emplace(object.GetID(), parentID);
			}
			aScene.GameObjects.emplace(object.GetID(), std::move(object));
		}
	}

	for (auto& [childID, parentID] : childlist)
	{
		aScene.GameObjects.at(parentID).AddChild(&aScene.GameObjects.at(childID));
	}
	return aStream;
}

std::ostream& operator<<(std::ostream& aStream, const Scene& aScene)
{
	// save
	Binary::eType type = Binary::Scene;
	unsigned gameobjectCount = static_cast<unsigned>(aScene.GameObjects.size());

	aStream.write(reinterpret_cast<char*>(&type), sizeof(type));
	aStream.write(aScene.Name.c_str(), aScene.Name.size() + 1);
	aStream.write(reinterpret_cast<const char*>(&aScene.GameObjectIDCount), sizeof(aScene.GameObjectIDCount));
	aStream.write(reinterpret_cast<char*>(&gameobjectCount), sizeof(gameobjectCount));
	for (auto& object : aScene.GameObjects)
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
	std::getline(aStream, aScene.Name, '\0');
	aStream.read(reinterpret_cast<char*>(&aScene.GameObjectIDCount), sizeof(aScene.GameObjectIDCount));

	unsigned gameobjectCount = 0;
	aStream.read(reinterpret_cast<char*>(&gameobjectCount), sizeof(gameobjectCount));

	std::unordered_map<GameObjectID, GameObjectID> childlist;
	{
		GameObjectID parentID = GameObjectID::Invalid;
		for (unsigned i = 0; i < gameobjectCount; i++)
		{
			aStream.read(reinterpret_cast<char*>(&type), sizeof(type));
			if (type != Binary::GameObject)
			{
				throw std::runtime_error("SceneManager::LoadBinaryScene: Invalid Binary::Type when loading GameObject.");
			}

			std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
			parentID = object->Deserialize(aStream);
			if (parentID != GameObjectID::Invalid)
			{
				childlist.emplace(object->GetID(), parentID);
			}
			aScene.GameObjects.emplace(object->GetID(), object);
		}
	}

	for (auto& [childID, parentID] : childlist)
	{
		aScene.GameObjects.at(parentID)->AddChild(aScene.GameObjects.at(childID).get());
	}
	return aStream;
}

std::ostream& operator<<(std::ostream& aStream, const EditorScene& aScene)
{
	// save
	Binary::eType type = Binary::Scene;
	unsigned gameobjectCount = static_cast<unsigned>(aScene.GameObjects.size());

	aStream.write(reinterpret_cast<char*>(&type), sizeof(type));
	aStream.write(aScene.Name.c_str(), aScene.Name.size() + 1);
	aStream.write(reinterpret_cast<const char*>(&aScene.GameObjectIDCount), sizeof(aScene.GameObjectIDCount));
	aStream.write(reinterpret_cast<char*>(&gameobjectCount), sizeof(gameobjectCount));
	for (auto& object : aScene.GameObjects)
	{
		object.second->Serialize(aStream);
	}
	return aStream;
}
