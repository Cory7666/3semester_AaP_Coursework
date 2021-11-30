#include "main.h"

int main ()
{
    /* Включить нужную локализацию */
    setlocale(LC_ALL, "");

    /* Задать начальные настройки ncurses */
    initscr();
    raw();
    noecho();
    keypad(stdscr, 1);

    /* Начало выполнения основной программы */
    const int LAST_CURS_STATE = curs_set (0);
    wchar_t tmp_wcs[1025];                                    // Переменная для хранения временных строк
    mkey_t selected_key = 0;
    list_obj_t * main_list = lists_CreateNewListObject (),    // Список, содержащий все данные
               * save_list = NULL,                            // Список, который нужно сохранить в файл
               * search_results_list = lists_CreateNewListObject ();  // Список с результатами поиска
    int curr_selected_page = 1,
        curr_selected_table_row = 1;

    draw_win_background (stdscr);
    refresh ();
    
    while (
        (selected_key = tui_draw_popup_select (L"Программа для курсового проекта", L"Добро пожаловать в программу, созданную специально для курсового проекта.\nВыберите действие.", MAIN_MENU))
        != 3
    )
    {
        

        switch (selected_key)
        {
            // Выбрано "Работа с программой"
            case 1:
                {
                    /* Создать окна и панели для главного окна */

                    // Само окно
                    WINDOW * win_main = newwin (LINES - 2, COLS - 4, 1, 2);
                    PANEL  * panel_main = new_panel (win_main);
                    
                    update_panels ();

                    // Таблица
                    WINDOW * win_table = derwin (win_main, getmaxy(win_main) - (3 + 1 + 1 + 6 + 3 + 1), getmaxx (win_main) - 4, 4, 2);
                    PANEL  * panel_table = new_panel (win_table);

                    /* Отрисовать всё и вся */
                    tui_draw_popup_header (win_main, L"Таблица данных");

                    /* Отрисовать подсказки ввода */
                    mvwprintw (win_main, getmaxy(win_main) - 11, 1, "Действия над таблицей:                    ");
                    mvwprintw (win_main, getmaxy(win_main) - 10, 1, "  (← →) Выбор страницы данных.            ");
                    mvwprintw (win_main, getmaxy(win_main) - 9,  1, "  (↑ ↓) Выбор строки в таблице.           ");
                    mvwprintw (win_main, getmaxy(win_main) - 8,  1, "  (ENTER) Подтвердить выбор строки.       ");
                    mvwprintw (win_main, getmaxy(win_main) - 7,  1, "Действия над данными:                     ");
                    mvwprintw (win_main, getmaxy(win_main) - 6,  1, "  (P) Добавить данные.                    ");
                    mvwprintw (win_main, getmaxy(win_main) - 5,  1, "  (W) Сохранить в файл/Загрузить из файла.");
                    mvwprintw (win_main, getmaxy(win_main) - 4,  1, "  (F) Поиск.                              ");
                    mvwprintw (win_main, getmaxy(win_main) - 3,  1, "  (T) Задание по варианту.                ");
                    mvwprintw (win_main, getmaxy(win_main) - 2,  1, "  (E) Выйти из текущего окна.             ");

                    selected_key = MKEY_NULL;
                    do
                    {
                        switch (selected_key)
                        {
                            // Действие "Добавить элемент" 
                            case L'P': case L'p':
                                {
                                    /* Создать дополнительные окна */
                                    WINDOW * win_add_element = newwin (LINES - 2, COLS - 4, 1, 2);
                                    PANEL  * panel_add_element = new_panel (win_add_element);
                                    update_panels ();

                                    WINDOW * win_elem_info = derwin (win_add_element, getmaxy(win_add_element) - 3 - 3, getmaxx(win_add_element) - 2, 3, 1);
                                    PANEL  * panel_elem_info = new_panel (win_elem_info);
                                    update_panels ();

                                    list_data_t d;
                                    memset (&d, 0, sizeof (d));
                                    d.date.Y = 2021;
                                    d.date.M = 9;
                                    d.date.D = 1;

                                    list_elem_t * elem = lists_CreateNewElement (&d);
                                    mkey_t selected_field = 2;
                                    /* Помощь при ВВОДе */

                                    mvwprintw (win_add_element, getmaxy(win_add_element) - 2, 1, "(P) Сохранить. (E) Выйти.");

                                    selected_key = MKEY_NULL;

                                    do
                                    {
                                        switch (selected_key)
                                        {
                                            case MKEY_TAB:
                                                if (selected_field < 9) selected_field++;
                                                else selected_field = 1;
                                                break;
                                            
                                            case MKEY_ENTER: case MKEY_ARROW_RIGHT:
                                                switch (selected_field)
                                                {
                                                    case 1:
                                                        tui_draw_popup_text_message (L"Операция непозволена", L"Вы не можете установить ID. Это не разрешено.");
                                                        break;
                                                    case 2:
                                                        memset (tmp_wcs, 0, sizeof(tmp_wcs));    // Обнулить tmp_wcs
                                                        int2wcs (elem->data.cage_num, tmp_wcs);  // Записать в неё .cage_num
                                                        tui_draw_popup_form (L"Ввод номера вольера", L"Введите номер вольера.", L"#", tmp_wcs, 7, VMASK_DIGITS);
                                                        elem->data.cage_num = wcs2int (tmp_wcs); // Записать в .cage_num
                                                        break;
                                                    case 3:
                                                        memset (tmp_wcs, 0, sizeof(tmp_wcs));    // Обнулить tmp_wcs
                                                        wcscpy (tmp_wcs, elem->data.animal.areal);
                                                        tui_draw_popup_form (L"Ввод ариала", L"Введите ариал обитания животного.", L"НЕТ ЗНАЧЕНИЯ", tmp_wcs, ANIMAL_AREAL_MAX_LEN, VMASK_ANY_CHAR);
                                                        wcscpy (elem->data.animal.areal, tmp_wcs);
                                                        break;
                                                    case 4:
                                                        memset (tmp_wcs, 0, sizeof(tmp_wcs));    // Обнулить tmp_wcs
                                                        wcscpy (tmp_wcs, elem->data.animal.breed);
                                                        tui_draw_popup_form (L"Ввод породы", L"Введите название породы животного.", L"НЕТ ЗНАЧЕНИЯ", tmp_wcs, ANIMAL_BREED_MAX_LEN, VMASK_ANY_CHAR);
                                                        wcscpy (elem->data.animal.breed, tmp_wcs);
                                                        break;
                                                    case 5:
                                                        memset (tmp_wcs, 0, sizeof(tmp_wcs));    // Обнулить tmp_wcs
                                                        wcscpy (tmp_wcs, elem->data.animal.name);
                                                        tui_draw_popup_form (L"Ввод имени", L"Введите имя животного.", L"НЕТ ЗНАЧЕНИЯ", tmp_wcs, ANIMAL_NAME_MAX_LEN, VMASK_ANY_CHAR);
                                                        wcscpy (elem->data.animal.name, tmp_wcs);
                                                        break;
                                                    case 6:
                                                        memset (tmp_wcs, 0, sizeof(tmp_wcs));    // Обнулить tmp_wcs
                                                        wcscpy (tmp_wcs, elem->data.products.type);
                                                        tui_draw_popup_form (L"Ввод типа продукта", L"Введите тип продуктов.", L"НЕТ ЗНАЧЕНИЯ", tmp_wcs, PRODUCT_TYPE_MAX_LEN, VMASK_ANY_CHAR);
                                                        wcscpy (elem->data.products.type, tmp_wcs);
                                                        break;
                                                    case 7:
                                                        memset (tmp_wcs, 0, sizeof(tmp_wcs));    // Обнулить tmp_wcs
                                                        float2wcs (elem->data.products.weight, tmp_wcs);  // Записать в неё .weight
                                                        tui_draw_popup_form (L"Ввод веса", L"Введите вес продуктов.", L"Вес", tmp_wcs, 8, VMASK_DIGITS);
                                                        elem->data.products.weight = wcs2float (tmp_wcs); // Записать в .weight
                                                        break;
                                                    case 8:
                                                        memset (tmp_wcs, 0, sizeof(tmp_wcs));    // Обнулить tmp_wcs
                                                        float2wcs (elem->data.products.cost, tmp_wcs);  // Записать в неё .cost
                                                        tui_draw_popup_form (L"Ввод стоимости", L"Введите стоимость продуктов.", L"Цена", tmp_wcs, 8, VMASK_DIGITS);
                                                        elem->data.products.cost = wcs2float (tmp_wcs); // Записать в .cost
                                                        break;
                                                    case 9:
                                                        memset (tmp_wcs, 0, sizeof(tmp_wcs));    // Обнулить tmp_wcs
                                                        tui_draw_popup_date (L"Ввод даты", L"Введите дату поступления животного.", &elem->data.date);
                                                        break;
                                                }
                                                break;
                                            
                                            case L'P': case L'p':
                                                lists_InsertAsListsTail (main_list, elem);
                                                elem = lists_CreateNewElement (&d);
                                                break;
                                        }

                                        tui_print_element_info_window (win_elem_info, selected_field, elem);

                                        update_panels ();
                                        doupdate ();
                                    }
                                    while ((selected_key = getwchar ()) != L'E' && selected_key != L'e');


                                    /* Удалить дополнительные окна */
                                    del_panel (panel_elem_info);
                                    del_panel (panel_add_element);
                                    delwin (win_elem_info);
                                    delwin (win_add_element);
                                }
                                break;
                            
                            // Действие "Работа с файлами"
                            case L'W': case L'w':
                                // Выбор действия на файлами
                                selected_key = tui_draw_popup_select (
                                                    L"Сохранить/Загрузить",
                                                    L"Вы хотите ЗАГРУЗИТЬ данные из файла или СОХРАНИТЬ данные в файл?",
                                                    SAVE_OR_LOAD_MENU
                                                );

                                switch (selected_key)
                                {
                                    // Загрузить данные
                                    case 1:
                                        /* Выбрать тип загружаемого файла */
                                        selected_key = tui_draw_popup_select (
                                                    L"Загрузить данные",
                                                    L"Выберите тип файла, в котором хранятся данные:\n* .bin -> Специальный формат для хранения данных в бинарном формате. Только эта и никакая другая программа может расшифровать сохранённые данные.\n*  .csv -> Текстовый формат хранения данных. Таким образом вы можете внести данные как из Текстового редактора, так и импортировать из других программ, например, MS Excel.",
                                                    FILE_FORMATS_MENU
                                                );
                                        
                                        /* Обнулить значение tmp_wcs и Запросить имя загружаемого файла файла */
                                        memset (tmp_wcs, 0, sizeof(tmp_wcs));
                                        tui_draw_popup_form (
                                            L"Имя файла",
                                            L"Осталось ввести имя файла.\nПримечание: обычно все файлы сохранений хранятся в папке \"data\".",
                                            L" Введите имя файла",
                                            tmp_wcs,
                                            FILENAME_MAX_LENGTH,
                                            VMASK_ANY_CHAR | VMASK_DIGITS
                                        );

                                        /* Проверить на существование файл */
                                        if (is_exist_wcs (tmp_wcs))
                                        {
                                            tui_draw_popup_text_message (
                                                L"ОШИБКА!",
                                                L"Невозможно получить доступ к файлу.\nВозможно файл не существует или его нельзя прочитать.\nПовторите ввод позже..."
                                            );
                                            break;
                                        }

                                        /* Всё ок. Очистить данные в старых списках и загрузить новые данные */
                                        lists_CleanListObject (main_list);
                                        lists_CleanListObject (search_results_list);
                                        switch (selected_key)
                                        {
                                            case 1:
                                                read_from_bin (main_list, tmp_wcs);
                                                break;
                                            
                                            case 2:
                                                read_from_csv (main_list, tmp_wcs);
                                                break;
                                        }

                                        tui_draw_popup_text_message (
                                            L"УСПЕХ!",
                                            L"Все данные были считаны."
                                        );                                        

                                        break;
                                    
                                    // Сохранить данные
                                    case 2:
                                        /* Какие данные нужно сохранить */
                                        selected_key = tui_draw_popup_select (
                                                            L"Сохранить ...",
                                                            L"Выберите список, который хотите сохранить.",
                                                            LIST_TYPES_MENU
                                                        );
                                        /* Выбрать список для сохранения и проверить список на наличие данных */
                                        save_list = (selected_key == 1) ? (main_list) : (search_results_list);
                                        if (save_list->_length_ < 1)
                                        {
                                            tui_draw_popup_text_message (
                                                L"ОШИБКА!",
                                                L"Похоже, что данные отсутствуют, а значит нечего сохранять.\nДля начала добавьте какие-то данные."
                                            );
                                            break;
                                        }
                                        
                                        /*Всё ок. Выбрать тип сохраняемого файла */
                                        selected_key = tui_draw_popup_select (
                                                    (save_list == main_list) ? L"Сохранить все данные как ..." : L"Сохранить результаты поиска как ...",
                                                    L"Выберите тип файла для сохранения:\n* .bin -> Специальный формат для хранения данных в бинарном формате. Злоумышленнику не сможет скомпрометировать данные.\n*  .csv -> Текстовый формат хранения данных. Возможность как редактирования Текстовыми редакторами, так и экспорта в другие программы, например, MS Excel.",
                                                    FILE_FORMATS_MENU
                                                );
                                        
                                        /* Обнулить значение tmp_wcs и Запросить имя файла для сохранения */
                                        memset (tmp_wcs, 0, sizeof(tmp_wcs));
                                        tui_draw_popup_form (
                                            L"Имя файла",
                                            L"Осталось ввести имя файла.\nПримечание: по умолчанию все файлы сохранений хранятся в папке \"data\".",
                                            L" Введите имя файла",
                                            tmp_wcs,
                                            FILENAME_MAX_LENGTH,
                                            VMASK_ANY_CHAR | VMASK_DIGITS
                                        );
                                        
                                        switch (selected_key)
                                        {
                                            // Сохранить как бинарный файл
                                            case 1:
                                                save_as_bin (save_list, tmp_wcs);
                                                break;
                                            // Сохранить как текстовый файл
                                            case 2:
                                                save_as_csv (save_list, tmp_wcs);
                                                break;
                                        }

                                        tui_draw_popup_text_message (
                                            L"УСПЕХ!",
                                            L"Все данные были сохранены."
                                        );  

                                        break;
                                }

                                break;

                            // Действие "Поиск"
                            case L'F': case L'f':
                                break;
                            
                            // Действие "Задание по варианту"
                            case L'T': case L't':
                                break;
                            
                            // Изменение страницы
                            case MKEY_ARROW_DOWN:
                                if (curr_selected_table_row < getmaxy(win_table) - 4)
                                    curr_selected_table_row++;
                                break;
                            case MKEY_ARROW_UP:
                                if (curr_selected_table_row > 1)
                                    curr_selected_table_row--;
                                break;
                            
                            // Изменение строки таблицы
                            case MKEY_ARROW_LEFT:
                                if (curr_selected_page > 1)
                                    curr_selected_page--;
                                break;
                            case MKEY_ARROW_RIGHT:
                                if (curr_selected_page < main_list->_length_ / (getmaxy(win_table) - 4) + !!(main_list->_length_ % (getmaxy(win_table) - 4)))
                                    curr_selected_page++;
                                break;
                            
                            case MKEY_ENTER:
                                {
                                    /* Создать дополнительные окна */
                                    WINDOW * win_add_element = newwin (LINES - 2, COLS - 4, 1, 2);
                                    PANEL  * panel_add_element = new_panel (win_add_element);
                                    update_panels ();

                                    WINDOW * win_elem_info = derwin (win_add_element, getmaxy(win_add_element) - 3 - 3, getmaxx(win_add_element) - 2, 3, 1);
                                    PANEL  * panel_elem_info = new_panel (win_elem_info);
                                    update_panels ();

                                    length_t position = curr_selected_page * (getmaxy(win_table) - 4) + curr_selected_table_row - 1;
                                    list_elem_t * elem = lists_SearchElementByField (main_list, LIST_POSITION, (void *) &position);
                                    mkey_t selected_field = 2;
                                    /* Помощь при ВВОДе */

                                    mvwprintw (win_add_element, getmaxy(win_add_element) - 2, 1, "(P) Удалить. (I) Редактирование (ВКЛ ). (E) Выйти.");

                                    selected_key = MKEY_NULL;

                                    do
                                    {
                                        switch (selected_key)
                                        {
                                            case MKEY_TAB:
                                                if (selected_field < 9) selected_field++;
                                                else selected_field = 1;
                                                break;
                                            
                                            case MKEY_ENTER: case MKEY_ARROW_RIGHT:
                                                switch (selected_field)
                                                {
                                                    case 1:
                                                        tui_draw_popup_text_message (L"Операция непозволена", L"Вы не можете установить ID. Это не разрешено.");
                                                        break;
                                                    case 2:
                                                        memset (tmp_wcs, 0, sizeof(tmp_wcs));    // Обнулить tmp_wcs
                                                        int2wcs (elem->data.cage_num, tmp_wcs);  // Записать в неё .cage_num
                                                        tui_draw_popup_form (L"Ввод номера вольера", L"Введите номер вольера.", L"#", tmp_wcs, 7, VMASK_DIGITS);
                                                        elem->data.cage_num = wcs2int (tmp_wcs); // Записать в .cage_num
                                                        break;
                                                    case 3:
                                                        memset (tmp_wcs, 0, sizeof(tmp_wcs));    // Обнулить tmp_wcs
                                                        wcscpy (tmp_wcs, elem->data.animal.areal);
                                                        tui_draw_popup_form (L"Ввод ариала", L"Введите ариал обитания животного.", L"НЕТ ЗНАЧЕНИЯ", tmp_wcs, ANIMAL_AREAL_MAX_LEN, VMASK_ANY_CHAR);
                                                        wcscpy (elem->data.animal.areal, tmp_wcs);
                                                        break;
                                                    case 4:
                                                        memset (tmp_wcs, 0, sizeof(tmp_wcs));    // Обнулить tmp_wcs
                                                        wcscpy (tmp_wcs, elem->data.animal.breed);
                                                        tui_draw_popup_form (L"Ввод породы", L"Введите название породы животного.", L"НЕТ ЗНАЧЕНИЯ", tmp_wcs, ANIMAL_BREED_MAX_LEN, VMASK_ANY_CHAR);
                                                        wcscpy (elem->data.animal.breed, tmp_wcs);
                                                        break;
                                                    case 5:
                                                        memset (tmp_wcs, 0, sizeof(tmp_wcs));    // Обнулить tmp_wcs
                                                        wcscpy (tmp_wcs, elem->data.animal.name);
                                                        tui_draw_popup_form (L"Ввод имени", L"Введите имя животного.", L"НЕТ ЗНАЧЕНИЯ", tmp_wcs, ANIMAL_NAME_MAX_LEN, VMASK_ANY_CHAR);
                                                        wcscpy (elem->data.animal.name, tmp_wcs);
                                                        break;
                                                    case 6:
                                                        memset (tmp_wcs, 0, sizeof(tmp_wcs));    // Обнулить tmp_wcs
                                                        wcscpy (tmp_wcs, elem->data.products.type);
                                                        tui_draw_popup_form (L"Ввод типа продукта", L"Введите тип продуктов.", L"НЕТ ЗНАЧЕНИЯ", tmp_wcs, PRODUCT_TYPE_MAX_LEN, VMASK_ANY_CHAR);
                                                        wcscpy (elem->data.products.type, tmp_wcs);
                                                        break;
                                                    case 7:
                                                        memset (tmp_wcs, 0, sizeof(tmp_wcs));    // Обнулить tmp_wcs
                                                        float2wcs (elem->data.products.weight, tmp_wcs);  // Записать в неё .weight
                                                        tui_draw_popup_form (L"Ввод веса", L"Введите вес продуктов.", L"Вес", tmp_wcs, 8, VMASK_DIGITS);
                                                        elem->data.products.weight = wcs2float (tmp_wcs); // Записать в .weight
                                                        break;
                                                    case 8:
                                                        memset (tmp_wcs, 0, sizeof(tmp_wcs));    // Обнулить tmp_wcs
                                                        float2wcs (elem->data.products.cost, tmp_wcs);  // Записать в неё .cost
                                                        tui_draw_popup_form (L"Ввод стоимости", L"Введите стоимость продуктов.", L"Цена", tmp_wcs, 8, VMASK_DIGITS);
                                                        elem->data.products.cost = wcs2float (tmp_wcs); // Записать в .cost
                                                        break;
                                                    case 9:
                                                        memset (tmp_wcs, 0, sizeof(tmp_wcs));    // Обнулить tmp_wcs
                                                        tui_draw_popup_date (L"Ввод даты", L"Введите дату поступления животного.", &elem->data.date);
                                                        break;
                                                }
                                                break;
                                            
                                            case L'P': case L'p':
                                                if (elem == main_list->head)
                                                    main_list->head = elem->next;
                                                lists_DeleteElement (elem);
                                                main_list->_length_--;
                                                break;
                                        }

                                        tui_print_element_info_window (win_elem_info, selected_field, elem);

                                        update_panels ();
                                        doupdate ();
                                    }
                                    while (elem != NULL && (selected_key = getwchar ()) != L'E' && selected_key != L'e');


                                    /* Удалить дополнительные окна */
                                    del_panel (panel_elem_info);
                                    del_panel (panel_add_element);
                                    delwin (win_elem_info);
                                    delwin (win_add_element);
                                }
                                break;
                        }

                        tui_draw_table_in_window (win_table, *main_list, curr_selected_page, curr_selected_table_row);

                        update_panels ();
                        doupdate ();
                    }
                    while ((selected_key = getwchar()) != L'E' && selected_key != L'e');
                    
                    /* Удалить панели и окна */
                    del_panel (panel_table);
                    del_panel (panel_main);
                    delwin (win_table);
                    delwin (win_main);
                }
                break;
            
            // Выбрано "О программе"
            case 2:
                tui_draw_popup_text_message (
                    L"О программе",
                    L"Программа была написана для курсового проекта для предмета \"АиП\" (3 семестр).\n* Создатель: Филозоп Алексей.\n* Исходный код: github.com/Cory7666/3semester_AaP_Coursework_new.git"
                );
                break;
        }
    }

    /* Завершить выполнение программы */
    lists_DeleteListObject (&main_list);

    curs_set (LAST_CURS_STATE);
    endwin();
    return 0;
}

/********************************
 * Блок дополнительных функций. *
 ********************************/

void draw_win_background (WINDOW * win)
{
    wmove (win, 0, 0);
    for (size_t i = 0; i < (int) (getmaxx(win) * getmaxy(win) / 6); i++)
    {
        wprintw (win, "*@|@*-");
    }
}