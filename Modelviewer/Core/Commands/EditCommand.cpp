#include "EditCommand.h"
#include "../Modelviewer.h"

void EditCommand::LogError(const std::string& anError) const
{
    ModelViewer::Get().GetLogger().Err(anError);
}

void EditCommand::LogMessage(const std::string& anError) const
{
    ModelViewer::Get().GetLogger().Log(anError);
}

GameObject* EditCommand::GetGameObject(unsigned anID) const
{
    return ModelViewer::Get().GetGameObject(anID);
}

GameObject& EditCommand::AddGameObject(const GameObject& anObject)
{
    GameObject newObject(anObject);
    return ModelViewer::Get().myGameObjects.emplace(newObject.GetID(), std::move(newObject)).first->second;
}

GameObject& EditCommand::AddGameObject(GameObject&& anObject)
{
    return ModelViewer::Get().myGameObjects.emplace(anObject.GetID(), std::move(anObject)).first->second;
}

bool EditCommand::RemoveGameObject(unsigned anID)
{
    assert(anID != 0 && "Incorrect ID!");

    auto& gameObjects = ModelViewer::Get().myGameObjects;
    if (auto iter = gameObjects.find(anID); iter != gameObjects.end())
    {
        if (ModelViewer::Get().mySelectedObject == &iter->second)
        {
            ModelViewer::Get().mySelectedObject = nullptr;
        }
        gameObjects.erase(iter);
        return true;
    }
    return false;
}
