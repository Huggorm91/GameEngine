#include "Modelviewer.pch.h"
#include "EditCmd_AddGameObject.h"
#include "../Modelviewer.h"

EditCmd_AddGameObject::EditCmd_AddGameObject(const std::shared_ptr<GameObject>& anObject) : myObject(anObject), myID(anObject->GetID())
{
	myObject->MarkAsPrefab(myID);
}

EditCmd_AddGameObject::EditCmd_AddGameObject(GameObject&& anObject) : myObject(std::make_shared<GameObject>(std::move(anObject))), myID(anObject.GetID())
{
}

void EditCmd_AddGameObject::Undo()
{
	if (!RemoveGameObject(myID))
	{
		LogError("EditCmd_AddGameObject::Undo: Failed to remove GameObject " + std::to_string(static_cast<int>(myID)));
	}	
}

void EditCmd_AddGameObject::Execute()
{
	AddGameObject(myObject, nullptr);
}
