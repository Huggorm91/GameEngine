#pragma once
#include "XBoxController.h"
#include "Math/Math.hpp"
#pragma comment(lib, "Xinput9_1_0.lib")

namespace Crimson
{
	XBoxController::XBoxController() : myCurrentState(), myDeadzone(), myID(-1), myLeftStickInput(), myLeftTriggerInput(), myLeftVibration(),
		myPreviousLeftStickInput(), myPreviousLeftTriggerInput(), myPreviousRightStickInput(), myPreviousRightTriggerInput(), myPreviousState(),
		myRightStickInput(), myRightTriggerInput(), myRightVibration(), myVibration(), myIsVibrating(false)
	{
		myDeadzone.x = 0.15f;
		myDeadzone.y = 0.15f;
	}

	bool XBoxController::VerifyConnection()
	{
		int tempID = -1;

		for (DWORD i = 0; i < XUSER_MAX_COUNT && tempID == -1; i++)
		{
			XINPUT_STATE tempState;
			RtlZeroMemory(&tempState, sizeof(XINPUT_STATE));

			if (XInputGetState(i, &tempState) == ERROR_SUCCESS)
			{
				tempID = i;
			}
		}

		myID = tempID;

		return tempID != -1;
	}

	bool XBoxController::UpdateState()
	{
		if (myID == -1)
		{
			VerifyConnection();
		}

		if (myID != -1)
		{
			myPreviousState = myCurrentState;

			RtlZeroMemory(&myCurrentState, sizeof(XINPUT_STATE));
			if (XInputGetState(myID, &myCurrentState) != ERROR_SUCCESS)
			{
				myID = -1;
				ResetState();
				return false;
			}

			myPreviousLeftStickInput = myLeftStickInput;
			myPreviousRightStickInput = myRightStickInput;
			myPreviousLeftTriggerInput = myLeftTriggerInput;
			myPreviousRightTriggerInput = myRightTriggerInput;


			const float stickMultiplier = 3.0518509475997192297128208258309e-5f; // Same as dividing by 32767

			{ // Left Stick
				Vector2<float> leftStickNorm(
					Max(-1.0f, static_cast<float>(myCurrentState.Gamepad.sThumbLX) * stickMultiplier),
					Max(-1.0f, static_cast<float>(myCurrentState.Gamepad.sThumbLY) * stickMultiplier)
				);

				myLeftStickInput.x = (Abs(leftStickNorm.x) < myDeadzone.x ? 0.f : (Abs(leftStickNorm.x) - myDeadzone.x) * (leftStickNorm.x / Abs(leftStickNorm.x)));
				myLeftStickInput.y = (Abs(leftStickNorm.y) < myDeadzone.y ? 0.f : (Abs(leftStickNorm.y) - myDeadzone.y) * (leftStickNorm.y / Abs(leftStickNorm.y)));

				if (myDeadzone.x > 0.f)
				{
					myLeftStickInput.x *= 1.f / (1.f - myDeadzone.x);
				}
				if (myDeadzone.y > 0.f)
				{
					myLeftStickInput.y *= 1.f / (1.f - myDeadzone.y);
				}
			}

			{ // Right Stick
				Vector2<float> rightStickNorm(
					Max(-1.0f, myCurrentState.Gamepad.sThumbRX * stickMultiplier),
					Max(-1.0f, myCurrentState.Gamepad.sThumbRY * stickMultiplier)
				);

				myRightStickInput.x = (Abs(rightStickNorm.x) < myDeadzone.x ? 0.f : (Abs(rightStickNorm.x) - myDeadzone.x) * (rightStickNorm.x / Abs(rightStickNorm.x)));
				myRightStickInput.y = (Abs(rightStickNorm.y) < myDeadzone.y ? 0.f : (Abs(rightStickNorm.y) - myDeadzone.y) * (rightStickNorm.y / Abs(rightStickNorm.y)));

				if (myDeadzone.x > 0.f)
				{
					myRightStickInput.x *= 1.f / (1.f - myDeadzone.x);
				}
				if (myDeadzone.y > 0.f)
				{
					myRightStickInput.y *= 1.f / (1.f - myDeadzone.y);
				}
			}

			{ // Triggers
				const float triggerMultiplier = 0.0039215686274509803921568627451f; // Same as dividing by 255

				myLeftTriggerInput = myCurrentState.Gamepad.bLeftTrigger * triggerMultiplier;
				myRightTriggerInput = myCurrentState.Gamepad.bRightTrigger * triggerMultiplier;
			}

			if (myIsVibrating)
			{ // Vibration
				RtlZeroMemory(&myVibration, sizeof(XINPUT_VIBRATION));
				myVibration.wLeftMotorSpeed = myLeftVibration; // use any value between 0-65535 here
				myVibration.wRightMotorSpeed = myRightVibration; // use any value between 0-65535 here
				XInputSetState(myID, &myVibration);
			}

			return true;
		}

		return false;
	}

