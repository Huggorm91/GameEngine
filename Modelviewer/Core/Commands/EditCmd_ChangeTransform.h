#pragma once
#include "EditCommand.h"
#include <Vector3.hpp>

class EditCmd_ChangeTransform : public EditCommand
{
public:
	EditCmd_ChangeTransform(CommonUtilities::Vector3f& aValueToChange, const CommonUtilities::Vector3f& aNewValue, bool& aHasChangedFlag);
	~EditCmd_ChangeTransform() = default;

	void Undo() override;
	void Execute() override;
	bool Merge(const EditCommand* aCommand) override;

protected:
	bool* myHasChangedFlag;
	CommonUtilities::Vector3f* myPointer;
	CommonUtilities::Vector3f myNewValue;
	CommonUtilities::Vector3f myOldValue;
};