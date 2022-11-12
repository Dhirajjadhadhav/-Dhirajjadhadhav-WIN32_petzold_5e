/*---------------------------------------------------
    COOLOR.CPP   -- Color Uisng Scroll Bars
                    (c) Dhiraj Jadhav, 2022 
*/

#include <Windows.h>
#include <assert.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ScrollProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int idFocus;
WNDPROC Oldscroll[3];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

    TCHAR szClassName[] = TEXT("Colors1");
    TCHAR szWindowName[]  = TEXT("Color Scroll");

    HWND hWnd = NULL;
    HCURSOR hCursor = NULL;
    HBRUSH hBrush = NULL;
    HICON hIcon = NULL;
    HICON hIconSm = NULL;

    WNDCLASSEX Wnd;
    MSG msg;

    ZeroMemory(&Wnd, sizeof(WNDCLASSEX));
    ZeroMemory(&msg, sizeof(MSG));

    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    assert(hBrush != NULL);

    hCursor = LoadCursor(NULL, IDC_ARROW);
    assert(hCursor != NULL);

    hIcon = LoadIcon(NULL, IDI_APPLICATION);
    assert(hIcon != NULL);

    hIconSm =  LoadIcon(NULL, IDI_APPLICATION);
    assert(hIconSm != NULL);

    Wnd.cbSize = sizeof(WNDCLASSEX);
    Wnd.cbClsExtra = 0;
    Wnd.cbWndExtra =0;
    Wnd.hbrBackground = CreateSolidBrush(0);
    Wnd.hCursor = hCursor;
    Wnd.hIcon = hIcon;
    Wnd.hIconSm = hIconSm;
    Wnd.hInstance = hInstance;
    Wnd.lpfnWndProc = WndProc;
    Wnd.lpszClassName = szClassName;
    Wnd.lpszMenuName = NULL;
    Wnd.style = CS_VREDRAW | CS_HREDRAW;

    ATOM bRet = RegisterClassEx(&Wnd);
    if(0 == bRet)
    {
        MessageBox(NULL, TEXT("Window class registeration failed!!"), szClassName, MB_ICONERROR | MB_OK);
        return (0);
    }

    hWnd = CreateWindowEx(WS_EX_APPWINDOW, szClassName, szWindowName, WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,CW_USEDEFAULT,
                        (HWND)NULL, (HMENU)NULL, hInstance, (LPVOID)NULL);

    if(NULL == hWnd)
    {
        MessageBox(NULL, TEXT("Window Creation failed !!"), szWindowName, MB_ICONERROR | MB_OK);
        return (0);
    }

    ShowWindow(hWnd, nShowCmd);
    UpdateWindow(hWnd); 

    while(GetMessage(&msg, NULL, 0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (msg.wParam);

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static COLORREF crPrime[3] = {RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255)};
    static HBRUSH hBrush[3], hBrushStatic;
    static HWND hWndScroll[3], hWndLabel[3], hWndValue[3], hWndRect;
    static int color[3], cyChar;
    static RECT rcColor;
    static TCHAR* szColorLabel[] = {TEXT("Red"), TEXT("Green"), TEXT("Blue")};
    
    HINSTANCE hInstance;
    int i, cxClient, cyClient;
    TCHAR szBuffer[10]; 

    switch(uMsg)
    {
        case WM_CREATE:
            hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
            
            //Create the White-Rectangle Window  against Which the 
            // scroll bar Will be positioned. the Child Window ID is 9

        hWndRect  = CreateWindowEx(0, TEXT("static"), NULL, 
                                    WS_CHILD | WS_VISIBLE | SS_WHITERECT,
                                    0, 0, 0, 0,
                                    hWnd, (HMENU)9, hInstance, (LPVOID)NULL);
        for(i = 0; i<3;i++)
        {
            //the Three Scroll Bars Have IDs 0, 1, and 2, with 
            // scroll bar ranges from 0 throught 255
            hWndScroll[i] = CreateWindowEx(0, TEXT("scrollbar"), NULL, 
                                    WS_CHILD | WS_VISIBLE | 
                                    WS_TABSTOP | SBS_VERT, 
                                    0, 0, 0, 0,
                                    hWnd, (HMENU)i, hInstance, (LPVOID)NULL);

            SetScrollRange(hWndScroll[i], SB_CTL, 0, 255, FALSE);
            SetScrollPos(hWndScroll[i], SB_CTL, 0, FALSE);

                //The three color-name label have IDs 3, 4 and 5
                //and text string "Red", "Green" and "Blue".

            hWndLabel[i] = CreateWindowEx(0, TEXT("static"), szColorLabel[i], 
                                    WS_CHILD | WS_VISIBLE | SS_CENTER,
                                    0, 0, 0, 0,
                                    hWnd, (HMENU)(i+3), hInstance, NULL);
            
                //the Three color value text fields Have IDs 6,7 and 8 
                // and intial srting of "0"

                hWndValue[i] = CreateWindowEx(0, TEXT("static"), TEXT("0"), 
                                                WS_CHILD | WS_VISIBLE | SS_CENTER,
                                                0, 0, 0, 0,
                                                hWnd, (HMENU)(i+6), hInstance, NULL);
                
                Oldscroll[i] = (WNDPROC)SetWindowLong(hWndScroll[i], GWL_WNDPROC, (LONG)ScrollProc);

                hBrush[i] = CreateSolidBrush(crPrime[i]);
        }

        hBrushStatic = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));

        cyChar = HIWORD(GetDialogBaseUnits());
        return(0);

        case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);

        SetRect(&rcColor, cxClient/2, 0, cxClient, cyClient);

        MoveWindow(hWndRect, 0, 0, cxClient/2, cyClient, TRUE);

        for(i = 0; i<3; i++)
        {
            MoveWindow(hWndScroll[i], (2 * i + 1) * cxClient/14, 2 * cyChar,
                        cxClient/14, cyClient - 4 * cyChar, TRUE);
            
            MoveWindow(hWndLabel[i], 
                        (4 * i + 1) * cxClient/28, cyChar/2,
                        cxClient/7, cyChar, TRUE);
            
            MoveWindow(hWndValue[i], 
                        (4 * i + 1) * cxClient/28, 
                        cyClient -3 * cyChar/2,
                        cxClient/7, cyChar, TRUE);
        }
        SetFocus(hWnd);
        return (0);

    case WM_SETFOCUS:
        SetFocus(hWndScroll[idFocus]);
        return 0;

    case WM_VSCROLL:
        i = GetWindowLong((HWND)lParam, GWL_ID);

        switch(LOWORD(wParam))
        {
            case SB_PAGEDOWN:
                color[i] += 15;
                                            //fall through
            case SB_LINEDOWN:
                color[i] = min(255, color[i] + 1);
                break;
            
            case SB_PAGEUP:
                color[i] -= 15;
                                            //fall through
            case SB_LINEUP:
                color[i] = max(0, color[i]-1);
                break;

            case SB_TOP:
                color[i] = 0;
                break;

            case SB_BOTTOM:
                color[i] = 255;
                break;

            case SB_THUMBPOSITION:
            case SB_THUMBTRACK:
                color[i] = HIWORD(wParam);
                break;
            
            default:
                break; 
        }
        
        SetScrollPos(hWndScroll[i], SB_CTL, color[i], TRUE); 
        wsprintf(szBuffer, TEXT("%i"), color[i]);
        SetWindowText(hWndValue[i], szBuffer);

        DeleteObject((HBRUSH)
                    SetClassLong(hWnd, GCL_HBRBACKGROUND, 
                        (LONG)CreateSolidBrush(RGB(color[0], color[1], color[2]))));
        InvalidateRect(hWnd, &rcColor, TRUE);
        return (0);

    case WM_CTLCOLORSCROLLBAR:
        i = GetWindowLong((HWND)lParam, GWL_ID);
        return((LRESULT)hBrush[i]);
    
    case WM_CTLCOLORSTATIC:
        i = GetWindowLong((HWND)lParam, GWL_ID);

        if(i>=3 && i<=8)    //static text control
        {
            SetTextColor((HDC)wParam, crPrime[i%3]);
            SetBkColor((HDC)wParam, GetSysColor(COLOR_BTNHIGHLIGHT));
                return((LRESULT)hBrushStatic);
        }
        break;
    
    case WM_SYSCOLORCHANGE:
        DeleteObject(hBrushStatic);
        hBrushStatic = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));
        return (0);

    case WM_DESTROY:
        DeleteObject((HBRUSH)
                    SetClassLong(hWnd, GCL_HBRBACKGROUND, (LONG)
                        CreateSolidBrush(RGB(color[0], color[1], color[2]))));
        
        for(i = 0; i <3; i++)
            DeleteObject(hBrush[i]);

        DeleteObject(hBrushStatic);
        PostQuitMessage(0);
        return (0);
    }
    return(DefWindowProc(hWnd, uMsg, wParam , lParam));
}

LRESULT CALLBACK ScrollProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int id = GetWindowLong(hWnd, GWL_ID);

    switch(uMsg)
    {
        case WM_KEYDOWN:
            if(wParam == VK_TAB)
            SetFocus(GetDlgItem(GetParent(hWnd),
                    (id + (GetKeyState(VK_SHIFT)<0 ? 2 : 1)) % 3));


            break;
        
        case WM_SETFOCUS:
            idFocus = id;
            break;
    }
    return CallWindowProc(Oldscroll[id], hWnd, uMsg, wParam, lParam);
}




