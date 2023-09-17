#pragma once
#include "EditCommand.h"
#include "AssetManager/Assets/GameObject.h"

class EditCmd_ChangeGameObject : public EditCommand
{
public:
	EditCmd_ChangeGameObject(const GameObject& anObject);
	~EditCmd_ChangeGameObject() = default;

	void Undo() override;
	void Redo() override;

protected:
	std::shared_ptr<GameObject> myObject;
	unsigned myID;

	void Swap();
};