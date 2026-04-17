#include "global.h"

NoB* criar_no(int eh_folha) {
    NoB *novo = (NoB*)malloc(sizeof(NoB));
    novo->total_ids = 0;
    novo->eh_folha = eh_folha;

    // Inicializa os ponteiros de filhos como NULL
    for (int i = 0; i < ORDEM + 1; i++) {
        novo->filhos[i] = NULL;
    }
    return novo;
}

long buscar_arvore(NoB *raiz, int id) {
    if (raiz == NULL) return -1;

    int i = 0;
    // Percorre as chaves do nó atual enquanto o ID buscado for maior
    while (i < raiz->total_ids && id > raiz->ids[i]) {
        i++;
    }

    // Se encontramos o ID neste nó
    if (i < raiz->total_ids && id == raiz->ids[i]) {
        return raiz->offsets[i];
    }

    // Se chegamos numa folha e não achamos, o ID não existe
    if (raiz->eh_folha) {
        return -1;
    }

    // Se não achamos aqui e não é folha, desce para o filho correspondente
    return buscar_arvore(raiz->filhos[i], id);
}
