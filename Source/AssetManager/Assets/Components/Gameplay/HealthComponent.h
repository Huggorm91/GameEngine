#pragma once
#include "../Component.h"

BEGIN_COMPONENT(HealthComponent)
public:
	HealthComponent();
	HealthComponent(int aMaxHealth);
	HealthComponent(const HealthComponent& aComponent) = default;
	HealthComponent(HealthComponent&& aComponent) noexcept = default;
	HealthComponent(const Json::Value& aJson);
	~HealthComponent() = default;

	HealthComponent& operator=(const HealthComponent& aComponent) = default;
	HealthComponent& operator=(HealthComponent&& aComponent) noexcept = default;

	void TakeDamage(int aDamageAmount);
	void Heal(int aHealingAmount);
	void HealAll();

	void SetMaxHealth(int aMaxAmount);

	bool IsAlive() const;
	bool IsDead() const;

	void CreateImGuiComponents() override;
	Json::Value ToJson() const override;
	void Serialize(std::ostream& aStream) const override;
	void Deserialize(std::istream& aStream) override;

private:
	int myMaxHealth;
	int myCurrentHealth;
};