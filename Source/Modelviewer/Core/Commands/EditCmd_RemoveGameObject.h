#pragma once
#include "EditCommand.h"
#include "AssetManager/Assets/GameObject.h"

class EditCmd_RemoveGameObject : public EditCommand
{
public:
	EditCmd_RemoveGameObject(const std::shared_ptr<GameObject>& anObject);
	~EditCmd_RemoveGameObject() = default;

	void Undo() override;
	void Execute() override;

protected:
	std::shared_ptr<GameObject> myObject;
	unsigned myID;
};