#include "EditCmd_AddGameObject.h"
#include "../Modelviewer.h"

EditCmd_AddGameObject::EditCmd_AddGameObject(const GameObject& anObject) : myObject(std::make_shared<GameObject>(anObject)), myID(anObject.GetID())
{
	myObject->MarkAsPrefab(myID);
}

void EditCmd_AddGameObject::Undo()
{
	const bool success = ModelViewer::Get().RemoveGameObject(myID);
	assert(success && "Failed to remove GameObject");
}

void EditCmd_AddGameObject::Execute()
{
	GameObject copy(*myObject);
	copy.MarkAsPrefab(myID);
	ModelViewer::Get().AddGameObject(std::move(copy));
	copy = GameObject(); // To ensure internal pointer in BlackBoard of myObject is not deleted when swap is destroyed
}
