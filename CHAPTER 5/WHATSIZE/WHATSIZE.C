/*-----------------------------------------
    ALTWIND.C   -- Alternate and Windding Fill Modes
                     (c) Charles Petzold, 1998
--------------------------------------------*/
#include <Windows.h>
#include <assert.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nShowCmd)
{
    TCHAR szClassName[] = TEXT("altWind");
    TCHAR szWindowName[] = TEXT("Altering and Winding Fill Modes");

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
    wnd.hbrBackground = hBrush;
    wnd.hCursor = hCursor;
    wnd.hIcon = hIcon;
    wnd.hIconSm = hIconSm;
    wnd.hInstance = hInstance;
    wnd.lpfnWndProc = WndProc;
    wnd.lpszClassName = szClassName;
    wnd.lpszMenuName = NULL;
    wnd.style = CS_HREDRAW | CS_VREDRAW;    


    ATOM bRet = RegisterClassEx(&wnd);
    assert(bRet);

    hWnd = CreateWindowEx(WS_EX_APPWINDOW, szClassName, szWindowName, WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                        (HWND)NULL, (HMENU)NULL, hInstance, NULL);
    
    assert(hWnd != NULL);

    ShowWindow(hWnd, nShowCmd);
    UpdateWindow(hWnd);

    while(GetMessage(&msg, NULL, 0 , 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return(msg.wParam);
}

void Show(HWND hWnd, HDC hdc, int xText, int yText, int iMapMode, TCHAR* szMapMode)
{
    TCHAR szBuffer[60];
    RECT    rect;

    SaveDC(hdc);
    SetMapMode(hdc, iMapMode);
    GetClientRect(hWnd, &rect);
    DPtoLP(hdc, (PPOINT)&rect, 2);

    RestoreDC(hdc, -1);

    TextOut(hdc, xText, yText, szBuffer, wsprintf(szBuffer, TEXT("%-20s %7d %7d %7d %7d"), szMapMode, 
                    rect.left, rect.right, rect.top, rect.bottom));
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    static TCHAR szHeding[] = 
        TEXT("Mapping mode          left    right   top     Bottom");
    static TCHAR szUndLine[] = 
        TEXT("-------------        ------   -----   ----    -------"); 
    static int cxChar, cyChar;
    HDC hdc;
    PAINTSTRUCT ps;
    TEXTMETRIC tm;

    switch(uMsg)
    {
       case WM_CREATE:
            hdc = GetDC(hWnd);
            SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

            GetTextMetrics(hdc, &tm);
            cxChar  = tm.tmAveCharWidth;
            cyChar = tm.tmHeight + tm.tmExternalLeading;

            ReleaseDC(hWnd, hdc);
            return(0);

        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
            SetMapMode(hdc, MM_ANISOTROPIC);
            SetWindowExtEx(hdc, 1,1, NULL);
            SetViewportExtEx(hdc, cxChar, cyChar, NULL);

            TextOut(hdc, 1, 1, szHeding, lstrlen(szHeding));
            TextOut(hdc, 1, 2, szUndLine, lstrlen(szUndLine));

            Show(hWnd, hdc, 1, 3, MM_TEXT, TEXT("TEXT (pixel)"));
            Show(hWnd, hdc, 1, 4, MM_LOMETRIC, TEXT("LOMETRIC (.1 mm)"));
            Show(hWnd, hdc, 1, 5, MM_HIMETRIC, TEXT("HIMETRIC (.01 MM)"));
            Show(hWnd, hdc, 1, 6, MM_LOENGLISH, TEXT("LOENGLISH (.01 in)"));
            Show(hWnd, hdc, 1, 7, MM_HIENGLISH, TEXT("HIENGLISH (.001 in)"));
            Show(hWnd, hdc, 1, 8, MM_TWIPS, TEXT("TWIPS (1/1440 in)"));

            EndPaint(hWnd, &ps);
            return(0);

        case WM_DESTROY:
            PostQuitMessage(0);
            return(0);
        
        

    }
    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}