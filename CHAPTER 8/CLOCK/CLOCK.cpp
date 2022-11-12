/*---------------------------------------------------
    CLOCK.CPP   -- Analog Clock Program
                    (cpp)charels Petzod. 1998
-----------------------------------------------------*/

#include <Windows.h>
#include <math.h>
#include <assert.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")


#define ID_TIMER 1
#define TWOPI       (2* 3.14159)

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nShowCmd)
{
    static TCHAR szClassName[] = TEXT("Clock");
    static TCHAR szWindowName[] = TEXT("Analog Clock Program");

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

void SetIsotropic(HDC hdc, int cxClient, int cyClient)
{
    SetMapMode(hdc, MM_ISOTROPIC);
    SetWindowExtEx(hdc, 1000, 1000, NULL);
    SetViewportExtEx(hdc, cxClient/2, -cyClient/2, NULL);
    SetViewportOrgEx(hdc, cxClient/2, cyClient/2, NULL);
}
void RotatePoint(POINT pt[], int iNum, int iAngle)
{
    int i;
    POINT ptTemp;

    for(i = 0; i<iNum; i++)
    {
        ptTemp.x  = (int) (pt[i].x*cos(TWOPI * iAngle/360) + 
                    pt[i].y*sin(TWOPI * iAngle/360));
        
        ptTemp.y = (int)(pt[i].y * cos(TWOPI * iAngle/360) + 
                    pt[i].x * sin(TWOPI * iAngle/360));
        pt[i] = ptTemp;
    }
}

void DrawClock(HDC hdc)
{
    int iAngle;
    POINT pt[3];

    for(iAngle = 0; iAngle<360; iAngle += 6)
    {
        pt[0].x = 0;
        pt[0].y = 900;

        RotatePoint(pt, 1, iAngle);

        pt[2].x = pt[2].y = iAngle % 5 ? 33 : 100;

        pt[0].x -= pt[2].x/2;
        pt[0].y -= pt[2].y/2;

        pt[1].x = pt[0].x + pt[2].x;
        pt[1].y = pt[0].y + pt[2].y;

        SelectObject(hdc, GetStockObject(BLACK_BRUSH));

        Ellipse(hdc, pt[0].x, pt[0].y, pt[1].x, pt[1].y);
    }
}

void DrawHands(HDC hdc, SYSTEMTIME* pst, BOOL fChange)
{
    static POINT pt[3][5] = { 0, -150, -100, 0, 0, 600, -100, 0, 0, -150,
                              0, -200,   50, 0, 0, 800,  -50, 0, 0, -200,
                              0,    0,    0, 0, 0,   0,    0, 0, 0,  800 };
    int i, iAngle[3];
    POINT ptTemp[3][5];

    iAngle[0] = (pst->wHour * 30) % 360 + pst->wMinute/2;
    iAngle[1] = pst->wMinute * 6;
    iAngle[2] = pst->wSecond * 6;

    memcpy(ptTemp, pt, sizeof(pt));

    for(i = fChange ? 0 : 2; i<3; i++);
    {
        RotatePoint(ptTemp[i], 5, iAngle[i]);

        Polyline(hdc, ptTemp[i], 5);

    }
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static int cxClient, cyClient;
  static SYSTEMTIME stPrevious;
  BOOL fChang;
  HDC hdc;
  PAINTSTRUCT ps;
  SYSTEMTIME st;

  switch (uMsg)
  {
    case WM_CREATE:
        SetTimer(hWnd, ID_TIMER, 1000, NULL);
        GetLocalTime(&st);
        stPrevious = st;

        return (0);
    
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(wParam);
        return (0);
    
    case WM_TIMER:
        GetLocalTime(&st);

        fChang = st.wHour != stPrevious.wHour ||
                 st.wMinute != stPrevious.wMinute;
        
        hdc = GetDC(hWnd);

        SetIsotropic(hdc, cxClient, cyClient);

        SelectObject(hdc, GetStockObject(WHITE_PEN));
        DrawHands(hdc, &stPrevious, fChang);

        SelectObject(hdc, GetStockObject(BLACK_PEN));
        DrawHands(hdc, &stPrevious, TRUE);

        ReleaseDC(hWnd, hdc);

        stPrevious = st;
        return(0);

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        SetIsotropic(hdc, cxClient, cyClient);
        DrawClock(hdc);
        DrawHands(hdc, &stPrevious, TRUE);

        EndPaint(hWnd, &ps);    
        return(0);
    
    case WM_DESTROY:
        KillTimer(hWnd, ID_TIMER);
        PostQuitMessage(0);      
        return(0);
  }

    return(DefWindowProc(hWnd, uMsg, wParam, lParam));

}

