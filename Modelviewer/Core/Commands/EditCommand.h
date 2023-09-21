#pragma once
#include "AssetManager/Assets/GameObject.h"

class EditCommand
{
public:
	virtual ~EditCommand() = default;

	virtual void Undo() = 0;
	virtual void Execute() = 0;
	virtual bool Merge(const EditCommand* aCommand);

protected:
	void LogError(const std::string& anError) const;
	void LogMessage(const std::string& anError) const;

	std::shared_ptr<GameObject> GetGameObject(unsigned anID) const;

	std::shared_ptr<GameObject>& AddGameObject(const std::shared_ptr<GameObject>& anObject);
	std::shared_ptr<GameObject>& AddGameObject(GameObject&& anObject);

	bool RemoveGameObject(unsigned anID);
};