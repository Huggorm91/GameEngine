#pragma once
#include "CrimsonUtilities/Math/Vector2.hpp"
#include <Xinput.h>
#include <array>

#define XBOX_DPAD_UP 0x0001
#define XBOX_DPAD_DOWN 0x0002
#define XBOX_DPAD_LEFT 0x0004
#define XBOX_DPAD_RIGHT 0x0008

#define XBOX_START 0x0010
#define XBOX_BACK 0x0020

#define XBOX_L_THUMBSTICK 0x0040
#define XBOX_R_THUMBSTICK 0x0080

#define XBOX_L_SHOULDER 0x0100
#define XBOX_R_SHOULDER 0x0200

#define XBOX_A 0x1000
#define XBOX_B 0x2000
#define XBOX_X 0x4000
#define XBOX_Y 0x8000

class XBoxController
{
public:
	XBoxController();
	~XBoxController() = default;

	bool VerifyConnection();
	bool UpdateState();

	const XINPUT_GAMEPAD& GetState() const;
	const XINPUT_GAMEPAD& GetPreviousState() const;
	const std::array<WORD, 14>& GetKeyArray() const;

	bool GetButtonDown(const WORD& aWord) const;
	bool GetButtonHeld(const WORD& aWord) const;
	bool GetButtonUp(const WORD& aWord) const;

	Crimson::Vector2f GetLeftStick() const;
	Crimson::Vector2f GetLeftStickDelta() const;

	Crimson::Vector2f GetRightStick() const;
	Crimson::Vector2f GetRightStickDelta() const;

	float GetLeftTrigger() const;
	float GetLeftTriggerDelta() const;

	float GetRightTrigger() const;
	float GetRightTriggerDelta() const;

	void SetVibrationToggle(bool aState);
	void ToggleVibration();

	void Vibrate(WORD aLeftVibrationAmount, WORD aRightVibrationAmount);
	WORD GetLeftVibration();
	WORD GetRightVibration();

	void SetDeadZone(const Crimson::Vector2f& aDeadZone);
	Crimson::Vector2f GetDeadZone();

private:
	int myID;

	Crimson::Vector2f myPreviousLeftStickInput;
	Crimson::Vector2f myLeftStickInput;
	Crimson::Vector2f myPreviousRightStickInput;
	Crimson::Vector2f myRightStickInput;

	float myPreviousLeftTriggerInput;
	float myLeftTriggerInput;
	float myPreviousRightTriggerInput;
	float myRightTriggerInput;

	XINPUT_STATE myPreviousState;
	XINPUT_STATE myCurrentState;
	XINPUT_VIBRATION myVibration;
	WORD myLeftVibration;
	WORD myRightVibration;

	bool myIsVibrating;
	Crimson::Vector2f myDeadzone;

	void ResetState();
};