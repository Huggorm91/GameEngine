#include "MuninGraph.pch.h"
#include "SGNode_MathOps.h"
#include "Math/Vector3.hpp"

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

	return 0;
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

	return 0;
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

	return 0;
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

	return 0;
}

void SGNode_MathAddVec::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<Crimson::Vector3f>("Vector A", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Vector B", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

size_t SGNode_MathAddVec::DoOperation()
{
	Crimson::Vector3f inA = Crimson::Vector3f::Null;
	Crimson::Vector3f inB = Crimson::Vector3f::Null;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		const Crimson::Vector3f result = inA + inB;
		SetPinData("Result", result);
		return ExitViaPin("Out");
	}

	return 0;
}

void SGNode_MathSubVec::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<Crimson::Vector3f>("Vector A", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Vector B", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

size_t SGNode_MathSubVec::DoOperation()
{
	Crimson::Vector3f inA = Crimson::Vector3f::Null;
	Crimson::Vector3f inB = Crimson::Vector3f::Null;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		const Crimson::Vector3f result = inA - inB;
		SetPinData("Result", result);
		return ExitViaPin("Out");
	}

	return 0;
}

void SGNode_MathMulVec::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<Crimson::Vector3f>("Vector A", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Vector B", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

size_t SGNode_MathMulVec::DoOperation()
{
	Crimson::Vector3f inA = Crimson::Vector3f::Null;
	Crimson::Vector3f inB = Crimson::Vector3f::Null;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		const Crimson::Vector3f result = inA * inB;
		SetPinData("Result", result);
		return ExitViaPin("Out");
	}

	return 0;
}

void SGNode_MathDivVec::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<Crimson::Vector3f>("Vector A", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Vector B", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

size_t SGNode_MathDivVec::DoOperation()
{
	Crimson::Vector3f inA = Crimson::Vector3f::Null;
	Crimson::Vector3f inB = Crimson::Vector3f::Null;

	if (GetPinData("A", inA) && GetPinData("B", inB))
	{
		const Crimson::Vector3f result = inA / inB;
		SetPinData("Result", result);
		return ExitViaPin("Out");
	}

	return 0;
}