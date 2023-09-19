#include "EditCmd_ChangeGameObject.h"
#include "../Modelviewer.h"

EditCmd_ChangeGameObject::EditCmd_ChangeGameObject(const std::shared_ptr<GameObject>& anObject) : myObject(anObject), myID(anObject->GetID())
{
	myObject->MarkAsPrefab(myID);
}

void EditCmd_ChangeGameObject::Undo()
{
	Swap();
}

void EditCmd_ChangeGameObject::Execute()
{
	Swap();
}

void EditCmd_ChangeGameObject::Swap()
{
	std::shared_ptr<GameObject> object = GetGameObject(myID);
	assert(object && "Could not find GameObject!");

	GameObject swap(std::move(*object));
	*object = std::move(*myObject);
	*myObject = std::move(swap);
	swap = GameObject(); // To ensure internal pointer in BlackBoard of myObject is not deleted when swap is destroyed
}
