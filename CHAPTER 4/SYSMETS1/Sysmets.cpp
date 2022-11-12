#include <Windows.h> 
#include <assert.h>
#include "Sysmets.h"

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    static TCHAR szClassName[] = TEXT("The Standard Window"); 
    static TCHAR szAppName[] = TEXT("Displaying the system metrics"); 

    HWND hWnd = NULL; 

    WNDCLASSEX wnd; 
    MSG msg; 

    ZeroMemory(&wnd, sizeof(WNDCLASSEX)); 
    ZeroMemory(&msg, sizeof(MSG)); 

    wnd.cbSize = sizeof(WNDCLASSEX); 
    wnd.cbClsExtra = 0; 
    wnd.cbWndExtra = 0; 
    wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wnd.hCursor = LoadCursor(NULL, IDC_ARROW); 
    wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION); 
    wnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION); 
    wnd.hInstance = hInstance; 
    wnd.lpfnWndProc = WndProc; 
    wnd.lpszClassName = szClassName; 
    wnd.lpszMenuName = NULL; 
    wnd.style = CS_HREDRAW | CS_VREDRAW; 

    assert(RegisterClassEx(&wnd) != 0); 

    hWnd = CreateWindowEx(WS_EX_APPWINDOW, szClassName, szAppName, WS_OVERLAPPEDWINDOW, 
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
        NULL, NULL, hInstance, NULL); 
    assert(hWnd); 

    ShowWindow(hWnd, nShowCmd); 
    UpdateWindow(hWnd); 

    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }

    return (msg.wParam); 
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static int cxChar, cyChar, cxCaps; 
    
    TCHAR szBuffer[16]; 
    int i, iLen; 

    HDC hdc = NULL; 

    TEXTMETRIC tm; 
    PAINTSTRUCT ps; 

    switch(uMsg)
    {
        case WM_CREATE: 
        {
            hdc = GetDC(hWnd); 
            GetTextMetrics(hdc, &tm); 
            cxChar = tm.tmAveCharWidth; 
            cyChar = tm.tmHeight; 
            cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2; 
            ReleaseDC(hWnd, hdc); 
            hdc = NULL; 
            break; 
        }

        case WM_PAINT: 
        {
            hdc = BeginPaint(hWnd, &ps); 
            for(i = 0; i < NUMLINES; ++i)
            {
                TextOut(hdc, 0, i * cyChar, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel)); 
                TextOut(hdc, 22 * cxCaps, i * cyChar, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc)); 
                SetTextAlign(hdc, TA_TOP | TA_RIGHT); 
                ZeroMemory(szBuffer, 16); 
                iLen = wsprintf(szBuffer, "%5d", GetSystemMetrics(sysmetrics[i].iIndex), iLen);
                TextOut(hdc, 22 * cxCaps + 40 * cxChar, i * cyChar, szBuffer, iLen); 
                SetTextAlign(hdc, TA_TOP | TA_LEFT);   
            }
            EndPaint(hWnd, &ps); 
            break; 
        }

        case WM_DESTROY: 
        {
            PostQuitMessage(0); 
            break; 
        }
    }

    return (DefWindowProc(hWnd, uMsg, wParam, lParam)); 
}