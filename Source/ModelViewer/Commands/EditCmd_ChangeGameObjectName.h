#pragma once
#include "EditCommand.h"
#include "AssetManager/Assets/GameObject.h"

class EditCmd_ChangeGameObjectName : public EditCommand
{
public:
	EditCmd_ChangeGameObjectName(GameObject* anObject, const std::string& aNewName);
	~EditCmd_ChangeGameObjectName() = default;

	void Undo() override;
	void Execute() override;

protected:
	GameObject* myObject;
	std::string myOldName;
	std::string myOldImguiName;
	std::string myNewName;
};