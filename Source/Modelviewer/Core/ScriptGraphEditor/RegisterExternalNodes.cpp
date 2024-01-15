#include "ModelViewer.pch.h"
#include "RegisterExternalNodes.h"

#include "Math/Vector3.hpp"

//void MVNode_TestNode::Init()
//{
//	CreateExecPin("In", PinDirection::Input, true);
//	CreateExecPin("Out", PinDirection::Output, true);
//	CreateDataPin<float>("Value", PinDirection::Input);
//}
//
//size_t MVNode_TestNode::DoOperation()
//{
//	return ExitViaPin("Out");
//}
//
//void MVNode_TestExecless::Init()
//{
//	CreateDataPin<bool>("Test", PinDirection::Input);
//	CreateDataPin<bool>("Value", PinDirection::Output);
//}
//
//size_t MVNode_TestExecless::DoOperation()
//{
//	bool b = false;
//	GetPinData("Test", b);
//	SetPinData("Value", b);
//	return Exit();
//}
//
//void MVNode_TestMultiInput::Init()
//{
//	CreateDataPin<float>("A", PinDirection::Input);
//	CreateDataPin<float>("B", PinDirection::Input);
//	CreateDataPin<float>("C", PinDirection::Input);
//	CreateDataPin<float>("Result", PinDirection::Output);
//}
//
//size_t MVNode_TestMultiInput::DoOperation()
//{
//	return Exit();
//}
//
//bool MVNode_TestMultiInput::IsSimpleNode() const
//{
//	return false;
//}

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

	return 0;
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
	return 0;
}
