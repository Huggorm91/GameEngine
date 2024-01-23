#pragma once

namespace Crimson
{
	enum class eMessageType : int
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
	};
}