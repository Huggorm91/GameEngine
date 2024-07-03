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
		Zoom = 0xFB,
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

	constexpr inline std::string KeyToString(eKey aKey)
	{
		switch (aKey)
		{
		case Crimson::eKey::None:
		{
			return "None";
		}
		case Crimson::eKey::Any:
		{
			return "Any";
		}
		case Crimson::eKey::MouseLeftButton:
		{
			return "LeftMouse";
		}
		case Crimson::eKey::MouseRightButton:
		{
			return "RightMouse";
		}
		case Crimson::eKey::Cancel:
		{
			return "Cancel";
		}
		case Crimson::eKey::MouseMiddleButton:
		{
			return "MiddleMouse";
		}
		case Crimson::eKey::MouseXButton1:
		{
			return "MouseX1";
		}
		case Crimson::eKey::MouseXButton2:
		{
			return "MouseX2";
		}
		case Crimson::eKey::BackSpace:
		{
			return "BackSpace";
		}
		case Crimson::eKey::Tab:
		{
			return "Tab";
		}
		case Crimson::eKey::Clear:
		{
			return "Clear";
		}
		case Crimson::eKey::Enter:
		{
			return "Enter";
		}
		case Crimson::eKey::Shift:
		{
			return "Shift";
		}
		case Crimson::eKey::Ctrl:
		{
			return "Ctrl";
		}
		case Crimson::eKey::Alt:
		{
			return "Alt";
		}
		case Crimson::eKey::Pause:
		{
			return "Pause";
		}
		case Crimson::eKey::CapsLock:
		{
			return "CapsLock";
		}
		case Crimson::eKey::Kana:
		{
			return "Kana";
		}
		case Crimson::eKey::ImeOn:
		{
			return "ImeOn";
		}
		case Crimson::eKey::Junja:
		{
			return "Junja";
		}
		case Crimson::eKey::Final:
		{
			return "Final";
		}
		case Crimson::eKey::Kanji:
		{
			return "Kanji";
		}
		case Crimson::eKey::ImeOff:
		{
			return "ImeOff";
		}
		case Crimson::eKey::Esc:
		{
			return "Esc";
		}
		case Crimson::eKey::Convert:
		{
			return "Convert";
		}
		case Crimson::eKey::NonConvert:
		{
			return "NonConvert";
		}
		case Crimson::eKey::Accept:
		{
			return "Accept";
		}
		case Crimson::eKey::ModeChange:
		{
			return "ModeChange";
		}
		case Crimson::eKey::SpaceBar:
		{
			return "Space";
		}
		case Crimson::eKey::PageUp:
		{
			return "PageUp";
		}
		case Crimson::eKey::PageDown:
		{
			return "PageDown";
		}
		case Crimson::eKey::End:
		{
			return "End";
		}
		case Crimson::eKey::Home:
		{
			return "Home";
		}
		case Crimson::eKey::Left:
		{
			return "Left";
		}
		case Crimson::eKey::Up:
		{
			return "Up";
		}
		case Crimson::eKey::Right:
		{
			return "Right";
		}
		case Crimson::eKey::Down:
		{
			return "Down";
		}
		case Crimson::eKey::Select:
		{
			return "Select";
		}
		case Crimson::eKey::Print:
		{
			return "Print";
		}
		case Crimson::eKey::Execute:
		{
			return "Execute";
		}
		case Crimson::eKey::PrintScreen:
		{
			return "PrintScreen";
		}
		case Crimson::eKey::Insert:
		{
			return "Insert";
		}
		case Crimson::eKey::Delete:
		{
			return "Delete";
		}
		case Crimson::eKey::Help:
		{
			return "Help";
		}
		case Crimson::eKey::Num0:
		{
			return "0";
		}
		case Crimson::eKey::Num1:
		{
			return "1";
		}
		case Crimson::eKey::Num2:
		{
			return "2";
		}
		case Crimson::eKey::Num3:
		{
			return "3";
		}
		case Crimson::eKey::Num4:
		{
			return "4";
		}
		case Crimson::eKey::Num5:
		{
			return "5";
		}
		case Crimson::eKey::Num6:
		{
			return "6";
		}
		case Crimson::eKey::Num7:
		{
			return "7";
		}
		case Crimson::eKey::Num8:
		{
			return "8";
		}
		case Crimson::eKey::Num9:
		{
			return "9";
		}
		case Crimson::eKey::A:
		{
			return "A";
		}
		case Crimson::eKey::B:
		{
			return "B";
		}
		case Crimson::eKey::C:
		{
			return "C";
		}
		case Crimson::eKey::D:
		{
			return "D";
		}
		case Crimson::eKey::E:
		{
			return "E";
		}
		case Crimson::eKey::F:
		{
			return "F";
		}
		case Crimson::eKey::G:
		{
			return "G";
		}
		case Crimson::eKey::H:
		{
			return "H";
		}
		case Crimson::eKey::I:
		{
			return "I";
		}
		case Crimson::eKey::J:
		{
			return "J";
		}
		case Crimson::eKey::K:
		{
			return "K";
		}
		case Crimson::eKey::L:
		{
			return "L";
		}
		case Crimson::eKey::M:
		{
			return "M";
		}
		case Crimson::eKey::N:
		{
			return "N";
		}
		case Crimson::eKey::O:
		{
			return "O";
		}
		case Crimson::eKey::P:
		{
			return "P";
		}
		case Crimson::eKey::Q:
		{
			return "Q";
		}
		case Crimson::eKey::R:
		{
			return "R";
		}
		case Crimson::eKey::S:
		{
			return "S";
		}
		case Crimson::eKey::T:
		{
			return "T";
		}
		case Crimson::eKey::U:
		{
			return "U";
		}
		case Crimson::eKey::V:
		{
			return "V";
		}
		case Crimson::eKey::W:
		{
			return "W";
		}
		case Crimson::eKey::X:
		{
			return "X";
		}
		case Crimson::eKey::Y:
		{
			return "Y";
		}
		case Crimson::eKey::Z:
		{
			return "Z";
		}
		case Crimson::eKey::LeftWindowsKey:
		{
			return "LeftWindows";
		}
		case Crimson::eKey::RightWindowsKey:
		{
			return "RightWindows";
		}
		case Crimson::eKey::Apps:
		{
			return "Apps";
		}
		case Crimson::eKey::Sleep:
		{
			return "Sleep";
		}
		case Crimson::eKey::NumPad0:
		{
			return "NumPad0";
		}
		case Crimson::eKey::NumPad1:
		{
			return "NumPad1";
		}
		case Crimson::eKey::NumPad2:
		{
			return "NumPad2";
		}
		case Crimson::eKey::NumPad3:
		{
			return "NumPad3";
		}
		case Crimson::eKey::NumPad4:
		{
			return "NumPad4";
		}
		case Crimson::eKey::NumPad5:
		{
			return "NumPad5";
		}
		case Crimson::eKey::NumPad6:
		{
			return "NumPad6";
		}
		case Crimson::eKey::NumPad7:
		{
			return "NumPad7";
		}
		case Crimson::eKey::NumPad8:
		{
			return "NumPad8";
		}
		case Crimson::eKey::NumPad9:
		{
			return "NumPad9";
		}
		case Crimson::eKey::Multiply:
		{
			return "*";
		}
		case Crimson::eKey::Plus:
		{
			return "+";
		}
		case Crimson::eKey::Separator:
		{
			return "Separator";
		}
		case Crimson::eKey::Minus:
		{
			return "-";
		}
		case Crimson::eKey::Decimal:
		{
			return ",";
		}
		case Crimson::eKey::Divide:
		{
			return "/";
		}
		case Crimson::eKey::F1:
		{
			return "F1";
		}
		case Crimson::eKey::F2:
		{
			return "F2";
		}
		case Crimson::eKey::F3:
		{
			return "F3";
		}
		case Crimson::eKey::F4:
		{
			return "F4";
		}
		case Crimson::eKey::F5:
		{
			return "F5";
		}
		case Crimson::eKey::F6:
		{
			return "F6";
		}
		case Crimson::eKey::F7:
		{
			return "F7";
		}
		case Crimson::eKey::F8:
		{
			return "F8";
		}
		case Crimson::eKey::F9:
		{
			return "F9";
		}
		case Crimson::eKey::F10:
		{
			return "F10";
		}
		case Crimson::eKey::F11:
		{
			return "F11";
		}
		case Crimson::eKey::F12:
		{
			return "F12";
		}
		case Crimson::eKey::F13:
		{
			return "F13";
		}
		case Crimson::eKey::F14:
		{
			return "F14";
		}
		case Crimson::eKey::F15:
		{
			return "F15";
		}
		case Crimson::eKey::F16:
		{
			return "F16";
		}
		case Crimson::eKey::F17:
		{
			return "F17";
		}
		case Crimson::eKey::F18:
		{
			return "F18";
		}
		case Crimson::eKey::F19:
		{
			return "F19";
		}
		case Crimson::eKey::F20:
		{
			return "F20";
		}
		case Crimson::eKey::F21:
		{
			return "F21";
		}
		case Crimson::eKey::F22:
		{
			return "F22";
		}
		case Crimson::eKey::F23:
		{
			return "F23";
		}
		case Crimson::eKey::F24:
		{
			return "F24";
		}
		case Crimson::eKey::NumLock:
		{
			return "NumLock";
		}
		case Crimson::eKey::ScrollLock:
		{
			return "ScrollLock";
		}
		case Crimson::eKey::OEM_FJ_Jisho:
		{
			return "FJ_Jisho";
		}
		case Crimson::eKey::OEM_FJ_Masshou:
		{
			return "FJ_Masshou";
		}
		case Crimson::eKey::OEM_FJ_Touroku:
		{
			return "FJ_Touroku";
		}
		case Crimson::eKey::OEM_FJ_Loya:
		{
			return "FJ_Loya";
		}
		case Crimson::eKey::OEM_FJ_Roya:
		{
			return "FJ_Roya";
		}
		case Crimson::eKey::LeftShift:
		{
			return "LeftShift";
		}
		case Crimson::eKey::RightShift:
		{
			return "RightShift";
		}
		case Crimson::eKey::LeftCtrl:
		{
			return "LeftCtrl";
		}
		case Crimson::eKey::RightCtrl:
		{
			return "RightCtrl";
		}
		case Crimson::eKey::LeftAlt:
		{
			return "LeftAlt";
		}
		case Crimson::eKey::RightAlt:
		{
			return "RightAlt";
		}
		case Crimson::eKey::BrowserBack:
		{
			return "BrowserBack";
		}
		case Crimson::eKey::BrowserForward:
		{
			return "BrowserForward";
		}
		case Crimson::eKey::BrowserRefresh:
		{
			return "BrowserRefresh";
		}
		case Crimson::eKey::BrowserStop:
		{
			return "BrowserStop";
		}
		case Crimson::eKey::BrowserSearch:
		{
			return "BrowserSearch";
		}
		case Crimson::eKey::BrowserFavorites:
		{
			return "BrowserFavorites";
		}
		case Crimson::eKey::BrowserHome:
		{
			return "BrowserHome";
		}
		case Crimson::eKey::VolumeMute:
		{
			return "VolumeMute";
		}
		case Crimson::eKey::VolumeDown:
		{
			return "VolumeDown";
		}
		case Crimson::eKey::VolumeUp:
		{
			return "VolumeUp";
		}
		case Crimson::eKey::MediaNextTrack:
		{
			return "MediaNextTrack";
		}
		case Crimson::eKey::MediaPrevTrack:
		{
			return "MediaPrevTrack";
		}
		case Crimson::eKey::MediaStop:
		{
			return "MediaStop";
		}
		case Crimson::eKey::MediaPlayPause:
		{
			return "MediaPlayPause";
		}
		case Crimson::eKey::LaunchMail:
		{
			return "LaunchMail";
		}
		case Crimson::eKey::LaunchMediaSelect:
		{
			return "LaunchMediaSelect";
		}
		case Crimson::eKey::LaunchApp1:
		{
			return "LaunchApp1";
		}
		case Crimson::eKey::LaunchApp2:
		{
			return "LaunchApp2";
		}
		case Crimson::eKey::OEM_1:
		{
			return "OEM_1";
		}
		case Crimson::eKey::OEM_Plus:
		{
			return "OEM_Plus";
		}
		case Crimson::eKey::OEM_Comma:
		{
			return "OEM_Comma";
		}
		case Crimson::eKey::OEM_Minus:
		{
			return "OEM_Minus";
		}
		case Crimson::eKey::OEM_Period:
		{
			return "OEM_Period";
		}
		case Crimson::eKey::OEM_2:
		{
			return "OEM_2";
		}
		case Crimson::eKey::OEM_3:
		{
			return "OEM_3";
		}
		case Crimson::eKey::Gamepad_A:
		{
			return "Gamepad_A";
		}
		case Crimson::eKey::Gamepad_B:
		{
			return "Gamepad_B";
		}
		case Crimson::eKey::Gamepad_X:
		{
			return "Gamepad_X";
		}
		case Crimson::eKey::Gamepad_Y:
		{
			return "Gamepad_Y";
		}
		case Crimson::eKey::Gamepad_RightShoulder:
		{
			return "Gamepad_RightShoulder";
		}
		case Crimson::eKey::Gamepad_LeftShoulder:
		{
			return "Gamepad_LeftShoulder";
		}
		case Crimson::eKey::Gamepad_LeftTrigger:
		{
			return "Gamepad_LeftTrigger";
		}
		case Crimson::eKey::Gamepad_RightTrigger:
		{
			return "Gamepad_RightTrigger";
		}
		case Crimson::eKey::Gamepad_DpadUp:
		{
			return "Gamepad_DpadUp";
		}
		case Crimson::eKey::Gamepad_DpadDown:
		{
			return "Gamepad_DpadDown";
		}
		case Crimson::eKey::Gamepad_DpadLeft:
		{
			return "Gamepad_DpadLeft";
		}
		case Crimson::eKey::Gamepad_DpadRight:
		{
			return "Gamepad_DpadRight";
		}
		case Crimson::eKey::Gamepad_Menu:
		{
			return "Gamepad_Menu";
		}
		case Crimson::eKey::Gamepad_View:
		{
			return "Gamepad_View";
		}
		case Crimson::eKey::Gamepad_LeftThumbStickButton:
		{
			return "Gamepad_LeftThumbStickButton";
		}
		case Crimson::eKey::Gamepad_RightThumbStickButton:
		{
			return "Gamepad_RightThumbStickButton";
		}
		case Crimson::eKey::OEM_4:
		{
			return "OEM_4";
		}
		case Crimson::eKey::OEM_5:
		{
			return "OEM_5";
		}
		case Crimson::eKey::OEM_6:
		{
			return "OEM_6";
		}
		case Crimson::eKey::OEM_7:
		{
			return "OEM_7";
		}
		case Crimson::eKey::OEM_8:
		{
			return "OEM_8";
		}
		case Crimson::eKey::OEM_AX:
		{
			return "OEM_AX";
		}
		case Crimson::eKey::OEM_102:
		{
			return "OEM_102";
		}
		case Crimson::eKey::ICO_Help:
		{
			return "ICO_Help";
		}
		case Crimson::eKey::ICO_00:
		{
			return "ICO_00";
		}
		case Crimson::eKey::ProcessKey:
		{
			return "ProcessKey";
		}
		case Crimson::eKey::ICO_Clear:
		{
			return "ICO_Clear";
		}
		case Crimson::eKey::Packet:
		{
			return "Packet";
		}
		case Crimson::eKey::OEM_Reset:
		{
			return "OEM_Reset";
		}
		case Crimson::eKey::OEM_Jump:
		{
			return "OEM_Jump";
		}
		case Crimson::eKey::OEM_PA1:
		{
			return "OEM_PA1";
		}
		case Crimson::eKey::OEM_PA2:
		{
			return "OEM_PA2";
		}
		case Crimson::eKey::OEM_PA3:
		{
			return "OEM_PA3";
		}
		case Crimson::eKey::OEM_WsCtrl:
		{
			return "OEM_WsCtrl";
		}
		case Crimson::eKey::OEM_CuSel:
		{
			return "OEM_CuSel";
		}
		case Crimson::eKey::OEM_Attn:
		{
			return "OEM_Attn";
		}
		case Crimson::eKey::OEM_Finish:
		{
			return "OEM_Finish";
		}
		case Crimson::eKey::OEM_Copy:
		{
			return "OEM_Copy";
		}
		case Crimson::eKey::OEM_Auto:
		{
			return "OEM_Auto";
		}
		case Crimson::eKey::OEM_Enlw:
		{
			return "OEM_Enlw";
		}
		case Crimson::eKey::OEM_BackTab:
		{
			return "OEM_BackTab";
		}
		case Crimson::eKey::Attn:
		{
			return "Attn";
		}
		case Crimson::eKey::CrSel:
		{
			return "CrSel";
		}
		case Crimson::eKey::ExSel:
		{
			return "ExSel";
		}
		case Crimson::eKey::EraseEOF:
		{
			return "EraseEOF";
		}
		case Crimson::eKey::Play:
		{
			return "Play";
		}
		case Crimson::eKey::Zoom:
		{
			return "Zoom";
		}
		case Crimson::eKey::NoName:
		{
			return "NoName";
		}
		case Crimson::eKey::PA1:
		{
			return "PA1";
		}
		case Crimson::eKey::OEM_Clear:
		{
			return "OEM_Clear";
		}
		default:
		{
			return "Unknown";
		}
		}
	}
}