#include "pch.h"
#include "SGNode_Gate.h"

IMPLEMENT_GRAPH_NODE(SGNode_Gate, ScriptGraphNode);

SGNode_Gate::SGNode_Gate()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("True", PinDirection::Output);
	CreateExecPin("False", PinDirection::Output);

	CreateDataPin<bool>("Condition", PinDirection::Input);
}

NodeResult SGNode_Gate::Enter(size_t anEntryPinId, NodeEntryType aEntryType)
{
	ClearError();
	if (aEntryType == NodeEntryType::New)
	{
		myIsOpen = true;
	}

	bool condition = false;
	if (GetPinData("Condition", condition))
	{
		if (myIsOpen && condition)
		{
			myIsOpen = false;
			return ExecPin("True");
		}
		else if (!condition)
		{
			return ExecPin("False");
		}
		else
		{
			return NoExec();
		}
	}

	return Error("Something went wrong evaluating condition!");
}