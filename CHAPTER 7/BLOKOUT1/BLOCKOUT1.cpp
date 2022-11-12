/*-----------------------------------------------------
    BLOCKOUT1.CPP    Mouse Button Demo program
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
    TCHAR szClassName[] = TEXT("Blpkout1");
    TCHAR szWindowname[] = TEXT("Mouse Button Demo");

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

void DrawBOXOutLine(HWND hWnd, POINT ptBeg, POINT ptEnd)
{
    HDC hdc;

    hdc = GetDC(hWnd);

    SetROP2(hdc, R2_NOT);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);

    ReleaseDC(hWnd, hdc);

}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static BOOL fBlocking, fValidBox;
    static POINT ptBeg, ptEnd, ptBoxBeg, ptBoxEnd;
    HDC hdc;
    PAINTSTRUCT ps;

    switch(uMsg)
    {
        case WM_LBUTTONDOWN:
            ptBeg.x = ptEnd.x = LOWORD(lParam);
            ptBeg.x = ptEnd.y = HIWORD(lParam);

            DrawBOXOutLine(hWnd, ptBeg, ptEnd);

            SetCursor(LoadCursor(NULL, IDC_CROSS));

            fBlocking = TRUE;
            return(0);

        case WM_MOUSEMOVE:
            if(fBlocking)
            {
                SetCursor(LoadCursor(NULL, IDC_CROSS));

                DrawBOXOutLine(hWnd, ptBeg,ptEnd);

                ptEnd.x = LOWORD(lParam);
                ptEnd.y = HIWORD(lParam);

                DrawBOXOutLine(hWnd, ptBeg, ptEnd);
            }
            return(0);
        
        case WM_LBUTTONUP:
            if(fBlocking)
            {
                DrawBOXOutLine(hWnd, ptBeg, ptEnd);

                ptBoxBeg = ptBeg;
                ptBoxEnd.x = LOWORD(lParam);
                ptBoxEnd.y = HIWORD(lParam);

                SetCursor(LoadCursor(NULL, IDC_ARROW));
                fBlocking = FALSE;
                fValidBox = TRUE;

                InvalidateRect(hWnd, NULL, TRUE);
            }
            return(0);

        case WM_CHAR:
            if(fBlocking & wParam == '\x1B')        //i.e. ESCAPE
            {
                DrawBOXOutLine(hWnd, ptBeg, ptEnd);

                SetCursor(LoadCursor(NULL, IDC_ARROW));

                fBlocking = FALSE;

            }
            return(0);

        case WM_PAINT:
            hdc= BeginPaint(hWnd, &ps);

            if(fValidBox)
            {
                SetROP2(hdc, R2_NOT);
                SelectObject(hdc, GetStockObject(NULL_BRUSH));
                Rectangle(hdc, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);   
            }

            EndPaint(hWnd, &ps);
            return(0);
        
        case WM_DESTROY:
            PostQuitMessage(0);
            return(0);
    }
    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}

