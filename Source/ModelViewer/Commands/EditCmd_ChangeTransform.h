#pragma once
#include "EditCommand.h"
#include "Math/Vector3.hpp"

class EditCmd_ChangeTransform : public EditCommand
{
public:
	EditCmd_ChangeTransform(Crimson::Vector3f& aValueToChange, const Crimson::Vector3f& aNewValue, bool& aHasChangedFlag);
	~EditCmd_ChangeTransform() = default;

	void Undo() override;
	void Execute() override;
	bool Merge(const EditCommand* aCommand) override;

protected:
	bool* myHasChangedFlag;
	Crimson::Vector3f* myPointer;
	Crimson::Vector3f myNewValue;
	Crimson::Vector3f myOldValue;
};