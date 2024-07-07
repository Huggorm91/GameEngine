#include "AssetManager.pch.h"
#include "AMNodes_GameObject.h"
#include "Modelviewer/Modelviewer.h"
#include "AssetManager.h"

IMPLEMENT_GRAPH_NODE(AMNode_GameObjectGetThis, ScriptGraphNode);

AMNode_GameObjectGetThis::AMNode_GameObjectGetThis()
{
	CreateDataPin<void*>("Owner", PinDirection::Input);
	CreateDataPin<unsigned>("GameObject ID", PinDirection::Output);
}

NodeResult AMNode_GameObjectGetThis::DoOperation()
{
	void* owner = nullptr;

	if (GetPinData("Owner", owner))
	{
		if (owner)
		{
			SetPinData("GameObject ID", static_cast<GameObject*>(owner)->GetID());
			return NoExec();
		}
		return Error("No owner is set!");
	}

	return Error("Something went wrong getting owner!");
}

IMPLEMENT_GRAPH_NODE(AMNode_GameObjectCreate, ScriptGraphNode);

AMNode_GameObjectCreate::AMNode_GameObjectCreate()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<std::string>("Type", PinDirection::Input);
	SetPinData("Type", std::string("Empty"));
	CreateDataPin<unsigned>("GameObject ID", PinDirection::Output);
}

