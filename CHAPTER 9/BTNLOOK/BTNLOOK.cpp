/*---------------------------------------------------
    BTNLOOK.CPP   -- Button Look Program  
                    (c)charels Petzod. 1998
-----------------------------------------------------*/

#include <Windows.h>
#include <assert.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")


struct 
{
    int     iStyle;
    TCHAR*   szText;
}
button[] = {
    BS_PUSHBUTTON,          TEXT("PUSHBUTTON"),
    BS_DEFPUSHBUTTON,       TEXT("DEFPUSHBUTTON"),
    BS_CHECKBOX,            TEXT("CHECKBOX"),
    BS_AUTOCHECKBOX,        TEXT("AUTOCHECKBOX"),
    BS_RADIOBUTTON,         TEXT("BS_RADIOBUTTON"),
    BS_3STATE,              TEXT("BS_3STATE"),
    BS_AUTO3STATE,          TEXT("BS_AUTO3STATE"),
    BS_GROUPBOX,            TEXT("BS_GROUPBOX"),
    BS_AUTORADIOBUTTON,     TEXT("BS_AUTORADIOBUTTON"),
    BS_OWNERDRAW,           TEXT("BS_OWNERDRAE")

};

#define NUM (sizeof button / sizeof button[0])

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nShowCmd)
{
    static TCHAR szClassName[] = TEXT("Beeper1");
    static TCHAR szWindowName[] = TEXT("Beeper1 Timer Demo");

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
    static HWND hwndButton[NUM];
    static RECT rect;
    static TCHAR szTop[]     = TEXT("message            wparam          lParam"),
                  szUnd[]    = TEXT("________           ______           _______"),
                  szFormat[] = TEXT("%-16s%04X-%04X     %04X-%04X"),
                  szBuffer[50];
    static int cxChar, cyChar;
    HDC hdc;
    PAINTSTRUCT ps;
    int i;

    switch (uMsg)
    {
    case WM_CREATE:
        cxChar = LOWORD(GetDialogBaseUnits());
        cyChar = HIWORD(GetDialogBaseUnits());

        for(i = 0; i<NUM; i++)
        {
            hwndButton[i] = CreateWindow(TEXT("button"), button[i].szText,
                                WS_CHILD | WS_VISIBLE | button[i].iStyle,
                                cxChar, cyChar * (1 + 2 * i), 
                                20 * cxChar, 7 * cyChar /4,
                                hWnd, (HMENU)i, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
        }
       return 0;

    
    case WM_SIZE:
        rect.left = 24 * cxChar;
        rect.top = 2 * cyChar;
        rect.right= LOWORD(lParam);
        rect.bottom = HIWORD(lParam);
        return 0;

    case WM_PAINT:
        InvalidateRect(hWnd, &rect, TRUE);

        hdc = BeginPaint(hWnd, &ps);
        SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
        SetBkMode(hdc, TRANSPARENT);

        TextOut(hdc, 24 * cxChar, cyChar, szTop, lstrlen(szTop));
        TextOut(hdc, 24 * cxChar, cyChar, szUnd, lstrlen(szUnd));

        EndPaint(hWnd, &ps);
        return 0;

    case WM_DRAWITEM:
    case WM_COMMAND:
        ScrollWindow(hWnd, 0, -cyChar, &rect, &rect);

            hdc = GetDC(hWnd);
            SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

            TextOut(hdc, 24* cxChar, cyChar * (rect.bottom/cyChar-1), szBuffer, wsprintf(szBuffer, szFormat, 
            uMsg == WM_DRAWITEM ? TEXT("WM_DRAWITEM") : TEXT("WM_COMMAND"), 
                HIWORD(wParam), LOWORD(wParam),
                HIWORD(lParam), LOWORD(lParam)));

            ReleaseDC(hWnd, hdc);
            ValidateRect(hWnd, &rect);
            break;
        
    case WM_DESTROY:
        PostQuitMessage(0);
        return (0);
    }

    return (DefWindowProc(hWnd, uMsg, wParam , lParam));
}

