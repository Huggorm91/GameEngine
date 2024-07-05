#include "pch.h"
#include "SGNode_ToString.h"

IMPLEMENT_GRAPH_NODE(SGNode_ToStringBool, ScriptGraphNode);

SGNode_ToStringBool::SGNode_ToStringBool()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<bool>("Input", PinDirection::Input);
	CreateDataPin<std::string>("Result", PinDirection::Output);
}

NodeResult SGNode_ToStringBool::DoOperation()
{
	bool input = 0;

	if (GetPinData("Input", input))
	{
		SetPinData("Result", input ? "True" : "False");
		return ExecPin("Out");
	}

	return Error("Something went wrong converting Input to a string!");
}

IMPLEMENT_GRAPH_NODE(SGNode_ToStringInt, ScriptGraphNode);

SGNode_ToStringInt::SGNode_ToStringInt()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<int>("Input", PinDirection::Input);
	CreateDataPin<std::string>("Result", PinDirection::Output);
}

NodeResult SGNode_ToStringInt::DoOperation()
{
	int input = 0;

	if (GetPinData("Input", input))
	{
		SetPinData("Result", std::to_string(input));
		return ExecPin("Out");
	}

	return Error("Something went wrong converting Input to a string!");
}

IMPLEMENT_GRAPH_NODE(SGNode_ToStringGameObjectID, ScriptGraphNode);

SGNode_ToStringGameObjectID::SGNode_ToStringGameObjectID()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<unsigned>("Input", PinDirection::Input);
	CreateDataPin<std::string>("Result", PinDirection::Output);
}

NodeResult SGNode_ToStringGameObjectID::DoOperation()
{
	unsigned input = 0;

	if (GetPinData("Input", input))
	{
		SetPinData("Result", std::to_string(input));
		return ExecPin("Out");
	}

	return Error("Something went wrong converting Input to a string!");
}

IMPLEMENT_GRAPH_NODE(SGNode_ToStringFloat, ScriptGraphNode);

SGNode_ToStringFloat::SGNode_ToStringFloat()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<float>("Input", PinDirection::Input);
	CreateDataPin<std::string>("Result", PinDirection::Output);
}

NodeResult SGNode_ToStringFloat::DoOperation()
{
	float input = 0;

	if (GetPinData("Input", input))
	{
		SetPinData("Result", std::to_string(input));
		return ExecPin("Out");
	}

	return Error("Something went wrong converting Input to a string!");
}

IMPLEMENT_GRAPH_NODE(SGNode_ToStringVector3, ScriptGraphNode);

SGNode_ToStringVector3::SGNode_ToStringVector3()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<Crimson::Vector3f>("Input", PinDirection::Input);
	CreateDataPin<std::string>("Result", PinDirection::Output);
}

NodeResult SGNode_ToStringVector3::DoOperation()
{
	Crimson::Vector3f input;

	if (GetPinData("Input", input))
	{
		SetPinData("Result", input.ToString());
		return ExecPin("Out");
	}

	return Error("Something went wrong converting Input to a string!");
}