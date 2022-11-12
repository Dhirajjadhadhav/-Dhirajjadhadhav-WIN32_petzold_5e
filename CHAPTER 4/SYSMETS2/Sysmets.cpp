#include <Windows.h> 
#include <assert.h>
#include "Sysmets.h"

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    static TCHAR szClassName[] = TEXT("The Standard Window"); 
    static TCHAR szAppName[] = TEXT("Displaying the system metrics"); 

    HWND hWnd = NULL; 

    WNDCLASSEX wnd; 
    MSG msg; 

    ZeroMemory(&wnd, sizeof(WNDCLASSEX)); 
    ZeroMemory(&msg, sizeof(MSG)); 

    wnd.cbSize = sizeof(WNDCLASSEX); 
    wnd.cbClsExtra = 0; 
    wnd.cbWndExtra = 0; 
    wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wnd.hCursor = LoadCursor(NULL, IDC_ARROW); 
    wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION); 
    wnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION); 
    wnd.hInstance = hInstance; 
    wnd.lpfnWndProc = WndProc; 
    wnd.lpszClassName = szClassName; 
    wnd.lpszMenuName = NULL; 
    wnd.style = CS_HREDRAW | CS_VREDRAW; 

    assert(RegisterClassEx(&wnd) != 0); 

    hWnd = CreateWindowEx(WS_EX_APPWINDOW, szClassName, szAppName, WS_OVERLAPPEDWINDOW | WS_VSCROLL, 
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
        NULL, NULL, hInstance, NULL); 
    assert(hWnd); 

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
    static int cxChar, cxCaps, cyChar, cyClient, iVscrollPos;
    HDC hdc;
    int i, y;
    PAINTSTRUCT ps;
    TCHAR szBuffer[10];
    TEXTMETRIC tm;

    switch(uMsg)
    {
        case WM_CREATE:
            hdc = GetDC(hWnd);

            GetTextMetrics(hdc, &tm);
            cxChar = tm.tmAveCharWidth;
            cyChar = tm.tmHeight + tm.tmExternalLeading;
            cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)* cxChar / 2;

            ReleaseDC(hWnd, hdc);

            SetScrollRange(hWnd, SB_VERT, 0, NUMLINES-1, FALSE);
            SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);
            return (0);
        
        case WM_SIZE:
            cyClient = HIWORD(lParam);
            return (0);

        case WM_VSCROLL:
            switch(LOWORD(wParam))
            {
                case SB_LINEUP:
                    iVscrollPos -= 1;
                    break;;
                
                case SB_LINEDOWN:
                    iVscrollPos +=1;
                    break;;
                
                case SB_PAGEDOWN:
                    iVscrollPos -= cyClient/cyChar;
                    break;
                
                case SB_PAGEUP:
                    iVscrollPos += cyClient/cyChar;
                    break;
                
                case SB_THUMBTRACK:
                    iVscrollPos = HIWORD(wParam);
                    break;
                
                default:
                    break;
            } 
            iVscrollPos = max(0, min(iVscrollPos, NUMLINES-1));

            if(iVscrollPos != GetScrollPos(hWnd, SB_VERT))
            {
                SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);
                InvalidateRect(hWnd, NULL, TRUE);
            }
            return (0);

        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);

            for(int i = 0; i<NUMLINES; i++)
            {
                y = cyChar * (i - iVscrollPos);
                
                TextOut(hdc, 0, y, 
                    sysmetrics[i].szLabel, 
                        lstrlen(sysmetrics[i].szLabel));

                TextOut(hdc, 22 * cxCaps, y, 
                    sysmetrics[i].szDesc, 
                        lstrlen(sysmetrics[i].szDesc));
                
                SetTextAlign(hdc, TA_TOP | TA_RIGHT);
                TextOut(hdc, 22 * cxCaps +  40 * cxChar, y, 
                        szBuffer, wsprintf(szBuffer, TEXT("%5d"), 
                            GetSystemMetrics(sysmetrics[i].iIndex)));

                SetTextAlign(hdc, TA_LEFT | TA_TOP);
            }
            EndPaint(hWnd, &ps);
            return (0);

        case WM_DESTROY:
            PostQuitMessage(0);
            return (0);
    }
    return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}
