/* --------------------------------------------------------
    OWNDRAW.CPP     -- Owner-Draw Button Demo Program
                    (c) Dhiraj Jadhav
-----------------------------------------------------------*/
#include <Windows.h>
#include <assert.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#define ID_SMALLER  1
#define ID_LARGER   2
#define BTN_WIDTH   (8 * cxChar)
#define BTN_HEIGHT  (4 * cyChar)

HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstace, LPSTR lpCmdLine, int nShowCmd)
{
    static TCHAR szClassName[] = TEXT("OwnDraw");
    static TCHAR szWindowName[] = TEXT("Owner-Draw Button Demo");
    
    HWND hWnd = NULL;
    HBRUSH hBrush = NULL;
    HCURSOR hCursor = NULL;
    HICON hIcon = NULL;
    HICON hIconSm = NULL;

    WNDCLASSEX Wnd;
    MSG msg;

    hInst = hInstance;

    ZeroMemory(&Wnd, sizeof(WNDCLASSEX));
    ZeroMemory(&msg, sizeof(MSG));

    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    assert(hBrush != NULL);

    hCursor = LoadCursor(NULL, IDC_ARROW);
    assert(hCursor != NULL);

    hIcon = LoadIcon(NULL, IDI_APPLICATION);
    assert(hIcon != NULL);

    hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    assert(hIconSm != NULL);

    Wnd.cbSize = sizeof(WNDCLASSEX);
    Wnd.cbClsExtra = 0;
    Wnd.cbWndExtra = 0;
    Wnd.hbrBackground = hBrush;
    Wnd.hCursor = hCursor;
    Wnd.hIcon = hIcon;
    Wnd.hIconSm = hIconSm;
    Wnd.lpfnWndProc = WndProc;
    Wnd.lpszClassName = szClassName;
    Wnd.lpszMenuName = szClassName;
    Wnd.style = CS_VREDRAW | CS_HREDRAW;

    ATOM bRet = RegisterClassEx(&Wnd);
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

void Triangle(HDC hdc, POINT pt[])
{
    SelectObject(hdc, GetStockObject(BLACK_BRUSH));
    Polygon(hdc, pt, 3);
    SelectObject(hdc, GetStockObject(WHITE_BRUSH));
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hWndSmaller, hWndLarger;
    static int cxClient, cyClient, cxChar, cyChar;
    int cx,cy;
    LPDRAWITEMSTRUCT pdis;
    POINT pt[3];
    RECT rc;

    switch (uMsg)
    {
    case WM_CREATE:
        cxChar = LOWORD(GetDialogBaseUnits());
        cyChar = HIWORD(GetDialogBaseUnits());
            //create the owner-draw pushbutton
        hWndSmaller = CreateWindowEx(0, TEXT("button"), TEXT(""),
                        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                        0, 0, BTN_WIDTH, BTN_HEIGHT,
                        hWnd, (HMENU)ID_SMALLER, hInst, NULL);

        hWndLarger = CreateWindowEx(0, TEXT("button"), TEXT(""), 
                        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                        0, 0, BTN_WIDTH, BTN_HEIGHT, 
                        hWnd, (HMENU)ID_LARGER, hInst, NULL);
        
        return (0);

    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);

            //Move the button to the new center 

        MoveWindow(hWndSmaller, cxClient/2 - 3 * BTN_WIDTH/2, 
                                cyClient/2 - BTN_HEIGHT/2, BTN_WIDTH, BTN_HEIGHT, TRUE);
        MoveWindow(hWndLarger, cxClient/2 + 3 * BTN_HEIGHT/2,
                               cyClient/2 - BTN_WIDTH/2, BTN_WIDTH, BTN_HEIGHT, TRUE);
        return (0);
    
    case WM_COMMAND:
        GetWindowRect(hWnd, &rc);

            //Make the Window 10% smaller or larger
        switch(wParam)
        {
            case ID_SMALLER:
                rc.left   += cxClient/20;
                rc.right  -= cxClient/20;
                rc.top    += cyClient/20;
                rc.bottom -= cyClient/20;
                break;

            case ID_LARGER:
                rc.left   -= cxClient/20;
                rc.right  += cxClient/20;
                rc.top    -= cyClient/20;
                rc.bottom += cyClient/20;

                break;
        }
        MoveWindow(hWnd, rc.left, rc.top, rc.right - rc.left,
                        rc.bottom - rc.top, TRUE);
        return (0);
    
    case WM_DRAWITEM:
        pdis = (LPDRAWITEMSTRUCT)lParam;

            //Fill area with Whilte and frame it black
        FillRect(pdis->hDC, &pdis->rcItem, (HBRUSH)GetStockObject(WHITE_BRUSH));

        FrameRect(pdis->hDC, &pdis->rcItem, (HBRUSH)GetStockObject(BLACK_BRUSH));

            //Draw inward  and outward  black Triangles

            cx = pdis->rcItem.right - pdis->rcItem.left;
            cy = pdis->rcItem.bottom - pdis->rcItem.top;

            switch (pdis->CtlID)
            {
            case ID_SMALLER:
                pt[0].x = 3 * cx/8; pt[0].y = 1 * cy/8;
                pt[1].x = 5 * cx/8; pt[1].y = 1 * cy/8;
                pt[2].x = 4 * cx/8; pt[2].y = 3 * cy/8;
                Triangle(pdis->hDC, pt);

                pt[0].x = 7 * cx/8; pt[0].y = 3 * cy/8;
                pt[1].x = 7 * cx/8; pt[1].y = 5 * cy/8;
                pt[2].x = 5 * cx/8; pt[2].y = 4 * cy/8;
                Triangle(pdis->hDC, pt);

                pt[0].x = 5 * cx/8; pt[0].y = 7 * cy/8;
                pt[1].x = 3 * cx/8; pt[1].y = 7 * cy/8;
                pt[2].x = 4 * cx/8; pt[2].y = 5 * cy/8;
                Triangle(pdis->hDC, pt);

                pt[0].x = 1 * cx/8; pt[0].y = 5 * cy/8;
                pt[1].x = 1 * cx/8; pt[1].y = 3 * cy/8;
                pt[2].x = 3 * cx/8; pt[2].y = 4 * cy/8;
                Triangle(pdis->hDC, pt);
                break;
            
            case ID_LARGER:
                pt[0].x = 5 * cx/8; pt[0].y = 3 * cy/8;
                pt[1].x = 3 * cx/8; pt[1].y = 3 * cy/8;
                pt[2].x = 4 * cx/8; pt[2].y = 1 * cy/8;
                Triangle(pdis->hDC, pt);

                pt[0].x = 5 * cx/8; pt[0].y = 5 * cy/8;
                pt[1].x = 5 * cx/8; pt[1].y = 3 * cy/8;
                pt[2].x = 7 * cx/8; pt[2].y = 4 * cy/8;
                Triangle(pdis->hDC, pt);

                pt[0].x = 3 * cx/8; pt[0].y = 5 * cy/8;
                pt[1].x = 5 * cx/8; pt[1].y = 5 * cy/8;
                pt[2].x = 4 * cx/8; pt[2].y = 7 * cy/8;
                Triangle(pdis->hDC, pt);

                pt[0].x = 3 * cx/8; pt[0].y = 3 * cy/8;
                pt[1].x = 3 * cx/8; pt[1].y = 5 * cy/8;
                pt[2].x = 1 * cx/8; pt[2].y = 4 * cy/8;
                Triangle(pdis->hDC, pt);
                break;
            }

                //Invert the rectangle if the button is selected
            if(pdis->itemState & ODS_SELECTED)
            {
                pdis->rcItem.left += cx/16;
                pdis->rcItem.top += cy/16;
                pdis->rcItem.right -= cx/16;
                pdis->rcItem.bottom -= cy/16;

                DrawFocusRect(pdis->hDC, &pdis->rcItem);
            }
            return (0);
    case WM_DESTROY:
        PostQuitMessage(0);
        return (0);
    }
    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}