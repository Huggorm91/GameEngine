#include "Modelviewer.pch.h"
#include "EditCmd_AddGameObjects.h"

EditCmd_AddGameObjects::EditCmd_AddGameObjects(): myObjects(GetSelectedObjects())
{
}

void EditCmd_AddGameObjects::Undo()
{
	ClearSelectedObjects();
	for (auto& object : myObjects)
	{
		if (!RemoveGameObject(object->GetID()))
		{
			LogError("EditCmd_AddGameObjects: Failed to remove GameObject with ID: " + std::to_string(object->GetID()));
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
