#include "__popups__.h"



void tui_draw_popup_header (WINDOW * win, const wchar_t * popup_name)
{
    box (win, 0, 0);

    mvwaddwstr (win, 2, 0, L"├");
    for (size_t i = 0; i < getmaxx(win) - 2; i++)
        waddwstr(win, L"╌");
    waddwstr(win, L"┤");

    mvwprintw(win, 1, (getmaxx(win) - getmin(wcslen(popup_name), getmaxx(win) - 2)) / 2, "%ls", popup_name);
}