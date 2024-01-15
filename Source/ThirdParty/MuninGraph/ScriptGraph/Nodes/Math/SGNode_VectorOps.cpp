#include "MuninGraph.pch.h"
#include "SGNode_VectorOps.h"
#include "Math/Vector3.hpp"

void MVNode_MakeVector::Init()
{
	CreateDataPin<float>("X", PinDirection::Input);
	CreateDataPin<float>("Y", PinDirection::Input);
	CreateDataPin<float>("Z", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

size_t MVNode_MakeVector::DoOperation()
{
	float X = 0, Y = 0, Z = 0;
	if (GetPinData("X", X) && GetPinData("Y", Y) && GetPinData("Z", Z))
	{
		Crimson::Vector3f result(X, Y, Z);
		SetPinData("Result", result);
		return Exit();
	}

	return 0;
}

void MVNode_BreakVector::Init()
{
	CreateDataPin<Crimson::Vector3f>("Vector", PinDirection::Input);
	CreateDataPin<float>("X", PinDirection::Output);
	CreateDataPin<float>("Y", PinDirection::Output);
	CreateDataPin<float>("Z", PinDirection::Output);
}

size_t MVNode_BreakVector::DoOperation()
{
	Crimson::Vector3f vector;
	if (GetPinData("Vector", vector))
	{
		SetPinData("X", vector.x);
		SetPinData("Y", vector.y);
		SetPinData("Z", vector.z);
		return Exit();
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

	if (GetPinData("Vector A", inA) && GetPinData("Vector B", inB))
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

	if (GetPinData("Vector A", inA) && GetPinData("Vector B", inB))
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

	if (GetPinData("Vector A", inA) && GetPinData("Vector B", inB))
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

	if (GetPinData("Vector A", inA) && GetPinData("Vector B", inB))
	{
		const Crimson::Vector3f result = inA / inB;
		SetPinData("Result", result);
		return ExitViaPin("Out");
	}

	return 0;
}