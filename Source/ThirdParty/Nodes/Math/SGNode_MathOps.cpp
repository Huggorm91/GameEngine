#include "MuninGraph.pch.h"
#include "SGNode_MathOps.h"

void SGNode_MathAdd::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<float>("A", PinDirection::Input);
	CreateDataPin<float>("B", PinDirection::Input);

	CreateDataPin<float>("Result", PinDirection::Output);
}

size_t SGNode_MathAdd::DoOperation()
{
	float inA = 0;
	float inB = 0;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		const float result = inA + inB;
		SetPinData("Result", result);
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}

void SGNode_MathSub::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<float>("A", PinDirection::Input);
	CreateDataPin<float>("B", PinDirection::Input);

	CreateDataPin<float>("Result", PinDirection::Output);
}

size_t SGNode_MathSub::DoOperation()
{
	float inA = 0;
	float inB = 0;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		const float result = inA - inB;
		SetPinData("Result", result);
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}

void SGNode_MathMul::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<float>("A", PinDirection::Input);
	CreateDataPin<float>("B", PinDirection::Input);

	CreateDataPin<float>("Result", PinDirection::Output);
}

size_t SGNode_MathMul::DoOperation()
{
	float inA = 0;
	float inB = 0;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		const float result = inA * inB;
		SetPinData("Result", result);
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}

void SGNode_MathDiv::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<float>("A", PinDirection::Input);
	CreateDataPin<float>("B", PinDirection::Input);

	CreateDataPin<float>("Result", PinDirection::Output);
}

size_t SGNode_MathDiv::DoOperation()
{
	float inA = 0;
	float inB = 0;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		const float result = inA / inB;
		SetPinData("Result", result);
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}

void SGNode_MathCos::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<float>("Value", PinDirection::Input);

	CreateDataPin<float>("Result", PinDirection::Output);
}

size_t SGNode_MathCos::DoOperation()
{
	float inA = 0;

	if (GetPinData("Value", inA))
	{
		const float result = cosf(inA);
		SetPinData("Result", result);
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}

void SGNode_MathSin::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<float>("Value", PinDirection::Input);

	CreateDataPin<float>("Result", PinDirection::Output);
}

size_t SGNode_MathSin::DoOperation()
{
	float inA = 0;

	if (GetPinData("Value", inA))
	{
		const float result = sinf(inA);
		SetPinData("Result", result);
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}

void SGNode_MathAbs::Init()
{
	CreateDataPin<float>("Value", PinDirection::Input);

	CreateDataPin<float>("Result", PinDirection::Output);
}

size_t SGNode_MathAbs::DoOperation()
{
	float inA = 0;

	if (GetPinData("Value", inA))
	{
		const float result = std::abs(inA);
		SetPinData("Result", result);
		return Exit();
	}

	return ExitWithError("Invalid input!");
}

void SGNode_MathATan2::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<float>("X", PinDirection::Input);
	CreateDataPin<float>("Y", PinDirection::Input);

	CreateDataPin<float>("Result", PinDirection::Output);
}

size_t SGNode_MathATan2::DoOperation()
{
	float x = 0;
	float y = 0;

	if (GetPinData("X", x) && GetPinData("Y", y))
	{
		const float result = atan2f(y, x);
		SetPinData("Result", result);
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}