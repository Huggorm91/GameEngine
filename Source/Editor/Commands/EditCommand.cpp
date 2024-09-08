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

std::shared_ptr<GameObject> EditCommand::GetGameObject(const UUIDv4::UUID& anID) const
{
	if (auto iter = ModelViewer::Get().myGameobjects.find(anID); iter != ModelViewer::Get().myGameobjects.end())
	{
		return iter->second;
	}
	return nullptr;
}

std::shared_ptr<GameObject>& EditCommand::AddGameObject(const std::shared_ptr<GameObject>& anObject, const std::unordered_map<UUIDv4::UUID, std::unordered_set<std::shared_ptr<GameObject>>>* aChildList) const
{
	if (aChildList)
	{
		if (auto iter = aChildList->find(anObject->GetUUID()); iter != aChildList->end())
		{
			for (auto& child : iter->second)
			{
				AddGameObject(child, aChildList);
			}
		}
	}	
	
	ModelViewer::Get().myImguiManager.AddGameObject(anObject.get());
	return ModelViewer::Get().myGameobjects.emplace(anObject->GetUUID(), anObject).first->second;
}

bool EditCommand::RemoveGameObject(const UUIDv4::UUID& anID) const
{
	for (auto& child : GetGameObject(anID)->GetChildren())
	{
		EraseObject(child->GetUUID());
	}
	return EraseObject(anID);
}

std::unordered_set<std::shared_ptr<GameObject>> EditCommand::GetSelectedObjects() const
{
	std::unordered_set<std::shared_ptr<GameObject>> result;
	for (auto& object : ModelViewer::Get().myImguiManager.mySelectedObjects)
	{
		result.emplace(GetGameObject(object->GetUUID()));
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

std::unordered_map<UUIDv4::UUID, std::unordered_set<std::shared_ptr<GameObject>>> EditCommand::GetChildrenOf(const std::shared_ptr<GameObject>& anObject) const
{
	std::unordered_map<UUIDv4::UUID, std::unordered_set<std::shared_ptr<GameObject>>> result;
	if (anObject->HasChild())
	{
		 result = GetChildrenInternal(anObject);
	}
	return result;
}

bool EditCommand::EraseObject(const UUIDv4::UUID& anID) const
{
	auto& gameObjects = ModelViewer::Get().myGameobjects;
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

std::unordered_map<UUIDv4::UUID, std::unordered_set<std::shared_ptr<GameObject>>> EditCommand::GetChildrenInternal(const std::shared_ptr<GameObject>& anObject) const
{
	std::unordered_map<UUIDv4::UUID, std::unordered_set<std::shared_ptr<GameObject>>> result;
	result.emplace(anObject->GetUUID(), GetChildList(anObject));
	for (auto& child : anObject->GetChildren())
	{
		auto childMap = GetChildrenInternal(GetGameObject(child->GetUUID()));
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
		result.emplace(GetGameObject(child->GetUUID()));
	}
	return result;
}
