#pragma once
#include <vector>

namespace CollisionLayer
{
	enum Layer : unsigned
	{
		Default,
		Ground,
		Player,
		Enemy,
		PlayerProjectile,
		EnemyProjectile,
		ExplodingBarrel,
		HealingZone,
		Count
	};

	inline static std::vector<std::string> globalLayerNames{
	"Default",
	"Ground",
	"Player",
	"Enemy",
	"PlayerProjectile",
	"EnemyProjectile",
	"ExplodingBarrel",
	"HealingZone"
	};
}