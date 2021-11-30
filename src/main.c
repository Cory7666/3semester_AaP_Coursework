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

    /*
    list_obj_t * list_obj = lists_CreateNewListObject();
    read_from_csv (list_obj, L"data.csv");

    WINDOW * win_table = newwin(20, COLS, 0, 0);

    tui_draw_table_in_window (win_table, *list_obj, 2, 0);
    refresh();
    wrefresh(win_table);

    getwchar();

    delwin(win_table);
    lists_DeleteListObject(list_obj);
    */

    /*
    udate_t d;
    wchar_t tmp_wcs[2000];

    memset (&d, 0, sizeof(udate_t));
    memset (tmp_wcs, 0, sizeof(tmp_wcs));

    tui_draw_popup_date (L"Ввод даты", L"Путник спросил у вас сегодняшнюю дату.\nЧто вы ему скажете?", &d);
    date2wcs (&d, tmp_wcs);

    mvprintw (0, 0, "Получена дата: %ls.", tmp_wcs);
    refresh ();

    getwchar ();
    */

    mkey_t key = 0;
    int selected_item = 2;
    list_data_t elem_data;
    elem_data.cage_num = 6956;
    wcscpy (elem_data.animal.areal, L"Бразилия");
    wcscpy (elem_data.animal.breed, L"Страус");
    wcscpy (elem_data.animal.name, L"Коля");
    wcscpy (elem_data.products.type, L"Семена тыквы");
    elem_data.products.cost = 1.99;
    elem_data.products.weight = 24.56;
    elem_data.date.Y = 2021;
    elem_data.date.M = 2;
    elem_data.date.D = 14;


    list_elem_t * elem =  lists_CreateNewElement (&elem_data);

    WINDOW * info_window = newwin (LINES - 2, COLS - 2, 1, 1);
    PANEL  * info_panel   = new_panel (info_window);

    do
    {
        switch (key)
        {
            case MKEY_ARROW_UP:
                selected_item--;
                break;
            case MKEY_ARROW_DOWN:
                selected_item ++;
                break;
        }

        tui_print_element_info_window (info_window, selected_item, elem);

        update_panels ();
        doupdate ();
    }
    while ((key = getwchar()) != MKEY_ENTER);


    lists_DeleteElement (elem);
    del_panel (info_panel);
    delwin (info_window);

    endwin();
    return 0;
}