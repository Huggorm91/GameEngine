#pragma once
#include <Matrix4x4.hpp>
#include <TgaFbxStructs.h>

CommonUtilities::Matrix4x4f ConvertMatrix(const TGA::FBX::Matrix& aMatrix);
CommonUtilities::Matrix4x4f ConvertTransposedMatrix(const TGA::FBX::Matrix& aMatrix);