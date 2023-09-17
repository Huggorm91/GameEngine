#include "EditCmd_ChangeGameObject.h"
#include "../Modelviewer.h"

EditCmd_ChangeGameObject::EditCmd_ChangeGameObject(const GameObject& anObject) : myObject(std::make_shared<GameObject>(anObject)), myID(anObject.GetID())
{
	myObject->MarkAsPrefab(myID);
}

void EditCmd_ChangeGameObject::Undo()
{
	Swap();
}

void EditCmd_ChangeGameObject::Redo()
{
	Swap();
}

void EditCmd_ChangeGameObject::Swap()
{
	GameObject* object = ModelViewer::Get().GetGameObject(myID);
	assert(object && "Could not find GameObject!");

	GameObject swap(std::move(*object));
	*object = std::move(*myObject);
	*myObject = std::move(swap);
	swap = GameObject(); // To ensure internal pointer in BlackBoard of myObject is not deleted when swap is destroyed
}
