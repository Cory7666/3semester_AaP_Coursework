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
#include "checker.h"
#include "lists.h"



#define TUI_HIGHLIGHT_ITEM_STYLE ((A_REVERSE) | (A_BOLD))



/* Тип массива из пунктов меню */
typedef wchar_t* menu_t[];



/* Получить количество строк, занимаемых переданным текстом. Также учитываются символы перехода на новую строку */
int get_message_lines_count (const wchar_t *, const int max_width);

/* Получить количество пунктов в меню */
int tui_get_menu_item_count (const menu_t);
/* Отрисовать вертикальное меню в заданном месте с заданным расстоянием между элементами */
void tui_draw_vmenu (WINDOW *, const int, const int, const int, const menu_t, const int);

/* Отобразить окно с обычным текстом */
void tui_draw_popup_text_message (const wchar_t *, const wchar_t *);
/* Отобразить окно с выбором */
int tui_draw_popup_select (const wchar_t *, const wchar_t *, const menu_t);
/* Отобразить окно с формой */
int tui_draw_popup_form (const wchar_t *, const wchar_t *, const wchar_t *, wchar_t *, int, checker_mode_t);

/* Нарисовать таблицу в выбранном окне, отступив от начала списка на num_page страниц */
int tui_draw_table_in_window (WINDOW *, const list_obj_t, const length_t, const int);


#endif // ! _TUI_POPUPS_H