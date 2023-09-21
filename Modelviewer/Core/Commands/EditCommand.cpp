#include "EditCommand.h"
#include "../Modelviewer.h"

bool EditCommand::Merge(const EditCommand*)
{
    return false;
}

void EditCommand::LogError(const std::string& anError) const
{
    ModelViewer::Get().GetLogger().Err(anError);
}

void EditCommand::LogMessage(const std::string& anError) const
{
    ModelViewer::Get().GetLogger().Log(anError);
}

std::shared_ptr<GameObject> EditCommand::GetGameObject(unsigned anID) const
{
    return ModelViewer::Get().GetGameObject(anID);
}

std::shared_ptr<GameObject>& EditCommand::AddGameObject(const std::shared_ptr<GameObject>& anObject)
{
    return ModelViewer::Get().myGameObjects.emplace(anObject->GetID(), anObject).first->second;
}

std::shared_ptr<GameObject>& EditCommand::AddGameObject(GameObject&& anObject)
{
    return ModelViewer::Get().myGameObjects.emplace(anObject.GetID(), std::make_shared<GameObject>(std::move(anObject))).first->second;
}

bool EditCommand::RemoveGameObject(unsigned anID)
{
    auto& gameObjects = ModelViewer::Get().myGameObjects;
    if (auto iter = gameObjects.find(anID); iter != gameObjects.end())
    {
        if (ModelViewer::Get().mySelectedObject.lock() == iter->second)
        {
            ModelViewer::Get().mySelectedObject.reset();
        }
        gameObjects.erase(iter);
        return true;
    }
    return false;
}
