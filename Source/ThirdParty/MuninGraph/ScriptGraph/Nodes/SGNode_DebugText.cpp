﻿#include "MuninGraph.pch.h"
#include "SGNode_DebugText.h"

#include <iostream>

static const std::string InExecPinLabel("In");
static const std::string OutExecPinLabel("Out");

void SGNode_DebugText::Init()
{
	CreateExecPin(InExecPinLabel, PinDirection::Input, true);
	CreateExecPin(OutExecPinLabel, PinDirection::Output, true);

	CreateDataPin<std::string>("Text", PinDirection::Input);

	SetPinData<std::string>("Text", "Test");
}

size_t SGNode_DebugText::DoOperation()
{
#ifndef _RETAIL
	std::string msg;
	if(GetPinData("Text", msg))
	{
		std::cout << msg << std::endl;
	}
#endif

	return ExitViaPin("Out");
}

GraphColor SGNode_DebugText::GetNodeHeaderColor() const
{
	return GraphColor(200, 150, 0, 255);
}
