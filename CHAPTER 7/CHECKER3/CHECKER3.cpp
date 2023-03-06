/*-----------------------------------------------------
    CHECKER2.CPP    Mouse Hit-Test Demo Program No. 2
                   (c) Charles Petzold, 1998
--------------------------------------------------------*/
#include<Windows.h>
#include<assert.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#define DIVISIONS 5

int flag = 0;
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

TCHAR szChildClass[] = TEXT("Checker3_Child");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstacne, LPSTR szCmdLine, int nShowCmd)
{
    TCHAR szClassName[] = TEXT("Checker2");
    TCHAR szWindowname[] = TEXT("Checker2 Mouse Hit-Test Demo");

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

    WndEx.lpfnWndProc = ChildWndProc;
    WndEx.cbWndExtra = sizeof(long);
    WndEx.hIcon = NULL;
    WndEx.lpszClassName = szChildClass;

    bRet =  RegisterClassEx(&WndEx);
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

    return (msg.wParam);
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndChild[DIVISIONS][DIVISIONS];
    int cxBlock, cyBlock, x, y;

    switch(uMsg)
    {
        case WM_CREATE:
            for(x = 0; x<DIVISIONS; x++)
                for(y = 0; y<DIVISIONS; y++)
                hwndChild[x][y] = CreateWindowEx(0, szChildClass, NULL, WS_CHILDWINDOW | WS_VISIBLE,
                                                    0,0,0,0,
                                                    hWnd, (HMENU)(y<<8 | x), 
                                                    (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
            return (0);

        case WM_SIZE:
            cxBlock = LOWORD(lParam) / DIVISIONS;
            cyBlock = HIWORD(lParam) / DIVISIONS;

            for(x = 0; x<DIVISIONS; x++)
                for(y = 0; y<DIVISIONS; y++)
                    MoveWindow(hwndChild[x][y], x*cxBlock, y * cyBlock, cxBlock, cyBlock, TRUE);
            return (0);

        case WM_LBUTTONDOWN:
            MessageBeep(0);
            return (0);
        
        case WM_DESTROY:
            PostQuitMessage(0);
            return (0);
    }
    return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;

    switch (uMsg)
    {
    case WM_CREATE:
        SetWindowLong(hWnd, 0, 0);          //on/off flag
        break;
    
    case WM_LBUTTONDOWN:
        SetWindowLong(hWnd, 0, 1 ^ GetWindowLong(hWnd, 0));
        InvalidateRect(hWnd, NULL, FALSE);
        return (0);
    
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        GetClientRect(hWnd, &rect);
        Rectangle(hdc, 0, 0, rect.right, rect.bottom);


        if(GetWindowLong(hWnd, 0))
        {
            if(flag)
            {
                flag =
                MoveToEx(hdc, 0, 0, NULL);
                LineTo(hdc, rect.right, rect.bottom);
                MoveToEx(hdc, 0, rect.bottom, NULL);
                LineTo(hdc, rect.right, 0);
            }
            else
            {
                flag = 1;
                Ellipse(hdc, rect.right/7, rect.bottom/8, 7 *rect.right/8, 7 * rect.bottom/8);
            }
            
        } 

        EndPaint(hWnd, &ps);
        return (0);
    }
    return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

