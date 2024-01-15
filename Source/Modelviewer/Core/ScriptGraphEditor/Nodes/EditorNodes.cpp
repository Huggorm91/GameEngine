#include "Modelviewer.pch.h"
#include "EditorNodes.h"
#include "Core/Modelviewer.h"

void SGNode_EditorGetCameraPos::Init()
{
	CreateDataPin<Crimson::Vector3f>("Position", PinDirection::Output);
}

size_t SGNode_EditorGetCameraPos::DoOperation()
{
	const Crimson::Vector3f pos = ModelViewer::GetCamera().GetPosition();
	SetPinData("Position", pos);

	return Exit();
}

void SGNode_EditorGetCreatedObject::Init()
{
	CreateDataPin<int>("Index", PinDirection::Input);
	CreateDataPin<GameObjectID>("ID", PinDirection::Output);
}

size_t SGNode_EditorGetCreatedObject::DoOperation()
{
	int index = 0;
	if (GetPinData("Index", index))
	{
		auto& createdObjects = ModelViewer::GetCreatedObjects();
		if (index < createdObjects.size() && index >= 0)
		{
			GameObjectID id = createdObjects[index];
			if (ModelViewer::Get().GetGameObject(id))
			{
				SetPinData("ID", static_cast<GameObjectID>(id));
				return Exit();
			}
			return ExitWithError("Object does not exist!");
		}
		return ExitWithError("Index out of range!");
	}

	return ExitWithError("Invalid input!");
}