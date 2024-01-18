#pragma once
#include "ColliderForwards.h"

bool Intersection(const BoxColliderComponent& aFirstBox, const BoxColliderComponent& aSecondBox);
bool Intersection(const BoxColliderComponent& aBox, const RayColliderComponent& aRay);
bool Intersection(const BoxColliderComponent& aBox, const SphereColliderComponent& aSphere);
bool Intersection(const BoxColliderComponent& aBox, const CapsuleColliderComponent& aCapsule);

bool Intersection(const SphereColliderComponent& aFirstSphere, const SphereColliderComponent& aSecondSphere);
bool Intersection(const SphereColliderComponent& aSphere, const RayColliderComponent& aRay);
bool Intersection(const SphereColliderComponent& aSphere, const CapsuleColliderComponent& aCapsule);

bool Intersection(const CapsuleColliderComponent& aFirstCapsule, const CapsuleColliderComponent& aSecondCapsule);
bool Intersection(const CapsuleColliderComponent& aCapsule, const RayColliderComponent& aRay);

bool Intersection(const RayColliderComponent& aFirstRay, const RayColliderComponent& aSecondRay);