#pragma once

namespace CommonUtilities
{
	enum eCollisionLayer
	{
		eGround,
		ePlayer,
		eEnemy,
		ePlayerProjectile,
		eEnemyProjectile,
		ePickUp,
		eCount
	};

	enum class eCollider : int
	{
		Sphere,
		AABB,
		Plane,
		PlaneVolume,
		Ray,
		Count
	};

	enum class eCollider2D : int
	{
		Circle,
		AABB,
		Line,
		LineVolume,
		Ray,
		Count
	};
}