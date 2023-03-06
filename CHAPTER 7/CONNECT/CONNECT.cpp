/*-----------------------------------------------------
    CONNECT.CPP -- connect-the-Dots Mouse Demo Program
                     (c) Charles Petzold, 1998
--------------------------------------------------------*/
#include<Windows.h>
#include<assert.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#define MAXPOINTS 1000

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstacne, LPSTR szCmdLine, int nShowCmd)
{
    TCHAR szClassName[] = TEXT("Connect");
    TCHAR szWindowname[] = TEXT("Connect-the-Points Mouse Demo");

    HWND hWnd = NULL;
    HBRUSH hBrush = NULL;
    HCURSOR hCursor = NULL;
    HICON hIcon = NULL ;
    HICON hIconSm = NULL;

    WNDCLASSEX WndEx;
    MSG msg;

    ZeroMemory(&WndEx, sizeof(WNDCLASSEX));
    ZeroMemory(&msg, sizeof(MSG));

    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    assert(hBrush);

    hCursor = LoadCursor(NULL, IDC_ARROW);
    assert(hCursor);

    hIcon = LoadIcon(NULL, IDI_APPLICATION);
    assert(hIcon);

    hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    assert(hIconSm);

    WndEx.cbSize = sizeof(WNDCLASSEX);
    WndEx.cbClsExtra = 0;
    WndEx.cbWndExtra = 0;
    WndEx.hbrBackground = hBrush;
    WndEx.hCursor = hCursor;
    WndEx.hIcon = hIcon;
    WndEx.hIconSm = hIconSm;
    WndEx.hInstance = hInstance;
    WndEx.lpfnWndProc = WndProc;
    WndEx.lpszClassName = szClassName;
    WndEx.lpszMenuName = NULL;
    WndEx.style = CS_HREDRAW | CS_VREDRAW;

    ATOM bRet = RegisterClassEx(&WndEx);
    assert(bRet);

    hWnd = CreateWindowEx(WS_EX_APPWINDOW, szClassName, szWindowname, WS_OVERLAPPEDWINDOW |  WS_VSCROLL | WS_HSCROLL,
                            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                            (HWND)NULL,(HMENU)NULL, hInstance, NULL);
    
    assert(hWnd != NULL);

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
    static POINT pt[MAXPOINTS];
    static int iCount;
    HDC hdc;
    int i,j;
    PAINTSTRUCT ps;

    switch(uMsg)
    {
    case WM_LBUTTONDOWN:
        iCount = 0;
        InvalidateRect(hWnd, NULL, TRUE);
        return(0);
    
    case WM_MOUSEMOVE:
        if(wParam & MK_LBUTTON && iCount<100)
        {
            pt[iCount].x = LOWORD(lParam);
            pt[iCount++].y = HIWORD(lParam);
            
            hdc = GetDC(hWnd);
            SetPixel(hdc, LOWORD(lParam), HIWORD(lParam), 0);
            ReleaseDC(hWnd, hdc);
        }
        return(0);
    case WM_LBUTTONUP:
        InvalidateRect(hWnd, NULL, TRUE);
        return(0);

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        
        SetCursor(LoadCursor(NULL, IDC_ARROW));
        ShowCursor(TRUE);

        for(i = 0; i<iCount-1; i++)
            for(j = i+1; i<iCount; j++)
            {
                MoveToEx(hdc,pt[i].x, pt[i].y, NULL);
                LineTo(hdc, pt[j].x, pt[j].y);
            }
        ShowCursor(FALSE);
        SetCursor(LoadCursor(NULL, IDC_ARROW));
        EndPaint(hWnd, &ps);
        return(0);
    
    case WM_DESTROY:
        PostQuitMessage(0);
        return(0);
    }

    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}

