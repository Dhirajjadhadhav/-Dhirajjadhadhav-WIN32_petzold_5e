/*-----------------------------------------
  POPPAD.C      -- Popup Wditor uing Child Window edit box
                (c) Dhiraj Jadhav, 2022  
*/

#include <Windows.h>
#include <assert.h> 

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#define ID_EDIT     1

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

TCHAR szClassName[] = TEXT("PopPad1");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nShowCmd)
{

    TCHAR szWindowName[] = TEXT("PopPad 1");

    HWND hWnd = NULL;
    HBRUSH hBrush = NULL;
    HCURSOR hCursor = NULL;
    HICON hIcon = NULL;
    HICON hIconSm = NULL;

    WNDCLASSEX wnd;
    MSG msg;

    ZeroMemory(&wnd, sizeof(WNDCLASSEX));
    ZeroMemory(&msg, sizeof(MSG));

    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    assert(hBrush != NULL);

    hCursor = LoadCursor(NULL, IDC_ARROW);
    assert(hCursor != NULL);

    hIcon = LoadIcon(NULL, IDI_APPLICATION);
    assert(hIcon != NULL);

    hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    assert(hIconSm != NULL);

    wnd.cbSize = sizeof(WNDCLASSEX);
    wnd.cbClsExtra = 0;
    wnd.cbWndExtra = 0;
    wnd.hbrBackground = hBrush;
    wnd.hCursor = hCursor;
    wnd.hIcon = hIcon;
    wnd.hIconSm = hIconSm;
    wnd.hInstance = hInstance;
    wnd.lpfnWndProc = WndProc;
    wnd.lpszClassName = szClassName;
    wnd.lpszMenuName = NULL;
    wnd.style = CS_VREDRAW | CS_HREDRAW;

    ATOM bRet = RegisterClassEx(&wnd);
    if(!bRet)
    {
        MessageBox(NULL, TEXT("Window Class Registeratin Failed!!!"), szClassName, MB_OK | MB_ICONERROR);
        return (0);
    }

    hWnd = CreateWindowEx(WS_EX_APPWINDOW, szClassName, szWindowName, WS_OVERLAPPEDWINDOW, 
                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                        (HWND)NULL, (HMENU)NULL, hInstance, (LPVOID)NULL);
    if(hWnd == NULL)
    {
        MessageBox(NULL, TEXT("Window Creation Failed !!!"), szWindowName, MB_OK | MB_ICONERROR);
        return (0);
    }

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
    static HWND hWndEdit;

    switch(uMsg)
    {
        case WM_CREATE:
            hWndEdit = CreateWindowEx(0, TEXT("edit"), NULL, 
                                    WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | 
                                            WS_BORDER | ES_LEFT | ES_MULTILINE |
                                            ES_AUTOHSCROLL | ES_AUTOVSCROLL  , 
                                            0, 0, 0, 0, hWnd, (HMENU)ID_EDIT,
                                            ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            
            return (0);
        
        case WM_SETFOCUS:
            SetFocus(hWndEdit);
            return (0);
        
        case WM_SIZE:
            MoveWindow(hWndEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
            return (0);
        
        case WM_COMMAND:

            if(LOWORD(wParam) == ID_EDIT)
                if(HIWORD(wParam) == EN_ERRSPACE || 
                    HIWORD(wParam) == EN_MAXTEXT)
                MessageBox(hWnd, TEXT("Edit Control out of space"), szClassName, MB_OK | MB_ICONERROR);
                return (0);
        
        case WM_DESTROY:
                PostQuitMessage(0);
                return (0);

    }
    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}