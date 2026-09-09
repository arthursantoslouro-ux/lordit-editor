#ifndef EDIT_H
#define EDIT_H

#include <stddef.h>

#define MAX_LINHAS 10000
#define MAX_COLUNAS 350

extern char buffer[MAX_LINHAS][MAX_COLUNAS];
extern size_t total_de_linhas;

void carregar_arquivo(const char *nome);
void salvar_arquivo(const char *nome);
void editar_arquivo(const char *nome);

#endif
