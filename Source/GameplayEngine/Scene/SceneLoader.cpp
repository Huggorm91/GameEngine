#include "GameplayEngine.pch.h"
#include "SceneLoader.h"

void SceneLoader::Init()
{
	UpdateFilePaths();
}

void SceneLoader::UpdateFilePaths()
{
	myFilePaths = Crimson::GetAllFilepathsInDirectory(GetPath(), GetExtension(), true);
	auto binaries = Crimson::GetAllFilepathsInDirectory(GetPath(), GetBinaryExtension(), true);
	for (auto& binary : binaries)
	{
		myFilePaths.emplace(binary);
	}
}

const std::unordered_set<std::string>& SceneLoader::GetScenelist()
{
	return myFilePaths;
}

Scene SceneLoader::GetScene(const std::string& aPath) const
{
	auto path = ValidatePath(aPath);
	if (path.empty())
	{
		throw std::runtime_error("SceneLoader: Could not load Scene from path: " + aPath);
	}

	if (Crimson::HasValidExtension(path, GetBinaryExtension()))
	{
		return LoadBinaryScene(path);
	}
	return LoadScene(path);
}

void SceneLoader::SaveScene(const std::string& aPath, const Scene& aScene, bool anAsBinary)
{
	if (anAsBinary)
	{
		auto path = ValidateSavePath(aPath, GetBinaryExtension());
		if (path.empty())
		{
			throw std::runtime_error("SceneLoader: Could not save Scene to path: " + aPath);
		}

		SaveSceneToBinary(path, aScene);
	}
	else
	{
		auto path = ValidateSavePath(aPath, GetExtension());
		if (path.empty())
		{
			throw std::runtime_error("SceneLoader: Could not save Scene to path: " + aPath);
		}

		SaveSceneToFile(path, aScene);
	}
}

std::string SceneLoader::ValidatePath(const std::string& aPath) const
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

std::string SceneLoader::ValidateSavePath(const std::string& aPath, const std::string& anExtension) const
{
	std::string result = aPath;
	if (!Crimson::HasValidExtension(result, anExtension))
	{
		result += anExtension;
	}
	return Crimson::CreateValidPath(result, GetPath());
}

Scene SceneLoader::LoadScene(const std::string& aPath) const
{
	Json::Value json;
	std::fstream fileStream(aPath, std::ios::in);
	if (fileStream)
	{
		fileStream >> json;
	}
	else
	{
		throw std::runtime_error("Could not open file at: " + aPath);
	}

	return json;
}

Scene SceneLoader::LoadBinaryScene(const std::string& aPath) const
{
	Scene result;
	std::fstream fileStream(aPath, std::ios::in | std::ios::binary);
	if (fileStream)
	{
		try
		{
			fileStream >> result;
		}
		catch (...)
		{
			std::throw_with_nested(std::runtime_error("Could not load scene at: " + aPath));
		}
	}
	else
	{
		throw std::runtime_error("Could not open file at: " + aPath);
	}

	return result;
}

void SceneLoader::SaveSceneToFile(const std::string& aPath, const Scene& aScene)
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
		throw std::runtime_error("Could not open file at: " + aPath);
	}
}

void SceneLoader::SaveSceneToBinary(const std::string& aPath, const Scene& aScene)
{
	std::fstream fileStream(aPath, std::ios::out | std::ios::trunc | std::ios::binary);
	if (fileStream)
	{
		fileStream << aScene;
	}
	else
	{
		throw std::runtime_error("Could not open file at: " + aPath);
	}
}