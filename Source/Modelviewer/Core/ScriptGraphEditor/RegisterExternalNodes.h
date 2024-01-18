#pragma once
#include "MuninScriptGraph.h"

// Example file for how to register nodes in another project, i.e. not inside MuninGraph.lib.
// To get all the nodes inside MuninGraph in your project to auto-register you need to link
// with /WHOLEARCHIVE:MuninGraph.lib or VS will optimize the auto reg away. Similarly if you
// have this file in a lib of your own you need to /WHOLEARCHIVE that file too. If this lives
// in an EXE project then it'll work as intended on its own.

// NOTE THE INCLUDE!

BeginScriptGraphNode(MVNode_Branch)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "Branch"; }
	size_t DoOperation() override;
};

BeginScriptGraphNode(MVNode_ForLoop)
{
public:
	void Init() override;
	std::string GetNodeTitle() const override { return "For"; }
	size_t DoOperation() override;
};