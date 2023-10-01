#pragma once
#include "EditCommand.h"
#include "AssetManager/Assets/GameObject.h"

class EditCmd_AddMultipleGameObjects : public EditCommand
{
public:
	EditCmd_AddMultipleGameObjects(const std::vector<std::shared_ptr<GameObject>>& anObjectList);
	~EditCmd_AddMultipleGameObjects() = default;

	void Undo() override;
	void Execute() override;

protected:
	std::vector<std::shared_ptr<GameObject>> myObjects;
};