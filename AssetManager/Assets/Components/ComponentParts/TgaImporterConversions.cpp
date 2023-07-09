#include "AssetManager.pch.h"
#include "TgaImporterConversions.h"

CommonUtilities::Matrix4x4f ConvertMatrix(const TGA::FBX::Matrix& aMatrix)
{
	return CommonUtilities::Matrix4x4f{{ aMatrix.m11, aMatrix.m21, aMatrix.m31, aMatrix.m41 },
									   { aMatrix.m12, aMatrix.m22, aMatrix.m32, aMatrix.m42 },
									   { aMatrix.m13, aMatrix.m23, aMatrix.m33, aMatrix.m43 },
									   { aMatrix.m14, aMatrix.m24, aMatrix.m34, aMatrix.m44 }};
}

CommonUtilities::Matrix4x4f ConvertTransposedMatrix(const TGA::FBX::Matrix& aMatrix)
{
	return CommonUtilities::Matrix4x4f{{ aMatrix.m11, aMatrix.m12, aMatrix.m13, aMatrix.m14 },
									   { aMatrix.m21, aMatrix.m22, aMatrix.m23, aMatrix.m24 },
									   { aMatrix.m31, aMatrix.m32, aMatrix.m33, aMatrix.m34 },
									   { aMatrix.m41, aMatrix.m42, aMatrix.m43, aMatrix.m44 }};
}
