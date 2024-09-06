#include "Editor.pch.h"
#include "EditCmd_AddGameObjects.h"

EditCmd_AddGameObjects::EditCmd_AddGameObjects(): myObjects(GetSelectedObjects())
{
}

void EditCmd_AddGameObjects::Undo()
{
	ClearSelectedObjects();
	for (auto& object : myObjects)
	{
		if (!RemoveGameObject(object->GetUUID()))
		{
			LogError("EditCmd_AddGameObjects: Failed to remove GameObject with ID: " + object->GetUUID().str());
		}
	}
}

void EditCmd_AddGameObjects::Execute()
{
	for (auto& object : myObjects)
	{
		AddGameObject(object, nullptr);
	}
	SetSelectedObjects(myObjects);
}
