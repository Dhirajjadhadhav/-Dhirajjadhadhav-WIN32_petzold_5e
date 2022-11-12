/*---------------------------------------------------
    SINEWAVE.C   -- Sine wave using polyline 
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
    static TCHAR szClassName[] = TEXT("SineWave");
    static TCHAR szWindowName[] = TEXT("Sine Wave using polygon");

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
    int i;
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

            MoveToEx(hdc, 0, cyClient/2, NULL);
            LineTo(hdc, cxClient, cyClient/2);

            for(i = 0; i<NUM; i++)
            {
                apt[i].x = i * cxClient / NUM;
                apt[i].y = (int)(cyClient / 2 * ( 1 - sin(TWOPI * i/NUM)));
            }
            Polyline(hdc, apt, NUM);
            return(0);

            case WM_DESTROY:
                PostQuitMessage(0);
                return(0);
    }

    return(DefWindowProc(hWnd, uMsg, wParam, lParam)); 
}

