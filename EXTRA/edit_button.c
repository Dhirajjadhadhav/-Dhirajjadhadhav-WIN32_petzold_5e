#include <windows.h>


#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#define ID_EDITBOX    1001
#define ID_BUTTON_OK  1002

void DrawCustomButton(LPDRAWITEMSTRUCT lpDrawItem) {

    COLORREF bgColor = RGB(255, 165, 0);     // orange
    COLORREF textColor = RGB(128, 128, 128);   // grey

    HBRUSH hBrush = CreateSolidBrush(bgColor);
    FillRect(lpDrawItem->hDC, &lpDrawItem->rcItem, hBrush);
    DeleteObject(hBrush);

    HFONT hFont = CreateFontW(
        20, 0, 0, 0,
        FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS,
        L"Helvetica"
    );
    HFONT hOldFont = SelectObject(lpDrawItem->hDC, hFont);

    SetTextColor(lpDrawItem->hDC, textColor);
    SetBkMode(lpDrawItem->hDC, TRANSPARENT);

    wchar_t text[256];
    GetWindowTextW(lpDrawItem->hwndItem, text, 256);

    RECT rcText = lpDrawItem->rcItem;
    DrawTextW(lpDrawItem->hDC, text, -1, &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(lpDrawItem->hDC, hOldFont);
    DeleteObject(hFont);

    if (lpDrawItem->itemState & ODS_FOCUS) {
        DrawFocusRect(lpDrawItem->hDC, &lpDrawItem->rcItem);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HFONT hFontButton = NULL;
    switch(uMsg) {
        case WM_CREATE:
        {
            LPCREATESTRUCTW pcs = (LPCREATESTRUCTW)lParam;

            HWND hEdit = CreateWindowExW(
                0,
                L"EDIT",
                L"",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                10, 10, 200, 25,
                hwnd,
                (HMENU)ID_EDITBOX,
                pcs->hInstance,
                NULL
            );

            HWND hButton = CreateWindowExW(
                0,
                L"BUTTON",
                L"Hello!",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
                220, 10, 100, 25,
                hwnd,
                (HMENU)ID_BUTTON_OK,
                pcs->hInstance,
                NULL
            );

            hFontButton = CreateFontW(
                20, 0, 0, 0,
                FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_SWISS,
                L"Arial"
            );
            
            SendMessageW(hButton, WM_SETFONT, (WPARAM)hFontButton, TRUE);
        }
        break;

        case WM_DRAWITEM:
        {
            LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lParam;
            if (lpDrawItem->CtlID == ID_BUTTON_OK) {
                DrawCustomButton(lpDrawItem);
                return TRUE;
            }
        }
        break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"MyWindowClass";

    WNDCLASSW wc = {0};
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursorW(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

    if (!RegisterClassW(&wc)) {
        return 0;
    }

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Window + input + button example",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 150,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {0};
    
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}