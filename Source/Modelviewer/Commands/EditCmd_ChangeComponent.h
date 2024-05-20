#pragma once
#include "EditCommand.h"
#include "../Modelviewer.h"
#include "AssetManager/Assets/GameObject.h"

template <class CompType>
class EditCmd_ChangeComponent : public EditCommand
{
public:
	// Will only compile if using a class derived from Component!
	EditCmd_ChangeComponent(const CompType& aComponent);
	~EditCmd_ChangeComponent() = default;

	void Undo() override;
	void Execute() override;

protected:
	std::shared_ptr<CompType> myComponent;
	unsigned myGameObjectID;
	unsigned myComponentID;

	void Swap();
};

template<class CompType>
inline EditCmd_ChangeComponent<CompType>::EditCmd_ChangeComponent(const CompType& aComponent) : myComponent(std::make_shared<CompType>(aComponent)), myGameObjectID(aComponent.GetParent().GetID()), myComponentID(aComponent.GetID())
{
	myComponent->MarkAsPrefabComponent(myComponentID);
}

template<class CompType>
inline void EditCmd_ChangeComponent<CompType>::Undo()
{
	Swap();
}

template<class CompType>
inline void EditCmd_ChangeComponent<CompType>::Execute()
{
	Swap();
}

template<class CompType>
inline void EditCmd_ChangeComponent<CompType>::Swap()
{
	GameObject* object = GetGameObject(myGameObjectID);
	assert(object && "Could not find GameObject!");

	CompType* component = object->GetComponent<CompType>(myComponentID);
	assert(component && "Could not find ComponentID in Gameobject");

	CompType swap = *component;
	swap.MarkAsPrefabComponent(myComponentID);

	*component = *myComponent;
	*myComponent = swap;

	component->Init(object);
}
