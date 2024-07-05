#include "pch.h"
#include "SGNode_FloatCompare.h"
#include "CrimsonUtilities/Math/Math.hpp"

IMPLEMENT_GRAPH_NODE(SGNode_FloatGreater, ScriptGraphNode);

SGNode_FloatGreater::SGNode_FloatGreater()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<float>("A", PinDirection::Input);
	CreateDataPin<float>("B", PinDirection::Input);
	CreateDataPin<bool>("Result", PinDirection::Output);
}

NodeResult SGNode_FloatGreater::DoOperation()
{
	float inA = 0;
	float inB = 0;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		SetPinData("Result", inA > inB);
		return ExecPin("Out");
	}

	return Error("Something went wrong comparing A and B!");
}

IMPLEMENT_GRAPH_NODE(SGNode_FloatLess, ScriptGraphNode);

SGNode_FloatLess::SGNode_FloatLess()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<float>("A", PinDirection::Input);
	CreateDataPin<float>("B", PinDirection::Input);
	CreateDataPin<bool>("Result", PinDirection::Output);
}

NodeResult SGNode_FloatLess::DoOperation()
{
	float inA = 0;
	float inB = 0;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		SetPinData("Result", inA < inB);
		return ExecPin("Out");
	}

	return Error("Something went wrong comparing A and B!");
}

IMPLEMENT_GRAPH_NODE(SGNode_FloatEqual, ScriptGraphNode);

SGNode_FloatEqual::SGNode_FloatEqual()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<float>("A", PinDirection::Input);
	CreateDataPin<float>("B", PinDirection::Input);
	CreateDataPin<bool>("Result", PinDirection::Output);
}

NodeResult SGNode_FloatEqual::DoOperation()
{
	float inA = 0;
	float inB = 0;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		SetPinData("Result", Crimson::GimbalCheck(inA, inB));
		return ExecPin("Out");
	}

	return Error("Something went wrong comparing A and B!");
}