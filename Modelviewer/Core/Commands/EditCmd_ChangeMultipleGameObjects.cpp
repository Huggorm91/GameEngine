#include "EditCmd_ChangeMultipleGameObjects.h"

EditCmd_ChangeMultipleGameObjects::EditCmd_ChangeMultipleGameObjects(const std::vector<std::shared_ptr<GameObject>>& anObjectList, const Transform& aChange): myObjects(anObjectList), myChange(aChange)
{
}

void EditCmd_ChangeMultipleGameObjects::Undo()
{
	assert(!"Not implemented!");
}

void EditCmd_ChangeMultipleGameObjects::Execute()
{
	assert(!"Not implemented!");
}

bool EditCmd_ChangeMultipleGameObjects::Merge(const EditCommand* aCommand)
{
	const EditCmd_ChangeMultipleGameObjects* pointer = dynamic_cast<const EditCmd_ChangeMultipleGameObjects*>(aCommand);
	if (pointer && myObjects.size() == pointer->myObjects.size())
	{
		for (size_t i = 0; i < myObjects.size(); i++)
		{
			if (myObjects[i] != pointer->myObjects[i])
			{
				return false;
			}
		}

		myChange = pointer->myChange;
		return true;
	}
	return false;
}
