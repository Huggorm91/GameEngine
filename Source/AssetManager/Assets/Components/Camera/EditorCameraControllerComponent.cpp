#include "AssetManager.pch.h"
#include "EditorCameraControllerComponent.h"
#include "GameplayEngine/Input/InputMapper.h"
#include "Assets/GameObject.h"
#include "PerspectiveCameraComponent.h"

EditorCameraControllerComponent::EditorCameraControllerComponent() : Component(ComponentType::EditorCameraController), myIsMoving(false), myMouseSensitivity(0.f), mySpeed(0.f)
{}

EditorCameraControllerComponent::EditorCameraControllerComponent(float aSpeed, float aSensitivity) : Component(ComponentType::EditorCameraController), myIsMoving(false), myMouseSensitivity(aSensitivity), mySpeed(aSpeed)
{}

void EditorCameraControllerComponent::Init(GameObject* aParent)
{
	Component::Init(aParent);
	SubscribeToEvents();
}

void EditorCameraControllerComponent::SetSpeed(float aSpeed)
{
	mySpeed = aSpeed;
}

void EditorCameraControllerComponent::SetMouseSensitivity(float aSensitivity)
{
	myMouseSensitivity = aSensitivity;
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
		Crimson::Vector2f distance = Engine::GetInputMapper().GetMouseMovement();
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
			Attach(Crimson::eInputEvent::MouseMove);
			auto& inputHandler = Engine::GetInputMapper();
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
			Detach(Crimson::eInputEvent::MouseMove);
			auto& inputHandler = Engine::GetInputMapper();
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
		float multiplier = mySpeed * Crimson::Time::GetUnscaledDeltaTime();
		if (Engine::GetInputMapper().GetKeyDownOrHeld(Crimson::eKey::Shift))
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

void EditorCameraControllerComponent::ComponentPointersInvalidated()
{
	RenewInputObserverPointer();
	SubscribeToEvents();
}

void EditorCameraControllerComponent::SubscribeToEvents()
{
	Attach(Crimson::eInputEvent::KeyHeld, Crimson::eKey::W);
	Attach(Crimson::eInputEvent::KeyHeld, Crimson::eKey::A);
	Attach(Crimson::eInputEvent::KeyHeld, Crimson::eKey::S);
	Attach(Crimson::eInputEvent::KeyHeld, Crimson::eKey::D);
	Attach(Crimson::eInputEvent::KeyHeld, Crimson::eKey::SpaceBar);
	Attach(Crimson::eInputEvent::KeyHeld, Crimson::eKey::Ctrl);

	Attach(Crimson::eInputEvent::KeyDown, Crimson::eKey::MouseRightButton);
	Attach(Crimson::eInputEvent::KeyUp, Crimson::eKey::MouseRightButton);
}
