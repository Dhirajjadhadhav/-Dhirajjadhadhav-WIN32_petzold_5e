/*-----------------------------------------------------
    SYSMETS4.CPP -- System Metrics Display Program No. 4
                    (c) Dhiraj Jadhav, 2022
--------------------------------------------------------*/
#include<Windows.h>
#include<assert.h>
#include "Sysmets.h"

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstacne, LPSTR szCmdLine, int nShowCmd)
{
    TCHAR szClassName[] = TEXT("SysMets4");
    TCHAR szWindowname[] = TEXT("Get System Metrics No. 4");

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
    
    static int cxChar , cyChar, cxCaps, cxClient, cyClient, iMaxWidth;
    HDC hdc;
    int i, iLen = 0, x, y, iVertPos, iHorzPos, iPaintBeg, iPainEnd;
    PAINTSTRUCT ps;
    SCROLLINFO si;
    TCHAR szBuffer[10];
    TEXTMETRIC tm;

    switch (uMsg)
    {

    case WM_CREATE:
        hdc = GetDC(hWnd);

        GetTextMetrics(hdc, &tm);
        cxChar = tm.tmAveCharWidth;
        cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2)  * cxChar/ 2;
        cyChar  = tm.tmHeight + tm.tmExternalLeading;

        ReleaseDC(hWnd, hdc);
                            //save the width of the three columns

        iMaxWidth = 40 * cxChar + 22* cxCaps;
        return(0);

    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
            //set vertical Scroll bar range  and page size
        
        si.cbSize = sizeof(si);
        si.fMask = SIF_RANGE | SIF_PAGE;
        si.nMin = 0;
        si.nMax  = NUMLINES - 1;
        si.nPage = cyClient / cyChar;
        SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

            //set horizontal Scroll bar range and page size
        si.cbSize = sizeof(si);
        si.fMask = SIF_RANGE | SIF_PAGE;
        si.nMin = 0;
        si.nMax = 2 + iMaxWidth/cxChar;
        si.nPage = cxClient / cxChar;
        SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
        return(0);

    case WM_VSCROLL:
            //Get all the vertial scroll bat information 
        si.cbSize = sizeof(si);
        si.fMask = SIF_ALL;
        GetScrollInfo(hWnd, SB_VERT, &si);

            //save the position for comparision later on

        iVertPos = si.nPos;

        switch(LOWORD(wParam))
        {
            case SB_TOP:
                si.nPos = si.nMin;
                break;
            
            case SB_BOTTOM:
                si.nPos = si.nMax;
                break;
            
            case SB_LINEUP:
                si.nPos -= 1;
                break;
            
            case SB_LINEDOWN:
                si.nPos += 1;
                break;
            
            case SB_PAGEDOWN:
                si.nPos += si.nPage;
                break;
            
            case SB_PAGEUP:
                si.nPos -=si.nPage;
                break;
            
            case SB_THUMBTRACK:
                si.nPos = si.nTrackPos;
                break;
            
            default:
                break;
        }
        //set the position and then retrive it. Due to adjestments
        // by window it might not  be the same as the value set.

        si.fMask = SIF_POS;
        SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
        GetScrollInfo(hWnd, SB_VERT, &si);

            //if the position has changed, Scroll the window and update it
        if(si.nPos != iVertPos)
        {
            ScrollWindow(hWnd, 0, cyChar *(iVertPos - si.nPos),  NULL, NULL);
            UpdateWindow(hWnd);
        }
        return(0);
    
    case WM_HSCROLL:
                //get all Horizontal scroll bar information
        
        si.cbSize = sizeof(si);
        si.fMask = SIF_ALL;

                //save the position for comparision later on
        GetScrollInfo(hWnd, SB_HORZ, &si);
        iHorzPos = si.nPos;

        switch(LOWORD(wParam))
        {
            case SB_LINELEFT:
                si.nPos -=1;
                break;
            
            case SB_LINERIGHT:
                si.nPos += 1;
                break;
            
            case SB_PAGELEFT:
                si.nPos -= si.nPage;
                break;
            
            case SB_PAGERIGHT:
                si.nPage += si.nPage;
                break;
            
            case SB_THUMBPOSITION:
                si.nPos = si.nTrackPos;
                break;
            
            default:
                break;
        }

                // Set the position and then retrive it. Due to adjustMents
                //by Window it might  not the same as value as the vaule set.
        
        si.fMask = SIF_POS;
        SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
        GetScrollInfo(hWnd, SB_HORZ, &si);

                // if the position has changed, Scroll the Window 
        if(si.nPos != iHorzPos)
        {
            ScrollWindow(hWnd, cxChar* (iHorzPos - si.nPos), 0, NULL, NULL);
        }
        return(0);
    
    case WM_KEYDOWN:
        switch(wParam)
        {
            case VK_HOME:   
                SendMessage(hWnd, WM_VSCROLL, SB_TOP, 0);
                break;
            
            case VK_END:
                SendMessage(hWnd, WM_VSCROLL, SB_BOTTOM, 0);
                break;
            
            case VK_PRIOR:
                SendMessage(hWnd, WM_VSCROLL, SB_PAGEUP, 0);
                break;
            
            case VK_NEXT:
                SendMessage(hWnd, WM_VSCROLL, SB_PAGEDOWN, 0);
                break;
            
            case VK_UP:
                SendMessage(hWnd, WS_VSCROLL, SB_LINEUP, 0);
                break;

            case VK_DOWN:
                SendMessage(hWnd, WM_VSCROLL, SB_LINEDOWN, 0);
                break;

            case VK_LEFT:
                SendMessage(hWnd, WM_HSCROLL, SB_PAGEUP, 0);
                break;
            
            case VK_RIGHT:
                SendMessage(hWnd, WM_HSCROLL, SB_PAGEDOWN, 0);
                break;
        }
        break;

    case WM_PAINT:

        hdc = BeginPaint(hWnd, &ps); 

            //Get Vertical Scroll bar position
        si.cbSize = sizeof(si);
        si.fMask = SIF_POS;
        GetScrollInfo(hWnd, SB_VERT, &si);
        iVertPos = si.nPos;

            //get Horozonatal Scroll bar position
        GetScrollInfo(hWnd, SB_HORZ, &si);
        iHorzPos = si.nPos;

            //find the painting limit 
        iPaintBeg = max(0, iVertPos + ps.rcPaint.top / cyChar);
        iPainEnd = min(NUMLINES - 1, iVertPos + ps.rcPaint.bottom /cyChar);

        for(i = iPaintBeg; i <= iPainEnd; ++i)
        {
             x = cxChar * (1-iHorzPos);
             y = cyChar * (i - iVertPos);

            TextOut(hdc, x, y, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel)); 
            TextOut(hdc, x + 22 * cxCaps, y, sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc)); 
            SetTextAlign(hdc, TA_TOP | TA_RIGHT); 
            ZeroMemory(szBuffer, 16); 
            iLen = wsprintf(szBuffer, "%5d", GetSystemMetrics(sysmetrics[i].iIndex), iLen);
            TextOut(hdc, x + 22 * cxCaps + 40 * cxChar, y, szBuffer, iLen); 
            SetTextAlign(hdc, TA_TOP | TA_LEFT);   
        }
       
        EndPaint(hWnd, &ps);
        return(0);

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}
