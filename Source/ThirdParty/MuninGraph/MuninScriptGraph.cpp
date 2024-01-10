#include "MuninGraph.pch.h"
#include "MuninScriptGraph.h"
#include "ScriptGraph/Types/RegisterScriptGraphTypes.h"
#include "ScriptGraph/Nodes/RegisterScriptGraphNodes.h"

// This function being here forces the initialization of this specific
// translation unit. This causes all the Registration stuff in the
// RegisterScriptGraphTypes.h and RegisterScriptGraphNodes.h to run
// as long as MuninScriptGraph.h is included somewhere in the final
// artifact.
extern "C" void __MSG_Module() { }