#pragma once
#include "GameObject.h"

class Prefab
{
	friend class GameObject;
	friend class PrefabManager;

public:
	Prefab(const Prefab&) = delete;
	Prefab(std::string aName);
	Prefab(Prefab&& aPrefab) noexcept;
	~Prefab() = default;

	Prefab& operator=(const Prefab& aPrefab) = delete;
	Prefab& operator=(Prefab&& aPrefab) noexcept;

	const GameObject& GetTemplate() const;
	GameObject& ChangeTemplate();

	void SaveTemplate() const;

private:
	GameObject* myTemplate;
	std::string myName;

	Prefab();
	Prefab(GameObject* aTemplate, std::string aName);
};