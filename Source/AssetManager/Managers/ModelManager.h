#pragma once
#include "../Assets/GameObject.h"
#include "../Assets/Components/Rendering/AnimatedMeshComponent.h"
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
	~ModelManager() = default;

	void Init();
	void GeneratePrimitives();

	GameObject* GetModel(const std::string& aPath, bool aShouldLogErrors);
	GameObject* GetModel(Primitives aPrimitive);

	Skeleton* GetSkeleton(const std::string& aPath, bool aShouldLogErrors);

	MeshComponent* GetMesh(const std::string& aPath, bool aShouldLogErrors);
	AnimatedMeshComponent* GetAnimatedMesh(const std::string& aPath, bool aShouldLogErrors);

	static inline const char* GetExtension(){ return ".fbx"; }
	static inline const char* GetPath(){ return "..\\Content\\Models\\"; }
	static inline const wchar_t* GetExtensionW(){ return L".fbx"; }
	static inline const wchar_t* GetPathW(){ return L"..\\Content\\Models\\"; }

private:
	std::unordered_map<std::string, GameObject> myModels;
	std::unordered_map<std::string, std::vector<MeshData>> myMeshData;
	std::unordered_map<std::string, Skeleton> mySkeletons;
	std::unordered_set<std::string> myFilePaths;

	GameObject* LoadModel(const std::string& aPath, bool aShouldLogErrors);
	Skeleton* LoadSkeleton(const std::string& aPath, bool aShouldLogErrors);
};