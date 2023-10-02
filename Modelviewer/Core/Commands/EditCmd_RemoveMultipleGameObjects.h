#pragma once
#include "EditCommand.h"
#include "AssetManager/Assets/GameObject.h"

class EditCmd_RemoveMultipleGameObjects : public EditCommand
{
public:
	EditCmd_RemoveMultipleGameObjects();
	~EditCmd_RemoveMultipleGameObjects() = default;

	void Undo() override;
	void Execute() override;

protected:
	std::unordered_set<std::shared_ptr<GameObject>> myObjects;
};