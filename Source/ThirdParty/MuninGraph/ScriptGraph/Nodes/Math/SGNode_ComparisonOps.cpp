#include "MuninGraph.pch.h"
#include "SGNode_ComparisonOps.h"

void SGNode_Not::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<bool>("Value", PinDirection::Input, true);

	CreateDataPin<bool>("Result", PinDirection::Output, true);
}

size_t SGNode_Not::DoOperation()
{
	bool value = false;
	if (GetPinData("Value", value))
	{
		SetPinData("Result", !value);
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}

void SGNode_CompFloatLess::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<float>("Value", PinDirection::Input);
	CreateDataPin<float>("Comparison", PinDirection::Input);

	CreateDataPin<bool>("Result", PinDirection::Output);
}

size_t SGNode_CompFloatLess::DoOperation()
{
	float value = 0.f, comparison = 0.f;
	if (GetPinData("Value", value) && GetPinData("Comparison", comparison))
	{
		SetPinData("Result", value < comparison);
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}

void SGNode_CompFloatGreater::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<float>("Value", PinDirection::Input);
	CreateDataPin<float>("Comparison", PinDirection::Input);

	CreateDataPin<bool>("Result", PinDirection::Output);
}

size_t SGNode_CompFloatGreater::DoOperation()
{
	float value = 0.f, comparison = 0.f;
	if (GetPinData("Value", value) && GetPinData("Comparison", comparison))
	{
		SetPinData("Result", value > comparison);
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}

void SGNode_CompFloatEqual::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<float>("Value", PinDirection::Input);
	CreateDataPin<float>("Comparison", PinDirection::Input);

	CreateDataPin<bool>("Result", PinDirection::Output);
}

size_t SGNode_CompFloatEqual::DoOperation()
{
	float value = 0.f, comparison = 0.f;
	if (GetPinData("Value", value) && GetPinData("Comparison", comparison))
	{
		SetPinData("Result", value == comparison);
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}

void SGNode_CompIntLess::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<int>("Value", PinDirection::Input);
	CreateDataPin<int>("Comparison", PinDirection::Input);

	CreateDataPin<bool>("Result", PinDirection::Output);
}

size_t SGNode_CompIntLess::DoOperation()
{
	int value = 0, comparison = 0;
	if (GetPinData("Value", value) && GetPinData("Comparison", comparison))
	{
		SetPinData("Result", value < comparison);
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}

void SGNode_CompIntGreater::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<int>("Value", PinDirection::Input);
	CreateDataPin<int>("Comparison", PinDirection::Input);

	CreateDataPin<bool>("Result", PinDirection::Output);
}

size_t SGNode_CompIntGreater::DoOperation()
{
	int value = 0, comparison = 0;
	if (GetPinData("Value", value) && GetPinData("Comparison", comparison))
	{
		SetPinData("Result", value > comparison);
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}

void SGNode_CompIntEqual::Init()
{
	CreateExecPin("In", PinDirection::Input, true);
	CreateExecPin("Out", PinDirection::Output, true);

	CreateDataPin<int>("Value", PinDirection::Input);
	CreateDataPin<int>("Comparison", PinDirection::Input);

	CreateDataPin<bool>("Result", PinDirection::Output);
}

size_t SGNode_CompIntEqual::DoOperation()
{
	int value = 0, comparison = 0;
	if (GetPinData("Value", value) && GetPinData("Comparison", comparison))
	{
		SetPinData("Result", value == comparison);
		return ExitViaPin("Out");
	}

	return ExitWithError("Invalid input!");
}