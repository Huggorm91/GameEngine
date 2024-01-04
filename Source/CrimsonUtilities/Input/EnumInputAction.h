﻿#pragma once

namespace Crimson
{
	enum class eKeyAction : int
	{
		All,
		KeyDown,
		KeyHeld,
		KeyUp,
		KeyDownOrHeld
	};

	enum class eInputAction : int
	{
		MenuUp,
		MenuDown,
		MenuLeft,
		MenuRight,
		MoveUp,
		MoveDown,
		MoveLeft,
		MoveRight,
		Jump,
		Duck,
		Attack,
		Shoot,
		Back,
		Accept,
		Decline,
		Sprint,
		Interact,
		Restart,
		DebugNextLevel,
		DebugPreviousLevel,
		MoveVerticaly,
		MoveHorizontaly,
		DodgeRoll,
		PauseGame, 
		Undo,
		Redo
	};
}
