#pragma once
#include "Types/TypeRegistry.h"
#include "CrimsonUtilities/Math/Vector3.hpp"

#pragma comment(linker, "/include:__MuninGraph_AutoRegTypes")

//~ Internal type to handle Owner variable on ScriptGraphs.
DECLARE_REGISTERED_TYPE(void*);
//~

DECLARE_REGISTERED_TYPE(bool);
DECLARE_REGISTERED_TYPE(int);
DECLARE_REGISTERED_TYPE(unsigned);
DECLARE_REGISTERED_TYPE(float);

DECLARE_REGISTERED_TYPE(Crimson::Vector3f);
DECLARE_REGISTERED_TYPE(std::string);