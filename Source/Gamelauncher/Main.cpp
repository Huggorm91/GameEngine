#include "Gamelauncher.pch.h"
#include "GameplayEngine/Engine.h"
#include "CrimsonUtilities/Time/Time.h"
#include "GameLauncher.h"

LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);

GameLauncher globalLauncher;

#pragma warning(disable:6387)
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	FILE* consoleOut;
	FILE* consoleErr;
	AllocConsole();
	freopen_s(&consoleOut, "CONOUT$", "w", stdout);
	freopen_s(&consoleErr, "CONOUT$", "w", stderr);
	setvbuf(consoleOut, nullptr, _IONBF, 1024);

	HWND consoleWindow = GetConsoleWindow();
	SetConsoleCtrlHandler(CtrlHandler, TRUE);
	const Crimson::Vector2i consoleSize = { 1280, 720 };
	int monitorCount = GetSystemMetrics(SM_CMONITORS);
	if (monitorCount > 1)
	{
		RECT virtualSize{};
		virtualSize.right = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		virtualSize.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		virtualSize.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
		virtualSize.top = GetSystemMetrics(SM_YVIRTUALSCREEN);

		RECT desktopSize;
		GetWindowRect(GetDesktopWindow(), &desktopSize);

		Crimson::Vector2i consolePos;
		if (virtualSize.left < 0 && desktopSize.right < virtualSize.right) // Secondary monitor to the left
		{
			consolePos.x = desktopSize.left - consoleSize.x;
			consolePos.y = desktopSize.top;
		}
		else if (desktopSize.right < virtualSize.right)// Secondary monitor to the right
		{
			consolePos.x = virtualSize.right - desktopSize.right;
			consolePos.y = desktopSize.top;
		}
		else if (virtualSize.top < 0) // Secondary monitor on top
		{
			// I am too lazy too account for anything else than the taskbar being on the bottom of the screen
			RECT taskbar;
			GetWindowRect(FindWindow(L"Shell_traywnd", NULL), &taskbar);
			const int taskbarHeight = taskbar.bottom - taskbar.top;
			consolePos.x = desktopSize.left;
			consolePos.y = desktopSize.top - consoleSize.y - taskbarHeight;
		}
		else // Secondary monitor below
		{
			consolePos.x = desktopSize.left;
			consolePos.y = virtualSize.bottom - desktopSize.bottom;
		}

		SetWindowPos(consoleWindow, HWND_TOP, consolePos.x, consolePos.y, consoleSize.x, consoleSize.y, 0);
	}
	else
	{
		RECT consolePos;
		GetWindowRect(consoleWindow, &consolePos);
		MoveWindow(consoleWindow, consolePos.left, consolePos.top, consoleSize.x, consoleSize.y, true);
	}

	globalLauncher.Initialize(hInstance, WinProc);
	return globalLauncher.Run();
}
#pragma warning(default:6387)

LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
	case WM_CLOSE:
	{
		globalLauncher.Shutdown();
		PostQuitMessage(0);
		return 0;
	}
	case WM_SETFOCUS:
	case WM_EXITSIZEMOVE:
	{
		Crimson::Time::ResetDeltaTime();
		break;
	}
	default:
		break;
	}

	if (Engine::HandleInput(uMsg, wParam, lParam))
	{
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
	case CTRL_CLOSE_EVENT:
	{
		globalLauncher.Shutdown();
		PostQuitMessage(0);
		return TRUE;
	}
	default:
		return FALSE;
	}
}