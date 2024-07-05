#pragma once

#include <string>
#include <Windows.h>
#include <tchar.h>

namespace lowo
{
    namespace services
    {
        namespace window
        {
            int get_compact_char_count(HWND wnd, LPCTSTR src);
            BOOL get_compact_text(HWND wnd, LPCTSTR src, std::basic_string<TCHAR>& dst);
            BOOL set_window_compact_text(HWND wnd, LPCTSTR src);

            void set_menu_font(HWND wnd, BOOL withChild = TRUE);
            void set_bold_font(HWND wnd, int increaseSize);
        }
    }
}
