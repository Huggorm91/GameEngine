#include "EditCmd_RemoveGameObject.h"
#include "../Modelviewer.h"

EditCmd_RemoveGameObject::EditCmd_RemoveGameObject(const std::shared_ptr<GameObject>& anObject) : myObject(anObject), myID(anObject->GetID()), myChildList(GetChildrenOf(anObject))
{
	myObject->MarkAsPrefab(myID);
}

void EditCmd_RemoveGameObject::Undo()
{
	AddGameObject(myObject, &myChildList);
}

void EditCmd_RemoveGameObject::Execute()
{
	if (!RemoveGameObject(myID))
	{
		LogError("EditCmd_RemoveGameObject::Execute: Failed to remove GameObject " + std::to_string(myID));
	}
}
