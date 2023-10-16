#pragma once
#include "Math/Matrix4x4.hpp"
#include <TgaFbxStructs.h>

Crimson::Matrix4x4f ConvertMatrix(const TGA::FBX::Matrix& aMatrix);
Crimson::Matrix4x4f ConvertTransposedMatrix(const TGA::FBX::Matrix& aMatrix);