	const XINPUT_GAMEPAD& XBoxController::GetState() const
	{
		return myCurrentState.Gamepad;
	}

	const XINPUT_GAMEPAD& XBoxController::GetPreviousState() const
	{
		return myPreviousState.Gamepad;
	}

	const std::array<WORD, 14>& XBoxController::GetKeyArray() const
	{
		static std::array<WORD, 14> localKeys{ XBOX_DPAD_UP, XBOX_DPAD_DOWN, XBOX_DPAD_LEFT, XBOX_DPAD_RIGHT,
			XBOX_START, XBOX_BACK, XBOX_L_THUMBSTICK, XBOX_R_THUMBSTICK, XBOX_L_SHOULDER, XBOX_R_SHOULDER,
			XBOX_A, XBOX_B, XBOX_X, XBOX_Y };
		return localKeys;
	}

	bool XBoxController::GetButtonDown(const WORD& aButton) const
	{
		return ((myCurrentState.Gamepad.wButtons & aButton) != 0 && (myPreviousState.Gamepad.wButtons & aButton) == 0);
	}

	bool XBoxController::GetButtonHeld(const WORD& aButton) const
	{
		return ((myCurrentState.Gamepad.wButtons & aButton) != 0 && (myPreviousState.Gamepad.wButtons & aButton) != 0);
	}

	bool XBoxController::GetButtonUp(const WORD& aButton) const
	{
		return ((myCurrentState.Gamepad.wButtons & aButton) == 0 && (myPreviousState.Gamepad.wButtons & aButton) != 0);
	}

	Vector2<float> XBoxController::GetLeftStick() const
	{
		return myLeftStickInput;
	}
	Vector2<float> XBoxController::GetLeftStickDelta() const
	{
		return myLeftStickInput - myPreviousLeftStickInput;
	}

	Vector2<float> XBoxController::GetRightStick() const
	{
		return myRightStickInput;
	}
	Vector2<float> XBoxController::GetRightStickDelta() const
	{
		return myRightStickInput - myPreviousRightStickInput;
	}


	float XBoxController::GetLeftTrigger() const
	{
		return myLeftTriggerInput;
	}
	float XBoxController::GetLeftTriggerDelta() const
	{
		return myLeftTriggerInput - myPreviousLeftTriggerInput;
	}

	float XBoxController::GetRightTrigger() const
	{
		return myRightTriggerInput;
	}
	float XBoxController::GetRightTriggerDelta() const
	{
		return myRightTriggerInput - myPreviousRightTriggerInput;
	}


	void XBoxController::SetVibrationToggle(bool aState)
	{
		myIsVibrating = aState;
		if (myIsVibrating == false)
		{
			myVibration.wLeftMotorSpeed = 0;
			myVibration.wRightMotorSpeed = 0;
			XInputSetState(myID, &myVibration);
		}
	}

	void XBoxController::ToggleVibration()
	{
		myIsVibrating = !myIsVibrating;
		if (myIsVibrating == false)
		{
			myVibration.wLeftMotorSpeed = 0;
			myVibration.wRightMotorSpeed = 0;
			XInputSetState(myID, &myVibration);
		}
	}

	void XBoxController::Vibrate(WORD aLeftVibrationAmount, WORD aRightVibrationAmount)
	{
		myLeftVibration = aLeftVibrationAmount;
		myRightVibration = aRightVibrationAmount;
	}

	WORD XBoxController::GetLeftVibration()
	{
		return myLeftVibration;
	}

	WORD XBoxController::GetRightVibration()
	{
		return myRightVibration;
	}

	void XBoxController::SetDeadZone(const Vector2<float>& aDeadZone)
	{
		myDeadzone = aDeadZone;
	}

	Vector2<float> XBoxController::GetDeadZone()
	{
		return myDeadzone;
	}

	void XBoxController::ResetState()
	{
		RtlZeroMemory(&myCurrentState, sizeof(XINPUT_STATE));
		RtlZeroMemory(&myPreviousState, sizeof(XINPUT_STATE));
		RtlZeroMemory(&myLeftVibration, sizeof(XINPUT_VIBRATION));
		RtlZeroMemory(&myRightVibration, sizeof(XINPUT_VIBRATION));

		myPreviousLeftStickInput.Zero();
		myLeftStickInput.Zero();
		myPreviousRightStickInput.Zero();
		myRightStickInput.Zero();

		myPreviousLeftTriggerInput = 0.f;
		myLeftTriggerInput = 0.f;
		myPreviousRightTriggerInput = 0.f;
		myRightTriggerInput = 0.f;
	}
}