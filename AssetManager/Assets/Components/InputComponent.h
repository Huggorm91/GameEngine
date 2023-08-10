#pragma once
#include "Component.h"
#include <InputMapper.h>

class InputComponent : public Component, public CommonUtilities::InputObserver
{
public:
	InputComponent() = default;
	~InputComponent() = default;

	void Update() override;

	void ReceiveEvent(CommonUtilities::eInputEvent anEvent, CommonUtilities::eKey aKey) override;
	void ReceiveEvent(CommonUtilities::eInputAction anEvent, float aValue) override;

void AddEvent(CommonUtilities::eInputEvent anEvent, CommonUtilities::eKey aKey, CommonUtilities::eInputAction aResponse);
void Subscribe (Component* aSubsciber, CommonUtilities::eInputAction anEvent);

	const InputComponent* GetTypePointer() const override;

private:
// List of subscribed components 
// List of new events (eInputEvent + eKey = eInputAction)
};