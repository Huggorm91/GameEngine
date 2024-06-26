// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef MODELVIEWER_PCH_H
#define MODELVIEWER_PCH_H

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX

// add headers that you want to pre-compile here
#include <fstream>
#include <string>
#include <array>
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <d3d11.h>
#include <wrl.h>
using namespace Microsoft::WRL;

#include "Logging/Logging.h"
#include "AssetManager/AssetManager.h"
#include "File/DirectoryFunctions.h"

#ifndef _RETAIL
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#pragma warning(push, 1)
#pragma warning(disable:4244)
#include "ThirdParty/MuninGraph/MuninGraph.h"
#include "ThirdParty/ScriptGraphNodes/ScriptGraphNodes.h"
#include "ThirdParty/MuninGraphEditor/ScriptGraphEditor/ScriptGraphEditor.h"
#pragma warning(pop)
#endif // !_RETAIL

#endif //MODELVIEWER_PCH_H