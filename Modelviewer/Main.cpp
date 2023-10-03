#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include "resource.h"
#include "Core/Modelviewer.h"
#include "Input/InputHandler.h"
#include "ThirdParty/DearImGui/ImGui/imgui.h"
#include "ThirdParty/DearImGui/ImGui/imgui_impl_win32.h"
#include "ThirdParty/DearImGui/ImGui/imgui_impl_dx11.h"

Crimson::InputHandler globalInputHandler;
LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    //constexpr SIZE windowSize = { 1920, 1080 };
    //constexpr LPCWSTR windowTitle = L"Modelviewer";

    ModelViewer& MV = ModelViewer::Get();

    FILE* consoleOut;
    FILE* consoleErr;
    AllocConsole();
    freopen_s(&consoleOut, "CONOUT$", "w", stdout);
    freopen_s(&consoleErr, "CONOUT$", "w", stderr);
    setvbuf(consoleOut, nullptr, _IONBF, 1024);

	HWND consoleWindow = GetConsoleWindow();
    const Crimson::Vector2i consoleSize = { 1280, 720 };
    int monitorCount = GetSystemMetrics(SM_CMONITORS);
    if (monitorCount > 1)
    {
        RECT virtualSize;
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
    
    MV.Initialize(hInstance, WinProc);
    return MV.Run();
}

LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    if (uMsg == WM_DESTROY || uMsg == WM_CLOSE)
    {
        PostQuitMessage(0);
    }
#ifndef _RETAIL
    if (uMsg == WM_DROPFILES)
    {
        ModelViewer::Get().SetDropFile((HDROP)wParam);
        return 0;
    }
#endif // !_RETAIL
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
    {
        return true;
    }
    if (globalInputHandler.UpdateEvents(uMsg, wParam, lParam))
    {
        return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}