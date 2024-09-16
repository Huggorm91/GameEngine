#pragma once
#include "EnumKeys.h"
#include "EnumInputEvents.h"
#include "EnumInputAction.h"

// Looses all attachments when copied or moved
class InputObserver
{
public:
	InputObserver();
	InputObserver(const InputObserver&);
	InputObserver(InputObserver&&) noexcept;
	virtual ~InputObserver();

	virtual void ReceiveEvent(Crimson::eInputEvent, Crimson::eKey) {}

	// float value will be different depending on what Keybind was triggered.
	// Key		| KeyUp = 0.f	:	KeyHeld = 1.f	:	KeyDown = 2.f
	// Xbox		| Sticks = 0.f to 1.f	:	Trigger = 0.f to 1.f
	// Mouse	| Move = Pixeldistance moved	:	Scroll = Delta
	virtual void ReceiveEvent(Crimson::eInputAction, float) {}

protected:
	void Attach(Crimson::eInputEvent anEvent, Crimson::eKey aKey = Crimson::eKey::None);
	void Attach(Crimson::eInputAction anAction, Crimson::eKeyAction aKeyAction = Crimson::eKeyAction::All);

	void Detach(Crimson::eInputEvent anEvent, Crimson::eKey aKey = Crimson::eKey::None);
	void Detach(Crimson::eInputAction anAction, Crimson::eKeyAction aKeyAction = Crimson::eKeyAction::All);
	void DetachAll();

	// Manually resets the pointer and detaches from all events
	void RenewInputObserverPointer();
private:
	InputObserver* myPointer;
};