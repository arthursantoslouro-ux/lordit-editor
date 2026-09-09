#include <stdio.h>
#include <ncurses.h>
#include "edit.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Uso: lordit <arquivo>\n");
        return 1;
    }

    initscr();

    keypad(stdscr, TRUE);
    noecho();
    raw();

    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);
    init_pair(3, COLOR_BLACK, COLOR_GREEN);

    bkgd(COLOR_PAIR(1));

    editar_arquivo(argv[1]);

    endwin();

    return 0;
}
