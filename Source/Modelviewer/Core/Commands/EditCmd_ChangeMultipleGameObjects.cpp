#include "Modelviewer.pch.h"
#include "EditCmd_ChangeMultipleGameObjects.h"

EditCmd_ChangeMultipleGameObjects::EditCmd_ChangeMultipleGameObjects(const Crimson::Vector3f& aChange, TransformType aType, Transform* anEditorTransform) : myType(aType), myEditorTransform(anEditorTransform), myChange(aChange), myObjects(GetSelectedObjects())
{
}

void EditCmd_ChangeMultipleGameObjects::Undo()
{
	switch (myType)
	{
	case EditCmd_ChangeMultipleGameObjects::TransformType::Position:
	{
		myEditorTransform->SetPosition(myEditorTransform->GetPosition() - myChange);
		for (auto& object : myObjects)
		{
			object->SetPosition(object->GetTransform().GetPosition() - myChange);
		}
		break;
	}
	case EditCmd_ChangeMultipleGameObjects::TransformType::Rotation:
	{
		myEditorTransform->SetRotation(myEditorTransform->GetRotation() - myChange);
		for (auto& object : myObjects)
		{
			object->SetRotation(object->GetTransform().GetRotation() - myChange);
		}
		break;
	}
	case EditCmd_ChangeMultipleGameObjects::TransformType::Scale:
	{
		myEditorTransform->SetScale(myEditorTransform->GetScale() - myChange);
		for (auto& object : myObjects)
		{
			object->SetScale(object->GetTransform().GetScale() - myChange);
		}
		break;
	}
	default:
		break;
	}
}

void EditCmd_ChangeMultipleGameObjects::Execute()
{
	switch (myType)
	{
	case EditCmd_ChangeMultipleGameObjects::TransformType::Position:
	{
		myEditorTransform->SetPosition(myEditorTransform->GetPosition() + myChange);
		for (auto& object : myObjects)
		{
			object->SetPosition(object->GetTransform().GetPosition() + myChange);
		}
		break;
	}
	case EditCmd_ChangeMultipleGameObjects::TransformType::Rotation:
	{
		myEditorTransform->SetRotation(myEditorTransform->GetRotation() + myChange);
		for (auto& object : myObjects)
		{
			object->SetRotation(object->GetTransform().GetRotation() + myChange);
		}
		break;
	}
	case EditCmd_ChangeMultipleGameObjects::TransformType::Scale:
	{
		myEditorTransform->SetScale(myEditorTransform->GetScale() + myChange);
		for (auto& object : myObjects)
		{
			object->SetScale(object->GetTransform().GetScale() + myChange);
		}
		break;
	}
	default:
		break;
	}
}

bool EditCmd_ChangeMultipleGameObjects::Merge(const EditCommand* aCommand)
{
	const EditCmd_ChangeMultipleGameObjects* pointer = dynamic_cast<const EditCmd_ChangeMultipleGameObjects*>(aCommand);
	if (pointer && myType == pointer->myType && myObjects.size() == pointer->myObjects.size())
	{
		for (size_t i = 0; i < myObjects.size(); i++)
		{
			if (myObjects != pointer->myObjects)
			{
				return false;
			}
		}

		if ((myChange).GetNormalizedNoAssert().Abs() == (pointer->myChange).GetNormalizedNoAssert().Abs())
		{
			myChange += pointer->myChange;
			return true;
		}
	}
	return false;
}