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

    draw_win_background (stdscr);
    refresh ();
    getwchar ();
    
    /* Завершить выполнение программы */
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