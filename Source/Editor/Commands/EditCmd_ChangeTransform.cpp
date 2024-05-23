#include "Editor.pch.h"
#include "EditCmd_ChangeTransform.h"
#include "../Modelviewer.h"

EditCmd_ChangeTransform::EditCmd_ChangeTransform(Crimson::Vector3f& aValueToChange, const Crimson::Vector3f& aNewValue, bool& aHasChangedFlag) : myPointer(&aValueToChange), myNewValue(aNewValue), myOldValue(aValueToChange), myHasChangedFlag(&aHasChangedFlag)
{
}

void EditCmd_ChangeTransform::Undo()
{
	*myPointer = myOldValue;
	*myHasChangedFlag = true;
}

void EditCmd_ChangeTransform::Execute()
{
	*myPointer = myNewValue;
	*myHasChangedFlag = true;
}

bool EditCmd_ChangeTransform::Merge(const EditCommand* aCommand)
{
	const EditCmd_ChangeTransform* pointer = dynamic_cast<const EditCmd_ChangeTransform*>(aCommand);
	if (pointer && myPointer == pointer->myPointer && (myOldValue - myNewValue).GetNormalizedNoAssert().Abs() == (pointer->myOldValue - pointer->myNewValue).GetNormalizedNoAssert().Abs())
	{
		myNewValue = pointer->myNewValue;
		return true;
	}
	return false;
}
