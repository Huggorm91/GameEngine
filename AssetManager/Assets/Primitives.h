#pragma once
#include "Components/ComponentParts/MeshElement.h"

MeshData CreateCubeMesh(const float aSize);
MeshData CreateCubeMesh(const CommonUtilities::Vector3f& aSize);

MeshData CreatePyramidMesh(const float aSize);
MeshData CreatePyramidMesh(const CommonUtilities::Vector3f& aSize);

MeshData CreateSphereMesh(const float aRadius, const int aSliceCount, const int aStackCount);
MeshData CreateSphereMesh2(const float aRadius, const int aSliceCount, const int aStackCount);
MeshData CreateSphereMesh3(const float aRadius, const int aSliceCount, const int aStackCount);
MeshData CreateSphereMesh4(const float aRadius, const int aSliceCount, const int aStackCount);
MeshData CreateSphereMesh5(const float aRadius, const int aSliceCount, const int aStackCount);

MeshData CreateInvertedCubeMesh(const float aSize);
MeshData CreateInvertedCubeMesh(const CommonUtilities::Vector3f& aSize);

MeshData CreateInvertedPyramidMesh(const float aSize);
MeshData CreateInvertedPyramidMesh(const CommonUtilities::Vector3f& aSize);

MeshData CreateInvertedSphereMesh(const float aRadius, const int aSliceCount, const int aStackCount);

void CreateTri(std::vector<unsigned int>& outIndices, unsigned int aPoint1, unsigned int aPoint2, unsigned int aPoint3);
void CreateQuad(std::vector<unsigned int>& outIndices, unsigned int aPoint1, unsigned int aPoint2, unsigned int aPoint3, unsigned int aPoint4);