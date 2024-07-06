#include "pch.h"
#include "SGNode_VectorFunctions.h"

IMPLEMENT_GRAPH_NODE(SGNode_Vector3Create, ScriptGraphNode);

SGNode_Vector3Create::SGNode_Vector3Create()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<float>("X", PinDirection::Input);
	CreateDataPin<float>("Y", PinDirection::Input);
	CreateDataPin<float>("Z", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

NodeResult SGNode_Vector3Create::DoOperation()
{
	float inX = 0.f;
	float inY = 0.f;
	float inZ = 0.f;

	if (GetPinData("X", inX) && GetPinData("Y", inY) && GetPinData("Z", inZ))
	{
		SetPinData("Result", Crimson::Vector3f(inX, inY, inZ));
		return ExecPin("Out");
	}

	return Error("Something went wrong creating vector!");
}

IMPLEMENT_GRAPH_NODE(SGNode_Vector3Break, ScriptGraphNode);

SGNode_Vector3Break::SGNode_Vector3Break()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<Crimson::Vector3f>("Value", PinDirection::Input);

	CreateDataPin<float>("X", PinDirection::Output);
	CreateDataPin<float>("Y", PinDirection::Output);
	CreateDataPin<float>("Z", PinDirection::Output);
}

NodeResult SGNode_Vector3Break::DoOperation()
{
	Crimson::Vector3f value;

	if (GetPinData("Value", value))
	{
		SetPinData("X", value.x);
		SetPinData("Y", value.y);
		SetPinData("Z", value.z);
		return ExecPin("Out");
	}

	return Error("Something went wrong breaking vector!");
}

IMPLEMENT_GRAPH_NODE(SGNode_Vector3Equal, ScriptGraphNode);

SGNode_Vector3Equal::SGNode_Vector3Equal()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<Crimson::Vector3f>("A", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("B", PinDirection::Input);

	CreateDataPin<bool>("Result", PinDirection::Output);
}

NodeResult SGNode_Vector3Equal::DoOperation()
{
	Crimson::Vector3f inA;
	Crimson::Vector3f inB;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		SetPinData("Result", inA == inB);
		return ExecPin("Out");
	}

	return Error("Something went wrong comparing A and B!");
}

IMPLEMENT_GRAPH_NODE(SGNode_Vector3MultiplyFloat, ScriptGraphNode);

SGNode_Vector3MultiplyFloat::SGNode_Vector3MultiplyFloat()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<Crimson::Vector3f>("Vector", PinDirection::Input);
	CreateDataPin<float>("Float", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

NodeResult SGNode_Vector3MultiplyFloat::DoOperation()
{
	Crimson::Vector3f inVector;
	float inFloat = 0.f;

	if (GetPinData("Vector", inVector) && GetPinData("Float", inFloat))
	{
		SetPinData("Result", inVector *= inFloat);
		return ExecPin("Out");
	}

	return Error("Something went wrong mulitplying vector and float!");
}
