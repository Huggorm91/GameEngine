#include "AssetManager.pch.h"
#include "ModelManager.h"
#include "../Assets/Primitives.h"
#include "../DirectoryFunctions.h"
#include <algorithm>
#include <Importer.h>
#undef LoadModel

ModelManager::ModelManager(const std::string& aPath) : myPath(aPath), myFilePaths(), myMeshData(), myModels(), mySkeletons()
{
}

void ModelManager::Init()
{
	myFilePaths = GetAllFilepathsInDirectory(myPath);
}

void ModelManager::GeneratePrimitives()
{
	{
		auto modelIter = myModels.emplace("cube", GameObject());
		GameObject& model = modelIter.first->second;
		auto elementIter = myMeshData.emplace("cube", std::vector<MeshData>{ CreateCubeMesh(100.f) });
		MeshComponent& mesh = model.AddComponent<MeshComponent>();
		mesh.Init(std::vector<MeshElement>{ MeshElement(elementIter.first->second.back()) }, "Cube", &modelIter.first->first);
	}

	{
		auto modelIter = myModels.emplace("sphere", GameObject());
		GameObject& model = modelIter.first->second;
		auto elementIter = myMeshData.emplace("sphere", std::vector<MeshData>{ CreateSphereMesh(50.f, 50, 50) });
		MeshComponent& mesh = model.AddComponent<MeshComponent>();
		mesh.Init(std::vector<MeshElement>{ MeshElement(elementIter.first->second.back()) }, "Sphere", &modelIter.first->first);
	}

	{
		auto modelIter = myModels.emplace("pyramid", GameObject());
		GameObject& model = modelIter.first->second;
		auto elementIter = myMeshData.emplace("pyramid", std::vector<MeshData>{ CreatePyramidMesh(100.f) });
		MeshComponent& mesh = model.AddComponent<MeshComponent>();
		mesh.Init(std::vector<MeshElement>{ MeshElement(elementIter.first->second.back()) }, "Pyramid", &modelIter.first->first);
	}

	{
		auto modelIter = myModels.emplace("invertedcube", GameObject());
		GameObject& model = modelIter.first->second;
		auto elementIter = myMeshData.emplace("invertedcube", std::vector<MeshData>{ CreateInvertedCubeMesh(100.f) });
		MeshComponent& mesh = model.AddComponent<MeshComponent>();
		mesh.Init(std::vector<MeshElement>{ MeshElement(elementIter.first->second.back()) }, "InvertedCube", &modelIter.first->first);
	}

	{
		auto modelIter = myModels.emplace("invertedsphere", GameObject());
		GameObject& model = modelIter.first->second;
		auto elementIter = myMeshData.emplace("invertedsphere", std::vector<MeshData>{ CreateInvertedSphereMesh(50.f, 50, 50) });
		MeshComponent& mesh = model.AddComponent<MeshComponent>();
		mesh.Init(std::vector<MeshElement>{ MeshElement(elementIter.first->second.back()) }, "InvertedSphere", &modelIter.first->first);
	}

	{
		auto modelIter = myModels.emplace("invertedpyramid", GameObject());
		GameObject& model = modelIter.first->second;
		auto elementIter = myMeshData.emplace("invertedpyramid", std::vector<MeshData>{ CreateInvertedPyramidMesh(100.f) });
		MeshComponent& mesh = model.AddComponent<MeshComponent>();
		mesh.Init(std::vector<MeshElement>{ MeshElement(elementIter.first->second.back()) }, "InvertedPyramid", &modelIter.first->first);
	}

	{
		auto modelIter = myModels.emplace("plane", GameObject());
		GameObject& model = modelIter.first->second;
		auto elementIter = myMeshData.emplace("plane", std::vector<MeshData>{ CreatePlaneMesh(100.f) });
		MeshComponent& mesh = model.AddComponent<MeshComponent>();
		mesh.Init(std::vector<MeshElement>{ MeshElement(elementIter.first->second.back()) }, "Plane", & modelIter.first->first);
	}
}

