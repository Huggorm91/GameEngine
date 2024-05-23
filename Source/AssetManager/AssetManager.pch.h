// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef ASSETMANAGER_PCH_H
#define ASSETMANAGER_PCH_H

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
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
using namespace Microsoft::WRL;

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/misc/cpp/imgui_stdlib.h"

#include "CrimsonUtilities/Time/Timer.h"
#include "CrimsonUtilities/Math/Matrix3x3.hpp"
#include "CrimsonUtilities/Json/JsonVector.hpp"
#include "CrimsonUtilities/File/DirectoryFunctions.h"
#include "CrimsonUtilities/Math/QuaternionTransform.h"

#include "Logging/Logging.h"
inline Logger AMLogger;
#endif //ASSETMANAGER_PCH_H
