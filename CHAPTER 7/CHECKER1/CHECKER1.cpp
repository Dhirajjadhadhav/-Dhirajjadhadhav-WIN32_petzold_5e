/*-----------------------------------------------------
    CHECKER1.CPP    Mouse Hit-Test Demo Program No. 1
                    (cpp) Dhiraj Jadhav, 2022
--------------------------------------------------------*/
#include<Windows.h>
#include<assert.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#define DIVISIONS 5

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstacne, LPSTR szCmdLine, int nShowCmd)
{
    TCHAR szClassName[] = TEXT("Checker1");
    TCHAR szWindowname[] = TEXT("Checker1 Mouse Hit-Test Demo");

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

    hWnd = CreateWindowEx(WS_EX_APPWINDOW, szClassName, szWindowname, WS_OVERLAPPEDWINDOW ,
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
    static BOOL fState[DIVISIONS][DIVISIONS];
    static int cxBlock, cyBlock;
    HDC hdc;
    int x, y;
    PAINTSTRUCT ps;
    RECT rect;

    switch (uMsg)
    {
    case WM_SIZE:
        cxBlock = LOWORD(lParam)/DIVISIONS;
        cyBlock = HIWORD(lParam)/DIVISIONS;
        return(0);
    
    case WM_LBUTTONDOWN:
        x = LOWORD(lParam);
        y = HIWORD(lParam);

        if(x<DIVISIONS && y<DIVISIONS)
        {
            fState[x][y] ^= 1;
            rect.left   = x * cxBlock;
            rect.top    = y * cyBlock;
            rect.right  = (x+1) * cxBlock;
            rect.bottom = (y+1) * cyBlock;
            InvalidateRect(hWnd, &rect, TRUE);            
        }
        else
            MessageBeep(0);
        return(0);

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        for(x = 0; x<DIVISIONS; x++)
        for(y = 0; y<DIVISIONS; y++)
        {
            Rectangle(hdc, x*cxBlock, y*cyBlock, (x+1) * cxBlock, (y+1) * cyBlock);
            if(fState[x][y])
            {
                MoveToEx(hdc, x * cxBlock, y * cyBlock, NULL);
                LineTo(hdc, (x+1)* cxBlock, (y+1) * cyBlock);
                MoveToEx(hdc, x * cxBlock, (y + 1)* cyBlock, NULL);
                LineTo(hdc, (x+1)* cxBlock, y * cyBlock);
            }
            EndPaint(hWnd, &ps);
        }
    case WM_DESTROY:
        PostQuitMessage(0);
        return(0);
    }
    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}

