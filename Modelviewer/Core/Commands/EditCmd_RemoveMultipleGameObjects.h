#pragma once
#include "EditCommand.h"
#include "AssetManager/Assets/GameObject.h"

class EditCmd_RemoveMultipleGameObjects : public EditCommand
{
public:
	EditCmd_RemoveMultipleGameObjects(const std::vector<std::shared_ptr<GameObject>>& anObjectList);
	~EditCmd_RemoveMultipleGameObjects() = default;

	void Undo() override;
	void Execute() override;

protected:
	std::vector<std::shared_ptr<GameObject>> myObjects;
};