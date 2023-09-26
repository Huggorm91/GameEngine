#pragma once
#include "EditCommand.h"
#include "AssetManager/Assets/GameObject.h"

class EditCmd_ChangeMultipleGameObjects : public EditCommand
{
public:
	EditCmd_ChangeMultipleGameObjects(const std::vector<std::shared_ptr<GameObject>>& anObjectList, const Transform& aChange);
	~EditCmd_ChangeMultipleGameObjects() = default;

	void Undo() override;
	void Execute() override;
	bool Merge(const EditCommand* aCommand) override;

protected:
	Transform myChange;
	std::vector<std::shared_ptr<GameObject>> myObjects;
};