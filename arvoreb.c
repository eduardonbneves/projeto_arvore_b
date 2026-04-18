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

void dividir_no(NoB *pai, int i, NoB *filho_cheio) {
    NoB *novo = criar_no(filho_cheio->eh_folha);
    int t = (ORDEM - 1) / 2; // Ponto médio
    novo->total_ids = t;

    // Passa a metade superior das chaves para o novo nó
    for (int j = 0; j < t; j++) {
        novo->ids[j] = filho_cheio->ids[j + t + 1];
        novo->offsets[j] = filho_cheio->offsets[j + t + 1];
    }

    if (!filho_cheio->eh_folha) {
        for (int j = 0; j <= t; j++) {
            novo->filhos[j] = filho_cheio->filhos[j + t + 1];
        }
    }

    filho_cheio->total_ids = t;

    // Abre espaço no pai para o novo filho
    for (int j = pai->total_ids; j >= i + 1; j--) {
        pai->filhos[j + 1] = pai->filhos[j];
    }
    pai->filhos[i + 1] = novo;

    // Sobe a chave mediana para o pai
    for (int j = pai->total_ids - 1; j >= i; j--) {
        pai->ids[j + 1] = pai->ids[j];
        pai->offsets[j + 1] = pai->offsets[j];
    }
    pai->ids[i] = filho_cheio->ids[t];
    pai->offsets[i] = filho_cheio->offsets[t];
    pai->total_ids++;
}

void inserir_nao_cheio(NoB *no, int id, long offset) {
    int i = no->total_ids - 1;
    if (no->eh_folha) {
        while (i >= 0 && id < no->ids[i]) {
            no->ids[i + 1] = no->ids[i];
            no->offsets[i + 1] = no->offsets[i];
            i--;
        }
        no->ids[i + 1] = id;
        no->offsets[i + 1] = offset;
        no->total_ids++;
    } else {
        while (i >= 0 && id < no->ids[i]) i--;
        i++;
        if (no->filhos[i]->total_ids == ORDEM - 1) {
            dividir_no(no, i, no->filhos[i]);
            if (id > no->ids[i]) i++;
        }
        inserir_nao_cheio(no->filhos[i], id, offset);
    }
}

void inserir_arvore(NoB **raiz, int id, long offset) {
    if (*raiz == NULL) {
        *raiz = criar_no(1);
        (*raiz)->ids[0] = id;
        (*raiz)->offsets[0] = offset;
        (*raiz)->total_ids = 1;
        return;
    }

    if ((*raiz)->total_ids == ORDEM - 1) {
        NoB *s = criar_no(0);
        s->filhos[0] = *raiz;
        dividir_no(s, 0, *raiz);
        int i = 0;
        if (s->ids[0] < id) i++;
        inserir_nao_cheio(s->filhos[i], id, offset);
        *raiz = s;
    } else {
        inserir_nao_cheio(*raiz, id, offset);
    }
}

// Função que percorre a árvore e grava ID e Offset em formato legível
void salvar_indice_texto_recursivo(NoB *raiz, FILE *arq) {
    if (raiz == NULL) return;

    // Salva os dados de cada ID presente neste nó
    for (int i = 0; i < raiz->total_ids; i++) {
        fprintf(arq, "%0*d|%ld\n", TAM_ID, raiz->ids[i], raiz->offsets[i]);
    }

    // Se não for folha, continua navegando para os filhos
    if (!raiz->eh_folha) {
        for (int i = 0; i <= raiz->total_ids; i++) {
            salvar_indice_texto_recursivo(raiz->filhos[i], arq);
        }
    }
}

// Função principal para abrir o arquivo e iniciar a gravação
void finalizar_indices(NoB *raiz) {
    // Usamos "w" (write) para sobrescrever o índice antigo com o estado atual da RAM
    FILE *arq = fopen(ARQUIVO_INDICE, "w");
    if (arq != NULL) {
        salvar_indice_texto_recursivo(raiz, arq);
        fclose(arq);
        printf("Arquivo '%s' gerado com sucesso!\n", ARQUIVO_INDICE);
    } else {
        printf("Erro ao criar o arquivo de indices.\n");
    }
}
