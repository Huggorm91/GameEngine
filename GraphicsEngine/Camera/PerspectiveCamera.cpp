#include "GraphicsEngine.pch.h"
#include "PerspectiveCamera.h"
#include <Conversions.hpp>
#include <Timer.h>
#include "../GraphicsEngine.h"
#include "../Commands/GfxCmd_SetFrameBuffer.h"

PerspectiveCamera::PerspectiveCamera() : myTransform(), myMovementSpeed(), myRotationSpeed(), myRotation(), myPosition(), myScreenResolution(), myIsAcceptingInput(false), myMouseSensitivity(1.f), myHasChanged(true), myCompassTransform(), myCompass(), myCompassOffset()
{
}

void PerspectiveCamera::Init(const CommonUtilities::Vector2f& aScreenSize)
{
	myScreenResolution = aScreenSize;

	myClipMatrix(3, 4) = 1.f;
	myClipMatrix(4, 4) = 0.f;

	myPosition = { 0.f, 100.f, 0.f };
	myRotationSpeed = 2.f;
	myMovementSpeed = 100.f;

	const float nearplane = 1.f;
	const float farplane = 10000.f;

	SetFOV(90.f);
	SetPlanes(nearplane, farplane);

	myCompassOffset = { 1.8f, 1.f, nearplane + 1.f };

	// Using UI
	//myCompass = GraphicsEngine::Get().GetLineDrawer().AddAxisLines({ 0.f, 0.f, 0.f }, .1f, false, { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 1.f }, GetColor(eColor::Red), GetColor(eColor::Green), GetColor(eColor::Blue), CommonUtilities::Matrix4x4f::Null);

	// Using worldspace
	myCompass = GraphicsEngine::Get().GetLineDrawer().AddAxisLines({ 0.f, 0.f, 0.f }, .1f, false);

	UpdateTransform();

	auto& inputHandler = *CommonUtilities::InputMapper::GetInstance();
	inputHandler.Attach(this, CommonUtilities::eInputEvent::KeyHeld, CommonUtilities::eKey::Q);
	inputHandler.Attach(this, CommonUtilities::eInputEvent::KeyHeld, CommonUtilities::eKey::E);
	inputHandler.Attach(this, CommonUtilities::eInputEvent::KeyHeld, CommonUtilities::eKey::W);
	inputHandler.Attach(this, CommonUtilities::eInputEvent::KeyHeld, CommonUtilities::eKey::A);
	inputHandler.Attach(this, CommonUtilities::eInputEvent::KeyHeld, CommonUtilities::eKey::S);
	inputHandler.Attach(this, CommonUtilities::eInputEvent::KeyHeld, CommonUtilities::eKey::D);
	inputHandler.Attach(this, CommonUtilities::eInputEvent::KeyHeld, CommonUtilities::eKey::SpaceBar);
	inputHandler.Attach(this, CommonUtilities::eInputEvent::KeyHeld, CommonUtilities::eKey::Ctrl);

	inputHandler.Attach(this, CommonUtilities::eInputEvent::KeyDown, CommonUtilities::eKey::MouseRightButton);
	inputHandler.Attach(this, CommonUtilities::eInputEvent::KeyUp, CommonUtilities::eKey::MouseRightButton);

}

void PerspectiveCamera::Update()
{
	if (myHasChanged)
	{
		UpdateTransform();
	}
	GraphicsEngine::Get().AddGraphicsCommand(std::make_shared<GfxCmd_SetFrameBuffer>(myTransform, myClipMatrix, myPosition));
}

