#include <locale.h>
#include "files.h"
#include "tui.h"

#define FILENAME_MAX_LENGTH 32

const menu_t MAIN_MENU = {
    L"   Начать работу    ",
    L"    О программе     ",
    L" Выйти из программы ",
    NULL
};

const menu_t SAVE_OR_LOAD_MENU = {
    L" Загрузить данные из файла ",
    L"  Сохранить данные в файл  ",
    NULL
};

const menu_t FILE_FORMATS_MENU = {
    L" .bin ",
    L" .csv ",
    NULL
};

const menu_t LIST_TYPES_MENU = {
    L"    Все данные    ",
    L" Результат поиска ",
    NULL
};


/************************************************************************************
 * Перечисление дополнительны функций, которые должны быть здесь просто потому что. *
 ************************************************************************************/

/*
 * Нарисовать задний фон для заданного окна.
 */
void draw_win_background (WINDOW *);