NodeResult AMNode_GameObjectCreate::DoOperation()
{
	std::string type;

	if (GetPinData("Type", type))
	{
		unsigned result = 0u;
		AssetManager::SetLogErrors(false);
		if (auto object = AssetManager::GetAsset<GameObject>(type); object.GetID() != 0u)
		{
			AssetManager::SetLogErrors(true);
			result = ModelViewer::Get().AddGameObject(std::move(object))->GetID();
			SetPinData("GameObject ID", result);
			return ExecPin("Out");
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

		if (result != 0u)
		{
			SetPinData("Object ID", result);
			return ExecPin("Out");
		}
	}

	return Error("Invalid input!");
}

IMPLEMENT_GRAPH_NODE(AMNode_GameObjectMove, ScriptGraphNode);

AMNode_GameObjectMove::AMNode_GameObjectMove()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<unsigned>("GameObject ID", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Movement", PinDirection::Input);
}

NodeResult AMNode_GameObjectMove::DoOperation()
{
	unsigned id = 0u;
	Crimson::Vector3f movement;

	if (GetPinData("GameObject ID", id) && GetPinData("Movement", movement))
	{
		auto object = ModelViewer::Get().GetGameObject(id);
		if (object)
		{
			object->SetPosition(object->GetTransform().GetPosition() + movement);
			return ExecPin("Out");
		}
		return Error("An object with this ID does not exist!");
	}

	return Error("Something went wrong moving object!");
}

IMPLEMENT_GRAPH_NODE(AMNode_GameObjectGetPosition, ScriptGraphNode);

AMNode_GameObjectGetPosition::AMNode_GameObjectGetPosition()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<unsigned>("GameObject ID", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Position", PinDirection::Output);
}

NodeResult AMNode_GameObjectGetPosition::DoOperation()
{
	unsigned id = 0u;

	if (GetPinData("GameObject ID", id))
	{
		auto object = ModelViewer::Get().GetGameObject(id);
		if (object)
		{
			SetPinData("Position", ModelViewer::Get().GetGameObject(id)->GetTransform().GetPosition());
			return ExecPin("Out");
		}
		return Error("An object with this ID does not exist!");
	}

	return Error("Something went wrong getting position!");
}

IMPLEMENT_GRAPH_NODE(AMNode_GameObjectSetPosition, ScriptGraphNode);

AMNode_GameObjectSetPosition::AMNode_GameObjectSetPosition()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<unsigned>("GameObject ID", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Position", PinDirection::Input);
}

NodeResult AMNode_GameObjectSetPosition::DoOperation()
{
	unsigned id = 0u;
	Crimson::Vector3f value;

	if (GetPinData("GameObject ID", id) && GetPinData("Position", value))
	{
		auto object = ModelViewer::Get().GetGameObject(id);
		if (object)
		{
			object->SetPosition(value);
			return ExecPin("Out");
		}
		return Error("An object with this ID does not exist!");
	}

	return Error("Something went wrong setting position!");
}

IMPLEMENT_GRAPH_NODE(AMNode_GameObjectGetRotation, ScriptGraphNode);

AMNode_GameObjectGetRotation::AMNode_GameObjectGetRotation()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<unsigned>("GameObject ID", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Rotation", PinDirection::Output);
}

NodeResult AMNode_GameObjectGetRotation::DoOperation()
{
	unsigned id = 0u;

	if (GetPinData("GameObject ID", id))
	{
		auto object = ModelViewer::Get().GetGameObject(id);
		if (object)
		{
			SetPinData("Rotation", ModelViewer::Get().GetGameObject(id)->GetTransform().GetRotationDegree());
			return ExecPin("Out");
		}
		return Error("An object with this ID does not exist!");
	}

	return Error("Something went wrong getting rotation!");
}

IMPLEMENT_GRAPH_NODE(AMNode_GameObjectSetRotation, ScriptGraphNode);

AMNode_GameObjectSetRotation::AMNode_GameObjectSetRotation()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<unsigned>("GameObject ID", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Rotation", PinDirection::Input);
}

NodeResult AMNode_GameObjectSetRotation::DoOperation()
{
	unsigned id = 0u;
	Crimson::Vector3f value;

	if (GetPinData("GameObject ID", id) && GetPinData("Rotation", value))
	{
		auto object = ModelViewer::Get().GetGameObject(id);
		if (object)
		{
			object->SetRotation(value);
			return ExecPin("Out");
		}
		return Error("An object with this ID does not exist!");
	}

	return Error("Something went wrong setting rotation!");
}

IMPLEMENT_GRAPH_NODE(AMNode_GameObjectGetScale, ScriptGraphNode);

AMNode_GameObjectGetScale::AMNode_GameObjectGetScale()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<unsigned>("GameObject ID", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Scale", PinDirection::Output);
}

NodeResult AMNode_GameObjectGetScale::DoOperation()
{
	unsigned id = 0u;

	if (GetPinData("GameObject ID", id))
	{
		auto object = ModelViewer::Get().GetGameObject(id);
		if (object)
		{
			SetPinData("Scale", ModelViewer::Get().GetGameObject(id)->GetTransform().GetScale());
			return ExecPin("Out");
		}
		return Error("An object with this ID does not exist!");
	}

	return Error("Something went wrong getting scale!");
}

IMPLEMENT_GRAPH_NODE(AMNode_GameObjectSetScale, ScriptGraphNode);

AMNode_GameObjectSetScale::AMNode_GameObjectSetScale()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<unsigned>("GameObject ID", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Scale", PinDirection::Input);
}

NodeResult AMNode_GameObjectSetScale::DoOperation()
{
	unsigned id = 0u;
	Crimson::Vector3f value;

	if (GetPinData("GameObject ID", id) && GetPinData("Scale", value))
	{
		auto object = ModelViewer::Get().GetGameObject(id);
		if (object)
		{
			object->SetScale(value);
			return ExecPin("Out");
		}
		return Error("An object with this ID does not exist!");
	}

	return Error("Something went wrong setting scale!");
}

IMPLEMENT_GRAPH_NODE(AMNode_GameObjectIDToString, ScriptGraphNode);

AMNode_GameObjectIDToString::AMNode_GameObjectIDToString()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<unsigned>("GameObject ID", PinDirection::Input);
	CreateDataPin<std::string>("Text", PinDirection::Output);
}

NodeResult AMNode_GameObjectIDToString::DoOperation()
{
	unsigned id = 0u;

	if (GetPinData("GameObject ID", id))
	{
		SetPinData("Text", "GameObject: " + std::to_string(id));
		return ExecPin("Out");
	}

	return Error("Something went wrong setting scale!");
}
