#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include "date.h"
#include "lists.h"
#include "files.h"
#include "additional_task.h"

#include "tui.h"

void print (list_elem_t * e, wchar_t * text)
{
    setlocale(LC_ALL, "");

    wchar_t date_wcs[1024];
    date2wcs(&e->data.date, date_wcs);
    swprintf(text, 2048, L"%lu%c%lu%c%ls%c%ls%c%ls%c%ls%c%.3Lf%c%.2Lf%c%ls",
            0, CSV_DELIMITER,
            e->data.cage_num, CSV_DELIMITER,
            e->data.animal.name, CSV_DELIMITER,
            e->data.animal.breed, CSV_DELIMITER,
            e->data.animal.areal, CSV_DELIMITER,
            e->data.products.type, CSV_DELIMITER,
            e->data.products.weight, CSV_DELIMITER,
            e->data.products.cost, CSV_DELIMITER,
            date_wcs
        );
}

void tui_draw_form_field (WINDOW * win, int y, int x, const wchar_t * placeholder, const wchar_t * value, int width);

int main ()
{
    const menu_t MAIN_MENU = {
        L"     Просто выйти      ",
        L" Выйти с большой буквы ",
        L"   Выйти через Ctrl+C  ",
        NULL
    };

    setlocale(LC_ALL, "");

    initscr();
    raw();
    noecho();
    keypad(stdscr, 1);

    // tui_draw_popup_text_message(L"Текстовое окно", L"merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp merp");
    //tui_draw_vmenu(stdscr, 0, 0, 1, MAIN_MENU, 0);
    
    // fwprintf (stderr, L"Выбрана кнопка: %2d.\n", tui_draw_popup_select (L"Выбор выхода из программы", L"Как предпочитаете выйти из этой моноформной программы? Всё остальное - это merp merp merp merp merp merp merp merp ....", MAIN_MENU));

    // tui_draw_form_field (stdscr, 0, 0, L"Введите имя", L"", 10);
    // refresh();
    // getwchar();

    for (size_t counter = 0; counter < LINES * COLS; counter ++)
        waddstr (stdscr, "-");
    
    refresh ();

    /*
    wchar_t value[200];
    memset(value, 0, sizeof(value));

    getwchar ();

    tui_draw_popup_form (L"А как вас зовут?",
    L"Вы вошли в таверну.\n"
    L"Вы были ослеплены светом ламп.\n\n"
    L"К вам подходит высокий бородатый мужчина, одетый в кожанную куртку. За спиной у него дробовик.\n"
    L"Он спросил ваше имя. Что ему ответите?"
    , L"Промямлите ваше имя", value, 199, VMASK_ANY_CHAR | VMASK_SPACES | VMASK_PUNCTS);

    refresh ();
    getwchar ();
    */

    list_obj_t * list_obj = lists_CreateNewListObject();
    read_from_csv (list_obj, L"data.csv");

    WINDOW * win_table = newwin(20, COLS, 0, 0);

    tui_draw_table_in_window (win_table, *list_obj, 2, 0);
    refresh();
    wrefresh(win_table);

    getwchar();

    delwin(win_table);
    lists_DeleteListObject(list_obj);

    endwin();
    return 0;
}