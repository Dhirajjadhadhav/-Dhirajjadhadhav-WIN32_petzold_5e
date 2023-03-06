/*-----------------------------------------------------
    kKEYVIEW.CPP -- Display keybord and charcter message
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
    TCHAR szClassName[] = TEXT("KeyView1");
    TCHAR szWindowname[] = TEXT("Keybord message Viewer #1");

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
    static int cxClientMax, cyClientMax, cxClient, cyClient, cxChar, cyChar;
    static int cLinesMax, cLines;
    static PMSG pmsg;
    static RECT rectScroll;
    static TCHAR szTop[] =  TEXT("Message           key             char     ")
                            TEXT("Repeat    scan    Ext     ALT     Prev    Tran");
    static TCHAR szUnd[] = TEXT("________           ____            ______   ")
                            TEXT("______    ____    ___     ___     ____    ____");
    static TCHAR* szFormat[2] = {
            TEXT("%-13s %3d %-15s%c%6u %4d %3s %4s %4s"),
            TEXT("%-13s             0x%04X%1s%c %6u %4d %3s %3s %4s %4s") };
    
    static TCHAR* szYes = TEXT("Yes");
    static TCHAR* szNo = TEXT("No");
    static TCHAR* szDown = TEXT("Down");
    static TCHAR* szUp = TEXT("UP");

    static TCHAR* szMessage[]  = {
            TEXT("WM_KEYDOWN"), TEXT("WM_KEYUP"),
            TEXT("WM_CHAR"), TEXT("WM_DEADCHAR"),
            TEXT("WM_SYSKEYDOWN"), TEXT("WM_SYSKEYUP"),
            TEXT("WM_SYSCHAR"), TEXT("SYSDEADCHAR") };

    HDC     hdc;
    int     i, iType;
    PAINTSTRUCT ps;
    TCHAR szBuffer[128], szKeyName[32];
    TEXTMETRIC tm;

    switch(uMsg)
    {
        case WM_CREATE:
        case WM_DISPLAYCHANGE:

                //GET MAXIMUM SIZE OF CLIENT AREA
            cxClientMax = GetSystemMetrics(SM_CXMAXIMIZED);
            cyClientMax  = GetSystemMetrics(SM_CYMAXIMIZED);

                //Get charater size for fixed-pitch font
            hdc = GetDC(hWnd);
            
            SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
            GetTextMetrics(hdc, &tm);
            cxChar = tm.tmAveCharWidth;
            cyChar = tm.tmHeight;

            ReleaseDC(hWnd, hdc);

                    //Allocate memory for Diplaying lies
            if(pmsg)
                free(pmsg);
            
            cLinesMax  = cyClient/cyChar;
            pmsg = (PMSG)malloc(cLinesMax * sizeof(MSG));
            cLines = 0;
                                //fall thruogh
            
        case WM_SIZE:
            if(uMsg == WM_SIZE)
            {
                cxClient = LOWORD(lParam);
                cyClient = HIWORD(lParam);
            }
                    //calculate Scrolling retcangle
            rectScroll.left = 0;
            rectScroll.right = cxClient;
            rectScroll.top = cyChar;
            rectScroll.bottom = cyChar * (cyClient/cyChar);

            InvalidateRect(hWnd, NULL , TRUE);
            return(0);

        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_CHAR:
        case WM_DEADCHAR:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_SYSCHAR:
        case WM_SYSDEADCHAR:

                //Rearrange Storage array
            for(i = cLinesMax-1; i>0 ; i--)
            {
                pmsg[i] = pmsg[i-1]; 
            }
                //storage new message 

            pmsg[0].hwnd = hWnd;
            pmsg[0].message = uMsg;
            pmsg[0].wParam = wParam;
            pmsg[0].lParam = lParam;

            cLines = min(cLines+1, cLinesMax);

                //scroll up the display 
            ScrollWindow(hWnd, 0, -cyChar, &rectScroll, &rectScroll);
            break;
                            //i.e. call DefWindowProc so sys message work

            case WM_PAINT:
                hdc = BeginPaint(hWnd, &ps);

                SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
                SetBkMode(hdc, TRANSPARENT);
                TextOut(hdc, 0 , 0, szTop,lstrlen(szTop));
                TextOut(hdc, 0, 0, szUnd, lstrlen(szUnd));


                for(i = 0; i<min(cLines, cyClient/cyChar - 1); i++)
                {
                    iType = pmsg[i].message == WM_CHAR || 
                            pmsg[i].message == WM_SYSCHAR ||
                            pmsg[i].message == WM_DEADCHAR ||
                            pmsg[i].message == WM_SYSDEADCHAR;

                    GetKeyNameText(pmsg[i].lParam, szKeyName, sizeof(szKeyName)/sizeof(TCHAR));

                    TextOut(hdc, 0, (cyClient/cyChar- 1 - i) * cyChar, szBuffer, wsprintf(szBuffer, szFormat[iType], 
                                            szMessage[pmsg[i].message - WM_KEYFIRST], pmsg[i].wParam,
                                            (PTSTR)(iType?TEXT(" ") : szKeyName), 
                                            (TCHAR)(iType? pmsg[i].wParam : ' '), 
                                            LOWORD(pmsg[i].lParam), 
                                            HIWORD(pmsg[i].lParam )& 0xFF,
                                            0x01000000 & pmsg[i].lParam ? szYes : szNo,
                                            0x20000000 & pmsg[i].lParam ? szYes : szNo,
                                            0x40000000 & pmsg[i].lParam ? szDown : szUp,
                                            0x80000000 & pmsg[i].lParam ? szUp   : szDown));
                }     
                EndPaint(hWnd, &ps);
                return(0);
        
        case WM_DESTROY:
            PostQuitMessage(0);
            return(0);  

    }
    return(DefWindowProc(hWnd, uMsg, wParam, lParam)); 
}
