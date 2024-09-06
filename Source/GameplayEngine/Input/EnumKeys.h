#pragma once

namespace Crimson
{
	enum class eKey : int
	{
		None = -1,
		Any = 0,

		MouseLeftButton = 0x01,
		MouseRightButton = 0x02,

		Cancel = 0x03,

		MouseMiddleButton = 0x04,
		MouseXButton1 = 0x05,
		MouseXButton2 = 0x06,

		BackSpace = 0x08,
		Tab = 0x09,
		Clear = 0x0C,
		Enter = 0x0D,
		Return = 0x0D,

		Shift = 0x10,
		Control = 0x11,
		Ctrl = 0x11,
		Menu = 0x12,
		Alt = 0x12,
		Pause = 0x13,
		CapsLock = 0x14,
		Capital = 0x14,

		Kana = 0x15,
		Hangul = 0x15,
		ImeOn = 0x16,
		Junja = 0x17,
		Final = 0x18,
		Hanja = 0x19,
		Kanji = 0x19,
		ImeOff = 0x1A,

		Escape = 0x1B,
		Esc = 0x1B,

		Convert = 0x1C,
		NonConvert = 0x1D,
		Accept = 0x1E,
		ModeChange = 0x1F,

		SpaceBar = 0x20,
		Prior = 0x21,
		PageUp = 0x21,
		Next = 0x22,
		PageDown = 0x22,
		End = 0x23,
		Home = 0x24,

		Left = 0x25,
		Up = 0x26,
		Right = 0x27,
		Down = 0x28,

		Select = 0x29,
		Print = 0x2A,
		Execute = 0x2B,
		Snapshot = 0x2C,
		PrintScreen = 0x2C,
		Insert = 0x2D,
		Ins = 0x2D,
		Delete = 0x2E,
		Del = 0x2E,
		Help = 0x2F,

		Num0 = 0x30,
		Num1 = 0x31,
		Num2 = 0x32,
		Num3 = 0x33,
		Num4 = 0x34,
		Num5 = 0x35,
		Num6 = 0x36,
		Num7 = 0x37,
		Num8 = 0x38,
		Num9 = 0x39,

		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,

		LWin = 0x5B,
		LeftWindowsKey = 0x5B,
		RWin = 0x5C,
		RightWindowsKey = 0x5C,
		Apps = 0x5D,
		ApplicationsKey = 0x5D,

		Sleep = 0x5F,

		NumPad0 = 0x60,
		NumPad1 = 0x61,
		NumPad2 = 0x62,
		NumPad3 = 0x63,
		NumPad4 = 0x64,
		NumPad5 = 0x65,
		NumPad6 = 0x66,
		NumPad7 = 0x67,
		NumPad8 = 0x68,
		NumPad9 = 0x69,

		Multiply = 0x6A,
		Add = 0x6B,
		Plus = 0x6B,
		Separator = 0x6C,
		Subtract = 0x6D,
		Minus = 0x6D,
		Decimal = 0x6E,
		Divide = 0x6F,

		F1 = 0x70,
		F2 = 0x71,
		F3 = 0x72,
		F4 = 0x73,
		F5 = 0x74,
		F6 = 0x75,
		F7 = 0x76,
		F8 = 0x77,
		F9 = 0x78,
		F10 = 0x79,
		F11 = 0x7A,
		F12 = 0x7B,
		F13 = 0x7C,
		F14 = 0x7D,
		F15 = 0x7E,
		F16 = 0x7F,
		F17 = 0x80,
		F18 = 0x81,
		F19 = 0x82,
		F20 = 0x83,
		F21 = 0x84,
		F22 = 0x85,
		F23 = 0x86,
		F24 = 0x87,

		NumLock = 0x90,
		ScrollLock = 0x91,

		OEM_NEC_Equal = 0x92,   // '=' key on numpad
		OEM_FJ_Jisho = 0x92,	// 'Dictionary' key
		OEM_FJ_Masshou = 0x93,  // 'Unregister word' key
		OEM_FJ_Touroku = 0x94,  // 'Register word' key
		OEM_FJ_Loya = 0x95,		// 'Left OYAYUBI' key
		OEM_FJ_Roya = 0x96,		// 'Right OYAYUBI' key

		LShift = 0xA0,
		LeftShift = 0xA0,
		RShift = 0xA1,
		RightShift = 0xA1,
		LControl = 0xA2,
		LeftCtrl = 0xA2,
		RControl = 0xA3,
		RightCtrl = 0xA3,
		LMenu = 0xA4,
		LeftAlt = 0xA4,
		RMenu = 0xA5,
		RightAlt = 0xA5,

