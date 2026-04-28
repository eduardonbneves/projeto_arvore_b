#include "global.h"

NoB *criar_no(int eh_folha) {
  NoB *novo = (NoB *)malloc(sizeof(NoB));
  novo->total_ids = 0;
  novo->eh_folha = eh_folha;

  // Inicializa os ponteiros de filhos como NULL
  for (int i = 0; i < ORDEM + 1; i++) {
    novo->filhos[i] = NULL;
  }
  return novo;
}

long buscar_arvore(NoB *raiz, int id) {
  if (raiz == NULL)
    return -1;

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

int atualizar_offset_arvore(NoB *raiz, int id, long novo_offset) {
  if (raiz == NULL)
    return 0;
  int i = 0;
  while (i < raiz->total_ids && id > raiz->ids[i])
    i++;
  if (i < raiz->total_ids && id == raiz->ids[i]) {
    raiz->offsets[i] = novo_offset;
    return 1;
  }
  if (raiz->eh_folha)
    return 0;
  return atualizar_offset_arvore(raiz->filhos[i], id, novo_offset);
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
    while (i >= 0 && id < no->ids[i])
      i--;
    i++;
    if (no->filhos[i]->total_ids == ORDEM - 1) {
      dividir_no(no, i, no->filhos[i]);
      if (id > no->ids[i])
        i++;
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
    if (s->ids[0] < id)
      i++;
    inserir_nao_cheio(s->filhos[i], id, offset);
    *raiz = s;
  } else {
    inserir_nao_cheio(*raiz, id, offset);
  }
}

void listar_todos_em_ordem(NoB *raiz) {
  if (raiz == NULL)
    return;

  int i;
  for (i = 0; i < raiz->total_ids; i++) {
    if (!raiz->eh_folha) {
      listar_todos_em_ordem(raiz->filhos[i]);
    }
    Veiculo v = ler_veiculo_arquivo(raiz->offsets[i]);
    if (v.id != -1) {
      printf("| ID: %0*d | Marca: %-15s | Modelo: %-15s | Ano: %-9s | Preco: "
             "R$%-10.2f |\n",
             TAM_ID, v.id, v.marca, v.modelo, v.ano, v.preco);
    }
  }
  if (!raiz->eh_folha) {
    listar_todos_em_ordem(raiz->filhos[i]);
  }
}

// Limpeza profunda da arvore via In-Order/Pos-Ordem para evitar Leak
void liberar_arvore(NoB *raiz) {
  if (raiz == NULL)
    return;
  if (!raiz->eh_folha) {
    for (int i = 0; i <= raiz->total_ids; i++) {
      liberar_arvore(raiz->filhos[i]);
    }
  }
  free(raiz);
}

void remover_de_folha(NoB *no, int idx) {
  for (int i = idx + 1; i < no->total_ids; ++i) {
    no->ids[i - 1] = no->ids[i];
    no->offsets[i - 1] = no->offsets[i];
  }
  no->total_ids--;
}

void fundir(NoB *no, int idx) {
  NoB *filho = no->filhos[idx];
  NoB *irmao = no->filhos[idx + 1];

  filho->ids[filho->total_ids] = no->ids[idx];
  filho->offsets[filho->total_ids] = no->offsets[idx];

  for (int i = 0; i < irmao->total_ids; ++i) {
    filho->ids[i + filho->total_ids + 1] = irmao->ids[i];
    filho->offsets[i + filho->total_ids + 1] = irmao->offsets[i];
  }

  if (!filho->eh_folha) {
    for (int i = 0; i <= irmao->total_ids; ++i) {
      filho->filhos[i + filho->total_ids + 1] = irmao->filhos[i];
    }
  }

  for (int i = idx + 1; i < no->total_ids; ++i) {
    no->ids[i - 1] = no->ids[i];
    no->offsets[i - 1] = no->offsets[i];
  }

  for (int i = idx + 2; i <= no->total_ids; ++i) {
    no->filhos[i - 1] = no->filhos[i];
  }

  filho->total_ids += irmao->total_ids + 1;
  no->total_ids--;

  free(irmao);
}

void pegar_emprestado_anterior(NoB *no, int idx) {
  NoB *filho = no->filhos[idx];
  NoB *irmao = no->filhos[idx - 1];

  for (int i = filho->total_ids - 1; i >= 0; --i) {
    filho->ids[i + 1] = filho->ids[i];
    filho->offsets[i + 1] = filho->offsets[i];
  }

  if (!filho->eh_folha) {
    for (int i = filho->total_ids; i >= 0; --i) {
      filho->filhos[i + 1] = filho->filhos[i];
    }
  }

  filho->ids[0] = no->ids[idx - 1];
  filho->offsets[0] = no->offsets[idx - 1];

  if (!filho->eh_folha) {
    filho->filhos[0] = irmao->filhos[irmao->total_ids];
  }

  no->ids[idx - 1] = irmao->ids[irmao->total_ids - 1];
  no->offsets[idx - 1] = irmao->offsets[irmao->total_ids - 1];

  filho->total_ids += 1;
  irmao->total_ids -= 1;
}

void pegar_emprestado_proximo(NoB *no, int idx) {
  NoB *filho = no->filhos[idx];
  NoB *irmao = no->filhos[idx + 1];

  filho->ids[filho->total_ids] = no->ids[idx];
  filho->offsets[filho->total_ids] = no->offsets[idx];

  if (!filho->eh_folha) {
    filho->filhos[filho->total_ids + 1] = irmao->filhos[0];
  }

  no->ids[idx] = irmao->ids[0];
  no->offsets[idx] = irmao->offsets[0];

  for (int i = 1; i < irmao->total_ids; ++i) {
    irmao->ids[i - 1] = irmao->ids[i];
    irmao->offsets[i - 1] = irmao->offsets[i];
  }

  if (!irmao->eh_folha) {
    for (int i = 1; i <= irmao->total_ids; ++i) {
      irmao->filhos[i - 1] = irmao->filhos[i];
    }
  }

  filho->total_ids += 1;
  irmao->total_ids -= 1;
}

void preencher(NoB *no, int idx) {
  int min_keys = (ORDEM - 1) / 2;
  if (idx != 0 && no->filhos[idx - 1]->total_ids >= min_keys + 1) {
    pegar_emprestado_anterior(no, idx);
  } else if (idx != no->total_ids &&
             no->filhos[idx + 1]->total_ids >= min_keys + 1) {
    pegar_emprestado_proximo(no, idx);
  } else {
    if (idx != no->total_ids) {
      fundir(no, idx);
    } else {
      fundir(no, idx - 1);
    }
  }
}

void remover_de_no(NoB *no, int id);

void remover_de_nao_folha(NoB *no, int idx) {
  int id = no->ids[idx];
  int min_keys = (ORDEM - 1) / 2;

  if (no->filhos[idx]->total_ids >= min_keys + 1) {
    NoB *atual = no->filhos[idx];
    while (!atual->eh_folha)
      atual = atual->filhos[atual->total_ids];

    int pred_id = atual->ids[atual->total_ids - 1];
    long pred_offset = atual->offsets[atual->total_ids - 1];

    no->ids[idx] = pred_id;
    no->offsets[idx] = pred_offset;
    remover_de_no(no->filhos[idx], pred_id);
  } else if (no->filhos[idx + 1]->total_ids >= min_keys + 1) {
    NoB *atual = no->filhos[idx + 1];
    while (!atual->eh_folha)
      atual = atual->filhos[0];

    int suc_id = atual->ids[0];
    long suc_offset = atual->offsets[0];

    no->ids[idx] = suc_id;
    no->offsets[idx] = suc_offset;
    remover_de_no(no->filhos[idx + 1], suc_id);
  } else {
    fundir(no, idx);
    remover_de_no(no->filhos[idx], id);
  }
}

void remover_de_no(NoB *no, int id) {
  int idx = 0;
  while (idx < no->total_ids && no->ids[idx] < id)
    ++idx;

  if (idx < no->total_ids && no->ids[idx] == id) {
    if (no->eh_folha) {
      remover_de_folha(no, idx);
    } else {
      remover_de_nao_folha(no, idx);
    }
  } else {
    if (no->eh_folha) {
      return;
    }

    int flag = ((idx == no->total_ids) ? 1 : 0);
    int min_keys = (ORDEM - 1) / 2;

    if (no->filhos[idx]->total_ids < min_keys + 1) {
      preencher(no, idx);
    }

    if (flag && idx > no->total_ids) {
      remover_de_no(no->filhos[idx - 1], id);
    } else {
      remover_de_no(no->filhos[idx], id);
    }
  }
}

int remover_arvore(NoB **raiz, int id) {
  if (!*raiz) {
    return 0; // Arvore vazia
  }

  long offset = buscar_arvore(*raiz, id);
  if (offset == -1) {
    return 0; // ID nao encontrado
  }

  remover_de_no(*raiz, id);

  if ((*raiz)->total_ids == 0) {
    NoB *tmp = *raiz;
    if ((*raiz)->eh_folha) {
      *raiz = NULL;
    } else {
      *raiz = (*raiz)->filhos[0];
    }
    free(tmp);
  }
  return 1;
}
