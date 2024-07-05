#include "WndService.h"

#include <map>
#include <memory>
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

#define BEGIN_NAMESPACE namespace lowo { namespace services { namespace window {
#define END_NAMESPACE   }}}

BEGIN_NAMESPACE

namespace implementation
{
    namespace constant
    {
        NONCLIENTMETRICS& get_nc_metrics()
        {
            static class _nc_metrics : public NONCLIENTMETRICS
            {
            public:
                _nc_metrics()
                {
                    memset(this, 0, sizeof(NONCLIENTMETRICS));
                    cbSize = sizeof(NONCLIENTMETRICS);
                    SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), this, 0);
                }
            } r;
            return r;
        }
    }

    namespace models
    {
        class basic_font
        {
        public:
            HFONT get() const { return font_; }

        protected:
            basic_font() : font_(NULL) {}
            basic_font(basic_font&& o)
            {
                font_ = o.font_;
                o.font_ = NULL;
            }
            virtual ~basic_font() { if (font_) DeleteObject(font_); }

            void set(HFONT font) { font_ = font; }
            void create_indirect(const LOGFONT* plf) { font_ = CreateFontIndirect(plf); }

        private:
            HFONT font_;
        };

        class menu_font : public basic_font
        {
        public:
            menu_font()
            {
                LOGFONT lf;
                lf = constant::get_nc_metrics().lfMenuFont;
                lf.lfQuality = CLEARTYPE_QUALITY;

                create_indirect(&lf);
            }

            menu_font(menu_font&& o) : basic_font(std::move(o)) {}
        };

        class bold_font : public basic_font
        {
        public:
            bold_font(LONG increase_size = 0)
            {
                LOGFONT lf;
                lf = constant::get_nc_metrics().lfMenuFont;
                lf.lfHeight -= increase_size;
                lf.lfWeight = FW_BOLD;
                lf.lfQuality = CLEARTYPE_QUALITY;

                create_indirect(&lf);
            }

            bold_font(bold_font&& o) : basic_font(std::move(o)) {}
        };
    }

    int get_average_char_width(const LPTEXTMETRIC ptm, LPCTSTR src)
    {
        int ret = 0;
        int len = 0;
        DWORD dwError = ERROR_SUCCESS;
        if (ptm && src && ((len = (int)_tcslen(src)) != 0))
        {
            int total = 0;
            for (int i = 0; i < len; i++)
            {
                if (_istascii(src[i]))  total += ptm->tmAveCharWidth;
                else                    total += ptm->tmMaxCharWidth;
            }
            ret = (total / len);
        }
        else
        {
            dwError = ERROR_INVALID_PARAMETER;
        }
        SetLastError(dwError);
        return ret;
    }

    HFONT get_menu_font()
    {
        static models::menu_font r;
        return r.get();
    }

    HFONT get_bold_font(int increase_size)
    {
        typedef std::map<int, models::bold_font> font_map;
        static font_map fonts;
        auto it = fonts.find(increase_size);
        if (it == fonts.end())
        {
            it = fonts.insert(font_map::value_type(increase_size, models::bold_font(increase_size))).first;
        }
        return it->second.get();
    }

    BOOL CALLBACK set_font_child_callback(HWND wnd, LPARAM param)
    {
        SendMessage(wnd, WM_SETFONT, (WPARAM)param, (LPARAM)TRUE);
        EnumChildWindows(wnd, set_font_child_callback, param);
        return TRUE;
    }
}

END_NAMESPACE

int lowo::services::window::get_compact_char_count(HWND wnd, LPCTSTR src)
{
    int ret = 0;
    DWORD dwError = ERROR_SUCCESS;
    if (wnd && src)
    {
        HDC dc = GetDC(wnd);
        if (dc)
        {
            TEXTMETRIC tm;
            if (GetTextMetrics(dc, &tm))
            {
                int char_width = implementation::get_average_char_width(&tm, src);
                if (char_width)
                {
                    char_width = MulDiv(char_width, GetDeviceCaps(dc, LOGPIXELSX), 96);

                    RECT rect;
                    GetClientRect(wnd, &rect);
                    ret = (rect.right - rect.left) / char_width;
                }
            }
            ReleaseDC(wnd, dc);
        }
    }
    else
    {
        dwError = ERROR_INVALID_PARAMETER;
    }
    SetLastError(dwError);
    return ret;
}

BOOL lowo::services::window::get_compact_text(HWND wnd, LPCTSTR src, std::basic_string<TCHAR>& dst)
{
    DWORD dwError = ERROR_SUCCESS;
    int cch = get_compact_char_count(wnd, src);
    if (cch)
    {
        LPTSTR pszDst = new TCHAR[cch + 2];
        if (PathCompactPathEx(pszDst, src, cch + 1, 0)) dst = pszDst;
        else                                            dwError = ERROR_BAD_ENVIRONMENT;
        delete[] pszDst;
    }
    else
    {
        dwError = GetLastError();
        if (dwError == ERROR_SUCCESS)
        {
            dwError = ERROR_BAD_ENVIRONMENT;
        }
    }
    SetLastError(dwError);
    return (dwError == ERROR_SUCCESS);
}

BOOL lowo::services::window::set_window_compact_text(HWND wnd, LPCTSTR src)
{
    DWORD dwError = ERROR_SUCCESS;
    std::basic_string<TCHAR> text;
    if (get_compact_text(wnd, src, text))   SetWindowText(wnd, text.c_str());
    else                                    dwError = GetLastError();
    SetLastError(dwError);
    return (dwError == ERROR_SUCCESS);
}

void lowo::services::window::set_menu_font(HWND wnd, BOOL withChild)
{
    if (withChild)  EnumChildWindows(wnd, implementation::set_font_child_callback, (LPARAM)implementation::get_menu_font());
    else            SendMessage(wnd, WM_SETFONT, (WPARAM)implementation::get_menu_font(), (LPARAM)TRUE);
}

void lowo::services::window::set_bold_font(HWND wnd, int increaseSize)
{
    SendMessage(wnd, WM_SETFONT, (WPARAM)implementation::get_bold_font(increaseSize), (LPARAM)TRUE);
}
