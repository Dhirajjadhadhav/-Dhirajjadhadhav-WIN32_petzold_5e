/*---------------------------------------------------
    BEEPER1.CPP   -- Timer Demo Program No. 2 
                    (c)charels Petzod. 1998
-----------------------------------------------------*/

#include <Windows.h>
#include <math.h>
#include <assert.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")


#define ID_TIMER 1

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT iTimerID,DWORD dwTime);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nShowCmd)
{
    static TCHAR szClassName[] = TEXT("Beeper2");
    static TCHAR szWindowName[] = TEXT("Beeper2 Timer Demo");

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
   switch(uMsg)
   {
        case WM_CREATE:
            SetTimer(hWnd, ID_TIMER, 1000, TimerProc);
            return(0);
        
        case WM_DESTROY:
            KillTimer(hWnd, ID_TIMER);
            PostQuitMessage(0);
            return(0); 
   }
   return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT iTimerID, DWORD dwTime)
{
   static BOOL fFlipFlop = FALSE;
    HBRUSH hBrush;
    HDC hdc;
    RECT rc;

    MessageBeep(-1);
    fFlipFlop = !fFlipFlop;

    GetClientRect(hWnd, &rc);

    hdc = GetDC(hWnd);

    hBrush = CreateSolidBrush(fFlipFlop ? RGB(255,0,0) : RGB(0, 0, 255));

    FillRect(hdc, &rc, hBrush);
    ReleaseDC(hWnd, hdc);
    DeleteObject(hBrush);
}


