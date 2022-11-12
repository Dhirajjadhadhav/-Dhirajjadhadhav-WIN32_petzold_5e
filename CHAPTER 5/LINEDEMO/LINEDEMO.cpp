/*---------------------------------------------------
    SINEWAVE.C   -- Line-Drawing Demonstration Program 
                    (c)charels Petzod. 1998
-----------------------------------------------------*/

#include <Windows.h>
#include <math.h>
#include <assert.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")


#define NUM     1000
#define TWOPI   (2 * 3.14159)

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nShowCmd)
{
    static TCHAR szClassName[] = TEXT("LineDemo");
    static TCHAR szWindowName[] = TEXT("Line Demonstration");

    HWND hWnd = NULL;
    HBRUSH hBrush = NULL;
    HCURSOR hCursor = NULL;
    HICON hIcon = NULL;
    HICON hIconSm = NULL;

    WNDCLASSEX wnd;
    MSG msg;

    ZeroMemory(&wnd, sizeof(WNDCLASSEX));
    ZeroMemory(&msg, sizeof(MSG));

    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    assert(hBrush);

    hCursor = LoadCursor(NULL, IDC_ARROW);
    assert(hCursor);

    hIcon = LoadIcon(NULL, IDI_APPLICATION);
    assert(hIcon);

    hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    assert(hIconSm);


    wnd.cbSize = sizeof(WNDCLASSEX);
    wnd.cbClsExtra = 0;
    wnd.cbWndExtra = 0;
    wnd.hbrBackground =  hBrush;
    wnd.hCursor = hCursor;
    wnd.hIcon = hIcon;
    wnd.hIconSm =hIconSm;
    wnd.hInstance = hInstance;
    wnd.lpfnWndProc = WndProc;
    wnd.lpszClassName = szClassName;
    wnd.lpszMenuName = NULL;

    ATOM bRet = RegisterClassEx(&wnd);

    assert(bRet);

    hWnd = CreateWindowEx(WS_EX_APPWINDOW, szClassName, szWindowName, WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                        (HWND)NULL, (HMENU)NULL, hInstance, NULL);

    assert(hWnd);

    ShowWindow(hWnd, nShowCmd);
    UpdateWindow(hWnd);

    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static int cxClient, cyClient;
    HDC hdc;
    PAINTSTRUCT ps;
    POINT apt[NUM];

    switch(uMsg)
    {
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            return(0);
        
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);

            Rectangle(hdc, cxClient/8, cyClient/8, 7 * cxClient/8, 7 * cyClient/8);

            MoveToEx(hdc, 0, 0, NULL);
            LineTo(hdc, cxClient, cyClient);

            MoveToEx(hdc, 0, cyClient, NULL);
            LineTo(hdc, cxClient, 0);

            Ellipse(hdc, cxClient/8, cyClient/8, 7 * cxClient/8, 7 * cyClient/8);

            RoundRect(hdc, cxClient/4, cyClient/4, 3*cxClient/4, 3 * cyClient/4, cxClient/4, cyClient/4);

            EndPaint(hWnd, &ps);

            return(0);

            case WM_DESTROY:
                PostQuitMessage(0);
                return(0);
    }

    return(DefWindowProc(hWnd, uMsg, wParam, lParam)); 
}

