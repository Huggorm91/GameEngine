#pragma once

namespace Crimson
{
	enum class eMessageType : unsigned
	{
		Node_TimerEnded,
		Node_TimerStopped,
		Node_TimerStopAll,

		Collision_OnTriggerEnter,
		Collision_OnTriggerExit,
		Collision_OnTriggerStay,
		Collision_OnCollisionEnter,
		Collision_OnCollisionExit,
		Collision_OnCollisionStay,

		GameObject_TakeDamage,
		GameObject_Died,

		Count
	};
}