#include "Modelviewer.pch.h"
#include "FlowControlNodes.h"

void MVNode_IfElse::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("True", PinDirection::Output);
	CreateExecPin("False", PinDirection::Output);

	CreateDataPin<bool>("Condition", PinDirection::Input);
}

size_t MVNode_IfElse::DoOperation()
{
	bool condition = false;
	if (GetPinData("Condition", condition))
	{
		return ExitViaPin(condition ? "True" : "False");
	}

	return ExitWithError("Invalid Input!");
}

void MVNode_ForLoop::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Loop", PinDirection::Output);

	CreateDataPin<int>("Start", PinDirection::Input);
	CreateDataPin<int>("End", PinDirection::Input);
	CreateDataPin<int>("Index", PinDirection::Output);

	CreateExecPin("Completed", PinDirection::Output);
}

size_t MVNode_ForLoop::DoOperation()
{
	int start = 0, end = 0, index = 0;
	if (GetPinData("Start", start) && GetPinData("End", end))
	{
		for (int index = start; index < end; index++)
		{
			SetPinData("Index", index);
			ExitViaPin("Loop");
		}
		return ExitViaPin("Completed");
	}
	return ExitWithError("Invalid Input!");
}

void MVNode_Sequence::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("1", PinDirection::Output);
	CreateExecPin("2", PinDirection::Output);
	CreateExecPin("3", PinDirection::Output);
	CreateExecPin("4", PinDirection::Output);
	CreateExecPin("5", PinDirection::Output);
	CreateExecPin("6", PinDirection::Output);
}

size_t MVNode_Sequence::DoOperation()
{
	ExitViaPin("1");
	ExitViaPin("2");
	ExitViaPin("3");
	ExitViaPin("4");
	ExitViaPin("5");
	ExitViaPin("6");
}

void MVNode_Gate::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("True", PinDirection::Output);
	CreateExecPin("False", PinDirection::Output);

	CreateDataPin<bool>("Condition", PinDirection::Input);
	myHasActivated = false;
}

size_t MVNode_Gate::DoOperation()
{
	bool condition = false;
	if (GetPinData("Condition", condition))
	{
		if (!myHasActivated && condition)
		{
			myHasActivated = true;
			return ExitViaPin("True");
		}
		else if (!condition)
		{
			return ExitViaPin("False");
		}
		else
		{
			return Exit();
		}
	}

	return ExitWithError("Invalid Input!");
}