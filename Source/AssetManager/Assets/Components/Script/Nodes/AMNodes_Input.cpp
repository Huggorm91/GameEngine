#include "AssetManager.pch.h"
#include "AMNodes_Input.h"
#include "Input/InputMapper.h"

IMPLEMENT_GRAPH_NODE(AMNode_GetKeyDown, ScriptGraphNode);

AMNode_GetKeyDown::AMNode_GetKeyDown()
{
	CreateDataPin<Crimson::eKey>("Key", PinDirection::Input);
	CreateDataPin<bool>("Result", PinDirection::Output);
}

NodeResult AMNode_GetKeyDown::DoOperation()
{
	Crimson::eKey key = Crimson::eKey::None;

	if (GetPinData("Key", key))
	{
		SetPinData("Result", Crimson::InputMapper::GetInstance()->GetKeyDown(key));
		return NoExec();
	}

	return Error("Something went wrong getting input!");
}

IMPLEMENT_GRAPH_NODE(AMNode_GetKeyHeld, ScriptGraphNode);

AMNode_GetKeyHeld::AMNode_GetKeyHeld()
{
	CreateDataPin<Crimson::eKey>("Key", PinDirection::Input);
	CreateDataPin<bool>("Result", PinDirection::Output);
}

NodeResult AMNode_GetKeyHeld::DoOperation()
{
	Crimson::eKey key = Crimson::eKey::None;

	if (GetPinData("Key", key))
	{
		SetPinData("Result", Crimson::InputMapper::GetInstance()->GetKeyHeld(key));
		return NoExec();
	}

	return Error("Something went wrong getting input!");
}

IMPLEMENT_GRAPH_NODE(AMNode_GetKeyUp, ScriptGraphNode);

AMNode_GetKeyUp::AMNode_GetKeyUp()
{
	CreateDataPin<Crimson::eKey>("Key", PinDirection::Input);
	CreateDataPin<bool>("Result", PinDirection::Output);
}

NodeResult AMNode_GetKeyUp::DoOperation()
{
	Crimson::eKey key = Crimson::eKey::None;

	if (GetPinData("Key", key))
	{
		SetPinData("Result", Crimson::InputMapper::GetInstance()->GetKeyUp(key));
		return NoExec();
	}

	return Error("Something went wrong getting input!");
}

IMPLEMENT_GRAPH_NODE(AMNode_GetKeyDownOrHeld, ScriptGraphNode);

AMNode_GetKeyDownOrHeld::AMNode_GetKeyDownOrHeld()
{
	CreateDataPin<Crimson::eKey>("Key", PinDirection::Input);
	CreateDataPin<bool>("Result", PinDirection::Output);
}

NodeResult AMNode_GetKeyDownOrHeld::DoOperation()
{
	Crimson::eKey key = Crimson::eKey::None;

	if (GetPinData("Key", key))
	{
		SetPinData("Result", Crimson::InputMapper::GetInstance()->GetKeyDownOrHeld(key));
		return NoExec();
	}

	return Error("Something went wrong getting input!");
}
