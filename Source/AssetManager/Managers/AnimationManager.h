#pragma once
#include "Assets/Animation/BlendSpace.h"
#include <unordered_map>
#include <unordered_set>

class AnimationManager
{
public:
	AnimationManager() = default;
	~AnimationManager() = default;

	void Init();

	void UpdateFilePaths();
	const std::unordered_set<std::string>& GetAnimationlist();

	AnimationData* GetAnimation(const std::string& aPath, bool aShouldLogErrors);
	BlendSpace GetBlendSpace(const std::string& aPath, bool aShouldLogErrors);

	void SaveBlendSpace(const BlendSpace& aBlendSpace, const std::string& aPath);

	static inline const char* GetExtension(){ return ".fbx"; }
	static inline const char* GetBlendSpaceExtension() { return ".bld"; }
	static inline const char* GetPath(){ return "..\\Content\\Animations\\"; }
	static inline const wchar_t* GetExtensionW(){ return L".fbx"; }
	static inline const wchar_t* GetPathW(){ return L"..\\Content\\Animations\\"; }

private:
	std::unordered_map<std::string, BlendSpace> myBlendSpaces;
	std::unordered_map<std::string, AnimationData> myAnimationData;
	std::unordered_set<std::string> myFilePaths;

	AnimationData* LoadAnimation(const std::string& aPath, bool aShouldLogErrors);
	BlendSpace LoadBlendSpace(const std::string& aPath, bool aShouldLogErrors);
};