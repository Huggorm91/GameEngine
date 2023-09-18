#pragma once
#include "EditCommand.h"
#include "../Modelviewer.h"
#include "AssetManager/Assets/GameObject.h"

template <class CompType>
class EditCmd_AddComponent : public EditCommand
{
public:
	// Will only compile if using a class derived from Component!
	EditCmd_AddComponent(const CompType& aComponent);
	~EditCmd_AddComponent() = default;

	void Undo() override;
	// Potential future problem: GameObject ComponentContainer will grow for every Redo-Call
	void Execute() override;

protected:
	std::shared_ptr<CompType> myComponent;
	unsigned myGameObjectID;
	unsigned myComponentID;
};

template<class CompType>
inline EditCmd_AddComponent<CompType>::EditCmd_AddComponent(const CompType& aComponent) : myComponent(std::make_shared<CompType>(aComponent)), myGameObjectID(aComponent.GetParent().GetID()), myComponentID(aComponent.GetID())
{
	myComponent->MarkAsPrefabComponent(myComponentID);
}

template<class CompType>
inline void EditCmd_AddComponent<CompType>::Undo()
{
	GameObject* object = GetGameObject(myGameObjectID);
	assert(object && "Could not find GameObject!");

	const bool success = object->RemoveComponent<CompType>(myComponentID);
	assert(success && "Failed removing Component!");
}

template<class CompType>
inline void EditCmd_AddComponent<CompType>::Execute()
{
	GameObject* object = GetGameObject(myGameObjectID);
	assert(object && "Could not find GameObject!");

	CompType& component = object->AddComponent<CompType>(*myComponent);
	component.MarkAsPrefabComponent(myComponentID);
}