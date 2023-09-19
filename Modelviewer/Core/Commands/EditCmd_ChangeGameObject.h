#pragma once
#include "EditCommand.h"
#include "AssetManager/Assets/GameObject.h"

class EditCmd_ChangeGameObject : public EditCommand
{
public:
	EditCmd_ChangeGameObject(const std::shared_ptr<GameObject>& anObject);
	~EditCmd_ChangeGameObject() = default;

	void Undo() override;
	void Execute() override;

protected:
	std::shared_ptr<GameObject> myObject;
	unsigned myID;

	void Swap();
};