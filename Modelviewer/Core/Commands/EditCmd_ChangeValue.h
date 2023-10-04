#pragma once
#include "EditCommand.h"
#include "Math/Math.hpp"

template <class T>
class EditCmd_ChangeValue : public EditCommand
{
public:
	EditCmd_ChangeValue();
	EditCmd_ChangeValue(T& aValueToChange, const T& aNewValue);
	EditCmd_ChangeValue(T* aValueToChange, const T& aNewValue);
	~EditCmd_ChangeValue() = default;

	void Undo() override;
	void Execute() override;
	bool Merge(const EditCommand* aCommand) override;

private:
	T* myPointer;
	T myNewValue;
	T myOldValue;
};

template<class T>
inline EditCmd_ChangeValue<T>::EditCmd_ChangeValue(): myPointer(nullptr), myNewValue(), myOldValue()
{
}

template<class T>
inline EditCmd_ChangeValue<T>::EditCmd_ChangeValue(T& aValueToChange, const T& aNewValue): myPointer(&aValueToChange), myNewValue(aNewValue), myOldValue(aValueToChange)
{
}

template<class T>
inline EditCmd_ChangeValue<T>::EditCmd_ChangeValue(T* aValueToChange, const T& aNewValue) : myPointer(aValueToChange), myNewValue(aNewValue), myOldValue(*aValueToChange)
{
}

template<class T>
inline void EditCmd_ChangeValue<T>::Undo()
{
	*myPointer = myOldValue;
}

template<class T>
inline void EditCmd_ChangeValue<T>::Execute()
{
	*myPointer = myNewValue;
}

template<class T>
inline bool EditCmd_ChangeValue<T>::Merge(const EditCommand* aCommand)
{
	const EditCmd_ChangeValue<T>* pointer = dynamic_cast<const EditCmd_ChangeValue<T>*>(aCommand);
	if (pointer && myPointer == pointer->myPointer)
	{
		myNewValue = pointer->myNewValue;
		*myPointer = myNewValue;
		return true;
	}
	return false;
}
