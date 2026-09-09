#include <stdio.h>
#include <stddef.h>
#include <ncurses.h>

char buffer[130];
size_t total_de_linhas = 0
;
void editar(FILE *file) {
    if (file == NULL) {
        perror("lordit");
        return;
    }

    total_de_linhas = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        total_de_linhas++;
    }

}
