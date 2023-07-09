#include "AssetManager.pch.h"
#include "InputComponent.h"

void InputComponent::Update()
{
	if (!myIsActive)
	{
		return;
	}

}

void InputComponent::ReceiveEvent(CommonUtilities::eInputEvent anEvent, CommonUtilities::eKey aKey)
{
}

void InputComponent::ReceiveEvent(CommonUtilities::eInputAction anEvent, float aValue)
{
}

const InputComponent* InputComponent::GetTypePointer() const
{
    return this;
}
