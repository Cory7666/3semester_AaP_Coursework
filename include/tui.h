#ifndef _TUI_POPUPS_H
#define _TUI_POPUPS_H

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>

#include <ncursesw/ncurses.h>
#include <ncursesw/panel.h>

#include "keyboard_keys.h"
#include "cmp_funcs.h"



#define TUI_HIGHLIGHT_ITEM_STYLE ((A_REVERSE) | (A_BOLD))



/* Тип массива из пунктов меню */
typedef wchar_t* menu_t[];



/* Получить количество пунктов в меню */
int tui_get_menu_item_count (const menu_t);
/* Отрисовать вертикальное меню в заданном месте с заданным расстоянием между элементами */
void tui_draw_vmenu (WINDOW *, const int, const int, const int, const menu_t, const int);

/* Отобразить окно с обычным текстом */
void tui_draw_popup_text_message (const wchar_t *, const wchar_t *);
/* Отобразить окно с выбором */
int tui_draw_popup_select (const wchar_t *, const wchar_t *, const menu_t);



#endif // ! _TUI_POPUPS_H