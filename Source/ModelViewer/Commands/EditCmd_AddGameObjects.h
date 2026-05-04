#pragma once
#include "EditCommand.h"
#include "AssetManager/Assets/GameObject.h"

class EditCmd_AddGameObjects : public EditCommand
{
public:
	EditCmd_AddGameObjects();
	~EditCmd_AddGameObjects() = default;

	void Undo() override;
	void Execute() override;

protected:
	std::unordered_set<std::shared_ptr<GameObject>> myObjects;
};