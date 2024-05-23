#pragma once
#include "EditCommand.h"
#include "AssetManager/Assets/GameObject.h"

class EditCmd_ChangeMultipleGameObjects : public EditCommand
{
public:
	enum class TransformType
	{
		Position,
		Rotation,
		Scale
	};

	EditCmd_ChangeMultipleGameObjects(const Crimson::Vector3f& aChange, TransformType aType, Transform* anEditorTransform);
	~EditCmd_ChangeMultipleGameObjects() = default;

	void Undo() override;
	void Execute() override;
	bool Merge(const EditCommand* aCommand) override;

protected:
	TransformType myType;
	Transform* myEditorTransform;
	Crimson::Vector3f myChange;
	std::unordered_set<std::shared_ptr<GameObject>> myObjects;
};