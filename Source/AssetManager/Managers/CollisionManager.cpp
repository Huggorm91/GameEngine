#include "AssetManager.pch.h"
#include "CollisionManager.h"
#include "Assets/GameObject.h"

void CollisionManager::AddCollider(ColliderComponent* aCollider)
{
	if (auto iter = myCollisionObjects.find(aCollider->GetLayer()); iter != myCollisionObjects.end())
	{
		iter->second.emplace(aCollider);
	}
	else
	{
		myCollisionObjects.emplace(aCollider->GetLayer(), std::unordered_set<ColliderComponent*>());
		myCollisionObjects.at(aCollider->GetLayer()).emplace(aCollider);
	}
}

void CollisionManager::RemoveCollider(ColliderComponent* aCollider)
{
	if (auto iter = myCollisionObjects.find(aCollider->GetLayer()); iter != myCollisionObjects.end())
	{
		if (auto componentIter = iter->second.find(aCollider); componentIter != iter->second.end())
		{
			iter->second.erase(componentIter);
		}
	}
}

void CollisionManager::CheckCollisions()
{
	for (auto& [layer, componentList] : myCollisionObjects)
	{
		for (auto& component : componentList)
		{
			for (auto& [collidingLayer, collidingComponentList] : myCollisionObjects)
			{
				// Early escape if component doesnt collide with this layer
				if (component->CollidesWithLayer(collidingLayer) == false)
				{
					continue;
				}

				ColliderComponent* lower;
				ColliderComponent* higher;
				for (auto& collidingComponent : collidingComponentList)
				{
					// Check if self
					if (component == collidingComponent)
					{
						continue;
					}

					// Check if they already have collided this frame
					lower = component < collidingComponent ? component : collidingComponent;
					higher = lower == collidingComponent ? component : collidingComponent;
					if (auto iter = myCollisions.find(lower); iter != myCollisions.end())
					{
						if (iter->second.find(higher) != iter->second.end())
						{
							continue;
						}
					}

					if (component->IsColliding(*collidingComponent))
					{
						// Add to Current collision
						if (auto iter = myCollisions.find(lower); iter != myCollisions.end())
						{
							iter->second.emplace(higher);
						}
						else
						{
							myCollisions.emplace(lower, std::unordered_set{ higher });
						}

						// Check if they were colliding last frame
						if (auto lowerIter = myPreviousCollisions.find(lower); lowerIter != myPreviousCollisions.end())
						{
							if (auto higherIter = lowerIter->second.find(higher); higherIter != lowerIter->second.end())
							{
								// Remove from previous collision (Since they are still colliding)
								if (lowerIter->second.size() > 1)
								{
									lowerIter->second.erase(higherIter);
								}
								else
								{
									myPreviousCollisions.erase(lowerIter);
								}

								CollisionStay(component, collidingComponent);
							}
							else
							{
								CollisionEnter(component, collidingComponent);
							}
						}
						else
						{
							CollisionEnter(component, collidingComponent);
						}
					}
				}
			}
		}
	}

	for (auto& [firstCollider, collideList] : myPreviousCollisions)
	{
		for (auto& secondCollider : collideList)
		{
			CollisionExit(firstCollider, secondCollider);
		}
	}
}

void CollisionManager::EndFrame()
{
	myCollisionObjects.clear();
	myPreviousCollisions = myCollisions;
	myCollisions.clear();
}

void CollisionManager::Reset()
{
	myCollisionObjects.clear();
	myPreviousCollisions.clear();
	myCollisions.clear();
}

void CollisionManager::CollisionEnter(ColliderComponent* aFirst, ColliderComponent* aSecond) const
{
	if (aFirst->IsTrigger() || aSecond->IsTrigger())
	{
		aSecond->GetParent().OnTriggerEnter(aFirst->GetLayer(), aFirst);
		aFirst->GetParent().OnTriggerEnter(aSecond->GetLayer(), aSecond);
	}
	else
	{
		aSecond->GetParent().OnCollisionEnter(aFirst->GetLayer(), aFirst);
		aFirst->GetParent().OnCollisionEnter(aSecond->GetLayer(), aSecond);
	}
}

void CollisionManager::CollisionStay(ColliderComponent* aFirst, ColliderComponent* aSecond) const
{
	if (aFirst->IsTrigger() || aSecond->IsTrigger())
	{
		aSecond->GetParent().OnTriggerStay(aFirst->GetLayer(), aFirst);
		aFirst->GetParent().OnTriggerStay(aSecond->GetLayer(), aSecond);
	}
	else
	{
		aSecond->GetParent().OnCollisionStay(aFirst->GetLayer(), aFirst);
		aFirst->GetParent().OnCollisionStay(aSecond->GetLayer(), aSecond);
	}
}

void CollisionManager::CollisionExit(ColliderComponent* aFirst, ColliderComponent* aSecond) const
{
	if (aFirst->IsTrigger() || aSecond->IsTrigger())
	{
		aSecond->GetParent().OnTriggerExit(aFirst->GetLayer(), aFirst);
		aFirst->GetParent().OnTriggerExit(aSecond->GetLayer(), aSecond);
	}
	else
	{
		aSecond->GetParent().OnCollisionExit(aFirst->GetLayer(), aFirst);
		aFirst->GetParent().OnCollisionExit(aSecond->GetLayer(), aSecond);
	}
}