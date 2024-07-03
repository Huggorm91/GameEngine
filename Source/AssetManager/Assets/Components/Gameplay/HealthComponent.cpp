#include "AssetManager.pch.h"
#include "HealthComponent.h"
#include "PostMaster/PostMaster.h"

HealthComponent::HealthComponent() : Component(ComponentType::Health), myMaxHealth(0), myCurrentHealth(0)
{}

HealthComponent::HealthComponent(int aMaxHealth) : Component(ComponentType::Health), myMaxHealth(aMaxHealth), myCurrentHealth(aMaxHealth)
{}

HealthComponent::HealthComponent(const Json::Value& aJson) : Component(aJson), myMaxHealth(aJson["MaxHealth"].asInt()), myCurrentHealth(myMaxHealth)
{}

void HealthComponent::TakeDamage(int aDamageAmount)
{
	if (IsDead())
	{
		return;
	}

	myCurrentHealth -= aDamageAmount;
	if (IsAlive())
	{
		Crimson::PostMaster::GetInstance()->AddMessage({Crimson::eMessageType::GameObject_TakeDamage, static_cast<int>(GetParentID())});
	}
	else
	{
		Crimson::PostMaster::GetInstance()->AddMessage({ Crimson::eMessageType::GameObject_Died, static_cast<int>(GetParentID()) });
	}
}

void HealthComponent::Heal(int aHealingAmount)
{
	myCurrentHealth += aHealingAmount;
	if (myCurrentHealth > myMaxHealth)
	{
		myCurrentHealth = myMaxHealth;
	}
}

void HealthComponent::HealAll()
{
	myCurrentHealth = myMaxHealth;
}

void HealthComponent::SetMaxHealth(int aMaxAmount)
{
	myMaxHealth = aMaxAmount;
}

bool HealthComponent::IsAlive() const
{
	return myCurrentHealth > 0;
}

bool HealthComponent::IsDead() const
{
	return myCurrentHealth < 1;
}

void HealthComponent::CreateImGuiComponents()
{
	Component::CreateImGuiComponents();
	ImGui::Text(("Current Health: " + std::to_string(myCurrentHealth)).c_str());
	ImGui::InputInt("Max Health", &myMaxHealth);
	if (ImGui::Button("Restore Health"))
	{
		HealAll();
	}
}

Json::Value HealthComponent::ToJson() const
{
	auto result = Component::ToJson();
	result["MaxHealth"] = myMaxHealth;
	return result;
}

void HealthComponent::Serialize(std::ostream& aStream) const
{
	Component::Serialize(aStream);
	aStream.write(reinterpret_cast<const char*>(&myMaxHealth), sizeof(myMaxHealth));
}

void HealthComponent::Deserialize(std::istream& aStream)
{
	Component::Deserialize(aStream);
	aStream.read(reinterpret_cast<char*>(&myMaxHealth), sizeof(myMaxHealth));
}