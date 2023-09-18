#include "AssetManager.pch.h"
#include "Prefab.h"
#include "AssetManager.h"

Prefab::Prefab() : myTemplate(nullptr), myName()
{
}

Prefab::Prefab(GameObject* aTemplate, std::string aName): myTemplate(aTemplate), myName(aName)
{

}

Prefab::Prefab(std::string aName): myTemplate(AssetManager::myPrefabManager.GetTemplate(aName)), myName(aName)
{
}

Prefab::Prefab(Prefab&& aPrefab) noexcept : myTemplate(aPrefab.myTemplate), myName(aPrefab.myName)
{
}

Prefab& Prefab::operator=(Prefab&& aPrefab) noexcept
{
	myTemplate = aPrefab.myTemplate;
	myName = aPrefab.myName;
	return *this;
}

const GameObject& Prefab::GetTemplate() const
{
	return *myTemplate;
}

GameObject& Prefab::ChangeTemplate()
{
	return *myTemplate;
}

void Prefab::SaveTemplate() const
{
	AssetManager::myPrefabManager.SavePrefab(myName, *myTemplate);
}