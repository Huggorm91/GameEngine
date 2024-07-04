#include "pch.h"
#include "SGNode_ForLoop.h"

IMPLEMENT_GRAPH_NODE(SGNode_ForLoop, ScriptGraphNode);

SGNode_ForLoop::SGNode_ForLoop()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Loop", PinDirection::Output);
	CreateExecPin("Completed", PinDirection::Output);

	CreateDataPin<int>("Start", PinDirection::Input);
	CreateDataPin<int>("End", PinDirection::Input);

	CreateDataPin<int>("Index", PinDirection::Output);
}

NodeResult SGNode_ForLoop::Enter(size_t anEntryPinId, NodeEntryType aEntryType)
{
	ClearError();
	if (aEntryType == NodeEntryType::New)
	{
		if (GetPinData("Start", myIndex) && GetPinData("End", myEnd))
		{
			if (myIndex < 0)
			{
				return Error("Can not have negative index!");
			}
			if (myEnd < myIndex)
			{
				return Error("End is less than Start!");
			}
		}
		else
		{
			return Error("Something went wrong reading index!");
		}
	}

	if (myIndex < myEnd)
	{
		SetPinData("Index", myIndex++);
		return ExecPin("Loop", NodeResultState::InProgress);
	}
	else if (myIndex == myEnd)
	{
		return ExecPin("Completed");
	}

	return Error("For loop went out of index! This should not happen.");
}