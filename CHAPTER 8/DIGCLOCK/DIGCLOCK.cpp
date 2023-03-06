/*---------------------------------------------------
    DIGCLOCK.CPP   -- digital clock
                     (c) Charles Petzold, 1998
-----------------------------------------------------*/

#include <Windows.h>
#include <assert.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")


#define ID_TIMER 1

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nShowCmd)
{
    static TCHAR szClassName[] = TEXT("DigClock");
    static TCHAR szWindowName[] = TEXT("Digital Clock");

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

void DisplayDigit(HDC hdc, int iNumber)
{
    static BOOL fSevenSegment[10][7] = {
                        1, 1, 1, 0, 1, 1, 1,        //0
                        0, 0, 1, 0, 0, 1, 0,        //1
                        1, 0, 1, 1, 1, 0, 1,        //2
                        1, 0, 1, 1, 0, 1, 1,        //3
                        0, 1, 1, 1, 0, 1, 0,        //4
                        1, 1, 0, 1, 0, 1, 1,        //5
                        1, 1, 0, 1, 1, 1, 1,        //6
                        1, 0, 1, 0, 0, 1, 0,        //7
                        1, 1, 1, 1, 1, 1, 1,        //8
                        1, 1, 1, 1, 0, 1, 1,        //9
    };

    static POINT ptSegment[7][6] = {
                         7,  6, 11,  2, 31,  2, 35,  6, 31, 10, 11, 10,
                         6,  7, 10, 11, 10, 31,  6, 35,  2, 31,  2, 11,
                        36,  7, 40, 11, 40, 31, 36, 35, 32, 31, 32, 11,
                         7, 36, 11, 32, 31, 32, 35, 36, 31, 40, 11, 40,
                         6, 37, 10, 41, 10, 61,  6, 65,  2, 61,  2, 41,
                        36, 37, 40, 41, 40, 61, 36, 65, 32, 61, 32, 41,
                         7, 66, 11, 62, 31, 62, 35, 66, 31, 70, 11, 70,
    };

    int iSeg;
    for(iSeg = 0; iSeg<7; iSeg++)
        if(fSevenSegment[iNumber][iSeg])
            Polygon(hdc, ptSegment[iSeg], 6);
}
void DisplayTwoDigit(HDC hdc, int iNumber, BOOL fSuppress)
{
    if(!fSuppress || (iNumber/10 != 0))
        DisplayDigit(hdc, iNumber/10);
    OffsetWindowOrgEx(hdc, -42, 0, NULL);
    DisplayDigit(hdc, iNumber%10);
    OffsetWindowOrgEx(hdc, -42, 0, NULL);
}

void DisplayColon(HDC hdc)
{
    POINT ptColon[2][4] = { 2, 21, 6, 17, 10, 21, 6, 25,
                            2, 51, 6, 47, 10, 51, 6, 55 };
    Polygon(hdc, ptColon[0], 4);
    Polygon(hdc, ptColon[1], 4);

    OffsetWindowOrgEx(hdc, -12, 0, NULL);
} 

void DisplyayTime(HDC hdc, BOOL f24Hour, BOOL fSuppress)
{
    SYSTEMTIME st;

    GetLocalTime(&st);

    if(f24Hour)
        DisplayTwoDigit(hdc, st.wHour, fSuppress);
    else    
        DisplayTwoDigit(hdc, (st.wHour %= 12)? st.wHour : 12, fSuppress);

    DisplayColon(hdc);
    DisplayTwoDigit(hdc, st.wMinute, FALSE);
    DisplayColon(hdc);
    DisplayTwoDigit(hdc, st.wSecond, FALSE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static BOOL f24Hour, fSuppress;
    static HBRUSH hBrushRed;
    static int cxClient, cyClient;
    HDC hdc;
    PAINTSTRUCT ps;
    TCHAR szBufffer[2]; 

    switch(uMsg)
    {
        case WM_CREATE:
            hBrushRed = CreateSolidBrush(RGB(255, 0, 0));
            SetTimer(hWnd , ID_TIMER, 1000, NULL);
                                                    //fall thrugh
        case WM_SETTINGCHANGE:
            GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, szBufffer, 2);
            f24Hour = (szBufffer[0] == '1');

            GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITLZERO, szBufffer, 2);
            fSuppress = (szBufffer[0] == '0');

            InvalidateRect(hWnd, NULL, TRUE);
            return(0);

        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            return(0);
        
        case WM_TIMER:
            InvalidateRect(hWnd, NULL, TRUE);
            return(0);
        
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);

            SetMapMode(hdc, MM_ISOTROPIC);
            SetWindowExtEx(hdc, 276, 72, NULL);   //276, 72
            SetViewportExtEx(hdc, cxClient, cyClient, NULL);

            SetWindowOrgEx(hdc, -138, -36, NULL);             //138, 36
            SetViewportExtEx(hdc, cxClient/2, cyClient/2, NULL);    //cxClient/2, cyClient/2

            SelectObject(hdc, GetStockObject(NULL_BRUSH));
            SelectObject(hdc, hBrushRed);

            DisplyayTime(hdc, f24Hour, fSuppress);

            EndPaint(hWnd, &ps);
            return(0);

        case WM_DESTROY:
            KillTimer(hWnd, ID_TIMER);
            DeleteObject(hBrushRed);
            PostQuitMessage(0);
            return (0);
    }   

    return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

