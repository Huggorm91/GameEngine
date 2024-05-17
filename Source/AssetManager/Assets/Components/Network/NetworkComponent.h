#pragma once
#include "../Component.h"
#include <bitset>

class NetworkComponent : public Component
{
public:
	NetworkComponent();
	NetworkComponent(const NetworkComponent& aComponent) = default;
	NetworkComponent(NetworkComponent&& aComponent) noexcept = default;
	NetworkComponent(const Json::Value& aJson);
	~NetworkComponent() = default;

	NetworkComponent& operator=(const NetworkComponent& aComponent) = default;
	NetworkComponent& operator=(NetworkComponent&& aComponent) noexcept = default;

	// void Update() override;

	// void TransformHasChanged() const override;

	// void CreateImGuiComponents(const std::string& aWindowName) override;
	Json::Value ToJson() const override;
	inline std::string ToString() const override;
	inline const NetworkComponent* GetTypePointer() const override;

private:
	enum SyncFlags
	{
		eSyncTransform,
		eSyncCount
	};
	std::bitset<eSyncCount> mySyncFlags;

	enum StateFlags
	{
		eTransformHasChanged,
		eStateCount
	};
	std::bitset<eStateCount> myRaisedFlags;
};