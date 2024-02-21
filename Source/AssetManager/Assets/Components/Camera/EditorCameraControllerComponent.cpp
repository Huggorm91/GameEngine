#include "AssetManager.pch.h"
#include "EditorCameraControllerComponent.h"
#include "Input/InputMapper.h"
#include "Time/Timer.h"
#include "Assets/GameObject.h"
#include "PerspectiveCameraComponent.h"

EditorCameraControllerComponent::EditorCameraControllerComponent() : Component(ComponentType::EditorCameraController), myIsMoving(false), myMouseSensitivity(0.f), mySpeed(0.f)
{}

EditorCameraControllerComponent::EditorCameraControllerComponent(float aSpeed, float aSensitivity) : Component(ComponentType::EditorCameraController), myIsMoving(false), myMouseSensitivity(aSensitivity), mySpeed(aSpeed)
{}

EditorCameraControllerComponent::~EditorCameraControllerComponent()
{
	auto& input = *Crimson::InputMapper::GetInstance();

	input.Detach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::W);
	input.Detach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::A);
	input.Detach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::S);
	input.Detach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::D);
	input.Detach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::SpaceBar);
	input.Detach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::Ctrl);

	input.Detach(this, Crimson::eInputEvent::KeyDown, Crimson::eKey::MouseRightButton);
	input.Detach(this, Crimson::eInputEvent::KeyUp, Crimson::eKey::MouseRightButton);

	input.Detach(this, Crimson::eInputEvent::MouseMove);
}

void EditorCameraControllerComponent::Init(GameObject* aParent)
{
	Component::Init(aParent);
	auto& input = *Crimson::InputMapper::GetInstance();

	input.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::W);
	input.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::A);
	input.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::S);
	input.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::D);
	input.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::SpaceBar);
	input.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::Ctrl);

	input.Attach(this, Crimson::eInputEvent::KeyDown, Crimson::eKey::MouseRightButton);
	input.Attach(this, Crimson::eInputEvent::KeyUp, Crimson::eKey::MouseRightButton);
}

void EditorCameraControllerComponent::SetSpeed(float aSpeed)
{
	mySpeed = aSpeed;
}

void EditorCameraControllerComponent::SetMouseSensitivity(float aSensitivity)
{
	myMouseSensitivity = aSensitivity;
}

inline std::string EditorCameraControllerComponent::ToString() const
{
    return "EditorCameraController";
}

inline const EditorCameraControllerComponent* EditorCameraControllerComponent::GetTypePointer() const
{
    return this;
}

void EditorCameraControllerComponent::ReceiveEvent(Crimson::eInputEvent anEvent, Crimson::eKey aKey)
{
	if (!myIsActive || !myParent->IsActive())
	{
		return;
	}

	switch (anEvent)
	{
	case Crimson::eInputEvent::MouseMove:
	{
		float multiplier = myMouseSensitivity * .01f;
		auto& camera = myParent->GetComponent<PerspectiveCameraComponent>();
		Crimson::Vector2f distance = Crimson::InputMapper::GetInstance()->GetMouseMovement();
		Crimson::Vector3f rotation = camera.GetRadianRotation();
		rotation.x += distance.y * multiplier;
		rotation.y += distance.x * multiplier;
		camera.SetRadianRotation(rotation);
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
			myIsMoving = true;
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
			myIsMoving = false;
		}
		break;
	}
	case Crimson::eInputEvent::KeyHeld:
	{
		if (myIsMoving == false)
		{
			break;
		}

		auto& camera = myParent->GetComponent<PerspectiveCameraComponent>();
		float multiplier = mySpeed * Crimson::Timer::GetUnscaledDeltaTime();
		if (Crimson::InputMapper::GetInstance()->GetKeyDownOrHeld(Crimson::eKey::Shift))
		{
			multiplier *= 2.f;
		}
		Crimson::Matrix4x4f rotationMatrix = Crimson::Matrix4x4f::CreateRotationAroundX(camera.GetRadianRotation().x) * Crimson::Matrix4x4f::CreateRotationAroundY(camera.GetRadianRotation().y);

		switch (aKey)
		{
		case Crimson::eKey::W:
		{
			Crimson::Vector4f movement = {};
			movement.z = multiplier;
			movement *= rotationMatrix;
			camera.AddToPosition(movement);
			break;
		}
		case Crimson::eKey::S:
		{
			Crimson::Vector4f movement = {};
			movement.z = -multiplier;
			movement *= rotationMatrix;
			camera.AddToPosition(movement);
			break;
		}
		case Crimson::eKey::A:
		{
			Crimson::Vector4f movement = {};
			movement.x = -multiplier;
			movement *= rotationMatrix;
			camera.AddToPosition(movement);
			break;
		}
		case Crimson::eKey::D:
		{
			Crimson::Vector4f movement = {};
			movement.x = multiplier;
			movement *= rotationMatrix;
			camera.AddToPosition(movement);
			break;
		}
		case Crimson::eKey::SpaceBar:
		{
			Crimson::Vector4f movement = {};
			movement.y = multiplier;
			movement *= rotationMatrix;
			camera.AddToPosition(movement);
			break;
		}
		case Crimson::eKey::Ctrl:
		{
			Crimson::Vector4f movement = {};
			movement.y = -multiplier;
			movement *= rotationMatrix;
			camera.AddToPosition(movement);
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
