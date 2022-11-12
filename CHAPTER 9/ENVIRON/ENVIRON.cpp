/*-----------------------------------------------
    ENVIRON.CPP     -- Enviroment List Box
                    (c)Dhiraj Jadhav, 2022
--------------------------------------------------*/

#include <Windows.h>
#include <assert.h>

#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#define ID_LIST     1
#define ID_TEXT     2


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nShowCmd)
{
    TCHAR szClassName[] = TEXT("Environ");
    TCHAR szWindowName[] = TEXT("Environment List Box");

    HWND hWnd = NULL;
    HBRUSH hBrush = NULL;
    HCURSOR hCurSor = NULL;
    HICON hIcon = NULL;
    HICON hIconSm = NULL;

    WNDCLASSEX WndEx;
    MSG msg;

    ZeroMemory(&WndEx, sizeof(WNDCLASSEX));
    ZeroMemory(&msg, sizeof(MSG));

    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    assert(hBrush != NULL);

    hCurSor = LoadCursor(NULL, IDC_ARROW);
    assert(hCurSor != NULL);

    hIcon = LoadIcon(NULL, IDI_APPLICATION);
    assert(hBrush != NULL);

    hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    assert(hIconSm != NULL);

    WndEx.cbSize = sizeof(WNDCLASSEX);
    WndEx.cbClsExtra = 0;
    WndEx.cbWndExtra = 0;
    WndEx.hbrBackground = hBrush;
    WndEx.hCursor = hCurSor;
    WndEx.hIcon = hIcon;
    WndEx.hIconSm = hIconSm;
    WndEx.hInstance = hInstance;
    WndEx.lpfnWndProc = WndProc;
    WndEx.lpszClassName = szClassName;
    WndEx.lpszMenuName = szClassName;
    WndEx.style = CS_HREDRAW | CS_VREDRAW;

    ATOM bRet = RegisterClassEx(&WndEx);
    if(!bRet)
    {
        MessageBox(NULL, TEXT("Window Class Registration Failed!!!!"), szClassName, MB_OK | MB_ICONERROR);
        return (0);
    }

    hWnd = CreateWindowEx(WS_EX_APPWINDOW, szClassName, szWindowName, WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
                        (HWND)NULL, (HMENU)NULL,hInstance, (LPVOID)NULL);
    
    if(hWnd == NULL)
    {
        MessageBox(NULL, TEXT("Window Creation Failed!!!!"), szWindowName, MB_OK | MB_ICONERROR);
        return (0);
    }

    ShowWindow(hWnd, nShowCmd);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (msg.wParam);
}
void FillListBox(HWND hWndList)
{
    int iLength;
    TCHAR* pVarBlock, *pVarBeg, *pVarEnd, *pVarName;
    
    pVarBlock = GetEnvironmentStrings();        //Get Pointer to enviroment Block

    while (*pVarBlock)
    {
        if(*pVarBlock != '=')               //Skip variable names beginning With '='
        {
            pVarBeg = pVarBlock;            //Begining of variable Name 
            while (*pVarBlock++ != '=')     // Scan until '='
                pVarEnd = pVarBlock-1;      //Points to '=' sign
                iLength = pVarEnd-pVarBeg;  //Length of variable name

                        //Allocate memory for the variable name and terminating
                        //Zero . Copy the Variable name and append a Zero.
                pVarName = (TCHAR*)calloc(iLength + 1, sizeof(TCHAR));
                CopyMemory(pVarName, pVarBeg, iLength * sizeof(TCHAR));
                pVarName[iLength] = '\0';

                        //put the variable name in the list box ans free memory 
                SendMessage(hWndList, LB_ADDSTRING, 0, (LPARAM)pVarName);
                free(pVarName);   
        }
        while(*pVarBlock++ != '\0');        //Scan until terminating zero
    }
    FreeEnvironmentStrings(pVarBlock);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    static HWND hWndList, hWndText;
    int iIndex, iLength, cxChar, cyChar;
    TCHAR* pVarName, *pVarValue;

    switch (uMsg)
    {
    case WM_CREATE:
        cxChar = LOWORD(GetDialogBaseUnits());
        cyChar = HIWORD(GetDialogBaseUnits());

            //Create Listbox and static text Window
        hWndList = CreateWindowEx(0, TEXT("listbox"), NULL,
                            WS_CHILD | WS_VISIBLE | LBS_STANDARD,
                            cxChar, cyChar * 3,
                            cxChar * 16 + GetSystemMetrics(SM_CXVSCROLL),
                            cyChar * 5,
                            hWnd, (HMENU)ID_LIST, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), 
                            NULL);
        
        hWndList = CreateWindowEx(0, TEXT("static"), NULL,
                            WS_CHILD | WS_VISIBLE | SS_LEFT,
                            cxChar, cyChar,
                            GetSystemMetrics(SM_CXSCREEN),
                            cyChar,
                            hWnd, (HMENU)ID_TEXT, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), 
                            NULL);
        FillListBox(hWndList);
        return (0);

    case WM_SETFOCUS:
        SetFocus(hWndList);
        return (0);
    
    case WM_COMMAND:
        if(LOWORD(wParam) == ID_LIST && HIWORD(wParam) == LBN_SELCHANGE)
        {
                //get curretnt selection.
            iIndex = SendMessage(hWndList, LB_GETCURSEL, 0, 0);
             
        }

    }
    return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}