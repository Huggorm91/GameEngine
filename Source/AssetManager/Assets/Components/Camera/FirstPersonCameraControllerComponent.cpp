#include "AssetManager.pch.h"
#include "FirstPersonCameraControllerComponent.h"
#include "GameplayEngine/Input/InputMapper.h"
#include "Assets/GameObject.h"
#include "PerspectiveCameraComponent.h"

FirstPersonCameraControllerComponent::FirstPersonCameraControllerComponent() : Component(ComponentType::FirstPersonCameraController), myMouseSensitivity(0.f), mySpeed(0.f)
{
}

FirstPersonCameraControllerComponent::FirstPersonCameraControllerComponent(float aSpeed, float aSensitivity) : Component(ComponentType::FirstPersonCameraController), myMouseSensitivity(aSensitivity), mySpeed(aSpeed)
{
}

FirstPersonCameraControllerComponent::FirstPersonCameraControllerComponent(const Json::Value& aJson) : Component(aJson), myMouseSensitivity(aJson["MouseSensitivity"].asFloat()), mySpeed(aJson["Speed"].asFloat())
{
}

void FirstPersonCameraControllerComponent::Init(GameObject* aParent)
{
	Component::Init(aParent);
	auto& input = Engine::GetInputMapper();

	input.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::W);
	input.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::A);
	input.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::S);
	input.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::D);
	input.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::SpaceBar);
	input.Attach(this, Crimson::eInputEvent::KeyHeld, Crimson::eKey::Ctrl);

	input.Attach(this, Crimson::eInputEvent::MouseMove);
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
