#include "pch.h"
#include "SGNode_MathAdd.h"

IMPLEMENT_GRAPH_NODE(SGNode_MathAdd, ScriptGraphNode);

SGNode_MathAdd::SGNode_MathAdd()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<float>("A", PinDirection::Input);
	CreateDataPin<float>("B", PinDirection::Input);
	CreateDataPin<float>("Result", PinDirection::Output);
}

NodeResult SGNode_MathAdd::DoOperation()
{
	float inA = 0;
	float inB = 0;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		const float result = inA + inB;
		SetPinData("Result", result);
		return ExecPin("Out");
	}

	return Error("Something went wrong adding up A and B!");
}