#include "GameplayEngine.pch.h"
#include "CollisionManager.h"
#include "../Engine.h"
#include "../PostMaster/PostMaster.h"
#include "AssetManager/Assets/GameObject.h"
#include "AssetManager/Assets/Components/Collision/ColliderComponent.h"

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
	myRemovedColliders.emplace(aCollider);
}

void CollisionManager::ChangeLayer(ColliderComponent* aCollider, CollisionLayer::Layer aNewLayer)
{
	myRemovedColliders.emplace(aCollider);

	if (auto iter = myCollisionObjects.find(aNewLayer); iter != myCollisionObjects.end())
	{
		iter->second.emplace(aCollider);
	}
	else
	{
		myCollisionObjects.emplace(aNewLayer, std::unordered_set<ColliderComponent*>());
		myCollisionObjects.at(aNewLayer).emplace(aCollider);
	}
}

void CollisionManager::CheckCollisions()
{
	for (auto& [layer, componentList] : myCollisionObjects)
	{
		for (auto& component : componentList)
		{
			// Check if removed
			if (myRemovedColliders.contains(component))
			{
				continue;
			}

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

					// Check if removed
					if (myRemovedColliders.contains(collidingComponent))
					{
						continue;
					}

					// Check if they already have collided this frame
					lower = component < collidingComponent ? component : collidingComponent;
					higher = lower == collidingComponent ? component : collidingComponent;
					if (auto iter = myCollisions.find(lower); iter != myCollisions.end())
					{
						if (iter->second.contains(higher))
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

	// Handle Collision Exit
	for (auto& [firstCollider, collideList] : myPreviousCollisions)
	{
		// Check if removed
		if (myRemovedColliders.contains(firstCollider))
		{
			continue;
		}
		for (auto& secondCollider : collideList)
		{
			// Check if removed
			if (myRemovedColliders.contains(secondCollider))
			{
				continue;
			}
			CollisionExit(firstCollider, secondCollider);
		}
	}
	myRemovedColliders.clear();
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
		if (aFirst->IsTrigger())
		{
			Engine::GetPostMaster().SendInstantMessage({ Crimson::eMessageType::Collision_OnTriggerEnter, std::pair(aFirst->GetParentID(), aSecond->GetParentID()) });
		}
		else
		{
			Engine::GetPostMaster().SendInstantMessage({ Crimson::eMessageType::Collision_OnTriggerEnter, std::pair(aSecond->GetParentID(), aFirst->GetParentID()) });
		}
	}
	else
	{
		aSecond->GetParent().OnCollisionEnter(aFirst->GetLayer(), aFirst);
		aFirst->GetParent().OnCollisionEnter(aSecond->GetLayer(), aSecond);
		Engine::GetPostMaster().SendInstantMessage({ Crimson::eMessageType::Collision_OnCollisionEnter, std::pair(aFirst->GetParentID(), aSecond->GetParentID())});
	}
}

void CollisionManager::CollisionStay(ColliderComponent* aFirst, ColliderComponent* aSecond) const
{
	if (aFirst->IsTrigger() || aSecond->IsTrigger())
	{
		aSecond->GetParent().OnTriggerStay(aFirst->GetLayer(), aFirst);
		aFirst->GetParent().OnTriggerStay(aSecond->GetLayer(), aSecond);
		if (aFirst->IsTrigger())
		{
			Engine::GetPostMaster().SendInstantMessage({ Crimson::eMessageType::Collision_OnTriggerStay, std::pair(aFirst->GetParentID(), aSecond->GetParentID())});
		}
		else
		{
			Engine::GetPostMaster().SendInstantMessage({ Crimson::eMessageType::Collision_OnTriggerStay, std::pair(aSecond->GetParentID(), aFirst->GetParentID()) });
		}
	}
	else
	{
		aSecond->GetParent().OnCollisionStay(aFirst->GetLayer(), aFirst);
		aFirst->GetParent().OnCollisionStay(aSecond->GetLayer(), aSecond);
		Engine::GetPostMaster().SendInstantMessage({ Crimson::eMessageType::Collision_OnCollisionStay, std::pair(aFirst->GetParentID(), aSecond->GetParentID())});
	}
}

void CollisionManager::CollisionExit(ColliderComponent* aFirst, ColliderComponent* aSecond) const
{
	if (aFirst->IsTrigger() || aSecond->IsTrigger())
	{
		aSecond->GetParent().OnTriggerExit(aFirst->GetLayer(), aFirst);
		aFirst->GetParent().OnTriggerExit(aSecond->GetLayer(), aSecond);
		if (aFirst->IsTrigger())
		{
			Engine::GetPostMaster().SendInstantMessage({ Crimson::eMessageType::Collision_OnTriggerExit, std::pair(aFirst->GetParentID(), aSecond->GetParentID())});
		}
		else
		{
			Engine::GetPostMaster().SendInstantMessage({ Crimson::eMessageType::Collision_OnTriggerExit, std::pair(aSecond->GetParentID(), aFirst->GetParentID()) });
		}
	}
	else
	{
		aSecond->GetParent().OnCollisionExit(aFirst->GetLayer(), aFirst);
		aFirst->GetParent().OnCollisionExit(aSecond->GetLayer(), aSecond);
		Engine::GetPostMaster().SendInstantMessage({ Crimson::eMessageType::Collision_OnCollisionExit, std::pair(aFirst->GetParentID(), aSecond->GetParentID())});
	}
}