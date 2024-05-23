// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef GAMEPLAYENGINE_PCH_H
#define GAMEPLAYENGINE_PCH_H

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
#include <wrl.h>
using namespace Microsoft::WRL;

#include "AssetManager/AssetManager.h"

#include "CrimsonUtilities/Math/Matrix3x3.hpp"
#include "CrimsonUtilities/Json/JsonVector.hpp"
#include "CrimsonUtilities/File/DirectoryFunctions.h"

#endif //GAMEPLAYENGINE_PCH_H
