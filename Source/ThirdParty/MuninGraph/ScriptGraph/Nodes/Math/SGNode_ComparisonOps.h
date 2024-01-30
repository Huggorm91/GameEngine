#pragma once
#include "ScriptGraph/ScriptGraphNode.h"

BeginScriptGraphNode(SGNode_Not)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Not"; }
	std::string GetDescription() const override { return "Inverts a bool."; }
	std::string GetNodeCategory() const override { return "Compare"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_CompFloatLess)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Less than"; }
	std::string GetDescription() const override { return "Check if a float is less than another float."; }
	std::string GetNodeCategory() const override { return "Compare Float"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_CompFloatGreater)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Greater than"; }
	std::string GetDescription() const override { return "Check if a float is greater than another float."; }
	std::string GetNodeCategory() const override { return "Compare Float"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_CompFloatEqual)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Equals"; }
	std::string GetDescription() const override { return "Check if a float is equal to another float."; }
	std::string GetNodeCategory() const override { return "Compare Float"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_CompIntLess)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Less than"; }
	std::string GetDescription() const override { return "Check if an int is less than another int."; }
	std::string GetNodeCategory() const override { return "Compare Int"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_CompIntGreater)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Greater than"; }
	std::string GetDescription() const override { return "Check if an int is greater than another int."; }
	std::string GetNodeCategory() const override { return "Compare Int"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};

BeginScriptGraphNode(SGNode_CompIntEqual)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Equals"; }
	std::string GetDescription() const override { return "Check if an int is equal to another int."; }
	std::string GetNodeCategory() const override { return "Compare Int"; }
	size_t DoOperation() override;
	bool IsSimpleNode() const override { return false; }
};