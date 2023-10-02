#include "EditCmd_RemoveMultipleGameObjects.h"

EditCmd_RemoveMultipleGameObjects::EditCmd_RemoveMultipleGameObjects(): myObjects(GetSelectedObjects())
{
}

void EditCmd_RemoveMultipleGameObjects::Undo()
{
	for (auto& object : myObjects)
	{
		AddGameObject(object);
	}	
}

void EditCmd_RemoveMultipleGameObjects::Execute()
{
	for (auto& object : myObjects)
	{
		if (!RemoveGameObject(object->GetID()))
		{
			LogError("EditCmd_RemoveMultipleGameObjects: Failed to remove GameObject with ID: " + std::to_string(object->GetID()));
		}
	}
}
