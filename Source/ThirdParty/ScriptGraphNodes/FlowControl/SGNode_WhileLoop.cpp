#include "pch.h"
#include "SGNode_WhileLoop.h"

IMPLEMENT_GRAPH_NODE(SGNode_WhileLoop, ScriptGraphNode);

SGNode_WhileLoop::SGNode_WhileLoop()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Loop", PinDirection::Output);
	CreateExecPin("Completed", PinDirection::Output);

	CreateDataPin<bool>("Condition", PinDirection::Input);
}

NodeResult SGNode_WhileLoop::Enter(size_t anEntryPinId, NodeEntryType aEntryType)
{
	ClearError();
	bool condition = false;
	if (GetPinData("Condition", condition))
	{
		if (condition)
		{
			return ExecPin("Loop", NodeResultState::InProgress);
		}
		else
		{
			return ExecPin("Completed");
		}
	}

	return Error("Something went wrong evaluating condition!");
}