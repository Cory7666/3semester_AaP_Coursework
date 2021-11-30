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
                    mvwprintw (win_main, getmaxy(win_main) - 6,  1, "  (A) Добавить данные.                    ");
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
                            case L'A': case L'a':
                                {
                                    /* Создать дополнительные окна */
                                    WINDOW * win_add_element = newwin (LINES - 2, COLS - 4, 1, 2);
                                    PANEL  * panel_add_element = new_panel (win_add_element);
                                    update_panels ();

                                    


                                    /* Удалить дополнительные окна */
                                    del_panel (panel_add_element);
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
                                        if (!is_exist_wcs (tmp_wcs))
                                        {
                                            tui_draw_popup_text_message (
                                                L"ОШИБКА!",
                                                L"Невозможно получить доступ к файлу.\nВозможно файл не существует или его нельзя прочитать.\nПовторите ввод позже..."
                                            );
                                            break;
                                        }

                                        /* Всё ок. Очистить данные в старых списках и загрузить новые данные */
                                        switch (selected_key)
                                        {
                                            case 1:
                                                read_from_bin (main_list, tmp_wcs);
                                                break;
                                            
                                            case 2:
                                                read_from_csv (main_list, tmp_wcs);
                                                break;
                                        }
                                        lists_CleanListObject (&search_results_list);

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
                        }

                        tui_draw_table_in_window (win_table, *main_list, 1, 1);

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