		BrowserBack = 0xA6,
		BrowserForward = 0xA7,
		BrowserRefresh = 0xA8,
		BrowserStop = 0xA9,
		BrowserSearch = 0xAA,
		BrowserFavorites = 0xAB,
		BrowserHome = 0xAC,

		VolumeMute = 0xAD,
		VolumeDown = 0xAE,
		VolumeUp = 0xAF,

		MediaNextTrack = 0xB0,
		MediaPrevTrack = 0xB1,
		MediaStop = 0xB2,
		MediaPlayPause = 0xB3,

		LaunchMail = 0xB4,
		LaunchMediaSelect = 0xB5,
		LaunchApp1 = 0xB6,
		LaunchApp2 = 0xB7,

		OEM_1 = 0xBA,	   // ';:' for US
		OEM_Plus = 0xBB,   // '+' for any country/region
		OEM_Comma = 0xBC,  // ',' for any country/region
		OEM_Minus = 0xBD,  // '-' for any country/region
		OEM_Period = 0xBE, // '.' for any country/region
		OEM_2 = 0xBF,	   // '/?' for US
		OEM_3 = 0xC0,	   // '`~' for US

		Gamepad_A = 0xC3,
		Gamepad_B = 0xC4,
		Gamepad_X = 0xC5,
		Gamepad_Y = 0xC6,
		Gamepad_RightShoulder = 0xC7,
		Gamepad_LeftShoulder = 0xC8,
		Gamepad_LeftTrigger = 0xC9,
		Gamepad_RightTrigger = 0xCA,
		Gamepad_DpadUp = 0xCB,
		Gamepad_DpadDown = 0xCC,
		Gamepad_DpadLeft = 0xCD,
		Gamepad_DpadRight = 0xCE,
		Gamepad_Menu = 0xCF,
		Gamepad_Start = 0xCF,
		Gamepad_View = 0xD0,
		Gamepad_Back = 0xD0,
		Gamepad_LeftThumbStickButton = 0xD1,
		Gamepad_RightThumbStickButton = 0xD2,
		Gamepad_LeftThumbStickUp = 0xD3,
		Gamepad_LeftThumbStickDown = 0xD4,
		Gamepad_LeftThumbStickRight = 0xD5,
		Gamepad_LeftThumbStickLeft = 0xD6,
		Gamepad_RightThumbStickUp = 0xD7,
		Gamepad_RightThumbStickDown = 0xD8,
		Gamepad_RightThumbStickRight = 0xD9,
		Gamepad_RightThumbStickLeft = 0xDA,

		OEM_4 = 0xDB,  //  '[{' for US
		OEM_5 = 0xDC,  //  '\|' for US
		OEM_6 = 0xDD,  //  ']}' for US
		OEM_7 = 0xDE,  //  ''"' for US
		OEM_8 = 0xDF,

		OEM_AX = 0xE1,   //  'AX' key on Japanese AX kbd
		OEM_102 = 0xE2,  //  "<>" US standard, "\|" non US 102-key keyboard
		ICO_Help = 0xE3, //  Help key on ICO
		ICO_00 = 0xE4,   //  00 key on ICO

		ProcessKey = 0xE5,

		ICO_Clear = 0xE6,

		Packet = 0xE7,

		OEM_Reset = 0xE9,
		OEM_Jump = 0xEA,
		OEM_PA1 = 0xEB,
		OEM_PA2 = 0xEC,
		OEM_PA3 = 0xED,
		OEM_WsCtrl = 0xEE,
		OEM_CuSel = 0xEF,
		OEM_Attn = 0xF0,
		OEM_Finish = 0xF1,
		OEM_Copy = 0xF2,
		OEM_Auto = 0xF3,
		OEM_Enlw = 0xF4,
		OEM_BackTab = 0xF5,

		Attn = 0xF6,
		CrSel = 0xF7,
		ExSel = 0xF8,
		EraseEOF = 0xF9,
		Play = 0xFA,
		ZOoom = 0xFB,
		NoName = 0xFC,
		PA1 = 0xFD,
		OEM_Clear = 0xFE,

		MouseMove_Up = -2,
		MouseMove_Down = -3,
		MouseMove_Left = -4,
		MouseMove_Right = -5,
		MouseScroll_Up = -6,
		MouseScroll_Down = -7,
		MouseScroll_Left = -8,
		MouseScroll_Right = -9,
	};
}