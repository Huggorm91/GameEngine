#pragma once
#include "../Assets/Animation.h"
#include <unordered_map>
#include <unordered_set>

class AnimationManager
{
public:
	AnimationManager() = default;
	AnimationManager(const std::string& aPath);
	~AnimationManager() = default;

	void Init();

	Animation* GetAnimation(const std::string& aPath);

private:
	std::unordered_map<std::string, Animation> myAnimations;
	std::unordered_map<std::string, AnimationData> myAnimationData;
	std::unordered_set<std::string> myFilePaths;
	const std::string myPath = "Content/Animations/";

	Animation* LoadAnimation(const std::string& aPath);
};