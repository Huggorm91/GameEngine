#include "Editor.pch.h"
#include "EditCommand.h"
#include "../Modelviewer.h"

bool EditCommand::Merge(const EditCommand*)
{
	return false;
}

void EditCommand::LogError(const std::string& anError) const
{
	ModelViewer::Get().GetLogger().Err(anError);
}

void EditCommand::LogMessage(const std::string& anError) const
{
	ModelViewer::Get().GetLogger().Log(anError);
}

std::shared_ptr<GameObject> EditCommand::GetGameObject(unsigned anID) const
{
	return ModelViewer::Get().GetGameObject(anID);
}

std::shared_ptr<GameObject>& EditCommand::AddGameObject(const std::shared_ptr<GameObject>& anObject, const std::unordered_map<unsigned, std::unordered_set<std::shared_ptr<GameObject>>>* aChildList) const
{
	if (aChildList)
	{
		if (auto iter = aChildList->find(anObject->GetID()); iter != aChildList->end())
		{
			for (auto& child : iter->second)
			{
				AddGameObject(child, aChildList);
			}
		}
	}	
	
	ModelViewer::Get().myImguiManager.AddGameObject(anObject.get());
	return ModelViewer::Get().myScene.GameObjects.emplace(anObject->GetID(), anObject).first->second;
}

bool EditCommand::RemoveGameObject(unsigned anID) const
{
	for (auto& child : ModelViewer::Get().GetGameObject(anID)->GetChildren())
	{
		EraseObject(child->GetID());
	}
	return EraseObject(anID);
}

std::unordered_set<std::shared_ptr<GameObject>> EditCommand::GetSelectedObjects() const
{
	std::unordered_set<std::shared_ptr<GameObject>> result;
	auto& modelViewer = ModelViewer::Get();
	for (auto& object : modelViewer.myImguiManager.mySelectedObjects)
	{
		result.emplace(modelViewer.GetGameObject(object->GetID()));
	}
	return result;
}

void EditCommand::SetSelectedObjects(const std::unordered_set<std::shared_ptr<GameObject>>& aList) const
{
	auto& selectedObjects = ModelViewer::Get().myImguiManager.mySelectedObjects;
	selectedObjects.clear();
	for (auto& object : aList)
	{
		selectedObjects.emplace(object.get());
	}
}

void EditCommand::ClearSelectedObjects() const
{
	ModelViewer::Get().myImguiManager.mySelectedObjects.clear();
}

std::unordered_map<unsigned, std::unordered_set<std::shared_ptr<GameObject>>> EditCommand::GetChildrenOf(const std::shared_ptr<GameObject>& anObject) const
{
	std::unordered_map<unsigned, std::unordered_set<std::shared_ptr<GameObject>>> result;
	if (anObject->HasChild())
	{
		 result = GetChildrenInternal(anObject);
	}
	return result;
}

bool EditCommand::EraseObject(unsigned anID) const
{
	auto& gameObjects = ModelViewer::Get().myScene.GameObjects;
	if (auto iter = gameObjects.find(anID); iter != gameObjects.end())
	{
		auto& selectedObjects = ModelViewer::Get().myImguiManager.mySelectedObjects;
		for (auto i = selectedObjects.begin(); i != selectedObjects.end(); i++)
		{
			if (*i == iter->second.get())
			{
				selectedObjects.erase(i);
				break;
			}
		}

		gameObjects.erase(iter);
		return true;
	}
	return false;
}

std::unordered_map<unsigned, std::unordered_set<std::shared_ptr<GameObject>>> EditCommand::GetChildrenInternal(const std::shared_ptr<GameObject>& anObject) const
{
	std::unordered_map<unsigned, std::unordered_set<std::shared_ptr<GameObject>>> result;
	result.emplace(anObject->GetID(), GetChildList(anObject));
	for (auto& child : anObject->GetChildren())
	{
		auto childMap = GetChildrenInternal(ModelViewer::Get().GetGameObject(child->GetID()));
		if (!childMap.empty())
		{
			result.insert(childMap.begin(), childMap.end());
		}
	}
	return result;
}

std::unordered_set<std::shared_ptr<GameObject>> EditCommand::GetChildList(const std::shared_ptr<GameObject>& anObject) const
{
	std::unordered_set<std::shared_ptr<GameObject>> result;
	for (auto& child : anObject->GetChildren())
	{
		result.emplace(ModelViewer::Get().GetGameObject(child->GetID()));
	}
	return result;
}
