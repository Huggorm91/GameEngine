#pragma once
#include "../Assets/GameObject.h"
#include "../Assets/Components/Rendering/AnimatedMeshComponent.h"
#include <unordered_map>
#include <unordered_set>

enum class Primitives
{
	Cube,
	Pyramid,
	Sphere,
	InvertedCube,
	InvertedPyramid,
	InvertedSphere,
	Plane
};

class ModelManager
{
public:
	ModelManager() = default;
	ModelManager(const std::string& aPath);
	~ModelManager() = default;

	void Init();
	void GeneratePrimitives();

	GameObject* GetModel(const std::string& aPath);
	GameObject* GetModel(Primitives aPrimitive);

private:
	std::unordered_map<std::string, GameObject> myModels;
	std::unordered_map<std::string, std::vector<MeshData>> myMeshData;
	std::unordered_map<std::string, Skeleton> mySkeletons;
	std::unordered_set<std::string> myFilePaths;
	const std::string myPath = "Content/Models/";

	GameObject* LoadModel(const std::string& aPath);
};