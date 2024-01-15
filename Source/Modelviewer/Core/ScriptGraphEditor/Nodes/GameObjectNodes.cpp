#include "Modelviewer.pch.h"
#include "GameObjectNodes.h"
#include "Core/Modelviewer.h"

void SGNode_GameObjectCreate::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<std::string>("Type", PinDirection::Input);
	SetPinData<std::string>("Type", "Empty");

	CreateDataPin<GameObjectID>("Object ID", PinDirection::Output);
}

size_t SGNode_GameObjectCreate::DoOperation()
{
	std::string type = "";

	if (GetPinData("Type", type))
	{
		GameObjectID result = GameObjectID::Invalid;
		AssetManager::SetLogErrors(false);
		if (auto object = AssetManager::GetAsset<GameObject>(type); object.GetID() != GameObjectID::Invalid)
		{
			AssetManager::SetLogErrors(true);
			result = ModelViewer::Get().AddGameObject(std::move(object))->GetID();
			SetPinData("Object ID", result);
			return ExitViaPin("Out");
		}

		AssetManager::SetLogErrors(true);

		type = Crimson::ToLower(type);
		if (type == "empty")
		{
			result = ModelViewer::Get().AddGameObject()->GetID();
		}
		else if (type == "mesh" || type == "cube")
		{
			result = ModelViewer::Get().AddGameObject(AssetManager::GetAsset(Primitives::Cube))->GetID();
		}
		else if (type == "plane")
		{
			result = ModelViewer::Get().AddGameObject(AssetManager::GetAsset(Primitives::Plane))->GetID();
		}
		else if (type == "sphere")
		{
			result = ModelViewer::Get().AddGameObject(AssetManager::GetAsset(Primitives::Sphere))->GetID();
		}
		else if (type == "pyramid")
		{
			result = ModelViewer::Get().AddGameObject(AssetManager::GetAsset(Primitives::Pyramid))->GetID();
		}

		if (result != GameObjectID::Invalid)
		{
			SetPinData("Object ID", result);
			return ExitViaPin("Out");
		}		
	}

	return ExitWithError("Invalid input!");
}

void SGNode_GameObjectGet::Init()
{
	CreateDataPin<int>("ID", PinDirection::Input);

	CreateDataPin<GameObjectID>("Object ID", PinDirection::Output);
}

size_t SGNode_GameObjectGet::DoOperation()
{
	int id = 0;

	if (GetPinData("ID", id))
	{
		auto object = ModelViewer::Get().GetGameObject(static_cast<GameObjectID>(id));
		if (object)
		{
			SetPinData("Object ID", static_cast<GameObjectID>(id));
			return Exit();
		}
	}

	return ExitWithError("Invalid input!");
}

void SGNode_GameObjectGetPos::Init()
{
	CreateDataPin<GameObjectID>("ID", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Position", PinDirection::Output);
}

size_t SGNode_GameObjectGetPos::DoOperation()
{
	GameObjectID id = GameObjectID::Invalid;

	if (GetPinData("ID", id))
	{
		auto object = ModelViewer::Get().GetGameObject(id);
		if (object)
		{
			Crimson::Vector3f result = object->GetWorldPosition();
			SetPinData("Position", result);
			return Exit();
		}
	}

	return ExitWithError("Invalid input!");
}

void SGNode_GameObjectSetPos::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);
	CreateDataPin<GameObjectID>("ID", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Position", PinDirection::Input);
}

size_t SGNode_GameObjectSetPos::DoOperation()
{
	GameObjectID id = GameObjectID::Invalid;
	Crimson::Vector3f position = Crimson::Vector3f::Null;

	if (GetPinData("ID", id) && GetPinData("Position", position))
	{
		auto object = ModelViewer::Get().GetGameObject(id);
		if (object)
		{
			object->SetPosition(position);
			return ExitViaPin("Out");
		}
	}

	return ExitWithError("Invalid input!");
}