#pragma once
#include "Assets/Animation/Animation.h"
#include <unordered_map>
#include <unordered_set>

class AnimationManager
{
public:
	AnimationManager() = default;
	~AnimationManager() = default;

	void Init();

	Animation* GetAnimation(const std::string& aPath, bool aShouldLogErrors);

	static inline const char* GetExtension(){ return ".fbx"; }
	static inline const char* GetPath(){ return "..\\Content\\Animations\\"; }
	static inline const wchar_t* GetExtensionW(){ return L".fbx"; }
	static inline const wchar_t* GetPathW(){ return L"..\\Content\\Animations\\"; }

private:
	std::unordered_map<std::string, Animation> myAnimations;
	std::unordered_map<std::string, AnimationData> myAnimationData;
	std::unordered_set<std::string> myFilePaths;

	Animation* LoadAnimation(const std::string& aPath, bool aShouldLogErrors);
};