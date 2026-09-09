#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "edit.h"

char buffer[MAX_LINHAS][MAX_COLUNAS];
size_t total_de_linhas = 0;

void carregar_arquivo(const char *nome)
{
    FILE *arquivo = fopen(nome, "r");

    total_de_linhas = 0;

    if (arquivo == NULL) {
        total_de_linhas = 1;
        buffer[0][0] = '\0';
        return;
    }

    while (total_de_linhas < MAX_LINHAS &&
           fgets(buffer[total_de_linhas], MAX_COLUNAS, arquivo) != NULL) {

        buffer[total_de_linhas][strcspn(buffer[total_de_linhas], "\n")] = '\0';

        total_de_linhas++;
    }

    if (total_de_linhas == 0) {
        total_de_linhas = 1;
        buffer[0][0] = '\0';
    }

    fclose(arquivo);
}

void salvar_arquivo(const char *nome)
{
    FILE *arquivo = fopen(nome, "w");

    if (arquivo == NULL) {
        return;
    }

    for (size_t i = 0; i < total_de_linhas; i++) {
        fprintf(arquivo, "%s\n", buffer[i]);
    }

    fclose(arquivo);
}

void inserir_caractere(int linha, int coluna, int tecla)
{
    int tamanho = strlen(buffer[linha]);

    if (tamanho >= MAX_COLUNAS - 1) {
        return;
    }

    for (int i = tamanho; i >= coluna; i--) {
        buffer[linha][i + 1] = buffer[linha][i];
    }

    buffer[linha][coluna] = tecla;
}

void nova_linha(int linha, int coluna)
{
    if (total_de_linhas >= MAX_LINHAS) {
        return;
    }

    for (size_t i = total_de_linhas; i > (size_t)linha + 1; i--) {
        strcpy(buffer[i], buffer[i - 1]);
    }

    strcpy(buffer[linha + 1], buffer[linha] + coluna);
    buffer[linha][coluna] = '\0';

    total_de_linhas++;
}

void apagar_caractere(int linha, int coluna)
{
    int tamanho = strlen(buffer[linha]);

    if (coluna <= 0) {
        if (linha > 0) {
            int tamanho_anterior = strlen(buffer[linha - 1]);

            if (tamanho_anterior + tamanho < MAX_COLUNAS) {
                strcat(buffer[linha - 1], buffer[linha]);

                for (size_t i = linha; i < total_de_linhas - 1; i++) {
                    strcpy(buffer[i], buffer[i + 1]);
                }

                total_de_linhas--;
            }
        }

        return;
    }

    for (int i = coluna - 1; i < tamanho; i++) {
        buffer[linha][i] = buffer[linha][i + 1];
    }
}

void desenhar_tela(const char *nome, int linha_atual, int coluna_atual, int linha_inicio)
{
    int altura, largura;

    getmaxyx(stdscr, altura, largura);

    clear();

    attron(COLOR_PAIR(2));
    mvhline(0, 0, ' ', largura);
    mvprintw(0, 0, "lordit 1.0    %s", nome);
    attroff(COLOR_PAIR(2));

    for (int y = 1; y < altura - 1; y++) {
        int linha = linha_inicio + y - 1;

        if (linha >= 0 && linha < (int)total_de_linhas) {
            mvprintw(y, 0, "%s", buffer[linha]);
        }
    }

    attron(COLOR_PAIR(3));
    mvhline(altura - 1, 0, ' ', largura);
    mvprintw(altura - 1, 0,
             "lines: %zu  |  Ctrl+O: salvar  Ctrl+X: sair",
             total_de_linhas);
    attroff(COLOR_PAIR(3));

    int cursor_y = linha_atual - linha_inicio + 1;

    if (cursor_y >= 1 && cursor_y < altura - 1) {
        move(cursor_y, coluna_atual);
    }

    refresh();
}

void editar_arquivo(const char *nome)
{
    int linha_atual = 0;
    int coluna_atual = 0;
    int linha_inicio = 0;
    int tecla;

    carregar_arquivo(nome);

    curs_set(1);

    while (1) {
        int altura, largura;

        getmaxyx(stdscr, altura, largura);

        desenhar_tela(
            nome,
            linha_atual,
            coluna_atual,
            linha_inicio
        );

        tecla = getch();

        if (tecla == 24) {
            break;
        }

        if (tecla == 15) {
            salvar_arquivo(nome);
            continue;
        }

        if (tecla == KEY_RESIZE) {
            continue;
        }

        if (tecla == KEY_UP) {
            if (linha_atual > 0) {
                linha_atual--;

                if (coluna_atual > (int)strlen(buffer[linha_atual])) {
                    coluna_atual = strlen(buffer[linha_atual]);
                }
            }
        }

        else if (tecla == KEY_DOWN) {
            if (linha_atual < (int)total_de_linhas - 1) {
                linha_atual++;

                if (coluna_atual > (int)strlen(buffer[linha_atual])) {
                    coluna_atual = strlen(buffer[linha_atual]);
                }
            }
        }

        else if (tecla == KEY_LEFT) {
            if (coluna_atual > 0) {
                coluna_atual--;
            }
            else if (linha_atual > 0) {
                linha_atual--;
                coluna_atual = strlen(buffer[linha_atual]);
            }
        }

        else if (tecla == KEY_RIGHT) {
            if (coluna_atual < (int)strlen(buffer[linha_atual])) {
                coluna_atual++;
            }
            else if (linha_atual < (int)total_de_linhas - 1) {
                linha_atual++;
                coluna_atual = 0;
            }
        }

        else if (tecla == KEY_BACKSPACE || tecla == 127 || tecla == 8) {
            apagar_caractere(linha_atual, coluna_atual);

            if (coluna_atual > 0) {
                coluna_atual--;
            }
            else if (linha_atual > 0) {
                linha_atual--;
                coluna_atual = strlen(buffer[linha_atual]);
            }
        }

        else if (tecla == '\n' || tecla == KEY_ENTER) {
            nova_linha(linha_atual, coluna_atual);

            linha_atual++;
            coluna_atual = 0;
        }

        else if (tecla >= 32 && tecla <= 126) {
            inserir_caractere(linha_atual, coluna_atual, tecla);
            coluna_atual++;
        }

        if (linha_atual < linha_inicio) {
            linha_inicio = linha_atual;
        }

        if (linha_atual >= linha_inicio + altura - 2) {
            linha_inicio = linha_atual - altura + 3;
        }

        if (linha_inicio < 0) {
            linha_inicio = 0;
        }

        if (linha_atual >= (int)total_de_linhas) {
            linha_atual = total_de_linhas - 1;
        }

        if (coluna_atual > (int)strlen(buffer[linha_atual])) {
            coluna_atual = strlen(buffer[linha_atual]);
        }

        if (coluna_atual >= largura) {
            coluna_atual = largura - 1;
        }
    }

    curs_set(0);
}