GameObject* ModelManager::GetModel(const std::string& aPath)
{
	if (auto iter = myModels.find(aPath); iter != myModels.end())
	{
		return &iter->second;
	}
	else
	{
		std::string path = AddExtensionIfMissing(aPath, ".fbx");
		path = GetValidPath(path, myPath, false);
		if (path.empty())
		{
			std::string lowerCase = aPath;
			std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), [](unsigned char c) { return std::tolower(c); });

			if (auto iter = myModels.find(lowerCase); iter != myModels.end())
			{
				return &iter->second;
			}
		}
		else
		{
			return LoadModel(path);
		}
	}
	AMLogger.Err("ModelManager: Could not find a Model: " + aPath);
	return nullptr;
}

GameObject* ModelManager::GetModel(Primitives aPrimitive)
{
	switch (aPrimitive)
	{
	case Primitives::Cube:
		return &myModels["cube"];
	case Primitives::Pyramid:
		return &myModels["pyramid"];
	case Primitives::Sphere:
		return &myModels["sphere"];
	case Primitives::InvertedCube:
		return &myModels["invertedcube"];
	case Primitives::InvertedPyramid:
		return &myModels["invertedpyramid"];
	case Primitives::InvertedSphere:
		return &myModels["invertedsphere"];
	case Primitives::Plane:
		return &myModels["plane"];
	default:
		AMLogger.Err("ModelManager: Primitive does not exist");
		return nullptr;;
	}
}

Skeleton* ModelManager::GetSkeleton(const std::string& aPath)
{
	if (auto iter = mySkeletons.find(aPath); iter != mySkeletons.end())
	{
		return &iter->second;
	}
	else
	{
		return LoadSkeleton(aPath);
	}
	AMLogger.Err("ModelManager: Could not find a Skeleton: " + aPath);
	return nullptr;
}

MeshComponent* ModelManager::GetMesh(const std::string& aPath)
{
	if (auto model = GetModel(aPath); model != nullptr)
	{
		return &model->GetComponent<MeshComponent>();
	}

	AMLogger.Err("ModelManager: Could not find a MeshComponent: " + aPath);
	return nullptr;
}

AnimatedMeshComponent* ModelManager::GetAnimatedMesh(const std::string& aPath)
{
	if (auto model = GetModel(aPath); model != nullptr)
	{
		return &model->GetComponent<AnimatedMeshComponent>();
	}

	AMLogger.Err("ModelManager: Could not find an AnimatedMeshComponent: " + aPath);
	return nullptr;
}

GameObject* ModelManager::LoadModel(const std::string& aPath)
{
	TGA::FBX::Mesh tgaMesh;
	bool success = false;
	try
	{
		success = TGA::FBX::Importer::LoadMeshA(aPath, tgaMesh);
	}
	catch (const std::exception& e)
	{
		AMLogger.Err("ModelManager: Could not load mesh from: " + aPath);
		AMLogger.LogException(e);
	}

	if (success)
	{
		auto modelIter = myModels.emplace(aPath, GameObject());
		if (modelIter.second == false)
		{
			return nullptr;
		}
		GameObject& model = modelIter.first->second;
		auto& dataList = myMeshData.emplace(aPath, std::vector<MeshData>()).first->second;
		dataList.reserve(tgaMesh.Elements.size());
		std::vector<MeshElement> elements;
		for (auto& tgaElement : tgaMesh.Elements)	// Load elements
		{
			dataList.emplace_back(tgaElement);
			MeshData& element = dataList.back();
			elements.emplace_back(element);

			std::vector<Vertex> mdlVertices;
			for (auto& tgaVertex : tgaElement.Vertices)	// Load vertices
			{
				mdlVertices.emplace_back(tgaVertex);
			}

			if (!RHI::CreateVertexBuffer(element.myVertexBuffer, mdlVertices))
			{
				AMLogger.Err("ModelManager: Could not create a vertex buffer for: " + aPath);
				myMeshData.erase(aPath);
				myModels.erase(aPath);
				return nullptr;
			}

			std::vector<unsigned int> mdlIndices;
			for (auto& tgaIndex : tgaElement.Indices)	// Load indices
			{
				mdlIndices.emplace_back(tgaIndex);
			}

			if (!RHI::CreateIndexBuffer(element.myIndexBuffer, mdlIndices))
			{
				AMLogger.Err("ModelManager: Could not create an index buffer for: " + aPath);
				myMeshData.erase(aPath);
				myModels.erase(aPath);
				return nullptr;
			}
		}

		if (tgaMesh.Skeleton.GetRoot())	// Load Skeleton
		{
			auto skeletonIter = mySkeletons.emplace(aPath, tgaMesh.Skeleton);
			skeletonIter.first->second.SetPath(&skeletonIter.first->first);
			model.AddComponent(AnimatedMeshComponent(tgaMesh, elements, &modelIter.first->first, &skeletonIter.first->second));
		}
		else
		{
			model.AddComponent(MeshComponent(tgaMesh, elements, &modelIter.first->first));
		}

		return &model;
	}

	AMLogger.Err("ModelManager: Failed to load model from: " + aPath);
	myMeshData.erase(aPath);
	myModels.erase(aPath);
	return nullptr;
}

