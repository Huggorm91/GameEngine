#pragma once
#include "EditCommand.h"
#include "AssetManager/Assets/GameObject.h"

class EditCmd_AddMultipleGameObjects : public EditCommand
{
public:
	EditCmd_AddMultipleGameObjects();
	~EditCmd_AddMultipleGameObjects() = default;

	void Undo() override;
	void Execute() override;

protected:
	std::unordered_set<std::shared_ptr<GameObject>> myObjects;
};