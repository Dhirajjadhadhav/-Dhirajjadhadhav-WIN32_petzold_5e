/*-----------------------------------------------------
    CLOVER.CPP -- clover Drawing program Using Regions
                   (c) Charles Petzold, 1998
--------------------------------------------------------*/
#include<Windows.h>
#include<assert.h>
#include<math.h>

#define TWO_PI (2 * 3.14159)

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstacne, LPSTR szCmdLine, int nShowCmd)
{
    TCHAR szClassName[] = TEXT("Clover");
    TCHAR szWindowname[] = TEXT("Draw clover");

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
    static  HRGN   hRgnClip;
    static int cxClient, cyClient;
    double fAngle, fRadius;
    HCURSOR hCursor;
    HRGN hRgnTemp[6];
    HDC hdc;
    int i;
    PAINTSTRUCT ps;

    switch (uMsg)
    {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        
        hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
        ShowCursor(TRUE);
        
        if(hRgnClip)
            DeleteObject(hRgnClip);
        
        hRgnTemp[0] = CreateEllipticRgn(0, cyClient/3, cxClient/2, 2 * cyClient / 3);
        hRgnTemp[1] = CreateEllipticRgn(cxClient/2, cyClient/3, cxClient, 2 * cyClient/3);
        hRgnTemp[2] = CreateEllipticRgn(cxClient/3, 0, 2 * cxClient / 3, cyClient/2);
        hRgnTemp[3] = CreateEllipticRgn(cxClient/3, cyClient/2, 2* cxClient/3, cyClient);

        hRgnTemp[4] = CreateEllipticRgn(0, 0, 1, 1);
        hRgnTemp[5] = CreateEllipticRgn(0, 0, 1 , 1 );
        CombineRgn(hRgnTemp[4], hRgnTemp[0], hRgnTemp[1], RGN_OR);
        CombineRgn(hRgnTemp[5], hRgnTemp[2], hRgnTemp[3], RGN_OR);
        CombineRgn(hRgnClip, hRgnTemp[4], hRgnTemp[5], RGN_XOR);

        for(i = 0; i<6; ++i)
            DeleteObject(hRgnTemp[i]);
        
        SetCursor(hCursor);
        ShowCursor(FALSE);   
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        
        SetViewportOrgEx(hdc, cxClient/2, cyClient/2, NULL);
        SelectClipRgn(hdc, hRgnClip);

        fRadius = _hypot (cxClient/2.0, cyClient/ 2.0); 

        for(fAngle = 0.0; fAngle <TWO_PI; fAngle += TWO_PI/360)
        {
            MoveToEx(hdc, 0, 0, NULL);
            LineTo(hdc, (int)(fRadius * cos(fAngle) + 0.5), (int)(-fRadius * sin(fAngle) + 0.5));   
        }
        
        EndPaint(hWnd, &ps);
        return(0);

    case WM_DESTROY:
        DeleteObject(hRgnClip);
        PostQuitMessage(0);
        break;
    
    default:
        break;
    }

    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}
