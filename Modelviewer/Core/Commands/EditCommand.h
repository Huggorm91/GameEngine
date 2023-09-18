#pragma once
#include "AssetManager/Assets/GameObject.h"

class EditCommand
{
public:
	virtual ~EditCommand() = default;

	virtual void Undo() = 0;
	virtual void Execute() = 0;

protected:
	void LogError(const std::string& anError) const;
	void LogMessage(const std::string& anError) const;

	GameObject* GetGameObject(unsigned anID) const;

	GameObject& AddGameObject(const GameObject& anObject);
	GameObject& AddGameObject(GameObject&& anObject);

	bool RemoveGameObject(unsigned anID);
};