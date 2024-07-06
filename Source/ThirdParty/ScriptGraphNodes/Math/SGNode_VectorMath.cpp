#include "pch.h"
#include "SGNode_VectorMath.h"

IMPLEMENT_GRAPH_NODE(SGNode_Vector3Add, ScriptGraphNode);

SGNode_Vector3Add::SGNode_Vector3Add()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<Crimson::Vector3f>("A", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("B", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

NodeResult SGNode_Vector3Add::DoOperation()
{
	Crimson::Vector3f inA;
	Crimson::Vector3f inB;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		SetPinData("Result", inA + inB);
		return ExecPin("Out");
	}

	return Error("Something went wrong adding up A and B!");
}

IMPLEMENT_GRAPH_NODE(SGNode_Vector3Subtract, ScriptGraphNode);

SGNode_Vector3Subtract::SGNode_Vector3Subtract()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<Crimson::Vector3f>("A", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("B", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

NodeResult SGNode_Vector3Subtract::DoOperation()
{
	Crimson::Vector3f inA;
	Crimson::Vector3f inB;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		SetPinData("Result", inA - inB);
		return ExecPin("Out");
	}

	return Error("Something went wrong subtracting A and B!");
}

IMPLEMENT_GRAPH_NODE(SGNode_Vector3Multiplication, ScriptGraphNode);

SGNode_Vector3Multiplication::SGNode_Vector3Multiplication()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<Crimson::Vector3f>("A", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("B", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

NodeResult SGNode_Vector3Multiplication::DoOperation()
{
	Crimson::Vector3f inA;
	Crimson::Vector3f inB;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		SetPinData("Result", inA * inB);
		return ExecPin("Out");
	}

	return Error("Something went wrong multiplying A and B!");
}

IMPLEMENT_GRAPH_NODE(SGNode_Vector3Division, ScriptGraphNode);

SGNode_Vector3Division::SGNode_Vector3Division()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<Crimson::Vector3f>("A", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("B", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

NodeResult SGNode_Vector3Division::DoOperation()
{
	Crimson::Vector3f inA;
	Crimson::Vector3f inB;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		SetPinData("Result", inA / inB);
		return ExecPin("Out");
	}

	return Error("Something went wrong dividing A and B!");
}

IMPLEMENT_GRAPH_NODE(SGNode_Vector3Abs, ScriptGraphNode);

SGNode_Vector3Abs::SGNode_Vector3Abs()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<Crimson::Vector3f>("Value", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

NodeResult SGNode_Vector3Abs::DoOperation()
{
	Crimson::Vector3f value;

	if (GetPinData("Value", value))
	{
		SetPinData("Result", value.Abs());
		return ExecPin("Out");
	}

	return Error("Something went wrong getting absolute value!");
}

IMPLEMENT_GRAPH_NODE(SGNode_Vector3Clamp, ScriptGraphNode);

SGNode_Vector3Clamp::SGNode_Vector3Clamp()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<Crimson::Vector3f>("Value", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Min", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Max", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

NodeResult SGNode_Vector3Clamp::DoOperation()
{
	Crimson::Vector3f min;
	Crimson::Vector3f max;
	Crimson::Vector3f value;

	if (GetPinData("Min", min) && GetPinData("Max", max) && GetPinData("Value", value))
	{
		SetPinData("Result", value.Clamp(min, max));
		return ExecPin("Out");
	}

	return Error("Something went wrong clamping value!");
}

IMPLEMENT_GRAPH_NODE(SGNode_Vector3ClampMagnitude, ScriptGraphNode);

SGNode_Vector3ClampMagnitude::SGNode_Vector3ClampMagnitude()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<Crimson::Vector3f>("Value", PinDirection::Input);
	CreateDataPin<float>("Max length", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

NodeResult SGNode_Vector3ClampMagnitude::DoOperation()
{
	float max = 0.f;
	Crimson::Vector3f value;

	if (GetPinData("Max length", max) && GetPinData("Value", value))
	{
		SetPinData("Result", value.ClampMagnitude(max));
		return ExecPin("Out");
	}

	return Error("Something went wrong clamping magnitude of value!");
}

IMPLEMENT_GRAPH_NODE(SGNode_Vector3Lerp, ScriptGraphNode);

SGNode_Vector3Lerp::SGNode_Vector3Lerp()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<Crimson::Vector3f>("From", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("To", PinDirection::Input);
	CreateDataPin<float>("Percentage", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

NodeResult SGNode_Vector3Lerp::DoOperation()
{
	Crimson::Vector3f from;
	Crimson::Vector3f to;
	float percentage = 0.f;

	if (GetPinData("From", from) && GetPinData("To", to) && GetPinData("Percentage", percentage))
	{
		SetPinData("Result", Crimson::Lerp(from, to, percentage));
		return ExecPin("Out");
	}

	return Error("Something went wrong when trying to lerp value!");
}

IMPLEMENT_GRAPH_NODE(SGNode_Vector3Distance, ScriptGraphNode);

SGNode_Vector3Distance::SGNode_Vector3Distance()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<Crimson::Vector3f>("A", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("B", PinDirection::Input);
	CreateDataPin<float>("Result", PinDirection::Output);
}

NodeResult SGNode_Vector3Distance::DoOperation()
{
	Crimson::Vector3f inA;
	Crimson::Vector3f inB;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		SetPinData("Result", (inA - inB).Length());
		return ExecPin("Out");
	}

	return Error("Something went wrong getting distance between A and B!");
}

IMPLEMENT_GRAPH_NODE(SGNode_Vector3Dot, ScriptGraphNode);

SGNode_Vector3Dot::SGNode_Vector3Dot()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<Crimson::Vector3f>("A", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("B", PinDirection::Input);
	CreateDataPin<float>("Result", PinDirection::Output);
}

NodeResult SGNode_Vector3Dot::DoOperation()
{
	Crimson::Vector3f inA;
	Crimson::Vector3f inB;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		SetPinData("Result", inA.Dot(inB));
		return ExecPin("Out");
	}

	return Error("Something went wrong calculating dot of A and B!");
}


IMPLEMENT_GRAPH_NODE(SGNode_Vector3Cross, ScriptGraphNode);

SGNode_Vector3Cross::SGNode_Vector3Cross()
{
	CreateExecPin("In", PinDirection::Input);
	CreateExecPin("Out", PinDirection::Output);

	CreateDataPin<Crimson::Vector3f>("A", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("B", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

NodeResult SGNode_Vector3Cross::DoOperation()
{
	Crimson::Vector3f inA;
	Crimson::Vector3f inB;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		SetPinData("Result", inA.Cross(inB));
		return ExecPin("Out");
	}

	return Error("Something went wrong calculating cross of A and B!");
}
