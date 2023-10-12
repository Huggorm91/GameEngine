#include "EditCmd_RemoveGameObjects.h"

EditCmd_RemoveGameObjects::EditCmd_RemoveGameObjects(): myObjects(GetSelectedObjects()), myChildList()
{
	for (auto& object : myObjects)
	{
		auto childMap = GetChildrenOf(object);
		if (!childMap.empty())
		{
			myChildList.insert(childMap.begin(), childMap.end());
		}
	}
}

void EditCmd_RemoveGameObjects::Undo()
{
	for (auto& object : myObjects)
	{
		AddGameObject(object, &myChildList);
	}	
	SetSelectedObjects(myObjects);
}

void EditCmd_RemoveGameObjects::Execute()
{
	ClearSelectedObjects();
	for (auto& object : myObjects)
	{
		if (!RemoveGameObject(object->GetID()))
		{
			LogError("EditCmd_RemoveGameObjects: Failed to remove GameObject with ID: " + std::to_string(object->GetID()));
		}
	}
}