Skeleton* ModelManager::LoadSkeleton(const std::string& aPath)
{
	std::string path = AddExtensionIfMissing(aPath, ".fbx");
	path = GetValidPath(path, myPath);
	if (path.empty())
	{
		AMLogger.Err("ModelManager: Could not load model from path: " + aPath);
		return nullptr;
	}

	TGA::FBX::Mesh tgaMesh;
	bool success = false;
	try
	{
		success = TGA::FBX::Importer::LoadMeshA(path, tgaMesh);
	}
	catch (const std::exception& e)
	{
		AMLogger.Err("ModelManager: Could not load mesh from: " + aPath);
		AMLogger.LogException(e);
	}

	if (success)
	{
		auto modelIter = myModels.emplace(aPath, GameObject());
		if (modelIter.second == false)
		{
			return nullptr;
		}
		GameObject& model = modelIter.first->second;
		auto& dataList = myMeshData.emplace(aPath, std::vector<MeshData>()).first->second;
		dataList.reserve(tgaMesh.Elements.size());
		std::vector<MeshElement> elements;
		for (auto& tgaElement : tgaMesh.Elements)	// Load elements
		{
			dataList.emplace_back(tgaElement);
			MeshData& element = dataList.back();
			elements.emplace_back(element);

			std::vector<Vertex> mdlVertices;
			for (auto& tgaVertex : tgaElement.Vertices)	// Load vertices
			{
				mdlVertices.emplace_back(tgaVertex);
			}

			if (!RHI::CreateVertexBuffer(element.myVertexBuffer, mdlVertices))
			{
				AMLogger.Err("ModelManager: Could not create a vertex buffer for: " + aPath);
				myMeshData.erase(aPath);
				myModels.erase(aPath);
				return nullptr;
			}

			std::vector<unsigned int> mdlIndices;
			for (auto& tgaIndex : tgaElement.Indices)	// Load indices
			{
				mdlIndices.emplace_back(tgaIndex);
			}

			if (!RHI::CreateIndexBuffer(element.myIndexBuffer, mdlIndices))
			{
				AMLogger.Err("ModelManager: Could not create an index buffer for: " + aPath);
				myMeshData.erase(aPath);
				myModels.erase(aPath);
				return nullptr;
			}
		}

		if (tgaMesh.Skeleton.GetRoot())	// Load Skeleton
		{
			auto skeletonIter = mySkeletons.emplace(aPath, tgaMesh.Skeleton);
			skeletonIter.first->second.SetPath(&skeletonIter.first->first);
			model.AddComponent(AnimatedMeshComponent(tgaMesh, elements, &modelIter.first->first, &skeletonIter.first->second));
			return &skeletonIter.first->second;
		}
	}

	AMLogger.Err("ModelManager: Failed to load skeleton from: " + aPath);
	myMeshData.erase(aPath);
	myModels.erase(aPath);
	return nullptr;
}
