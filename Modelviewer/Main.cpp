#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include "resource.h"
#include "Core/Modelviewer.h"
#include <InputHandler.h>
#include "ThirdParty/DearImGui/ImGui/imgui.h"
#include "ThirdParty/DearImGui/ImGui/imgui_impl_win32.h"
#include "ThirdParty/DearImGui/ImGui/imgui_impl_dx11.h"

CommonUtilities::InputHandler globalInputHandler;
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

    constexpr SIZE windowSize = { 1920, 1080 };
    constexpr LPCWSTR windowTitle = L"Modelviewer";

    ModelViewer& MV = ModelViewer::Get();

    FILE* consoleOut;
    FILE* consoleErr;
    AllocConsole();
    freopen_s(&consoleOut, "CONOUT$", "w", stdout);
    freopen_s(&consoleErr, "CONOUT$", "w", stderr);
    setvbuf(consoleOut, nullptr, _IONBF, 1024);

	HWND consoleWindow = GetConsoleWindow();
    RECT consoleSize;
    GetWindowRect(consoleWindow, &consoleSize);
    MoveWindow(consoleWindow, consoleSize.left, consoleSize.top, 1280, 720, true);

    MV.Initialize(hInstance, windowSize, WinProc, windowTitle);
    return MV.Run();
}

LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
    {
        return true;
    }
    if (uMsg == WM_DESTROY || uMsg == WM_CLOSE)
    {
        PostQuitMessage(0);
    }
    if (globalInputHandler.UpdateEvents(uMsg, wParam, lParam))
    {
        return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}