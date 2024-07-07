#include "pch.h"
#include "SGNode_BoolLogic.h"

IMPLEMENT_GRAPH_NODE(SGNode_Not, ScriptGraphNode);

SGNode_Not::SGNode_Not()
{
	CreateDataPin<bool>("Value", PinDirection::Input);
	CreateDataPin<bool>("Result", PinDirection::Output);
}

NodeResult SGNode_Not::DoOperation()
{
	bool value = false;

	if (GetPinData("Value", value))
	{
		SetPinData("Result", !value);
		return NoExec();
	}

	return Error("Something went wrong inverting value!");
}

IMPLEMENT_GRAPH_NODE(SGNode_And, ScriptGraphNode);

SGNode_And::SGNode_And()
{
	CreateDataPin<bool>("A", PinDirection::Input);
	CreateDataPin<bool>("B", PinDirection::Input);
	CreateDataPin<bool>("Result", PinDirection::Output);
}

NodeResult SGNode_And::DoOperation()
{
	bool inA = false;
	bool inB = false;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		SetPinData("Result", inA && inB);
		return NoExec();
	}

	return Error("Something went wrong inverting value!");
}

IMPLEMENT_GRAPH_NODE(SGNode_Or, ScriptGraphNode);

SGNode_Or::SGNode_Or()
{
	CreateDataPin<bool>("A", PinDirection::Input);
	CreateDataPin<bool>("B", PinDirection::Input);
	CreateDataPin<bool>("Result", PinDirection::Output);
}

NodeResult SGNode_Or::DoOperation()
{
	bool inA = false;
	bool inB = false;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		SetPinData("Result", inA || inB);
		return NoExec();
	}

	return Error("Something went wrong inverting value!");
}