#include <stdio.h>
#include <ncurses.h>
#include <string.h>

char *buffer[1000];
char linhas[350];
int coluna, ndl; // essa variavel e o numero de linhas coloquei esse nome (ndl) pra nao collidir com a variavel linhas


void abrirarquivo(int argc, char *argv[]) {
FILE *arquivo = fopen(argv[1], "r");

if (arquivo == NULL) {
	perror("kmode");
	return;
}
start_color();
init_pair(1, COLOR_WHITE, COLOR_BLACK);
init_pair(2, COLOR_BLACK, COLOR_GREEN);
getmaxyx(stdscr, ndl, coluna);
attron(COLOR_PAIR(2));
for (int i = 0; i < ndl; i++) {
mvprintw(0, i, " ");
}
mvprintw(0, 0, "kmode\t                         %s                                      ", argv[1]);
attroff(COLOR_PAIR(2));
refresh();

int y = 1;


while (fgets(linhas, sizeof(linhas), arquivo)) {
	mvprintw(y, 0, "%s", linhas);
	y++;
}

 	attron(COLOR_PAIR(1));

	refresh();

	fclose(arquivo);
}



int main(int argc, char *argv[]) {
	initscr();
	abrirarquivo(argc, argv);
	getch();
	endwin();
	return 0;
}
