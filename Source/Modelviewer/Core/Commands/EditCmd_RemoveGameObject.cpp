#include "Modelviewer.pch.h"
#include "EditCmd_RemoveGameObject.h"
#include "../Modelviewer.h"

EditCmd_RemoveGameObject::EditCmd_RemoveGameObject(const std::shared_ptr<GameObject>& anObject) : myObject(anObject), myID(anObject->GetID())
{
	myObject->MarkAsPrefab(myID);
}

void EditCmd_RemoveGameObject::Undo()
{
	AddGameObject(myObject);
}

void EditCmd_RemoveGameObject::Execute()
{
	if (!RemoveGameObject(myID))
	{
		LogError("EditCmd_RemoveGameObject::Execute: Failed to remove GameObject " + std::to_string(myID));
	}
}
