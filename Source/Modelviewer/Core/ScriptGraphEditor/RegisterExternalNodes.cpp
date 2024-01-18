#include "ModelViewer.pch.h"
#include "RegisterExternalNodes.h"

#include "Math/Vector3.hpp"

void MVNode_Branch::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("True", PinDirection::Output);
	CreateExecPin("False", PinDirection::Output);

	CreateDataPin<bool>("Condition", PinDirection::Input);
}

size_t MVNode_Branch::DoOperation()
{
	bool condition = false;
	if(GetPinData("Condition", condition))
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

	return ExitWithError("Invalid Input!");
}
