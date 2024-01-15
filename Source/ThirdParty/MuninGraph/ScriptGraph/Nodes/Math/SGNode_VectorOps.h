#pragma once
#include "ScriptGraph/ScriptGraphNode.h"

BeginScriptGraphNode(MVNode_MakeVector)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Make Vector"; }
	std::string GetDescription() const override { return "Creates a Vector from 3 floats."; }
	std::string GetNodeCategory() const override { return "Vector"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(MVNode_BreakVector)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Break Vector"; }
	std::string GetDescription() const override { return "Splits a Vector into 3 floats."; }
	std::string GetNodeCategory() const override { return "Vector"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_MathAddVec)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Add"; }
	std::string GetDescription() const override { return "Calculates the addition between two Vectors."; }
	std::string GetNodeCategory() const override { return "Vector"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_MathSubVec)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Subtract"; }
	std::string GetDescription() const override { return "Calculates the subtraction between two Vectors."; }
	std::string GetNodeCategory() const override { return "Vector"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_MathMulVec)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Multiplication"; }
	std::string GetDescription() const override { return "Calculates the multiplication between two Vectors."; }
	std::string GetNodeCategory() const override { return "Vector"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_MathDivVec)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Division"; }
	std::string GetDescription() const override { return "Calculates the division between two Vectors."; }
	std::string GetNodeCategory() const override { return "Vector"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};