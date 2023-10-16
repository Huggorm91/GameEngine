#include "Modelviewer.pch.h"
#include "EditCmd_AddMultipleGameObjects.h"

EditCmd_AddMultipleGameObjects::EditCmd_AddMultipleGameObjects(): myObjects(GetSelectedObjects())
{
}

void EditCmd_AddMultipleGameObjects::Undo()
{
	for (auto& object : myObjects)
	{
		if (!RemoveGameObject(object->GetID()))
		{
			LogError("EditCmd_RemoveMultipleGameObjects: Failed to remove GameObject with ID: " + std::to_string(object->GetID()));
		}
	}
}

void EditCmd_AddMultipleGameObjects::Execute()
{
	for (auto& object : myObjects)
	{
		AddGameObject(object);
	}
}
