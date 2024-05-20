#pragma once
#include "Types/TypeRegistry.h"

#pragma comment(linker, "/include:__MuninGraph_AutoRegTypes")

//~ Internal type to handle Owner variable on ScriptGraphs.
DECLARE_REGISTERED_TYPE(void*);
//~

DECLARE_REGISTERED_TYPE(float);
DECLARE_REGISTERED_TYPE(std::string);