void PerspectiveCamera::ReceiveEvent(CommonUtilities::eInputEvent anEvent, CommonUtilities::eKey aKey)
{
	switch (anEvent)
	{
	case CommonUtilities::eInputEvent::MouseMove:
	{
		myHasChanged = true;
		float multiplier = myMouseSensitivity * .01f;
		CommonUtilities::Vector2f distance = CommonUtilities::InputMapper::GetInstance()->GetMouseMovement();
		myRotation.x += distance.y * multiplier;
		myRotation.y += distance.x * multiplier;
		break;
	}
	case CommonUtilities::eInputEvent::KeyDown:
	{
		if (aKey == CommonUtilities::eKey::MouseRightButton)
		{
			auto& inputHandler = *CommonUtilities::InputMapper::GetInstance();
			inputHandler.Attach(this, CommonUtilities::eInputEvent::MouseMove);
			inputHandler.CaptureMouse(true);
			inputHandler.HideMouse();
			myIsAcceptingInput = true;
		}
		break;
	}
	case CommonUtilities::eInputEvent::KeyUp:
	{
		if (aKey == CommonUtilities::eKey::MouseRightButton)
		{
			auto& inputHandler = *CommonUtilities::InputMapper::GetInstance();
			inputHandler.Detach(this, CommonUtilities::eInputEvent::MouseMove);
			inputHandler.ReleaseMouse();
			inputHandler.ShowMouse();
			myIsAcceptingInput = false;
		}
		break;
	}
	case CommonUtilities::eInputEvent::KeyHeld:
	{
		if (myIsAcceptingInput == false)
		{
			break;
		}

		myHasChanged = true;
		float multiplier = CommonUtilities::InputMapper::GetInstance()->GetKeyHeld(CommonUtilities::eKey::Shift) ? 2.f : CommonUtilities::InputMapper::GetInstance()->GetKeyHeld(CommonUtilities::eKey::Shift) ? 2.f : 1.f;
		float timeDelta = CommonUtilities::Timer::GetDeltaTime();
		switch (aKey)
		{
		case CommonUtilities::eKey::Q:
		{
			myRotation.y -= myRotationSpeed * timeDelta;
			break;
		}
		case CommonUtilities::eKey::E:
		{
			myRotation.y += myRotationSpeed * timeDelta;
			break;
		}
		case CommonUtilities::eKey::W:
		{
			CommonUtilities::Vector4f movement = {};
			movement.z = myMovementSpeed * timeDelta * multiplier;
			movement *= CommonUtilities::Matrix4x4f::CreateRotationAroundX(myRotation.x) * CommonUtilities::Matrix4x4f::CreateRotationAroundY(myRotation.y);
			myPosition += static_cast<CommonUtilities::Vector3f>(movement);
			break;
		}
		case CommonUtilities::eKey::S:
		{
			CommonUtilities::Vector4f movement = {};
			movement.z = myMovementSpeed * timeDelta * multiplier;
			movement *= CommonUtilities::Matrix4x4f::CreateRotationAroundX(myRotation.x) * CommonUtilities::Matrix4x4f::CreateRotationAroundY(myRotation.y);
			myPosition -= static_cast<CommonUtilities::Vector3f>(movement);
			break;
		}
		case CommonUtilities::eKey::A:
		{
			CommonUtilities::Vector4f movement = {};
			movement.x = myMovementSpeed * timeDelta * multiplier;
			movement *= CommonUtilities::Matrix4x4f::CreateRotationAroundX(myRotation.x) * CommonUtilities::Matrix4x4f::CreateRotationAroundY(myRotation.y);
			myPosition -= static_cast<CommonUtilities::Vector3f>(movement);
			break;
		}
		case CommonUtilities::eKey::D:
		{
			CommonUtilities::Vector4f movement = {};
			movement.x = myMovementSpeed * timeDelta * multiplier;
			movement *= CommonUtilities::Matrix4x4f::CreateRotationAroundX(myRotation.x) * CommonUtilities::Matrix4x4f::CreateRotationAroundY(myRotation.y);
			myPosition += static_cast<CommonUtilities::Vector3f>(movement);
			break;
		}
		case CommonUtilities::eKey::SpaceBar:
		{
			CommonUtilities::Vector4f movement = {};
			movement.y = myMovementSpeed * timeDelta * multiplier;
			movement *= CommonUtilities::Matrix4x4f::CreateRotationAroundX(myRotation.x) * CommonUtilities::Matrix4x4f::CreateRotationAroundY(myRotation.y);
			myPosition += static_cast<CommonUtilities::Vector3f>(movement);
			break;
		}
		case CommonUtilities::eKey::Ctrl:
		{
			CommonUtilities::Vector4f movement = {};
			movement.y = myMovementSpeed * timeDelta * multiplier;
			movement *= CommonUtilities::Matrix4x4f::CreateRotationAroundX(myRotation.x) * CommonUtilities::Matrix4x4f::CreateRotationAroundY(myRotation.y);
			myPosition -= static_cast<CommonUtilities::Vector3f>(movement);
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

void PerspectiveCamera::ReceiveEvent(CommonUtilities::eInputAction, float)
{
}

void PerspectiveCamera::SetPosition(const CommonUtilities::Vector3f& aPosition)
{
	myPosition = aPosition;
	myHasChanged = true;
}

void PerspectiveCamera::SetRotation(const CommonUtilities::Vector3f& aRotation)
{
	myRotation = aRotation;
	myHasChanged = true;
}

void PerspectiveCamera::SetFOV(float aDegree)
{
	myClipMatrix(1, 1) = CalculateFOV(CommonUtilities::DegreeToRadian(aDegree));
	myClipMatrix(2, 2) = myClipMatrix(1, 1) * (myScreenResolution.x / myScreenResolution.y);
}

void PerspectiveCamera::SetPlanes(float aNearPlane, float aFarPlane)
{
	const float divideInverse = 1.f / (aFarPlane - aNearPlane);
	myClipMatrix(3, 3) = aFarPlane * divideInverse;
	myClipMatrix(4, 3) = -aNearPlane * aFarPlane * divideInverse;
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

CommonUtilities::Vector3f PerspectiveCamera::TransformToClipSpace(const CommonUtilities::Vector3f& aPoint) const
{
	CommonUtilities::Vector4f clipSpace = CommonUtilities::Vector4f(aPoint, 1.f) * myTransform * myClipMatrix;
	clipSpace.x /= clipSpace.w;
	clipSpace.y /= clipSpace.w;
	clipSpace.z /= clipSpace.w;
	return CommonUtilities::Vector3f{clipSpace.x, clipSpace.y, clipSpace.z};
}

CommonUtilities::Vector2f PerspectiveCamera::ConvertToScreenCoordinates(const CommonUtilities::Vector3f& aClipSpacePosition, float& anOutScale) const
{
	CommonUtilities::Vector2f screenPosition;
	float halfWidth = myScreenResolution.x * 0.5f;
	screenPosition.x = CommonUtilities::Lerp(halfWidth, myScreenResolution.x, aClipSpacePosition.x);

	float halfHeight = myScreenResolution.y * 0.5f;
	screenPosition.y = CommonUtilities::Lerp(halfHeight, myScreenResolution.y, aClipSpacePosition.y);

	anOutScale = 1.f - aClipSpacePosition.z;
	return screenPosition;
}

void PerspectiveCamera::UpdateTransform()
{
	myTransform = CommonUtilities::Matrix4x4f::CreateRotationAroundX(myRotation.x) * CommonUtilities::Matrix4x4f::CreateRotationAroundY(myRotation.y);

	myTransform(4, 1) = myPosition.x;
	myTransform(4, 2) = myPosition.y;
	myTransform(4, 3) = myPosition.z;

	// Transform if using UI
	//myCompassTransform = CommonUtilities::Matrix4x4f::CreateRotationAroundX(-myRotation.x) * CommonUtilities::Matrix4x4f::CreateRotationAroundY(-myRotation.y);
	//myCompassTransform(4, 3) = 0.5f;
	
	// Transform if using worldspace
	CommonUtilities::Vector3f forward = CommonUtilities::Vector4f{ myCompassOffset, 1.f } * myTransform;
	myCompassTransform(4, 1) = forward.x;
	myCompassTransform(4, 2) = forward.y;
	myCompassTransform(4, 3) = forward.z;
	myCompass.UpdateTransform(myCompassTransform);

	myTransform = CommonUtilities::Matrix4x4f::GetFastInverse(myTransform);
	myHasChanged = false;
}

float PerspectiveCamera::CalculateFOV(float aRadian) const
{
	return 1 / tan(aRadian * 0.5f);
}