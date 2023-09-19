#include "EditCmd_RemoveGameObject.h"
#include "../Modelviewer.h"

EditCmd_RemoveGameObject::EditCmd_RemoveGameObject(const std::shared_ptr<GameObject>& anObject) : myObject(anObject), myID(anObject->GetID())
{
	myObject->MarkAsPrefab(myID);
}

void EditCmd_RemoveGameObject::Undo()
{
	GameObject copy(*myObject);
	copy.MarkAsPrefab(myID);
	AddGameObject(std::move(copy));
	copy = GameObject(); // To ensure internal pointer in BlackBoard of myObject is not deleted when copy is destroyed
}

void EditCmd_RemoveGameObject::Execute()
{
	const bool success = RemoveGameObject(myID);
	assert(success && "Failed to remove GameObject");
}
