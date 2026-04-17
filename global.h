#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO_DADOS "veiculos.dat"
#define ARQUIVO_INDICE "indices.idx"

#define MAX_MARCA 20
#define MAX_MODELO 50
#define MAX_ANO 9
#define MAX_COR 15
#define MAX_COMBUSTIVEL 20
#define MAX_CAMBIO 20

typedef struct {
    int id;
    char marca[MAX_MARCA];
    char modelo[MAX_MODELO];
    char ano[MAX_ANO];
    char cor[MAX_COR];
    char combustivel[MAX_COMBUSTIVEL];
    char cambio[MAX_CAMBIO];
    int portas;
    float preco;
    int km;
    int status;                // 1 para ativo, 0 para removido (opcional)
} Veiculo;

#define ORDEM 5

typedef struct No {
    int total_ids;
    int ids[ORDEM];
    long offsets[ORDEM];
    struct No *filhos[ORDEM + 1];
    int eh_folha;
} NoB;


long salvar_veiculo_arquivo(Veiculo v);
Veiculo ler_veiculo_arquivo(long offset);

NoB* criar_no(int eh_folha);
void inserir_arvore(NoB **raiz, int id, long offset);
long buscar_arvore(NoB *raiz, int id);

#endif
