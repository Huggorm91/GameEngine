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
#include "Logging/Logging.h"
#include "AssetManager/AssetManager.h"
#include "File/DirectoryFunctions.h"

#ifndef _RETAIL
#include "ThirdParty/DearImGui/ImGui/imgui.h"
#include "ThirdParty/DearImGui/ImGui/imgui_impl_win32.h"
#include "ThirdParty/DearImGui/ImGui/imgui_impl_dx11.h"
#endif // !_RETAIL

#endif //MODELVIEWER_PCH_H