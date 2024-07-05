#include <Windows.h>
#include <tchar.h>

#include "../WndService.h"

LRESULT CALLBACK WndProc(HWND wnd, UINT message, WPARAM w, LPARAM l)
{
    switch (message)
    {
    case WM_CREATE:
        CreateWindow(TEXT("STATIC"), TEXT("Normal Font"), WS_VISIBLE | WS_CHILD, 10,  10, 200, 25, wnd, NULL, (HINSTANCE)GetWindowLongPtr(wnd, GWLP_HINSTANCE), NULL);
        lowo::services::window::set_menu_font(CreateWindow(TEXT("STATIC"), TEXT("Menu Font"), WS_VISIBLE | WS_CHILD, 10,  40, 200, 25, wnd, NULL, (HINSTANCE)GetWindowLongPtr(wnd, GWLP_HINSTANCE), NULL));
        lowo::services::window::set_bold_font(CreateWindow(TEXT("STATIC"), TEXT("Bold Font"), WS_VISIBLE | WS_CHILD, 10,  70, 200, 25, wnd, NULL, (HINSTANCE)GetWindowLongPtr(wnd, GWLP_HINSTANCE), NULL), 0);
        lowo::services::window::set_bold_font(CreateWindow(TEXT("STATIC"), TEXT("Bold Bigger Font"), WS_VISIBLE | WS_CHILD, 10, 100, 200, 25, wnd, NULL, (HINSTANCE)GetWindowLongPtr(wnd, GWLP_HINSTANCE), NULL), 3);
        CreateWindow(TEXT("STATIC"), TEXT("no compact very long string 1234567890 qwertyuiopasdfghjklzxcvbnm"), WS_VISIBLE | WS_CHILD, 10,  130, 200, 25, wnd, NULL, (HINSTANCE)GetWindowLongPtr(wnd, GWLP_HINSTANCE), NULL);
        lowo::services::window::set_window_compact_text(
            CreateWindow(TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD, 10, 160, 200, 25, wnd, NULL, (HINSTANCE)GetWindowLongPtr(wnd, GWLP_HINSTANCE), NULL),
            TEXT("compact very long string 1234567890 qwertyuiopasdfghjklzxcvbnm")
        );
        lowo::services::window::set_window_compact_text(
            CreateWindow(TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD, 10, 190, 300, 25, wnd, NULL, (HINSTANCE)GetWindowLongPtr(wnd, GWLP_HINSTANCE), NULL),
            TEXT("compact very long string 1234567890 qwertyuiopasdfghjklzxcvbnm")
        );
        lowo::services::window::set_window_compact_text(
            CreateWindow(TEXT("STATIC"), TEXT(""), WS_VISIBLE | WS_CHILD, 10, 220, 400, 25, wnd, NULL, (HINSTANCE)GetWindowLongPtr(wnd, GWLP_HINSTANCE), NULL),
            TEXT("compact very long string 1234567890 qwertyuiopasdfghjklzxcvbnm")
        );
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(wnd, message, w, l);
}

int WINAPI _tWinMain(HINSTANCE inst, HINSTANCE, LPTSTR cmdLine, int cmdShow)
{
    constexpr LPCTSTR className = TEXT("WndServiceTest_ClassName");

    WNDCLASS wc = {0, };
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = inst;
    wc.lpszClassName = className;

    RegisterClass(&wc);

    HWND wnd = CreateWindow(className, TEXT("Title"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, inst, NULL
    );

    if (wnd == NULL)    return 0;

    ShowWindow(wnd, cmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
