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

	return ExitWithError("Invalid input!");
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

	return ExitWithError("Invalid input!");
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

	return ExitWithError("Invalid input!");
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

	return ExitWithError("Invalid input!");
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

	return ExitWithError("Invalid input!");
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

	return ExitWithError("Invalid input!");
}

void SGNode_MathAbsVec::Init()
{
	CreateDataPin<Crimson::Vector3f>("Vector", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

size_t SGNode_MathAbsVec::DoOperation()
{
	Crimson::Vector3f vec = Crimson::Vector3f::Null;

	if (GetPinData("Vector", vec))
	{
		const Crimson::Vector3f result = vec.Abs();
		SetPinData("Result", result);
		return Exit();
	}

	return ExitWithError("Invalid input!");
}

void SGNode_MathClampVec::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<Crimson::Vector3f>("Value", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Min", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Max", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

size_t SGNode_MathClampVec::DoOperation()
{
	Crimson::Vector3f min = Crimson::Vector3f::Null;
	Crimson::Vector3f max = Crimson::Vector3f::Null;
	Crimson::Vector3f value = Crimson::Vector3f::Null;

	if (GetPinData("Min", min) && GetPinData("Max", max) && GetPinData("Value", value))
	{
		const Crimson::Vector3f result = value.Clamp(min, max);
		SetPinData("Result", result);
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}

void SGNode_MathClampMagnitudeVec::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<Crimson::Vector3f>("Value", PinDirection::Input);
	CreateDataPin<float>("Max length", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

size_t SGNode_MathClampMagnitudeVec::DoOperation()
{
	float max = 0.f;
	Crimson::Vector3f value = Crimson::Vector3f::Null;

	if (GetPinData("Max length", max) && GetPinData("Value", value))
	{
		const Crimson::Vector3f result = value.ClampMagnitude(max);
		SetPinData("Result", result);
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}

void SGNode_MathLerpVec::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<Crimson::Vector3f>("Value", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("From", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("To", PinDirection::Input);
	CreateDataPin<float>("Percentage", PinDirection::Input);

	CreateDataPin<Crimson::Vector3f>("Result", PinDirection::Output);
}

size_t SGNode_MathLerpVec::DoOperation()
{
	Crimson::Vector3f value = Crimson::Vector3f::Null;
	Crimson::Vector3f from = Crimson::Vector3f::Null;
	Crimson::Vector3f to = Crimson::Vector3f::Null;
	float percentage = 0.f;

	if (GetPinData("Value", value) && GetPinData("From", from) && GetPinData("To", to) && GetPinData("Percentage", percentage))
	{
		const Crimson::Vector3f result = value.Lerp(from, to, percentage);
		SetPinData("Result", result);
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}

void SGNode_MathDistanceVec::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<Crimson::Vector3f>("Vector A", PinDirection::Input);
	CreateDataPin<Crimson::Vector3f>("Vector B", PinDirection::Input);

	CreateDataPin<float>("Result", PinDirection::Output);
}

size_t SGNode_MathDistanceVec::DoOperation()
{
	Crimson::Vector3f inA = Crimson::Vector3f::Null;
	Crimson::Vector3f inB = Crimson::Vector3f::Null;

	if (GetPinData("Vector A", inA) && GetPinData("Vector B", inB))
	{
		const float result = (inA - inB).Length();
		SetPinData("Result", result);
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}