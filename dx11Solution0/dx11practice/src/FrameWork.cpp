#include "FrameWork.h"
#include "SystemDefs.h"
#include <iostream>
using namespace std;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

FrameWork::FrameWork(void) {

}

FrameWork::~FrameWork(void) {
    if (FULL_SCREEN) {
        ChangeDisplaySettings(NULL, 0);
    }
    UnregisterClass(m_applicationName, m_hInstance);
    m_hInstance = NULL;
}

bool FrameWork::Initialize() {
    if (!CreateDXWindow("DxPractice", WINDOW_POS_X, WINDOW_POS_Y, SCREEN_WIDTH, SCREEN_HEIGHT))
        return false;
    return true;
}

void FrameWork::Run() {
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            //todo
        }
    }
}

bool FrameWork::CreateDXWindow(const char * windowTitle, int x, int y, int width, int height) {
    HWND hwnd;
    WNDCLASSEX wc;

    m_applicationName = windowTitle;
    m_hInstance = GetModuleHandle(NULL);

    wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc      = WndProc;
    wc.cbClsExtra       = NULL;
    wc.cbWndExtra       = NULL;
    wc.hInstance        = m_hInstance;
    wc.hIcon            = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm          = wc.hIcon;
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = m_applicationName;
    wc.cbSize           = sizeof(WNDCLASSEX);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "RegisterClassEx() failed", "Error", MB_OK);
        return false;
    }
   
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    if (FULL_SCREEN) {
        DEVMODE dmScreenSettings;
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
        dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
        
        ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
    }
    else {
        screenWidth = width;
        screenHeight = height;
    }

    int nStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX;
    hwnd = CreateWindowEx(WS_EX_APPWINDOW, windowTitle, windowTitle, nStyle, x, y, screenWidth, screenHeight, NULL, NULL, m_hInstance, NULL);
    if (!hwnd) {
        MessageBox(NULL, "CreateWindowEx() failed", "Error", MB_OK);
        PostQuitMessage(0);
        return false;
    }

    ShowWindow(hwnd, SW_SHOW);
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message) {
    
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        break;

    case WM_CLOSE:
        PostQuitMessage(0);
        DestroyWindow(hwnd);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}