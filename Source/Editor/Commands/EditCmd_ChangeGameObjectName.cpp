#include "Editor.pch.h"
#include "EditCmd_ChangeGameObjectName.h"
#include "../Modelviewer.h"

EditCmd_ChangeGameObjectName::EditCmd_ChangeGameObjectName(GameObject* anObject, const std::string& aNewName) : myObject(anObject), myOldName(anObject->GetName()), myOldImguiName(ModelViewer::GetImguiManager().GetIndexName(anObject)), myNewName(aNewName)
{
}

void EditCmd_ChangeGameObjectName::Undo()
{
	myObject->SetName(myOldName);
	ModelViewer::GetImguiManager().ChangeIndexName(myObject, myOldImguiName);
}

void EditCmd_ChangeGameObjectName::Execute()
{
	myObject->SetName(myNewName);
	ModelViewer::GetImguiManager().ChangeIndexName(myObject, myNewName);
}
