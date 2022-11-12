#include<Windows.h>
#include<assert.h>

 #pragma comment(lib, "kernel32.lib")
 #pragma comment(lib, "gdi32.lib") 
 #pragma comment(lib, "user32.lib")


#define NUMLINES ((int)(sizeof(devcaps) / sizeof(devcaps[0])))

struct
{   
    int iIndex;
    const TCHAR* szLabel;
    const TCHAR* szDesc; 
}
devcaps[] =
{
    HORZSIZE,       TEXT("HORZSIZE"),       TEXT("Width in milimeter:"),
    VERTSIZE,       TEXT("VERTSIZE"),       TEXT("Height in milimeter:"),
    HORZRES,        TEXT("HORZRES"),        TEXT("Width in pixel:"),
    VERTRES,        TEXT("VERTRES"),        TEXT("Heigth in piexl:"),
    BITSPIXEL,      TEXT("BITSPIXEL"),      TEXT("Color bit per pixel:"),
    PLANES,         TEXT("PLANES"),         TEXT("Number of color planes:"),
    NUMBRUSHES,     TEXT("NUMBRUSHES"),     TEXT("Number of device brushes:"),
    NUMPENS,        TEXT("NUMPENS"),        TEXT("Number of device pen:"),
    NUMMARKERS,     TEXT("NUMMARKERS"),     TEXT("Number of device markers:"),
    NUMFONTS,       TEXT("NUMFONTS"),       TEXT("NUmber of device fonta:"),
    NUMCOLORS,      TEXT("NUMCOLORS"),      TEXT("Number of device color:"),
    PDEVICESIZE,    TEXT("PDEVICESIZE"),    TEXT("Size of device structure:"),
    ASPECTX,        TEXT("ASPECTX"),        TEXT("Relative width of pixel:"),
    ASPECTY,        TEXT("ASPECTY"),        TEXT("Relative height of pixel:"),
    ASPECTXY,       TEXT("ASPECTXY"),       TEXT("Relative diagonal of pixel:"),
    LOGPIXELSX,     TEXT("LOGPIXELSX"),     TEXT("Horizontal dots per inch:"),
    LOGPIXELSY,     TEXT("LOGPIXELSY"),     TEXT("Verical dots per inch:"),
    SIZEPALETTE,    TEXT("SIZEPALETTE"),    TEXT("Number of palete entries:"),
    NUMRESERVED,    TEXT("NUMREVERSED"),    TEXT("Reversed palette entries:"),
    COLORRES,       TEXT("COLORRES"),       TEXT("Actual color resolution:")

}; 

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstcne, LPSTR szCmdLine, int nShowCmd)
{
    static TCHAR szClassName[] = TEXT("devcaps1");
    static TCHAR szAppName[] = TEXT("Device capabilites");
    
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

    hWnd = CreateWindowEx(WS_EX_APPWINDOW, szClassName, szAppName, WS_OVERLAPPEDWINDOW, 
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
    static int cxChar, cxCaps, cyChar;
    TCHAR szBuffer[10];
    HDC hdc;
    int i;
    PAINTSTRUCT ps;
    TEXTMETRIC tm;

    switch(uMsg)
    {
        case WM_CREATE:
            hdc = GetDC(hWnd);

            GetTextMetrics(hdc, &tm);
            cxChar = tm.tmAveCharWidth;
            cxChar = (tm.tmPitchAndFamily & 1 ? 3 : 2) / 2;
            cyChar = tm.tmHeight + tm.tmExternalLeading;

            ReleaseDC(hWnd, hdc);
            return(0);

        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);

            for(i = 0; i<NUMLINES; i++)
            {

                TextOut(hdc, 0, cyChar * i, devcaps[i].szLabel, lstrlen(devcaps[i].szLabel));
                TextOut(hdc, 150* cxChar, i * cyChar, devcaps[i].szDesc, lstrlen(devcaps[i].szDesc));
                SetTextAlign(hdc, TA_RIGHT | TA_TOP);

                TextOut(hdc, 150 * cxChar + 250 * cxChar, cyChar * i, szBuffer, wsprintf(szBuffer, TEXT("%5d"), GetDeviceCaps(hdc, devcaps[i].iIndex)));
                SetTextAlign(hdc, TA_LEFT | TA_TOP);
            }
            EndPaint(hWnd, &ps);
            return(0);

            
        case WM_DESTROY:
            PostQuitMessage(0);
            return(0);
    }

    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}
