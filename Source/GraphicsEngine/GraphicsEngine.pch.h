// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef GRAPHICSENGINE_PCH
#define GRAPHICSENGINE_PCH

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX

// add headers that you want to pre-compile here
#include <Windows.h>
#include <d3d11.h>
#include <string>
#include <array>
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <wrl.h>
using namespace Microsoft::WRL;

#include "InterOp/RHI.h"
#include "Math/Matrix4x4.hpp"
#include "Logging/Logging.h"

inline Logger GELogger;
#endif //GRAPHICSENGINE_PCH