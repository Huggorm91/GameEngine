#pragma once
#include "../Component.h"

class HealthComponent : public Component
{
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

	bool IsAlive();
	bool IsDead();

	void CreateImGuiComponents(const std::string& aWindowName) override;
	Json::Value ToJson() const override;
	void Serialize(std::ostream& aStream) const override;
	void Deserialize(std::istream& aStream) override;

	inline std::string ToString() const override;
	inline const HealthComponent* GetTypePointer() const override;

private:
	int myMaxHealth;
	int myCurrentHealth;
};