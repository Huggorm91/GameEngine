#pragma once
#include "ScriptGraph/ScriptGraphNode.h"

BeginScriptGraphNode(SGNode_MathAdd)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Add"; }
	std::string GetDescription() const override { return "Calculates the addition between two float values."; }
	std::string GetNodeCategory() const override { return "Math"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_MathSub)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Subtract"; }
	std::string GetDescription() const override { return "Calculates the subtraction between two float values."; }
	std::string GetNodeCategory() const override { return "Math"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_MathMul)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Multiplication"; }
	std::string GetDescription() const override { return "Calculates the multiplication between two float values."; }
	std::string GetNodeCategory() const override { return "Math"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_MathDiv)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Division"; }
	std::string GetDescription() const override { return "Calculates the division between two float values."; }
	std::string GetNodeCategory() const override { return "Math"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_MathAddVec)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Add"; }
	std::string GetDescription() const override { return "Calculates the addition between two Vectors."; }
	std::string GetNodeCategory() const override { return "Vector Math"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_MathSubVec)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Subtract"; }
	std::string GetDescription() const override { return "Calculates the subtraction between two Vectors."; }
	std::string GetNodeCategory() const override { return "Vector Math"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_MathMulVec)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Multiplication"; }
	std::string GetDescription() const override { return "Calculates the multiplication between two Vectors."; }
	std::string GetNodeCategory() const override { return "Vector Math"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_MathDivVec)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Division"; }
	std::string GetDescription() const override { return "Calculates the division between two Vectors."; }
	std::string GetNodeCategory() const override { return "Vector Math"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};