#include "AssetManager.pch.h"
#include "Primitives.h"
#include <Random.h>
#include <math.h>
#include <Matrix3x3.hpp>

struct VertexInfo
{
	CommonUtilities::Vector3f Position;
	CommonUtilities::Vector4f Color;
};

MeshData CreateCubeMesh(const float aSize)
{
	return CreateCubeMesh({ aSize, aSize, aSize });
}

MeshData CreateCubeMesh(const CommonUtilities::Vector3f& aSize)
{
	MeshData result;
	result.myMeshName = "Cube";
	CommonUtilities::Vector3f halfSize = aSize * 0.5f;

	// up = Up | do = Down | Ri = Right | Le = Left | Ba = Back | Fr = Front
	CommonUtilities::Vector3f upRiFr = { halfSize.x, aSize.y, -halfSize.z };
	CommonUtilities::Vector3f upRiBa = { halfSize.x, aSize.y, halfSize.z };

	CommonUtilities::Vector3f upLeFr = { -halfSize.x, aSize.y, -halfSize.z };
	CommonUtilities::Vector3f upLeBa = { -halfSize.x, aSize.y, halfSize.z };

	CommonUtilities::Vector3f doRiFr = { halfSize.x, 0.f, -halfSize.z };
	CommonUtilities::Vector3f doRiBa = { halfSize.x, 0.f, halfSize.z };

	CommonUtilities::Vector3f doLeFr = { -halfSize.x, 0.f, -halfSize.z };
	CommonUtilities::Vector3f doLeBa = { -halfSize.x, 0.f, halfSize.z };

	std::vector<Vertex> mdlVertices = {
		// Top plane
		{
			upRiBa, {1.f, 0.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
		},
		{
			upRiFr, {1.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
		},
		{
			upLeFr, {0.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
		},
		{
			upLeBa, {0.f, 0.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
		},
		// Bottom plane
		{
			doRiFr, {0.f, 0.f}, {0.f, -1.f, 0.f}, {-1.f, 0.f, 0.f}
		},
		{
			doRiBa, {0.f, 1.f}, {0.f, -1.f, 0.f}, {-1.f, 0.f, 0.f}
		},
		{
			doLeBa, {1.f, 1.f}, {0.f, -1.f, 0.f}, {-1.f, 0.f, 0.f}
		},
		{
			doLeFr, {1.f, 0.f}, {0.f, -1.f, 0.f}, {-1.f, 0.f, 0.f}
		},
		// Right plane
		{
			doRiBa, {1.f, 1.f}, {1.f, 0.f, 0.f}, {0.f, 0.f, 1.f}
		},
		{
			doRiFr, {0.f, 1.f}, {1.f, 0.f, 0.f}, {0.f, 0.f, 1.f}
		},
		{
			upRiFr, {0.f, 0.f}, {1.f, 0.f, 0.f}, {0.f, 0.f, 1.f}
		},
		{
			upRiBa, {1.f, 0.f}, {1.f, 0.f, 0.f}, {0.f, 0.f, 1.f}
		},
		// Left plane
		{
			doLeFr, {1.f, 1.f}, {-1.f, 0.f, 0.f}, {0.f, 0.f, -1.f}
		},
		{
			doLeBa, {0.f, 1.f}, {-1.f, 0.f, 0.f}, {0.f, 0.f, -1.f}
		},
		{
			upLeBa, {0.f, 0.f}, {-1.f, 0.f, 0.f}, {0.f, 0.f, -1.f}
		},
		{
			upLeFr, {1.f, 0.f}, {-1.f, 0.f, 0.f}, {0.f, 0.f, -1.f}
		},
		// Back plane
		{
			doRiBa, {0.f, 1.f}, {0.f, 0.f, 1.f}, {-1.f, 0.f, 0.f}
		},
		{
			upRiBa, {0.f, 0.f}, {0.f, 0.f, 1.f}, {-1.f, 0.f, 0.f}
		},
		{
			upLeBa, {1.f, 0.f}, {0.f, 0.f, 1.f}, {-1.f, 0.f, 0.f}
		},
		{
			doLeBa, {1.f, 1.f}, {0.f, 0.f, 1.f}, {-1.f, 0.f, 0.f}
		},
		// Front plane
		{
			upRiFr, {1.f, 0.f}, {0.f, 0.f, -1.f}, {1.f, 0.f, 0.f}
		},
		{
			doRiFr, {1.f, 1.f}, {0.f, 0.f, -1.f}, {1.f, 0.f, 0.f}
		},
		{
			doLeFr, {0.f, 1.f}, {0.f, 0.f, -1.f}, {1.f, 0.f, 0.f}
		},
		{
			upLeFr, {0.f, 0.f}, {0.f, 0.f, -1.f}, {1.f, 0.f, 0.f}
		}
	};

	if (!RHI::CreateVertexBuffer(result.myVertexBuffer, mdlVertices))
	{
		AMLogger.Err("Could not create a vertex buffer for Cube!");
		return result;
	}

	std::vector<unsigned int> mdlIndices{};
	CreateQuad(mdlIndices, 0, 1, 2, 3);
	CreateQuad(mdlIndices, 4, 5, 6, 7);
	CreateQuad(mdlIndices, 8, 9, 10, 11);
	CreateQuad(mdlIndices, 12, 13, 14, 15);
	CreateQuad(mdlIndices, 16, 17, 18, 19);
	CreateQuad(mdlIndices, 20, 21, 22, 23);

	if (!RHI::CreateIndexBuffer(result.myIndexBuffer, mdlIndices))
	{
		AMLogger.Err("Could not create a index buffer for Cube!");
		return result;
	}

	result.myVertexCount = static_cast<UINT>(mdlVertices.size());
	result.myIndexCount = static_cast<UINT>(mdlIndices.size());
	result.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	result.myStride = sizeof(Vertex);

	return result;
}

MeshData CreatePyramidMesh(const float aSize)
{
	return CreatePyramidMesh({ aSize, aSize, aSize });
}

MeshData CreatePyramidMesh(const CommonUtilities::Vector3f& aSize)
{
	MeshData result;
	result.myMeshName = "Pyramid";
	CommonUtilities::Vector3f halfSize = aSize * 0.5f;

	CommonUtilities::Vector3f Top = { 0.f, aSize.y, 0.f };
	CommonUtilities::Vector3f BackLeft = { -halfSize.x, 0.f, halfSize.z };
	CommonUtilities::Vector3f BackRight = { halfSize.x, 0.f, halfSize.z };
	CommonUtilities::Vector3f FrontLeft = { -halfSize.x, 0.f, -halfSize.z };
	CommonUtilities::Vector3f FrontRight = { halfSize.x, 0.f, -halfSize.z };

	CommonUtilities::Vector3f backNormal = ((BackLeft - Top).Cross(BackRight - Top)).GetNormalized();
	CommonUtilities::Vector3f backTangent = (BackLeft - Top).GetNormalized();

	CommonUtilities::Vector3f frontNormal = ((FrontRight - Top).Cross(FrontLeft - Top)).GetNormalized();
	CommonUtilities::Vector3f frontTangent = (FrontRight - Top).GetNormalized();

	CommonUtilities::Vector3f rightNormal = ((BackRight - Top).Cross(FrontRight - Top)).GetNormalized();
	CommonUtilities::Vector3f rightTangent = (BackRight - Top).GetNormalized();

	CommonUtilities::Vector3f leftNormal = ((FrontLeft - Top).Cross(BackLeft - Top)).GetNormalized();
	CommonUtilities::Vector3f leftTangent = (FrontLeft - Top).GetNormalized();

	std::vector<Vertex> mdlVertices = {
		// Back Plane
		{
			Top, {0.f, 0.f}, backNormal, backTangent
		},
		{
			BackLeft, {1.f, 0.f}, backNormal, backTangent
		},
		{
			BackRight, {0.f, 1.f}, backNormal, backTangent
		},
		// Front Plane
		{
			Top, {0.f, 0.f}, frontNormal, frontTangent
		},
		{
			FrontRight, {1.f, 0.f}, frontNormal, frontTangent
		},
		{
			FrontLeft, {0.f, 1.f}, frontNormal, frontTangent
		},
		// Right Plane
		{
			Top, {0.f, 0.f}, rightNormal, rightTangent
		},
		{
			BackRight, {1.f, 0.f}, rightNormal, rightTangent
		},
		{
			FrontRight, {0.f, 1.f}, rightNormal, rightTangent
		},
		// Left Plane
		{
			Top, {0.f, 0.f}, leftNormal, leftTangent
		},
		{
			FrontLeft, {1.f, 0.f}, leftNormal, leftTangent
		},
		{
			BackLeft, {0.f, 1.f}, leftNormal, leftTangent
		},
		// Bottom Plane
		{
			FrontLeft, {0.f, 0.f}, {0.f, -1.f, 0.f}, {-1.f, 0.f, 0.f}
		},
		{
			FrontRight, {1.f, 0.f}, {0.f, -1.f, 0.f}, {-1.f, 0.f, 0.f}
		},
		{
			BackRight, {1.f, 1.f}, {0.f, -1.f, 0.f}, {-1.f, 0.f, 0.f}
		},
		{
			BackLeft, {0.f, 1.f}, {0.f, -1.f, 0.f}, {-1.f, 0.f, 0.f}
		}
	};

	if (!RHI::CreateVertexBuffer(result.myVertexBuffer, mdlVertices))
	{
		AMLogger.Err("Could not create a vertex buffer for Pyramid!");
		return result;
	}

	std::vector<unsigned int> mdlIndices{};
	CreateTri(mdlIndices, 0, 1, 2);
	CreateTri(mdlIndices, 3, 4, 5);
	CreateTri(mdlIndices, 6, 7, 8);
	CreateTri(mdlIndices, 9, 10, 11);
	CreateQuad(mdlIndices, 12, 13, 14, 15);

	if (!RHI::CreateIndexBuffer(result.myIndexBuffer, mdlIndices))
	{
		AMLogger.Err("Could not create a index buffer for Pyramid!");
		return result;
	}

	result.myVertexCount = static_cast<UINT>(mdlVertices.size());
	result.myIndexCount = static_cast<UINT>(mdlIndices.size());
	result.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	result.myStride = sizeof(Vertex);

	return result;
}

MeshData CreateSphereMesh4(const float aRadius, const int aParallelCount, const int aMeridianCount)
{
	MeshData result;
	result.myMeshName = "Sphere4";
	const float pi = 3.1415926535f;
	const float pi2 = 2.f * pi;
	float piInverse = 1.f / pi;
	float pi2Inverse = 1.f / pi2;
	auto random = []() {return CommonUtilities::Random::RandomNumber(1.0f); };

	// add top vertex
	std::vector<Vertex> mdlVertices;
	mdlVertices.emplace_back(CommonUtilities::Vector3f{ 0.f, aRadius * 2.f, 0.f }, CommonUtilities::Vector4f{ random(), random(), random(), 1.f }, CommonUtilities::Vector2f{ 0.5f, 0.f });

	CommonUtilities::Vector3f center{ 0.f, aRadius, 0.f };
	// generate vertices per stack / slice
	for (int i = 0; i < aMeridianCount - 1; i++)
	{
		float phi = pi * (i + 1) / aMeridianCount;
		for (int j = 0; j < aParallelCount; j++)
		{
			float theta = pi2 * j / aParallelCount;
			CommonUtilities::Vector3f pos;
			pos.x = aRadius * (std::sin(phi) * std::cos(theta));
			pos.y = aRadius * std::cos(phi) + aRadius;
			pos.z = aRadius * (std::sin(phi) * std::sin(theta));
			CommonUtilities::Vector3f directionToCenter = (center - pos).GetNormalized();
			mdlVertices.emplace_back(pos, CommonUtilities::Vector4f{ random(), random(), random(), 1.f }, CommonUtilities::Vector2f{ j == aParallelCount - 1 ? 1.f : .5f + (atan2(directionToCenter.z, directionToCenter.x) * pi2Inverse), i == aMeridianCount - 1 ? 1.f : .5f + asin(directionToCenter.y) * piInverse });
		}
	}

	// add bottom vertex
	mdlVertices.emplace_back(CommonUtilities::Vector3f{ 0.f, 0.f, 0.f }, CommonUtilities::Vector4f{ random(), random(), random(), 1.f }, CommonUtilities::Vector2f{ 0.5f, 1.f });

	if (!RHI::CreateVertexBuffer(result.myVertexBuffer, mdlVertices))
	{
		AMLogger.Err("Could not create a vertex buffer for Sphere!");
		return result;
	}

	// add top / bottom triangles
	const unsigned top = 0;
	const unsigned bottom = static_cast<unsigned int>(mdlVertices.size() - 1);
	std::vector<unsigned int> mdlIndices;
	for (int i = 0; i < aParallelCount; ++i)
	{
		auto i0 = i + 1;
		auto i1 = (i + 1) % aParallelCount + 1;
		CreateTri(mdlIndices, top, i1, i0);

		i0 = i + aParallelCount * (aMeridianCount - 2) + 1;
		i1 = (i + 1) % aParallelCount + aParallelCount * (aMeridianCount - 2) + 1;
		CreateTri(mdlIndices, bottom, i0, i1);
	}

	// add quads per stack / slice
	for (int j = 0; j < aMeridianCount - 2; j++)
	{
		auto j0 = j * aParallelCount + 1;
		auto j1 = (j + 1) * aParallelCount + 1;
		for (int i = 0; i < aParallelCount; i++)
		{
			auto i0 = j0 + i;
			auto i1 = j0 + (i + 1) % aParallelCount;
			auto i2 = j1 + (i + 1) % aParallelCount;
			auto i3 = j1 + i;
			CreateQuad(mdlIndices, i0, i1, i2, i3);
		}
	}

	if (!RHI::CreateIndexBuffer(result.myIndexBuffer, mdlIndices))
	{
		AMLogger.Err("Could not create a index buffer for Sphere!");
		return result;
	}

	result.myVertexCount = static_cast<UINT>(mdlVertices.size());
	result.myIndexCount = static_cast<UINT>(mdlIndices.size());
	result.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	result.myStride = sizeof(Vertex);

	return result;
}

MeshData CreateSphereMesh5(const float aRadius, const int aSlices, const int aStacks)
{
	MeshData result;
	result.myMeshName = "Sphere5";
	std::vector<Vertex> mdlVertices;

	float radiansPerStack = (180 / (aStacks)) * 0.0174532925f;
	float radiansPerSlice = (360 / aSlices) * 0.0174532925f;

	CommonUtilities::Matrix4x4f stackMatrix = CommonUtilities::Matrix4x4f::CreateRotationAroundX(radiansPerStack);
	CommonUtilities::Matrix4x4f sliceMatrix = CommonUtilities::Matrix4x4f::CreateRotationAroundY(radiansPerSlice);

	CommonUtilities::Vector4f direction(0, 1, 0, 0);
	CommonUtilities::Vector4f tangent(-1, 0, 0, 0);

	mdlVertices.emplace_back(CommonUtilities::Vector3f{ 0.f, aRadius * 2.f, 0.f }, CommonUtilities::Vector2f{ 0.5f, 0.f }, direction, CommonUtilities::Vector3f{ 1, 0, 0 });
	direction *= stackMatrix;

	for (int stack = 0; stack < aStacks - 1; stack++)
	{
		float UVY = ((float)stack + 1.0f) / (float)aStacks;
		for (int slice = 0; slice < aSlices; slice++)
		{
			float UVX = (((float)slice) / ((float)aSlices - 1.0f));

			CommonUtilities::Vector3f pos = direction * aRadius;
			pos.y += aRadius;
			mdlVertices.emplace_back(pos, CommonUtilities::Vector2f{ UVX, UVY }, direction, tangent);

			direction *= sliceMatrix;
			tangent *= sliceMatrix;
		}
		direction *= stackMatrix;
	}

	mdlVertices.emplace_back(CommonUtilities::Vector3f{ 0.f, 0.f, 0.f }, CommonUtilities::Vector2f{ 0.5f, 1.f }, CommonUtilities::Vector3f{ 0, -1, 0 }, CommonUtilities::Vector3f{ 1, 0, 0 });


	std::vector<unsigned int> mdlIndices;

	//top
	for (int index = 1; index < aSlices + 1; index++)
	{
		int nextIndex = index + 1;
		if (nextIndex >= aSlices + 1)
		{
			nextIndex = 1;
		}

		mdlIndices.push_back(0);
		mdlIndices.push_back(index);
		mdlIndices.push_back(nextIndex);
	}


	//Middle
	for (int stack = 1; stack < aStacks - 1; stack++)
	{
		int stackStartIndex = (stack - 1) * aSlices + 1;
		for (int slice = 0; slice < aSlices; slice++)
		{
			int currentNodeIndex = stackStartIndex + slice;
			int nextNodeIndex = currentNodeIndex + 1;
			if (nextNodeIndex >= (stackStartIndex + aSlices))
			{
				nextNodeIndex = stackStartIndex;
			}

			mdlIndices.push_back(currentNodeIndex);
			mdlIndices.push_back(currentNodeIndex + aSlices);
			mdlIndices.push_back(nextNodeIndex);

			mdlIndices.push_back(nextNodeIndex);
			mdlIndices.push_back(currentNodeIndex + aSlices);
			mdlIndices.push_back(nextNodeIndex + aSlices);
		}
	}


	//bottom
	int bottomStartIndex = mdlVertices.size() - (aSlices + 1);
	for (int index = bottomStartIndex; index < mdlVertices.size() - 1; index++)
	{
		int nextIndex = index + 1;
		if (nextIndex >= mdlVertices.size() - 1)
		{
			nextIndex = bottomStartIndex;
		}

		mdlIndices.push_back(index);
		mdlIndices.push_back(mdlVertices.size() - 1);
		mdlIndices.push_back(nextIndex);
	}


	if (!RHI::CreateVertexBuffer(result.myVertexBuffer, mdlVertices))
	{
		AMLogger.Err("Could not create a vertex buffer for Sphere!");
		return result;
	}

	if (!RHI::CreateIndexBuffer(result.myIndexBuffer, mdlIndices))
	{
		AMLogger.Err("Could not create a index buffer for Sphere!");
		return result;
	}

	result.myVertexCount = static_cast<UINT>(mdlVertices.size());
	result.myIndexCount = static_cast<UINT>(mdlIndices.size());
	result.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	result.myStride = sizeof(Vertex);

	return result;
}

MeshData CreateSphereMesh2(const float aRadius, const int aParallelCount, const int aMeridianCount)
{
	MeshData result;
	result.myMeshName = "Sphere2";
	float pi = 3.1415926535f;
	auto random = []() {return CommonUtilities::Random::RandomNumber(1.0f); };

	// add top vertex
	std::vector<Vertex> mdlVertices;
	mdlVertices.emplace_back(CommonUtilities::Vector3f{ 0.f, aRadius * 2.f, 0.f }, CommonUtilities::Vector4f{ random(), random(), random(), 1.f }, CommonUtilities::Vector2f{ 0.5f, 0.f });

	CommonUtilities::Vector3f center{ 0.f, aRadius, 0.f };
	CommonUtilities::Vector2f uvMultiplier;
	uvMultiplier.x = 1.f / aParallelCount;
	uvMultiplier.y = 1.f / aMeridianCount;
	// generate vertices per stack / slice
	for (int i = 0; i < aMeridianCount - 1; i++)
	{
		float phi = pi * (i + 1) / aMeridianCount;
		for (int j = 0; j < aParallelCount; j++)
		{
			float theta = 2.0f * pi * j / aParallelCount;
			CommonUtilities::Vector3f pos;
			pos.x = aRadius * (std::sin(phi) * std::cos(theta));
			pos.y = aRadius * std::cos(phi) + aRadius;
			pos.z = aRadius * (std::sin(phi) * std::sin(theta));
			mdlVertices.emplace_back(pos, CommonUtilities::Vector4f{ random(), random(), random(), 1.f }, CommonUtilities::Vector2f{ uvMultiplier.x * j, uvMultiplier.y * i });
		}
	}

	// add bottom vertex
	mdlVertices.emplace_back(CommonUtilities::Vector3f{ 0.f, 0.f, 0.f }, CommonUtilities::Vector4f{ random(), random(), random(), 1.f }, CommonUtilities::Vector2f{ 0.5f, 1.f });

	if (!RHI::CreateVertexBuffer(result.myVertexBuffer, mdlVertices))
	{
		AMLogger.Err("Could not create a vertex buffer for Sphere!");
		return result;
	}

	// add top / bottom triangles
	const unsigned top = 0;
	const unsigned bottom = static_cast<unsigned int>(mdlVertices.size() - 1);
	std::vector<unsigned int> mdlIndices;
	for (int i = 0; i < aParallelCount; ++i)
	{
		auto i0 = i + 1;
		auto i1 = (i + 1) % aParallelCount + 1;
		CreateTri(mdlIndices, top, i1, i0);

		i0 = i + aParallelCount * (aMeridianCount - 2) + 1;
		i1 = (i + 1) % aParallelCount + aParallelCount * (aMeridianCount - 2) + 1;
		CreateTri(mdlIndices, bottom, i0, i1);
	}

	// add quads per stack / slice
	for (int j = 0; j < aMeridianCount - 2; j++)
	{
		auto j0 = j * aParallelCount + 1;
		auto j1 = (j + 1) * aParallelCount + 1;
		for (int i = 0; i < aParallelCount; i++)
		{
			auto i0 = j0 + i;
			auto i1 = j0 + (i + 1) % aParallelCount;
			auto i2 = j1 + (i + 1) % aParallelCount;
			auto i3 = j1 + i;
			CreateQuad(mdlIndices, i0, i1, i2, i3);
		}
	}

	if (!RHI::CreateIndexBuffer(result.myIndexBuffer, mdlIndices))
	{
		AMLogger.Err("Could not create a index buffer for Sphere!");
		return result;
	}

	result.myVertexCount = static_cast<UINT>(mdlVertices.size());
	result.myIndexCount = static_cast<UINT>(mdlIndices.size());
	result.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	result.myStride = sizeof(Vertex);

	return result;
}

MeshData CreateSphereMesh3(const float aRadius, const int aParallelCount, const int aMeridianCount)
{
	MeshData result;
	result.myMeshName = "Sphere3";
	const float pi = 3.1415926535f;
	const float pi2 = 2.f * pi;
	float piInverse = 1.f / pi;
	float pi2Inverse = 1.f / pi2;
	auto random = []() {return CommonUtilities::Random::RandomNumber(1.0f); };


	CommonUtilities::Vector2f uvMultiplier;
	uvMultiplier.x = 1.f / aParallelCount;
	uvMultiplier.y = 1.f / aMeridianCount;
	CommonUtilities::Vector3f center{ 0.f, aRadius, 0.f };
	std::vector<Vertex> mdlVertices;
	std::vector<unsigned int> mdlIndices;
	for (int t = 0; t < aMeridianCount; t++) // stacks are ELEVATION so they count theta
	{
		float theta1 = ((float)(t) / aMeridianCount) * pi;
		float theta2 = ((float)(t + 1) / aMeridianCount) * pi;

		for (int p = 0; p < aParallelCount; p++) // slices are ORANGE SLICES so the count azimuth
		{
			float phi1 = ((float)(p) / aParallelCount) * pi2; // azimuth goes around 0 .. 2*PI
			float phi2 = ((float)(p + 1) / aParallelCount) * pi2;

			//phi2   phi1
			// |      |
			// 2------1 -- theta1
			// |\ _   |
			// |    \ |
			// 3------4 -- theta2
			//

			//vertex1 = vertex on a sphere of radius r at spherical coords theta1, phi1
			CommonUtilities::Vector3f pos1;
			pos1.x = aRadius * std::cos(phi1) * std::sin(theta1);
			pos1.y = (aRadius * std::sin(phi1) * std::sin(theta1)) + aRadius;
			pos1.z = aRadius * std::cos(theta1);

			//vertex2 = vertex on a sphere of radius r at spherical coords theta1, phi2
			CommonUtilities::Vector3f pos2;
			pos2.x = aRadius * std::cos(phi2) * std::sin(theta1);
			pos2.y = (aRadius * std::sin(phi2) * std::sin(theta1)) + aRadius;
			pos2.z = aRadius * std::cos(theta1);

			//vertex3 = vertex on a sphere of radius r at spherical coords theta2, phi2
			CommonUtilities::Vector3f pos3;
			pos3.x = aRadius * std::cos(phi2) * std::sin(theta2);
			pos3.y = (aRadius * std::sin(phi2) * std::sin(theta2)) + aRadius;
			pos3.z = aRadius * std::cos(theta2);

			//vertex4 = vertex on a sphere of radius r at spherical coords theta2, phi1
			CommonUtilities::Vector3f pos4;
			pos4.x = aRadius * std::cos(phi1) * std::sin(theta2);
			pos4.y = (aRadius * std::sin(phi1) * std::sin(theta2)) + aRadius;
			pos4.z = aRadius * std::cos(theta2);

			// x = r * cos(phi) * sin(theta)
			// y = r * sin(phi) * sin(theta)
			// z = r * cos(theta)

			// facing out
			if (t == 0) // top cap
			{
				unsigned Top = static_cast<unsigned>(mdlVertices.size());
				CommonUtilities::Vector3f d1 = (center - pos1).GetNormalized();
				mdlVertices.emplace_back(pos1, CommonUtilities::Vector4f{ random(), random(), random(), 1.f }, CommonUtilities::Vector2f{ .5f + (atan2(d1.z, d1.x) * pi2Inverse), .5f + asin(d1.y) * piInverse });

				unsigned BackRight = static_cast<unsigned>(mdlVertices.size());
				CommonUtilities::Vector3f d3 = (center - pos3).GetNormalized();
				mdlVertices.emplace_back(pos3, CommonUtilities::Vector4f{ random(), random(), random(), 1.f }, CommonUtilities::Vector2f{ .5f + (atan2(d3.z, d3.x) * pi2Inverse), .5f + asin(d3.y) * piInverse });

				unsigned v4 = static_cast<unsigned>(mdlVertices.size());
				CommonUtilities::Vector3f d4 = (center - pos4).GetNormalized();
				mdlVertices.emplace_back(pos4, CommonUtilities::Vector4f{ random(), random(), random(), 1.f }, CommonUtilities::Vector2f{ .5f + (atan2(d4.z, d4.x) * pi2Inverse), .5f + asin(d4.y) * piInverse });

				CreateTri(mdlIndices, v4, BackRight, Top);
			}
			else if (t + 1 == aMeridianCount) //end cap
			{
				unsigned Top = static_cast<unsigned>(mdlVertices.size());
				CommonUtilities::Vector3f d1 = (center - pos1).GetNormalized();
				mdlVertices.emplace_back(pos1, CommonUtilities::Vector4f{ random(), random(), random(), 1.f }, CommonUtilities::Vector2f{ .5f + (atan2(d1.z, d1.x) * pi2Inverse), .5f + asin(d1.y) * piInverse });

				unsigned BackLeft = static_cast<unsigned>(mdlVertices.size());
				CommonUtilities::Vector3f d2 = (center - pos2).GetNormalized();
				mdlVertices.emplace_back(pos2, CommonUtilities::Vector4f{ random(), random(), random(), 1.f }, CommonUtilities::Vector2f{ .5f + (atan2(d2.z, d2.x) * pi2Inverse), .5f + asin(d2.y) * piInverse });

				unsigned BackRight = static_cast<unsigned>(mdlVertices.size());
				CommonUtilities::Vector3f d3 = (center - pos3).GetNormalized();
				mdlVertices.emplace_back(pos3, CommonUtilities::Vector4f{ random(), random(), random(), 1.f }, CommonUtilities::Vector2f{ .5f + (atan2(d3.z, d3.x) * pi2Inverse), .5f + asin(d3.y) * piInverse });

				CreateTri(mdlIndices, BackRight, BackLeft, Top);
			}
			else // body, facing OUT:
			{
				unsigned Top = static_cast<unsigned>(mdlVertices.size());
				CommonUtilities::Vector3f d1 = (center - pos1).GetNormalized();
				mdlVertices.emplace_back(pos1, CommonUtilities::Vector4f{ random(), random(), random(), 1.f }, CommonUtilities::Vector2f{ .5f + (atan2(d1.z, d1.x) * pi2Inverse), .5f + asin(d1.y) * piInverse });

				unsigned BackLeft = static_cast<unsigned>(mdlVertices.size());
				CommonUtilities::Vector3f d2 = (center - pos2).GetNormalized();
				mdlVertices.emplace_back(pos2, CommonUtilities::Vector4f{ random(), random(), random(), 1.f }, CommonUtilities::Vector2f{ .5f + (atan2(d2.z, d2.x) * pi2Inverse), .5f + asin(d2.y) * piInverse });

				unsigned BackRight = static_cast<unsigned>(mdlVertices.size());
				CommonUtilities::Vector3f d3 = (center - pos3).GetNormalized();
				mdlVertices.emplace_back(pos3, CommonUtilities::Vector4f{ random(), random(), random(), 1.f }, CommonUtilities::Vector2f{ .5f + (atan2(d3.z, d3.x) * pi2Inverse), .5f + asin(d3.y) * piInverse });

				unsigned v4 = static_cast<unsigned>(mdlVertices.size());
				CommonUtilities::Vector3f d4 = (center - pos4).GetNormalized();
				mdlVertices.emplace_back(pos4, CommonUtilities::Vector4f{ random(), random(), random(), 1.f }, CommonUtilities::Vector2f{ .5f + (atan2(d4.z, d4.x) * pi2Inverse), .5f + asin(d4.y) * piInverse });

				CreateQuad(mdlIndices, v4, BackRight, BackLeft, Top);
			}
		}
	}

	if (!RHI::CreateVertexBuffer(result.myVertexBuffer, mdlVertices))
	{
		AMLogger.Err("Could not create a vertex buffer for Sphere!");
		return result;
	}

	if (!RHI::CreateIndexBuffer(result.myIndexBuffer, mdlIndices))
	{
		AMLogger.Err("Could not create a index buffer for Sphere!");
		return result;
	}

	result.myVertexCount = static_cast<UINT>(mdlVertices.size());
	result.myIndexCount = static_cast<UINT>(mdlIndices.size());
	result.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	result.myStride = sizeof(Vertex);

	return result;
}

MeshData CreateSphereMesh(const float aRadius, const int aSliceCount, const int aStackCount)
{
	MeshData result;
	result.myMeshName = "Sphere";
	const float pi = 3.1415926535f;
	const float pi2 = 2.f * pi;
	auto random = []() {return CommonUtilities::Random::RandomNumber(1.0f); };
	CommonUtilities::Vector3f center{ 0.f, aRadius, 0.f };

	// Add top vertex
	std::vector<Vertex> mdlVertices;
	mdlVertices.emplace_back(CommonUtilities::Vector3f{ 0.f, aRadius * 2.f, 0.f }, CommonUtilities::Vector2f{ 0.5f, 0.f }, CommonUtilities::Vector3f{ 0.f, 1.f, 0.f }, CommonUtilities::Vector3f{ 1.f, 0.f, 0.f });

	// +1.0f because there's a gap between the poles and the first parallel.
	const float latitudeSpacing = 1.0f / (aStackCount + 1.0f);
	const float longitudeSpacing = 1.0f / aSliceCount;
	CommonUtilities::Vector3f tangent(1.f, 0.f, 0.f);
	// generate vertices per stack / slice
	for (int latitude = 0; latitude < aStackCount; latitude++)
	{
		for (int longitude = 0; longitude <= aSliceCount; longitude++)
		{
			// Scale coordinates into the 0...1 texture coordinate range,
			CommonUtilities::Vector2f uv(longitude * longitudeSpacing, (latitude + 1) * latitudeSpacing);

			// Convert to spherical coordinates:
			// theta is a longitude angle (around the equator) in radians.
			// phi is a latitude angle (north or south of the equator).
			float theta = uv.x * pi2;
			float phi = (1.0f - uv.y - 0.5f) * pi;

			// This determines the radius of the ring of this line of latitude.
			// It's widest at the equator, and narrows as phi increases/decreases.
			float c = cos(phi);

			// Usual formula for a vector in spherical coordinates.
			// You can exchange x & z to wind the opposite way around the sphere.
			CommonUtilities::Vector3f pos(c * cos(theta), sin(phi), c * sin(theta));
			pos *= aRadius;
			pos.y += aRadius;

			CommonUtilities::Vector3f normal((pos - center).GetNormalized());

			//mdlVertices.emplace_back(pos, CommonUtilities::Vector4f{ random(), random(), random(), 1.f }, uv);
			mdlVertices.emplace_back(pos, uv, normal, tangent * CommonUtilities::Matrix3x3f::CreateRotationAroundY(theta));
		}
	}

	// Add bottom vertex
	mdlVertices.emplace_back(CommonUtilities::Vector3f{ 0.f, 0.f, 0.f }, CommonUtilities::Vector2f{ 0.5f, 1.f }, CommonUtilities::Vector3f{ 0.f, -1.f, 0.f }, CommonUtilities::Vector3f{ -1.f, 0.f, 0.f });

	if (!RHI::CreateVertexBuffer(result.myVertexBuffer, mdlVertices))
	{
		AMLogger.Err("Could not create a vertex buffer for Sphere!");
		return result;
	}

	// Add top triangles
	const unsigned top = 0;
	const unsigned bottom = static_cast<unsigned int>(mdlVertices.size() - 1);
	std::vector<unsigned int> mdlIndices;
	for (int i = 0; i < aSliceCount; i++)
	{
		CreateTri(mdlIndices, top, i + 2, i + 1);
	}

	// Each row has one more unique vertex than there are lines of longitude,
	// since we double a vertex at the texture seam.
	int rowLength = aSliceCount + 1;

	for (int latitude = 0; latitude < aStackCount - 1; latitude++)
	{
		// Plus one for the pole.
		int rowStart = latitude * rowLength + 1;
		for (int longitude = 0; longitude < aSliceCount; longitude++)
		{
			int firstCorner = rowStart + longitude;
			CreateQuad(mdlIndices, firstCorner + 1, firstCorner + rowLength + 1, firstCorner + rowLength, firstCorner);
		}
	}

	// Add bottom triangles
	int bottomRow = (aStackCount - 1) * rowLength + 1;
	for (int i = 0; i < aSliceCount; i++)
	{
		CreateTri(mdlIndices, bottom, bottomRow + i, bottomRow + i + 1);
	}

	if (!RHI::CreateIndexBuffer(result.myIndexBuffer, mdlIndices))
	{
		AMLogger.Err("Could not create a index buffer for Sphere!");
		return result;
	}

	result.myVertexCount = static_cast<UINT>(mdlVertices.size());
	result.myIndexCount = static_cast<UINT>(mdlIndices.size());
	result.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	result.myStride = sizeof(Vertex);

	return result;
}

MeshData CreateInvertedCubeMesh(const float aSize)
{
	return CreateInvertedCubeMesh({ aSize, aSize, aSize });
}

MeshData CreateInvertedCubeMesh(const CommonUtilities::Vector3f& aSize)
{
	MeshData result;
	result.myMeshName = "InvertedCube";
	CommonUtilities::Vector3f halfSize = aSize * 0.5f;

	// up = Up | do = Down | Ri = Right | Le = Left | Ba = Back | Fr = Front
	CommonUtilities::Vector3f upRiFr = { halfSize.x, aSize.y, -halfSize.z };
	CommonUtilities::Vector3f upRiBa = { halfSize.x, aSize.y, halfSize.z };

	CommonUtilities::Vector3f upLeFr = { -halfSize.x, aSize.y, -halfSize.z };
	CommonUtilities::Vector3f upLeBa = { -halfSize.x, aSize.y, halfSize.z };

	CommonUtilities::Vector3f doRiFr = { halfSize.x, 0.f, -halfSize.z };
	CommonUtilities::Vector3f doRiBa = { halfSize.x, 0.f, halfSize.z };

	CommonUtilities::Vector3f doLeFr = { -halfSize.x, 0.f, -halfSize.z };
	CommonUtilities::Vector3f doLeBa = { -halfSize.x, 0.f, halfSize.z };

	std::vector<Vertex> mdlVertices = {
		// Top plane
		{
			upRiFr, {0.f, 0.f}, {0.f, -1.f, 0.f}, {-1.f, 0.f, 0.f}
		},
		{
			upRiBa, {0.f, 1.f}, {0.f, -1.f, 0.f}, {-1.f, 0.f, 0.f}
		},
		{
			upLeBa, {1.f, 1.f}, {0.f, -1.f, 0.f}, {-1.f, 0.f, 0.f}
		},
		{
			upLeFr, {1.f, 0.f}, {0.f, -1.f, 0.f}, {-1.f, 0.f, 0.f}
		},
		// Bottom plane
		{
			doRiBa, {1.f, 0.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
		},
		{
			doRiFr, {1.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
		},
		{
			doLeFr, {0.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
		},
		{
			doLeBa, {0.f, 0.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
		},
		// Right plane
		{
			doRiFr, {1.f, 1.f}, {-1.f, 0.f, 0.f}, {0.f, 0.f, -1.f}
		},
		{
			doRiBa, {0.f, 1.f}, {-1.f, 0.f, 0.f}, {0.f, 0.f, -1.f}
		},
		{
			upRiBa, {0.f, 0.f}, {-1.f, 0.f, 0.f}, {0.f, 0.f, -1.f}
		},
		{
			upRiFr, {1.f, 0.f}, {-1.f, 0.f, 0.f}, {0.f, 0.f, -1.f}
		},
		// Left plane
		{
			doLeBa, {1.f, 1.f}, {1.f, 0.f, 0.f}, {0.f, 0.f, 1.f}
		},
		{
			doLeFr, {0.f, 1.f}, {1.f, 0.f, 0.f}, {0.f, 0.f, 1.f}
		},
		{
			upLeFr, {0.f, 0.f}, {1.f, 0.f, 0.f}, {0.f, 0.f, 1.f}
		},
		{
			upLeBa, {1.f, 0.f}, {1.f, 0.f, 0.f}, {0.f, 0.f, 1.f}
		},
		// Back plane
		{
			upRiBa, {1.f, 0.f}, {0.f, 0.f, -1.f}, {1.f, 0.f, 0.f}
		},
		{
			doRiBa, {1.f, 1.f}, {0.f, 0.f, -1.f}, {1.f, 0.f, 0.f}
		},
		{
			doLeBa, {0.f, 1.f}, {0.f, 0.f, -1.f}, {1.f, 0.f, 0.f}
		},
		{
			upLeBa, {0.f, 0.f}, {0.f, 0.f, -1.f}, {1.f, 0.f, 0.f}
		},
		// Front plane
		{
			doRiFr, {0.f, 1.f}, {0.f, 0.f, 1.f}, {-1.f, 0.f, 0.f}
		},
		{
			upRiFr, {0.f, 0.f}, {0.f, 0.f, 1.f}, {-1.f, 0.f, 0.f}
		},
		{
			upLeFr, {1.f, 0.f}, {0.f, 0.f, 1.f}, {-1.f, 0.f, 0.f}
		},
		{
			doLeFr, {1.f, 1.f}, {0.f, 0.f, 1.f}, {-1.f, 0.f, 0.f}
		}
	};

	if (!RHI::CreateVertexBuffer(result.myVertexBuffer, mdlVertices))
	{
		AMLogger.Err("Could not create a vertex buffer for InvertedCube!");
		return result;
	}

	std::vector<unsigned int> mdlIndices{};
	CreateQuad(mdlIndices, 0, 1, 2, 3);
	CreateQuad(mdlIndices, 4, 5, 6, 7);
	CreateQuad(mdlIndices, 8, 9, 10, 11);
	CreateQuad(mdlIndices, 12, 13, 14, 15);
	CreateQuad(mdlIndices, 16, 17, 18, 19);
	CreateQuad(mdlIndices, 20, 21, 22, 23);

	if (!RHI::CreateIndexBuffer(result.myIndexBuffer, mdlIndices))
	{
		AMLogger.Err("Could not create a index buffer for InvertedCube!");
		return result;
	}

	result.myVertexCount = static_cast<UINT>(mdlVertices.size());
	result.myIndexCount = static_cast<UINT>(mdlIndices.size());
	result.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	result.myStride = sizeof(Vertex);

	return result;
}

MeshData CreateInvertedPyramidMesh(const float aSize)
{
	return CreateInvertedPyramidMesh({ aSize, aSize, aSize });
}

MeshData CreateInvertedPyramidMesh(const CommonUtilities::Vector3f& aSize)
{
	MeshData result;
	result.myMeshName = "InvertedPyramid";
	CommonUtilities::Vector3f halfSize = aSize * 0.5f;

	CommonUtilities::Vector3f Top = { 0.f, aSize.y, 0.f };
	CommonUtilities::Vector3f BackLeft = { -halfSize.x, 0.f, halfSize.z };
	CommonUtilities::Vector3f BackRight = { halfSize.x, 0.f, halfSize.z };
	CommonUtilities::Vector3f FrontLeft = { -halfSize.x, 0.f, -halfSize.z };
	CommonUtilities::Vector3f FrontRight = { halfSize.x, 0.f, -halfSize.z };

	CommonUtilities::Vector3f backNormal = -((BackLeft - Top).Cross(BackRight - Top)).GetNormalized();
	CommonUtilities::Vector3f backTangent = (BackLeft - Top).GetNormalized();

	CommonUtilities::Vector3f frontNormal = -((FrontRight - Top).Cross(FrontLeft - Top)).GetNormalized();
	CommonUtilities::Vector3f frontTangent = (FrontRight - Top).GetNormalized();

	CommonUtilities::Vector3f rightNormal = -((BackRight - Top).Cross(FrontRight - Top)).GetNormalized();
	CommonUtilities::Vector3f rightTangent = (BackRight - Top).GetNormalized();

	CommonUtilities::Vector3f leftNormal = -((FrontLeft - Top).Cross(BackLeft - Top)).GetNormalized();
	CommonUtilities::Vector3f leftTangent = (FrontLeft - Top).GetNormalized();

	std::vector<Vertex> mdlVertices = {
		// Back Plane
		{
			Top, {1.f, 0.f}, backNormal, backTangent
		},
		{
			BackLeft, {0.f, 0.f}, backNormal, backTangent
		},
		{
			BackRight, {1.f, 1.f}, backNormal, backTangent
		},
		// Front Plane
		{
			Top, {1.f, 0.f}, frontNormal, frontTangent
		},
		{
			FrontRight, {0.f, 0.f}, frontNormal, frontTangent
		},
		{
			FrontLeft, {1.f, 1.f}, frontNormal, frontTangent
		},
		// Right Plane
		{
			Top, {1.f, 0.f}, rightNormal, rightTangent
		},
		{
			BackRight, {0.f, 0.f}, rightNormal, rightTangent
		},
		{
			FrontRight, {1.f, 1.f}, rightNormal, rightTangent
		},
		// Left Plane
		{
			Top, {1.f, 0.f}, leftNormal, leftTangent
		},
		{
			FrontLeft, {0.f, 0.f}, leftNormal, leftTangent
		},
		{
			BackLeft, {1.f, 1.f}, leftNormal, leftTangent
		},
		// Bottom Plane
		{
			FrontRight, {0.f, 0.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
		},
		{
			BackRight, {0.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
		},
		{
			BackLeft, {1.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
		},
		{
			FrontLeft, {1.f, 0.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
		}		
	};

	if (!RHI::CreateVertexBuffer(result.myVertexBuffer, mdlVertices))
	{
		AMLogger.Err("Could not create a vertex buffer for InvertedPyramid!");
		return result;
	}

	std::vector<unsigned int> mdlIndices{};
	CreateTri(mdlIndices, 2, 1, 0);
	CreateTri(mdlIndices, 5, 4, 3);
	CreateTri(mdlIndices, 8, 7, 6);
	CreateTri(mdlIndices, 11, 10, 9);
	CreateQuad(mdlIndices, 15, 14, 13, 12);

	if (!RHI::CreateIndexBuffer(result.myIndexBuffer, mdlIndices))
	{
		AMLogger.Err("Could not create a index buffer for InvertedPyramid!");
		return result;
	}

	result.myVertexCount = static_cast<UINT>(mdlVertices.size());
	result.myIndexCount = static_cast<UINT>(mdlIndices.size());
	result.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	result.myStride = sizeof(Vertex);

	return result;
}

MeshData CreateInvertedSphereMesh(const float aRadius, const int aSliceCount, const int aStackCount)
{
	MeshData result;
	result.myMeshName = "InvertedSphere";
	const float pi = 3.1415926535f;
	const float pi2 = 2.f * pi;
	auto random = []() {return CommonUtilities::Random::RandomNumber(1.0f); };
	CommonUtilities::Vector3f center{ 0.f, aRadius, 0.f };
	float radiansPerSlice = (360 / aSliceCount) * 0.0174532925f;
	CommonUtilities::Matrix3x3f sliceMatrix = CommonUtilities::Matrix3x3f::CreateRotationAroundY(radiansPerSlice);

	// Add top vertex
	std::vector<Vertex> mdlVertices;
	mdlVertices.emplace_back(CommonUtilities::Vector3f{ 0.f, aRadius * 2.f, 0.f }, CommonUtilities::Vector2f{ 0.5f, 0.f }, CommonUtilities::Vector3f{ 0.f, -1.f, 0.f }, CommonUtilities::Vector3f{ 1.f, 0.f, 0.f });

	// +1.0f because there's a gap between the poles and the first parallel.
	const float latitudeSpacing = 1.0f / (aStackCount + 1.0f);
	const float longitudeSpacing = 1.0f / aSliceCount;

	CommonUtilities::Vector3f tangent(-1.f, 0.f, 0.f);
// generate vertices per stack / slice
	for (int latitude = 0; latitude < aStackCount; latitude++)
	{
		for (int longitude = 0; longitude <= aSliceCount; longitude++)
		{
			// Scale coordinates into the 0...1 texture coordinate range,
			CommonUtilities::Vector2f uv(-longitude * longitudeSpacing, (latitude + 1) * latitudeSpacing);

			// Convert to spherical coordinates:
			// theta is a longitude angle (around the equator) in radians.
			// phi is a latitude angle (north or south of the equator).
			float theta = uv.x * pi2;
			float phi = (1.0f - uv.y - 0.5f) * pi;

			// This determines the radius of the ring of this line of latitude.
			// It's widest at the equator, and narrows as phi increases/decreases.
			float c = cos(phi);

			// Usual formula for a vector in spherical coordinates.
			// You can exchange x & z to wind the opposite way around the sphere.
			CommonUtilities::Vector3f pos(c * cos(theta), sin(phi), c * sin(theta));
			pos *= aRadius;
			pos.y += aRadius;

			CommonUtilities::Vector3f normal((center - pos).GetNormalized());
			uv.x = -uv.x;

			mdlVertices.emplace_back(pos, uv, normal, tangent * CommonUtilities::Matrix3x3f::CreateRotationAroundY(theta));
		}
	}

	// Add bottom vertex
	mdlVertices.emplace_back(CommonUtilities::Vector3f{ 0.f, 0.f, 0.f }, CommonUtilities::Vector2f{ 0.5f, 1.f }, CommonUtilities::Vector3f{ 0.f, 1.f, 0.f }, CommonUtilities::Vector3f{ -1.f, 0.f, 0.f });

	if (!RHI::CreateVertexBuffer(result.myVertexBuffer, mdlVertices))
	{
		AMLogger.Err("Could not create a vertex buffer for InvertedSphere!");
		return result;
	}

	// Add top triangles
	const unsigned top = 0;
	const unsigned bottom = static_cast<unsigned int>(mdlVertices.size() - 1);
	std::vector<unsigned int> mdlIndices;
	for (int i = 0; i < aSliceCount; i++)
	{
		CreateTri(mdlIndices, top, i + 2, i + 1);
		//CreateTri(mdlIndices, i + 1, i + 2, top);
	}

	// Each row has one more unique vertex than there are lines of longitude,
	// since we double a vertex at the texture seam.
	int rowLength = aSliceCount + 1;

	for (int latitude = 0; latitude < aStackCount - 1; latitude++)
	{
		// Plus one for the pole.
		int rowStart = latitude * rowLength + 1;
		for (int longitude = 0; longitude < aSliceCount; longitude++)
		{
			int firstCorner = rowStart + longitude;
			CreateQuad(mdlIndices, firstCorner + 1, firstCorner + rowLength + 1, firstCorner + rowLength, firstCorner);
			//CreateQuad(mdlIndices, firstCorner, firstCorner + rowLength, firstCorner + rowLength + 1, firstCorner + 1);
		}
	}

	// Add bottom triangles
	int bottomRow = (aStackCount - 1) * rowLength + 1;
	for (int i = 0; i < aSliceCount; i++)
	{
		CreateTri(mdlIndices, bottom, bottomRow + i, bottomRow + i + 1);
		//CreateTri(mdlIndices, bottomRow + i + 1, bottomRow + i, bottom);
	}

	if (!RHI::CreateIndexBuffer(result.myIndexBuffer, mdlIndices))
	{
		AMLogger.Err("Could not create a index buffer for InvertedSphere!");
		return result;
	}

	result.myVertexCount = static_cast<UINT>(mdlVertices.size());
	result.myIndexCount = static_cast<UINT>(mdlIndices.size());
	result.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	result.myStride = sizeof(Vertex);

	return result;
}

void CreateTri(std::vector<unsigned int>& outIndices, unsigned int aPoint1, unsigned int aPoint2, unsigned int aPoint3)
{
	outIndices.emplace_back(aPoint1);
	outIndices.emplace_back(aPoint2);
	outIndices.emplace_back(aPoint3);
}

void CreateQuad(std::vector<unsigned int>& outIndices, unsigned int aPoint1, unsigned int aPoint2, unsigned int aPoint3, unsigned int aPoint4)
{
	outIndices.emplace_back(aPoint1);
	outIndices.emplace_back(aPoint2);
	outIndices.emplace_back(aPoint3);

	outIndices.emplace_back(aPoint3);
	outIndices.emplace_back(aPoint4);
	outIndices.emplace_back(aPoint1);
}
