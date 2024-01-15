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

BeginScriptGraphNode(SGNode_MathCos)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Cos"; }
	std::string GetNodeCategory() const override { return "Math"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_MathSin)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Sin"; }
	std::string GetNodeCategory() const override { return "Math"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_MathAbs)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Absolute"; }
	std::string GetDescription() const override { return "Returns the positive value of the input."; }
	std::string GetNodeCategory() const override { return "Math"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_MathATan2)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "ATan2"; }
	std::string GetNodeCategory() const override { return "Math"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};