#pragma once
#include "EditCommand.h"
#include "AssetManager/Assets/GameObject.h"

class EditCmd_RemoveGameObject : public EditCommand
{
public:
	EditCmd_RemoveGameObject(const GameObject& anObject);
	~EditCmd_RemoveGameObject() = default;

	void Undo() override;
	void Redo() override;

protected:
	std::shared_ptr<GameObject> myObject;
	unsigned myID;
};