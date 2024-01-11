#pragma once
#include "EditCommand.h"
#include "AssetManager/Assets/GameObject.h"

class EditCmd_AddGameObject : public EditCommand
{
public:
	EditCmd_AddGameObject(const std::shared_ptr<GameObject>& anObject);
	EditCmd_AddGameObject(GameObject&& anObject);
	~EditCmd_AddGameObject() = default;

	void Undo() override;
	void Execute() override;

protected:
	GameObjectID myID;
	std::shared_ptr<GameObject> myObject;
};