/*---------------------------------------------------
    SINEWAVE.C   -- Bezier splines Demo 
                    (c)charels Petzod. 1998
-----------------------------------------------------*/

#include <Windows.h>
#include <math.h>
#include <assert.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nShowCmd)
{
    static TCHAR szClassName[] = TEXT("Bezier");
    static TCHAR szWindowName[] = TEXT("Bezier splines");

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

void DrawBezier(HDC hdc, POINT apt[])
{
    PolyBezier(hdc, apt, 4);

    MoveToEx(hdc, apt[0].x, apt[0].y, NULL);
    LineTo(hdc, apt[1].x, apt[1].y);

    MoveToEx(hdc, apt[2].x, apt[2].y, NULL);
    LineTo(hdc, apt[3].x, apt[3].y);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static POINT apt[4];

    int cxClient, cyClient;
    HDC hdc;
    PAINTSTRUCT ps;
   

    switch(uMsg)
    {
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);

            apt[0].x = cxClient/4;
            apt[0].y = cyClient/2;

            apt[1].x = cxClient/2;
            apt[1].y = cyClient/4;

            apt[2].x =  cxClient/2;
            apt[2].y = 3 * cyClient/4;
 
            apt[3].x = 3*cxClient/4;
            apt[3].y = cyClient/2;

            return(0);
        
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MOUSEMOVE:
            if(wParam &  MK_LBUTTON || wParam & MK_RBUTTON)
            {
                hdc = GetDC(hWnd);

                SelectObject(hdc, GetStockObject(BLACK_PEN));
                DrawBezier(hdc, apt);

                if(wParam & MK_LBUTTON)
                {
                    apt[1].x = LOWORD(lParam);
                    apt[1].y = HIWORD(lParam);
                }

                if(wParam & MK_RBUTTON)
                {
                    apt[2].x = LOWORD(lParam);
                    apt[2].y = HIWORD(lParam);
                }

                SelectObject(hdc, GetStockObject(BLACK_BRUSH));
                DrawBezier(hdc, apt);
                ReleaseDC(hWnd, hdc);
            }
            return(0);

        case WM_PAINT:

            InvalidateRect(hWnd, NULL, TRUE);

            hdc = BeginPaint(hWnd, &ps);

           DrawBezier(hdc, apt);

            EndPaint(hWnd, &ps);

            return(0);

            case WM_DESTROY:
                PostQuitMessage(0);
                return(0);
    }

    return(DefWindowProc(hWnd, uMsg, wParam, lParam)); 
}

