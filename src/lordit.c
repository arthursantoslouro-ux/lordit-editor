#include <stdio.h>
#include <ncurses.h>
#include "edit.h"

char linhas[350];
int coluna, ndl;
int linha_inicio = 0;

void abrirarquivo(int argc, char *argv[])
{
    if (argc < 2) {
        printw("Uso: lordit <arquivo>\n");
        return;
    }

    FILE *arquivo = fopen(argv[1], "r");

    if (arquivo == NULL) {
        perror("lordit");
        return;
    }

    editar(arquivo);
    rewind(arquivo);

    getmaxyx(stdscr, ndl, coluna);

    attron(COLOR_PAIR(2));
    mvhline(0, 0, ' ', coluna);
    mvprintw(0, 0, "lordit 1.0                             %s                  ", argv[1]);
    attroff(COLOR_PAIR(2));

    fseek(arquivo, 0, SEEK_SET);

    for (int i = 0; i < linha_inicio; i++) {
        if (fgets(linhas, sizeof(linhas), arquivo) == NULL) {
            break;
        }
    }

    int y = 1;

    while (y < ndl - 1 &&
           fgets(linhas, sizeof(linhas), arquivo) != NULL) {
        mvprintw(y, 0, "%s", linhas);
        y++;
    }

    attron(COLOR_PAIR(3));
    mvhline(ndl - 1, 0, ' ', coluna);
    mvprintw(ndl - 1, 0, "lines:  %zu", total_de_linhas);
    attroff(COLOR_PAIR(3));

    fclose(arquivo);
}

int main(int argc, char *argv[])
{
    initscr();
    keypad(stdscr, TRUE);

    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_BLUE);
    init_pair(3, COLOR_BLACK, COLOR_GREEN);

    abrirarquivo(argc, argv);

    refresh();

    int tecla;

    while ((tecla = getch()) != 'q') {

        if (tecla == KEY_DOWN) {
            if (linha_inicio < (int)total_de_linhas - (ndl - 2)) {
                linha_inicio++;
            }

            clear();
            abrirarquivo(argc, argv);
            refresh();
        }

        if (tecla == KEY_UP) {
            if (linha_inicio > 0) {
                linha_inicio--;
            }

            clear();
            abrirarquivo(argc, argv);
            refresh();
        }

        if (tecla == KEY_RESIZE) {
            clear();
            abrirarquivo(argc, argv);
            refresh();
        }
    }

    endwin();

    return 0;
}
