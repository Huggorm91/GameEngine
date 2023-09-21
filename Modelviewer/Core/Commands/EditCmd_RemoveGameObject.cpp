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
	const bool success = RemoveGameObject(myID);
	assert(success && "Failed to remove GameObject");
}
