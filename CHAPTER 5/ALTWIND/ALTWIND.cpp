/*-----------------------------------------------------
    ALTWIND.CPP -- Alternate and Winding Fill Modes
                    (c) Dhiraj Jadhav, 2022
--------------------------------------------------------*/
#include<Windows.h>
#include<assert.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstacne, LPSTR szCmdLine, int nShowCmd)
{
    TCHAR szClassName[] = TEXT("AltWind");
    TCHAR szWindowname[] = TEXT("Alternate and Winding Fill Modes");

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

    hWnd = CreateWindowEx(WS_EX_APPWINDOW, szClassName, szWindowname, WS_OVERLAPPEDWINDOW,
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
    static POINT aptFigure[10] = {10, 70, 50, 70, 50, 10, 90, 10, 90, 50, 
                                    30, 50, 30, 90, 70, 90, 70, 30, 10, 30};
    static int cxClient, cyClient;
    HDC hdc;
    int i;
    PAINTSTRUCT ps;
    POINT apt[10];

    switch (uMsg)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        SelectObject(hdc, GetStockObject(GRAY_BRUSH));
        for(i = 0; i<10; i++)
        {
            apt[i].x = cxClient * aptFigure[i].x / 200;
            apt[i].y = cyClient * aptFigure[i].y / 200;
        }

        SetPolyFillMode(hdc, ALTERNATE);
        Polygon(hdc, apt, 10);

        for(i = 0; i<10; i++)
        {
            apt[i].x += cxClient/2;
        }
        SetPolyFillMode(hdc, WINDING);
        Polygon(hdc, apt, 10);

        EndPaint(hWnd, &ps);
        return(0);

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    
    default:
        break;
    }

    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}
