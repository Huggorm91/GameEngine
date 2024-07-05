#include "pch.h"
#include "SGNode_FloatMath.h"

IMPLEMENT_GRAPH_NODE(SGNode_FloatAdd, ScriptGraphNode);

SGNode_FloatAdd::SGNode_FloatAdd()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<float>("A", PinDirection::Input);
	CreateDataPin<float>("B", PinDirection::Input);
	CreateDataPin<float>("Result", PinDirection::Output);
}

NodeResult SGNode_FloatAdd::DoOperation()
{
	float inA = 0;
	float inB = 0;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		SetPinData("Result", inA + inB);
		return ExecPin("Out");
	}

	return Error("Something went wrong adding up A and B!");
}

IMPLEMENT_GRAPH_NODE(SGNode_FloatSubtract, ScriptGraphNode);

SGNode_FloatSubtract::SGNode_FloatSubtract()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<float>("A", PinDirection::Input);
	CreateDataPin<float>("B", PinDirection::Input);
	CreateDataPin<float>("Result", PinDirection::Output);
}

NodeResult SGNode_FloatSubtract::DoOperation()
{
	float inA = 0;
	float inB = 0;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		SetPinData("Result", inA - inB);
		return ExecPin("Out");
	}

	return Error("Something went wrong subtracting A and B!");
}

IMPLEMENT_GRAPH_NODE(SGNode_FloatMultiplication, ScriptGraphNode);

SGNode_FloatMultiplication::SGNode_FloatMultiplication()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<float>("A", PinDirection::Input);
	CreateDataPin<float>("B", PinDirection::Input);
	CreateDataPin<float>("Result", PinDirection::Output);
}

NodeResult SGNode_FloatMultiplication::DoOperation()
{
	float inA = 0;
	float inB = 0;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		SetPinData("Result", inA * inB);
		return ExecPin("Out");
	}

	return Error("Something went wrong multiplying A and B!");
}

IMPLEMENT_GRAPH_NODE(SGNode_FloatDivision, ScriptGraphNode);

SGNode_FloatDivision::SGNode_FloatDivision()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<float>("A", PinDirection::Input);
	CreateDataPin<float>("B", PinDirection::Input);
	CreateDataPin<float>("Result", PinDirection::Output);
}

NodeResult SGNode_FloatDivision::DoOperation()
{
	float inA = 0;
	float inB = 0;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		SetPinData("Result", inA / inB);
		return ExecPin("Out");
	}

	return Error("Something went wrong dividing A and B!");
}

IMPLEMENT_GRAPH_NODE(SGNode_FloatCos, ScriptGraphNode);

SGNode_FloatCos::SGNode_FloatCos()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<float>("Value", PinDirection::Input);
	CreateDataPin<float>("Result", PinDirection::Output);
}

NodeResult SGNode_FloatCos::DoOperation()
{
	float value = 0;

	if (GetPinData("Value", value))
	{
		SetPinData("Result", cosf(value));
		return ExecPin("Out");
	}

	return Error("Something went wrong calculating cos!");
}

IMPLEMENT_GRAPH_NODE(SGNode_FloatSin, ScriptGraphNode);

SGNode_FloatSin::SGNode_FloatSin()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<float>("Value", PinDirection::Input);
	CreateDataPin<float>("Result", PinDirection::Output);
}

NodeResult SGNode_FloatSin::DoOperation()
{
	float value = 0;

	if (GetPinData("Value", value))
	{
		SetPinData("Result", sinf(value));
		return ExecPin("Out");
	}

	return Error("Something went wrong calculating sin!");
}

IMPLEMENT_GRAPH_NODE(SGNode_FloatAbs, ScriptGraphNode);

SGNode_FloatAbs::SGNode_FloatAbs()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<float>("Value", PinDirection::Input);
	CreateDataPin<float>("Result", PinDirection::Output);
}

NodeResult SGNode_FloatAbs::DoOperation()
{
	float value = 0;

	if (GetPinData("Value", value))
	{
		SetPinData("Result", std::abs(value));
		return ExecPin("Out");
	}

	return Error("Something went wrong getting absolute value!");
}

IMPLEMENT_GRAPH_NODE(SGNode_FloatATan, ScriptGraphNode);

SGNode_FloatATan::SGNode_FloatATan()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<float>("X", PinDirection::Input);
	CreateDataPin<float>("Result", PinDirection::Output);
}

NodeResult SGNode_FloatATan::DoOperation()
{
	float inX = 0;

	if (GetPinData("X", inX))
	{
		SetPinData("Result", atanf(inX));
		return ExecPin("Out");
	}

	return Error("Something went wrong calculating ATan!");
}

IMPLEMENT_GRAPH_NODE(SGNode_FloatATan2, ScriptGraphNode);

SGNode_FloatATan2::SGNode_FloatATan2()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<float>("X", PinDirection::Input);
	CreateDataPin<float>("Y", PinDirection::Input);
	CreateDataPin<float>("Result", PinDirection::Output);
}

NodeResult SGNode_FloatATan2::DoOperation()
{
	float inX = 0;
	float inY = 0;

	if (GetPinData("X", inX) && GetPinData("Y", inY))
	{
		SetPinData("Result", atan2f(inY, inX));
		return ExecPin("Out");
	}

	return Error("Something went wrong calculating ATan2!");
}