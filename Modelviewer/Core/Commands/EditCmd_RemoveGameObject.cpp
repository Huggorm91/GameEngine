#include "EditCmd_RemoveGameObject.h"
#include "../Modelviewer.h"

EditCmd_RemoveGameObject::EditCmd_RemoveGameObject(const GameObject& anObject) : myObject(std::make_shared<GameObject>(anObject)), myID(anObject.GetID())
{
	myObject->MarkAsPrefab(myID);
}

void EditCmd_RemoveGameObject::Undo()
{
	GameObject copy(*myObject);
	copy.MarkAsPrefab(myID);
	ModelViewer::Get().AddGameObject(std::move(copy));
	copy = GameObject(); // To ensure internal pointer in BlackBoard of myObject is not deleted when swap is destroyed
}

void EditCmd_RemoveGameObject::Execute()
{
	const bool success = ModelViewer::Get().RemoveGameObject(myID);
	assert(success && "Failed to remove GameObject");
}
