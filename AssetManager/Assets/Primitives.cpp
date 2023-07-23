#include "AssetManager.pch.h"
#include "Primitives.h"
#include <Random.h>
#include <math.h>
#include <Matrix3x3.hpp>

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

MeshData CreatePlaneMesh(const float aSize)
{
	return CreatePlaneMesh({ aSize, aSize });
}

MeshData CreatePlaneMesh(const CommonUtilities::Vector2f& aSize)
{
	MeshData result;
	result.myMeshName = "Plane";
	CommonUtilities::Vector2f halfSize = aSize * 0.5f;

	// up = Up | do = Down | Ri = Right | Le = Left
	CommonUtilities::Vector3f upRi = { halfSize.x, 0.f, halfSize.y };
	CommonUtilities::Vector3f upLe = { -halfSize.x, 0.f, halfSize.y };
	CommonUtilities::Vector3f doRi = { halfSize.x, 0.f, -halfSize.y };
	CommonUtilities::Vector3f doLe = { -halfSize.x, 0.f, -halfSize.y };

	std::vector<Vertex> mdlVertices = {
		// Top plane
		{
			upRi, {1.f, 0.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
		},
		{
			doRi, {1.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
		},
		{
			doLe, {0.f, 1.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
		},
		{
			upLe, {0.f, 0.f}, {0.f, 1.f, 0.f}, {1.f, 0.f, 0.f}
		},
		// Bottom plane
		{
			doLe, {1.f, 1.f}, {0.f, -1.f, 0.f}, {-1.f, 0.f, 0.f}
		},
		{
			doRi, {0.f, 1.f}, {0.f, -1.f, 0.f}, {-1.f, 0.f, 0.f}
		},
		{
			upRi, {0.f, 0.f}, {0.f, -1.f, 0.f}, {-1.f, 0.f, 0.f}
		},
		{
			upLe, {1.f, 0.f}, {0.f, -1.f, 0.f}, {-1.f, 0.f, 0.f}
		}
	};

	if (!RHI::CreateVertexBuffer(result.myVertexBuffer, mdlVertices))
	{
		AMLogger.Err("Could not create a vertex buffer for Plane!");
		return result;
	}

	std::vector<unsigned int> mdlIndices{};
	CreateQuad(mdlIndices, 0, 1, 2, 3);
	CreateQuad(mdlIndices, 4, 5, 6, 7);

	if (!RHI::CreateIndexBuffer(result.myIndexBuffer, mdlIndices))
	{
		AMLogger.Err("Could not create a index buffer for Plane!");
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
