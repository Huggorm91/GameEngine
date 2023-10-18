#include "GraphicsEngine.pch.h"
#include "PerspectiveCamera.h"
#include "Math/Conversions.hpp"
#include "Time/Timer.h"
#include "Input/InputMapper.h"
#include "../GraphicsEngine.h"
#include "../Commands/GfxCmd_SetFrameBuffer.h"
#include "../Drawer/LineDrawer.h"

PerspectiveCamera::PerspectiveCamera() : myTransform(), myMovementSpeed(), myRotationSpeed(), myRotation(), myPosition(), myScreenResolution(), myIsAcceptingInput(false), myMouseSensitivity(), myHasChanged(true), myCompassTransform(), myCompass(), myCompassOffset()
{
}

void PerspectiveCamera::Init(const Crimson::Vector2f& aScreenSize, float aSpeed, float aRotationSpeed, float aMouseSensitivity)
{
	myScreenResolution = aScreenSize;

	myClipMatrix.m34 = 1.f;
	myClipMatrix.m44 = 0.f;

	myPosition = { 0.f, 200.f, 0.f };
	myRotationSpeed = aRotationSpeed;
	myMovementSpeed = aSpeed;
	myMouseSensitivity = aMouseSensitivity;

	const float nearplane = 1.f;
	const float farplane = 10000.f;

	SetFOV(90.f);
	SetPlanes(nearplane, farplane);

	myCompassOffset = { 1.8f, 1.f, nearplane + 1.f };

	// Using UI
	//myCompass = GraphicsEngine::Get().GetLineDrawer().AddAxisLines({ 0.f, 0.f, 0.f }, .1f, false, { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 1.f }, GetColor(eColor::Red), GetColor(eColor::Green), GetColor(eColor::Blue), Crimson::Matrix4x4f::Null);

	// Using worldspace
#ifndef _RETAIL
	myCompass = GraphicsEngine::Get().GetLineDrawer().AddAxisLines({ 0.f, 0.f, 0.f }, .1f, false);
#endif // _RETAIL

	UpdateTransform();

	auto& inputHandler = *Crimson::InputMapper::GetInstance();
	inputHandler.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::Q);
	inputHandler.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::E);
	inputHandler.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::W);
	inputHandler.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::A);
	inputHandler.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::S);
	inputHandler.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::D);
	inputHandler.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::SpaceBar);
	inputHandler.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::Ctrl);

	inputHandler.Attach(this, Crimson::eInputEvent::KeyDown, Crimson::eKey::MouseRightButton);
	inputHandler.Attach(this, Crimson::eInputEvent::KeyUp, Crimson::eKey::MouseRightButton);

}

void PerspectiveCamera::Update()
{
	if (myHasChanged)
	{
		UpdateTransform();
	}
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_SetFrameBuffer>(myTransform, myClipMatrix, myPosition));
}

