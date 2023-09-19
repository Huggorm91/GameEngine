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
	const bool success = RemoveGameObject(myID);
	assert(success && "Failed to remove GameObject");
}

void EditCmd_AddGameObject::Execute()
{
	GameObject copy(*myObject);
	copy.MarkAsPrefab(myID);
	AddGameObject(std::move(copy));
	copy = GameObject(); // To ensure internal pointer in BlackBoard of myObject is not deleted when swap is destroyed
}
