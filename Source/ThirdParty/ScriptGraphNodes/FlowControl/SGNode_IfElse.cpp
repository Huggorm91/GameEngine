#include "pch.h"
#include "SGNode_IfElse.h"

IMPLEMENT_GRAPH_NODE(SGNode_IfElse, ScriptGraphNode);

SGNode_IfElse::SGNode_IfElse()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("True", PinDirection::Output);
	CreateExecPin("False", PinDirection::Output);

	CreateDataPin<bool>("Condition", PinDirection::Input);
}

NodeResult SGNode_IfElse::DoOperation()
{
	bool value = false;

	if (GetPinData("Condition", value))
	{
		if (value)
		{
			return ExecPin("True");
		}
		else
		{
			return ExecPin("False");
		}
	}

	return Error("Something went wrong evaluating condition!");
}