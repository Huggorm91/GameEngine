#include "AssetManager.pch.h"
#include "FirstPersonCameraControllerComponent.h"
#include "GameplayEngine/Input/InputMapper.h"
#include "Assets/GameObject.h"
#include "PerspectiveCameraComponent.h"

// TODO: Remove everything inside #ifndef _RETAIL
// Its only there for a school assignment

FirstPersonCameraControllerComponent::FirstPersonCameraControllerComponent() :
	Component(ComponentType::FirstPersonCameraController),
#ifndef _RETAIL
	myIsMoving(false),
#endif // !_RETAIL
	myMouseSensitivity(0.f),
	mySpeed(0.f)
{
}

FirstPersonCameraControllerComponent::FirstPersonCameraControllerComponent(float aSpeed, float aSensitivity) :
	Component(ComponentType::FirstPersonCameraController),
#ifndef _RETAIL
	myIsMoving(false),
#endif // !_RETAIL
	myMouseSensitivity(aSensitivity),
	mySpeed(aSpeed)
{
}

FirstPersonCameraControllerComponent::FirstPersonCameraControllerComponent(const Json::Value& aJson) :
	Component(aJson),
#ifndef _RETAIL
	myIsMoving(false),
#endif // !_RETAIL
	myMouseSensitivity(aJson["MouseSensitivity"].asFloat()),
	mySpeed(aJson["Speed"].asFloat())
{
}

void FirstPersonCameraControllerComponent::Init(GameObject* aParent)
{
	Component::Init(aParent);
	SubscribeToEvents();
}

void FirstPersonCameraControllerComponent::SetSpeed(float aSpeed)
{
	mySpeed = aSpeed;
}

void FirstPersonCameraControllerComponent::SetMouseSensitivity(float aSensitivity)
{
	myMouseSensitivity = aSensitivity;
}

void FirstPersonCameraControllerComponent::ReceiveEvent(Crimson::eInputEvent anEvent, Crimson::eKey aKey)
{
	if (!myIsActive || !myParent->IsActive())
	{
		return;
	}

	switch (anEvent)
	{
	case Crimson::eInputEvent::MouseMove:
	{
		auto& camera = myParent->GetComponent<PerspectiveCameraComponent>();
		float multiplier = myMouseSensitivity * .01f;
		Crimson::Vector2f distance = Engine::GetInputMapper().GetMouseMovement();
		Crimson::Vector3f rotation = myParent->GetTransform().GetRotationRadian();
		rotation.x += distance.y * multiplier;
		rotation.y += distance.x * multiplier;
		myParent->SetRotation(rotation, true);
		camera.SetRadianRotation(rotation);
		break;
	}
#ifndef _RETAIL
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
		else if (aKey == Crimson::eKey::Num1)
		{
			Engine::ToogleUsingGrid();
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
#endif // !_RETAIL
	case Crimson::eInputEvent::KeyHeld:
	{
		float multiplier = mySpeed * Crimson::Time::GetUnscaledDeltaTime();
		if (Engine::GetInputMapper().GetKeyDownOrHeld(Crimson::eKey::Shift))
		{
			multiplier *= 2.f;
		}
		auto& radianRotation = myParent->GetTransform().GetRotationRadian();
		Crimson::Matrix4x4f rotationMatrix = Crimson::Matrix4x4f::CreateRotationAroundX(radianRotation.x) * Crimson::Matrix4x4f::CreateRotationAroundY(radianRotation.y);

		switch (aKey)
		{
		case Crimson::eKey::W:
		{
			Crimson::Vector4f movement = {};
			movement.z = multiplier;
			movement *= rotationMatrix;
			myParent->AddToPosition(movement);
			break;
		}
		case Crimson::eKey::S:
		{
			Crimson::Vector4f movement = {};
			movement.z = -multiplier;
			movement *= rotationMatrix;
			myParent->AddToPosition(movement);
			break;
		}
		case Crimson::eKey::A:
		{
			Crimson::Vector4f movement = {};
			movement.x = -multiplier;
			movement *= rotationMatrix;
			myParent->AddToPosition(movement);
			break;
		}
		case Crimson::eKey::D:
		{
			Crimson::Vector4f movement = {};
			movement.x = multiplier;
			movement *= rotationMatrix;
			myParent->AddToPosition(movement);
			break;
		}
		case Crimson::eKey::SpaceBar:
		{
			Crimson::Vector4f movement = {};
			movement.y = multiplier;
			movement *= rotationMatrix;
			myParent->AddToPosition(movement);
			break;
		}
		case Crimson::eKey::Ctrl:
		{
			Crimson::Vector4f movement = {};
			movement.y = -multiplier;
			movement *= rotationMatrix;
			myParent->AddToPosition(movement);
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

void FirstPersonCameraControllerComponent::Serialize(std::ostream& aStream) const
{
	Component::Serialize(aStream);
	aStream.write(reinterpret_cast<const char*>(&myMouseSensitivity), sizeof(myMouseSensitivity));
	aStream.write(reinterpret_cast<const char*>(&mySpeed), sizeof(mySpeed));
}

void FirstPersonCameraControllerComponent::Deserialize(std::istream& aStream)
{
	aStream.read(reinterpret_cast<char*>((&myMouseSensitivity)), sizeof(myMouseSensitivity));
	aStream.read(reinterpret_cast<char*>(&mySpeed), sizeof(mySpeed));
}

Json::Value FirstPersonCameraControllerComponent::ToJson() const
{
	Json::Value result = Component::ToJson();
	result["MouseSensitivity"] = myMouseSensitivity;
	result["Speed"] = mySpeed;
	return result;
}

void FirstPersonCameraControllerComponent::ComponentPointersInvalidated()
{
	RenewInputObserverPointer();
	SubscribeToEvents();
}

void FirstPersonCameraControllerComponent::SubscribeToEvents()
{
	Attach(Crimson::eInputEvent::KeyHeld, Crimson::eKey::W);
	Attach(Crimson::eInputEvent::KeyHeld, Crimson::eKey::A);
	Attach(Crimson::eInputEvent::KeyHeld, Crimson::eKey::S);
	Attach(Crimson::eInputEvent::KeyHeld, Crimson::eKey::D);
	Attach(Crimson::eInputEvent::KeyHeld, Crimson::eKey::SpaceBar);
	Attach(Crimson::eInputEvent::KeyHeld, Crimson::eKey::Ctrl);
	Attach(Crimson::eInputEvent::KeyDown, Crimson::eKey::Num1);

#ifndef _RETAIL
	Attach(Crimson::eInputEvent::KeyDown, Crimson::eKey::MouseRightButton);
	Attach(Crimson::eInputEvent::KeyUp, Crimson::eKey::MouseRightButton);
#else
	Attach(Crimson::eInputEvent::MouseMove);
#endif // !_RETAIL
}
