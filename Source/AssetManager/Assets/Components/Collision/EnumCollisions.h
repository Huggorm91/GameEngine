#pragma once

enum eCollisionLayer : unsigned
{
	eCollisionLayer_Default,
	eCollisionLayer_Ground,
	eCollisionLayer_Player,
	eCollisionLayer_Enemy,
	eCollisionLayer_PlayerProjectile,
	eCollisionLayer_EnemyProjectile,
	eCollisionLayer_Count
};

inline static std::vector<std::string> globalLayerNames{
	"Default",
	"Ground",
	"Player",
	"Enemy",
	"PlayerProjectile",
	"EnemyProjectile",
};