#pragma once
#include "EditCommand.h"
#include "AssetManager/Assets/GameObject.h"

class EditCmd_RemoveGameObjects : public EditCommand
{
public:
	EditCmd_RemoveGameObjects();
	~EditCmd_RemoveGameObjects() = default;

	void Undo() override;
	void Execute() override;

protected:
	std::unordered_set<std::shared_ptr<GameObject>> myObjects;
	std::unordered_map<GameObjectID, std::unordered_set<std::shared_ptr<GameObject>>> myChildList;
};