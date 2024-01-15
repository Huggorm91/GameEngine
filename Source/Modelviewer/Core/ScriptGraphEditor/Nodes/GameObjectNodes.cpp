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
			ModelViewer::GetCreatedObjects().emplace_back(result);
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
			ModelViewer::GetCreatedObjects().emplace_back(result);
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

void SGNode_GameObjectGetRot::Init()
{
	CreateDataPin<GameObjectID>("ID", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Rotation", PinDirection::Output);
}

size_t SGNode_GameObjectGetRot::DoOperation()
{
	GameObjectID id = GameObjectID::Invalid;

	if (GetPinData("ID", id))
	{
		auto object = ModelViewer::Get().GetGameObject(id);
		if (object)
		{
			Crimson::Vector3f result = object->GetTransform().GetRotation();
			SetPinData("Rotation", result);
			return Exit();
		}
	}

	return ExitWithError("Invalid input!");
}

void SGNode_GameObjectSetRot::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);
	CreateDataPin<GameObjectID>("ID", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Rotation", PinDirection::Input);
}

size_t SGNode_GameObjectSetRot::DoOperation()
{
	GameObjectID id = GameObjectID::Invalid;
	Crimson::Vector3f rotation = Crimson::Vector3f::Null;

	if (GetPinData("ID", id) && GetPinData("Rotation", rotation))
	{
		auto object = ModelViewer::Get().GetGameObject(id);
		if (object)
		{
			object->SetRotation(rotation);
			return ExitViaPin("Out");
		}
	}

	return ExitWithError("Invalid input!");
}

void SGNode_GameObjectGetScale::Init()
{
	CreateDataPin<GameObjectID>("ID", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Scale", PinDirection::Output);
}

size_t SGNode_GameObjectGetScale::DoOperation()
{
	GameObjectID id = GameObjectID::Invalid;

	if (GetPinData("ID", id))
	{
		auto object = ModelViewer::Get().GetGameObject(id);
		if (object)
		{
			Crimson::Vector3f result = object->GetTransform().GetScale();
			SetPinData("Scale", result);
			return Exit();
		}
	}

	return ExitWithError("Invalid input!");
}

void SGNode_GameObjectSetScale::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);
	CreateDataPin<GameObjectID>("ID", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Scale", PinDirection::Input);
}

size_t SGNode_GameObjectSetScale::DoOperation()
{
	GameObjectID id = GameObjectID::Invalid;
	Crimson::Vector3f scale = Crimson::Vector3f::Null;

	if (GetPinData("ID", id) && GetPinData("Scale", scale))
	{
		auto object = ModelViewer::Get().GetGameObject(id);
		if (object)
		{
			object->SetScale(scale);
			return ExitViaPin("Out");
		}
	}

	return ExitWithError("Invalid input!");
}