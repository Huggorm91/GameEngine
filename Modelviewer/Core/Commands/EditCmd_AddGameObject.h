#pragma once
#include "EditCommand.h"
#include "AssetManager/Assets/GameObject.h"

class EditCmd_AddGameObject : public EditCommand
{
public:
	EditCmd_AddGameObject(const GameObject& anObject);
	~EditCmd_AddGameObject() = default;

	void Undo() override;
	void Redo() override;

protected:
	std::shared_ptr<GameObject> myObject;
	unsigned myID;
};