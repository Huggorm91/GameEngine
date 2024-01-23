#include "Modelviewer.pch.h"
#include "EditorNodes.h"
#include "Core/Modelviewer.h"

void MVNode_EditorGetCameraPos::Init()
{
	CreateDataPin<Crimson::Vector3f>("Position", PinDirection::Output);
}

size_t MVNode_EditorGetCameraPos::DoOperation()
{
	const Crimson::Vector3f pos = ModelViewer::GetCamera().GetWorldPosition();
	SetPinData("Position", pos);

	return Exit();
}

void MVNode_EditorGetCreatedObject::Init()
{
	CreateDataPin<int>("Index", PinDirection::Input);
	CreateDataPin<GameObjectID>("ID", PinDirection::Output);
}

size_t MVNode_EditorGetCreatedObject::DoOperation()
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

void MVNode_BuildString::Init()
{
	CreateDataPin<std::string>("Text1", PinDirection::Input, true);
	CreateDataPin<std::string>("Text2", PinDirection::Input, true);

	CreateDataPin<std::string>("Result", PinDirection::Output);
}

size_t MVNode_BuildString::DoOperation()
{
	std::string text1, text2;
	if (GetPinData("Text1", text1) && GetPinData("Text2", text2))
	{
		SetPinData("Result", text1 + text2);
		return Exit();
	}

	return ExitWithError("Invalid input!");
}