void PerspectiveCamera::ReceiveEvent(Crimson::eInputEvent anEvent, Crimson::eKey aKey)
{
	switch (anEvent)
	{
	case Crimson::eInputEvent::MouseMove:
	{
		myHasChanged = true;
		float multiplier = myMouseSensitivity * .01f;
		Crimson::Vector2f distance = Crimson::InputMapper::GetInstance()->GetMouseMovement();
		myRotation.x += distance.y * multiplier;
		myRotation.y += distance.x * multiplier;
		break;
	}
	case Crimson::eInputEvent::KeyDown:
	{
		if (aKey == Crimson::eKey::MouseRightButton)
		{
			auto& inputHandler = *Crimson::InputMapper::GetInstance();
			inputHandler.Attach(this, Crimson::eInputEvent::MouseMove);
			inputHandler.CaptureMouse(true);
			inputHandler.HideMouse();
			myIsAcceptingInput = true;
		}
		break;
	}
	case Crimson::eInputEvent::KeyUp:
	{
		if (aKey == Crimson::eKey::MouseRightButton)
		{
			auto& inputHandler = *Crimson::InputMapper::GetInstance();
			inputHandler.Detach(this, Crimson::eInputEvent::MouseMove);
			inputHandler.ReleaseMouse();
			inputHandler.ShowMouse();
			myIsAcceptingInput = false;
		}
		break;
	}
	case Crimson::eInputEvent::KeyHeld:
	{
		if (myIsAcceptingInput == false)
		{
			break;
		}

		myHasChanged = true;
		float multiplier = Crimson::InputMapper::GetInstance()->GetKeyHeld(Crimson::eKey::Shift) ? 2.f : Crimson::InputMapper::GetInstance()->GetKeyHeld(Crimson::eKey::Shift) ? 2.f : 1.f;
		float timeDelta = Crimson::Timer::GetDeltaTime();
		switch (aKey)
		{
		case Crimson::eKey::Q:
		{
			myRotation.y -= myRotationSpeed * timeDelta;
			break;
		}
		case Crimson::eKey::E:
		{
			myRotation.y += myRotationSpeed * timeDelta;
			break;
		}
		case Crimson::eKey::W:
		{
			Crimson::Vector4f movement = {};
			movement.z = myMovementSpeed * timeDelta * multiplier;
			movement *= Crimson::Matrix4x4f::CreateRotationAroundX(myRotation.x) * Crimson::Matrix4x4f::CreateRotationAroundY(myRotation.y);
			myPosition += movement;
			break;
		}
		case Crimson::eKey::S:
		{
			Crimson::Vector4f movement = {};
			movement.z = myMovementSpeed * timeDelta * multiplier;
			movement *= Crimson::Matrix4x4f::CreateRotationAroundX(myRotation.x) * Crimson::Matrix4x4f::CreateRotationAroundY(myRotation.y);
			myPosition -= movement;
			break;
		}
		case Crimson::eKey::A:
		{
			Crimson::Vector4f movement = {};
			movement.x = myMovementSpeed * timeDelta * multiplier;
			movement *= Crimson::Matrix4x4f::CreateRotationAroundX(myRotation.x) * Crimson::Matrix4x4f::CreateRotationAroundY(myRotation.y);
			myPosition -= movement;
			break;
		}
		case Crimson::eKey::D:
		{
			Crimson::Vector4f movement = {};
			movement.x = myMovementSpeed * timeDelta * multiplier;
			movement *= Crimson::Matrix4x4f::CreateRotationAroundX(myRotation.x) * Crimson::Matrix4x4f::CreateRotationAroundY(myRotation.y);
			myPosition += movement;
			break;
		}
		case Crimson::eKey::SpaceBar:
		{
			Crimson::Vector4f movement = {};
			movement.y = myMovementSpeed * timeDelta * multiplier;
			movement *= Crimson::Matrix4x4f::CreateRotationAroundX(myRotation.x) * Crimson::Matrix4x4f::CreateRotationAroundY(myRotation.y);
			myPosition += movement;
			break;
		}
		case Crimson::eKey::Ctrl:
		{
			Crimson::Vector4f movement = {};
			movement.y = myMovementSpeed * timeDelta * multiplier;
			movement *= Crimson::Matrix4x4f::CreateRotationAroundX(myRotation.x) * Crimson::Matrix4x4f::CreateRotationAroundY(myRotation.y);
			myPosition -= movement;
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
}

void PerspectiveCamera::ReceiveEvent(Crimson::eInputAction, float)
{
}

void PerspectiveCamera::SetPosition(const Crimson::Vector3f& aPosition)
{
	myPosition = aPosition;
	myHasChanged = true;
}

void PerspectiveCamera::SetRotation(const Crimson::Vector3f& aRotation)
{
	myRotation = aRotation;
	myHasChanged = true;
}

void PerspectiveCamera::SetFOV(float aDegree)
{
	myClipMatrix.m11 = CalculateFOV(Crimson::DegreeToRadian(aDegree));
	myClipMatrix.m22 = myClipMatrix.m11 * (myScreenResolution.x / myScreenResolution.y);
}

void PerspectiveCamera::SetPlanes(float aNearPlane, float aFarPlane)
{
	const float divideInverse = 1.f / (aFarPlane - aNearPlane);
	myClipMatrix.m33 = aFarPlane * divideInverse;
	myClipMatrix.m43 = -aNearPlane * aFarPlane * divideInverse;
}

void PerspectiveCamera::SetMovementSpeed(float aSpeed)
{
	myMovementSpeed = aSpeed;
}

void PerspectiveCamera::SetRotationSpeed(float aSpeed)
{
	myRotationSpeed = aSpeed;
}

void PerspectiveCamera::SetMouseSensitivity(float aValue)
{
	myMouseSensitivity = aValue;
}

Crimson::Vector3f PerspectiveCamera::TransformToClipSpace(const Crimson::Vector3f& aPoint) const
{
	Crimson::Vector4f clipSpace = Crimson::Vector4f(aPoint, 1.f) * myTransform * myClipMatrix;
	clipSpace.x /= clipSpace.w;
	clipSpace.y /= clipSpace.w;
	clipSpace.z /= clipSpace.w;
	return Crimson::Vector3f{clipSpace.x, clipSpace.y, clipSpace.z};
}

Crimson::Vector2f PerspectiveCamera::ConvertToScreenCoordinates(const Crimson::Vector3f& aClipSpacePosition, float& anOutScale) const
{
	Crimson::Vector2f screenPosition;
	float halfWidth = myScreenResolution.x * 0.5f;
	screenPosition.x = Crimson::Lerp(halfWidth, myScreenResolution.x, aClipSpacePosition.x);

	float halfHeight = myScreenResolution.y * 0.5f;
	screenPosition.y = Crimson::Lerp(halfHeight, myScreenResolution.y, aClipSpacePosition.y);

	anOutScale = 1.f - aClipSpacePosition.z;
	return screenPosition;
}

void PerspectiveCamera::UpdateTransform()
{
	myTransform = Crimson::Matrix4x4f::CreateRotationAroundX(myRotation.x) * Crimson::Matrix4x4f::CreateRotationAroundY(myRotation.y);

	myTransform.m41 = myPosition.x;
	myTransform.m42 = myPosition.y;
	myTransform.m43 = myPosition.z;

#ifndef _RETAIL
	// Transform if using UI
	//myCompassTransform = Crimson::Matrix4x4f::CreateRotationAroundX(-myRotation.x) * Crimson::Matrix4x4f::CreateRotationAroundY(-myRotation.y);
	//myCompassTransform(4, 3) = 0.5f;

	// Transform if using worldspace
	Crimson::Vector3f forward = Crimson::Vector4f{ myCompassOffset, 1.f } *myTransform;
	myCompassTransform.m41 = forward.x;
	myCompassTransform.m42 = forward.y;
	myCompassTransform.m43 = forward.z;
	myCompass.UpdateTransform(myCompassTransform);
#endif // _RETAIL

	myTransform = myTransform.GetInverse();
	myHasChanged = false;
}

float PerspectiveCamera::CalculateFOV(float aRadian) const
{
	return 1 / tan(aRadian * 0.5f);
}