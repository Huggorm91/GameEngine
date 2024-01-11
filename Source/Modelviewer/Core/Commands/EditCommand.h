#pragma once
class GameObject;

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

	std::shared_ptr<GameObject> GetGameObject(GameObjectID anID) const;
	std::shared_ptr<GameObject>& AddGameObject(const std::shared_ptr<GameObject>& anObject, const std::unordered_map<GameObjectID, std::unordered_set<std::shared_ptr<GameObject>>>* aChildList) const;
	bool RemoveGameObject(GameObjectID anID) const;

	std::unordered_set<std::shared_ptr<GameObject>> GetSelectedObjects() const;
	void SetSelectedObjects(const std::unordered_set<std::shared_ptr<GameObject>>& aList) const;
	void ClearSelectedObjects() const;

	std::unordered_map<GameObjectID, std::unordered_set<std::shared_ptr<GameObject>>> GetChildrenOf(const std::shared_ptr<GameObject>& anObject) const;

private:
	bool EraseObject(GameObjectID anID) const;

	std::unordered_map<GameObjectID, std::unordered_set<std::shared_ptr<GameObject>>> GetChildrenInternal(const std::shared_ptr<GameObject>& anObject) const;
	std::unordered_set<std::shared_ptr<GameObject>> GetChildList(const std::shared_ptr<GameObject>& anObject) const;
};