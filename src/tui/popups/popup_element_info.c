#include "__popups__.h"
#include "../forms/__forms__.h"

int tui_popup_edit_element_data (const wchar_t * title, list_data_t * edata)
{
    int
        flags = 0,
        selected_field = 1;
    wchar_t tmp_wcs[100];
    mkey_t key = MKEY_NULL;
    
    WINDOW * popup_win = newwin (22, 22 + ANIMAL_AREAL_MAX_LEN, 1, 1);
    PANEL  * popup_panel = new_panel (popup_win);
    update_panels ();

    tui_draw_popup_header (popup_win, title);
    
    do
    {
        /* Обработать нажатую клавишу */
        switch (key)
        {
            case MKEY_TAB:
                if (selected_field < 9)
                    selected_field++;
                else
                    selected_field = 1;
                break;
            
            case MKEY_ENTER:
                break;
            
            case L'A': case L'a':
                flags |= TUI_ADD_ELEMENT_FLAG;
                goto POPUP_ADD_ELEM_LOOP_EXIT;
                break;
            
            case L'E': case L'e':
                flags = TUI_ADD_ELEMENT_CACELLED;
                goto POPUP_ADD_ELEM_LOOP_EXIT;
                break;
            
            default:
                break;
        }



        /* Отрисовать список */

        memset (tmp_wcs, 0, sizeof(tmp_wcs)); if (edata->cage_num) int2wcs (edata->cage_num, tmp_wcs);
        if (selected_field == 1) wattron (popup_win, TUI_HIGHLIGHT_ITEM_STYLE);
        mvwprintw (popup_win, 5, 1,  " Номер вольера    "); tui_draw_form_field (popup_win, 5, 21, L"НЕТ НОМ", tmp_wcs, 7);
        if (selected_field == 1) wattroff(popup_win, TUI_HIGHLIGHT_ITEM_STYLE);



        memset (tmp_wcs, 0, sizeof(tmp_wcs)); date2wcs (&edata->date, tmp_wcs);
        if (selected_field == 2) wattron (popup_win, TUI_HIGHLIGHT_ITEM_STYLE);
        mvwprintw (popup_win, 6, 1,  " Дата поступления "); tui_draw_form_field (popup_win, 6, 21, L"НЕТ ДАТЫ", tmp_wcs, 10);
        if (selected_field == 2) wattroff(popup_win, TUI_HIGHLIGHT_ITEM_STYLE);



        mvwprintw (popup_win, 7, 1,  " О животном:      ");



        if (selected_field == 3) wattron (popup_win, TUI_HIGHLIGHT_ITEM_STYLE);
        mvwprintw (popup_win, 8, 1,  " * Ареал обитания "); tui_draw_form_field (popup_win, 8,  21, L"НЕТ АРЕАЛА", edata->animal.areal, ANIMAL_AREAL_MAX_LEN - 1);
        if (selected_field == 3) wattroff(popup_win, TUI_HIGHLIGHT_ITEM_STYLE);


        
        if (selected_field == 4) wattron (popup_win, TUI_HIGHLIGHT_ITEM_STYLE);
        mvwprintw (popup_win, 9, 1,  " * Порода         "); tui_draw_form_field (popup_win, 9,  21, L"НЕТ ПОРОДЫ", edata->animal.breed, ANIMAL_BREED_MAX_LEN - 1);
        if (selected_field == 4) wattroff(popup_win, TUI_HIGHLIGHT_ITEM_STYLE);



        if (selected_field == 5) wattron (popup_win, TUI_HIGHLIGHT_ITEM_STYLE);
        mvwprintw (popup_win, 10, 1, " * Имя            "); tui_draw_form_field (popup_win, 10, 21, L"НЕТ ИМЕНИ",  edata->animal.name,  ANIMAL_NAME_MAX_LEN  - 1);
        if (selected_field == 5) wattroff(popup_win, TUI_HIGHLIGHT_ITEM_STYLE);


        
        mvwprintw (popup_win, 11, 1, " О продуктах:     ");
        
        

        if (selected_field == 6) wattron (popup_win, TUI_HIGHLIGHT_ITEM_STYLE);
        mvwprintw (popup_win, 12, 1, " * Тип            "); tui_draw_form_field (popup_win, 12, 21, L"НЕТ ТИПА",   edata->products.type, PRODUCT_TYPE_MAX_LEN - 1);
        if (selected_field == 5) wattroff(popup_win, TUI_HIGHLIGHT_ITEM_STYLE);


        
        memset (tmp_wcs, 0, sizeof(tmp_wcs)); if (edata->products.weight) float2wcs (edata->products.weight, tmp_wcs);
        if (selected_field == 7) wattron (popup_win, TUI_HIGHLIGHT_ITEM_STYLE);
        mvwprintw (popup_win, 13, 1, " * Вес            "); tui_draw_form_field (popup_win, 13, 21, L"НЕТ ВЕС", tmp_wcs, 7);
        if (selected_field == 5) wattroff(popup_win, TUI_HIGHLIGHT_ITEM_STYLE);

        
        
        memset (tmp_wcs, 0, sizeof(tmp_wcs)); if (edata->products.cost) float2wcs (edata->products.cost, tmp_wcs);
        if (selected_field == 8) wattron (popup_win, TUI_HIGHLIGHT_ITEM_STYLE);
        mvwprintw (popup_win, 14, 1, " * Стоимость      "); tui_draw_form_field (popup_win, 14, 21, L"НЕТ ЦЕН", tmp_wcs, 7);
        if (selected_field == 5) wattroff(popup_win, TUI_HIGHLIGHT_ITEM_STYLE);
        
        

        if (selected_field == 9) wattron (popup_win, TUI_HIGHLIGHT_ITEM_STYLE);
        mvwprintw (popup_win, 16, 1, " Добавить в       ");
        if (selected_field == 5) wattroff(popup_win, TUI_HIGHLIGHT_ITEM_STYLE);

        mvwprintw (popup_win, 18, 1, "(A) Добавить. (E) Отменить добавление.");
    }
    while (1);

    POPUP_ADD_ELEM_LOOP_EXIT:


    del_panel (popup_panel);
    delwin (popup_win);
    return flags;
}

int tui_popup_show_only_element (const wchar_t * title, const list_elem_t * elem)
{

}