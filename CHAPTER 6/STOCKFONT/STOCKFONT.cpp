/*-----------------------------------------------------
    STCKFONT.CPP -- Stock font Object 
                   (c) Charles Petzold, 1998
--------------------------------------------------------*/
#include<Windows.h>
#include<assert.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstacne, LPSTR szCmdLine, int nShowCmd)
{
    TCHAR szClassName[] = TEXT("StockFont");
    TCHAR szWindowname[] = TEXT("Stock Font");

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
    static struct 
    {
        int idStockFont;
        const TCHAR * szStockFont;
    }
    stockfont[] = { OEM_FIXED_FONT,         TEXT("OME_FIXED_FONT"),
                    ANSI_FIXED_FONT,        TEXT("ANSI_FIXED_FONT"),
                    ANSI_VAR_FONT,          TEXT("ANSI_VAR_FONT"),
                    SYSTEM_FONT,            TEXT("SYSTEM_FONT"),
                    DEVICE_DEFAULT_FONT,    TEXT("DEVICE_DEFAULT_FONT"),
                    SYSTEM_FIXED_FONT,      TEXT("SYSTEM_FIXED_FONT"),
                    DEFAULT_GUI_FONT,       TEXT("DEFAULT_GUI_FONT") };
    
    static int iFont, cFonts = sizeof(stockfont)/sizeof(stockfont[0]);
    HDC hdc;
    int i,x, y, cxGrid , cyGrid;
    PAINTSTRUCT ps;
    TCHAR szFaceName[LF_FACESIZE], szBuffer[LF_FACESIZE + 64];
    TEXTMETRIC tm;

    switch (uMsg)
    {
    case WM_CREATE:
        SetScrollRange(hWnd, SB_VERT, 0, cFonts-1, TRUE);
        return(0);
    
    case WM_DISPLAYCHANGE:
        InvalidateRect(hWnd, NULL, TRUE);
        return(0);
    
    case WM_VSCROLL:
        switch(LOWORD(wParam))
        {
            case SB_TOP:            iFont = 0;              break;
            case SB_BOTTOM:         iFont = cFonts - 1;     break;
            case SB_LINEUP:   
            case SB_PAGEUP:         iFont -= 1;             break;
            case SB_LINEDOWN:
            case SB_PAGEDOWN:       iFont += 1;             break;
            case SB_THUMBPOSITION:  iFont = HIWORD(wParam);  break;
        }
        iFont = max(0, min(cFonts-1,iFont));
        SetScrollPos(hWnd, SB_VERT, iFont, TRUE);
        InvalidateRect(hWnd, NULL, TRUE);
        return(0);
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_HOME:   SendMessage(hWnd, WM_VSCROLL, SB_TOP, 0);       break;
        case VK_END:    SendMessage(hWnd, WM_VSCROLL, SB_BOTTOM, 0);    break;
        case VK_PRIOR:  
        case VK_LEFT:
        case VK_UP:     SendMessage(hWnd, WM_VSCROLL, SB_LINEUP, 0);    break;
        case VK_NEXT:   
        case VK_RIGHT:
        case VK_DOWN:   SendMessage(hWnd, WM_VSCROLL, SB_PAGEDOWN, 0);  break;   
        }
        return(0);
    
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        
        SelectObject(hdc, GetStockObject(stockfont[iFont].idStockFont));
        GetTextFace(hdc, LF_FACESIZE, szFaceName);
        GetTextMetrics(hdc, &tm);
        cxGrid = max(3 * tm.tmAveCharWidth, 2 * tm.tmMaxCharWidth);
        cyGrid = tm.tmHeight + 3;

        TextOut(hdc, 0, 0, szBuffer, 
                wsprintf(szBuffer, TEXT("%s: Face Name = %s, charSet = %i"),
                stockfont[iFont].szStockFont,
                szFaceName, tm.tmCharSet));

        SetTextAlign(hdc, TA_TOP | TA_CENTER);

                //vertical and horizonatl lines

        for(i = 0; i<17; i++)
        {
            MoveToEx(hdc, (i+2)* cxGrid, 2* cyGrid, NULL);
            LineTo(hdc, (i+2) * cxGrid, 19 * cyGrid);

            MoveToEx(hdc, cxGrid, (i+3) * cyGrid, NULL);
            LineTo(hdc, 18 * cxGrid, (i+3) * cyGrid);
        }        

            //Vertical and horozonatal line 
        for(int i = 0; i<16; i++)
        {
            TextOut(hdc, (2 * i +5) * cxGrid/2, 2* cyGrid + 2, szBuffer, 
                    wsprintf(szBuffer, TEXT("%X-"),  i));

            TextOut(hdc, 3 * cxGrid/2, (i+3)* cyGrid + 2, szBuffer,
                    wsprintf(szBuffer, TEXT("%X-"), i));
        }

        for(y = 0; y<16; y++)
        for(x = 0; x<16; x++)
        {
            TextOut(hdc, (2 * x + 5) *  cxGrid/2,
                    (y + 3) * cyGrid + 2, szBuffer,
                        wsprintf(szBuffer, TEXT("%c"), 16 * x + y));
        }
        EndPaint(hWnd, &ps);
        return(0);

        case WM_DESTROY:
            PostQuitMessage(0);
            return(0);
    }
    return(DefWindowProc(hWnd, uMsg, wParam , lParam));
}
