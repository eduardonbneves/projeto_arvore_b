#include "global.h"

long salvar_veiculo_arquivo(Veiculo v) {
  FILE *arq = fopen(ARQUIVO_DADOS, "a");
  if (arq == NULL)
    return -1;

  // Pega a posição atual antes de escrever
  fseek(arq, 0, SEEK_END);
  long offset = ftell(arq);

  // Escreve no formato ASCII (campos fixos e variveis com delimitador)
  // Exemplo: ID|MARCA (20 chars)|MODELO...
  fprintf(arq, "%0*d|%-50s|%s|%s|%s|%s|%s|%d|%.2f|%d\n", TAM_ID, v.id, v.marca,
          v.modelo, v.ano, v.cor, v.combustivel, v.cambio, v.portas, v.preco,
          v.km);

  fclose(arq);
  return offset;
}

// Funcao auxiliar segura para quebrar delimitadores
static char *strsep_custom(char **stringp, const char *delim) {
  char *start = *stringp;
  char *p;
  if (start == NULL)
    return NULL;
  p = strpbrk(start, delim);
  if (p) {
    *p = '\0';
    *stringp = p + 1;
  } else {
    *stringp = NULL;
  }
  return start;
}

Veiculo ler_veiculo_arquivo(long offset) {
  Veiculo v;
  memset(&v, 0, sizeof(Veiculo));
  v.id = -1; // Flag de erro, caso não leia corretamente

  FILE *arq = fopen(ARQUIVO_DADOS, "r");
  if (arq == NULL)
    return v;

  // Pula direto para o offset (posição em bytes)
  fseek(arq, offset, SEEK_SET);

  char linha[1024];
  if (fgets(linha, sizeof(linha), arq) != NULL) {
    linha[strcspn(linha, "\n")] = '\0'; // Remove o \n final

    char *linha_ptr = linha;
    char *token;

    token = strsep_custom(&linha_ptr, "|");
    if (token)
      v.id = atoi(token);

    token = strsep_custom(&linha_ptr, "|");
    if (token) {
      strncpy(v.marca, token, MAX_MARCA - 1);
      // Limpa trailing spaces gerados
      for (int i = strlen(v.marca) - 1; i >= 0; i--) {
        if (v.marca[i] == ' ')
          v.marca[i] = '\0';
        else
          break;
      }
    }

    token = strsep_custom(&linha_ptr, "|");
    if (token)
      strncpy(v.modelo, token, MAX_MODELO - 1);

    token = strsep_custom(&linha_ptr, "|");
    if (token)
      strncpy(v.ano, token, MAX_ANO - 1);

    token = strsep_custom(&linha_ptr, "|");
    if (token)
      strncpy(v.cor, token, MAX_COR - 1);

    token = strsep_custom(&linha_ptr, "|");
    if (token)
      strncpy(v.combustivel, token, MAX_COMBUSTIVEL - 1);

    token = strsep_custom(&linha_ptr, "|");
    if (token)
      strncpy(v.cambio, token, MAX_CAMBIO - 1);

    token = strsep_custom(&linha_ptr, "|");
    if (token)
      v.portas = atoi(token);

    token = strsep_custom(&linha_ptr, "|");
    if (token)
      v.preco = atof(token);

    token = strsep_custom(&linha_ptr, "|");
    if (token)
      v.km = atoi(token);
  }

  fclose(arq);
  return v;
}

// Função que percorre a árvore e grava ID e Offset em formato legível
void salvar_indice_texto_recursivo(NoB *raiz, FILE *arq) {
  if (raiz == NULL)
    return;

  // Salva os dados de cada ID presente neste nó apenas se não for fantasma
  for (int i = 0; i < raiz->total_ids; i++) {
    if (raiz->offsets[i] != -1) {
      fprintf(arq, "%0*d|%ld\n", TAM_ID, raiz->ids[i], raiz->offsets[i]);
    }
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
  // Usamos "w" (write) para sobrescrever o índice antigo com o estado atual da
  // RAM
  FILE *arq = fopen(ARQUIVO_INDICE, "w");
  if (arq != NULL) {
    salvar_indice_texto_recursivo(raiz, arq);
    fclose(arq);
    printf("Arquivo '%s' gerado com sucesso!\n", ARQUIVO_INDICE);
  } else {
    printf("Erro ao criar o arquivo de indices.\n");
  }
}

void carregar_arvore_do_indice(NoB **raiz, int *proximo_id) {
  FILE *arq = fopen(ARQUIVO_INDICE, "r");
  if (arq == NULL) {
    printf("Arquivo de indice nao encontrado. Iniciando arvore vazia.\n");
    return;
  }

  char linha[1024];
  int id_lido;
  long offset_lido;

  while (fgets(linha, sizeof(linha), arq)) {
    if (sscanf(linha, "%d|%ld", &id_lido, &offset_lido) == 2) {
      if (offset_lido != -1) {
        inserir_arvore(raiz, id_lido, offset_lido);
      }
      if (id_lido >= *proximo_id) {
        *proximo_id = id_lido + 1;
      }
    }
  }

  fclose(arq);
  printf("Arvore reconstruida com sucesso a partir de %s!\n", ARQUIVO_INDICE);
}

void compactar_banco_recursivo(NoB *raiz, FILE *novo_arq) {
  if (raiz != NULL) {
    int i;
    for (i = 0; i < raiz->total_ids; i++) {
      compactar_banco_recursivo(raiz->filhos[i], novo_arq);
      if (raiz->offsets[i] != -1) {
        Veiculo v = ler_veiculo_arquivo(raiz->offsets[i]);
        if (v.id != -1) {
          long novo_offset = ftell(novo_arq);
          fprintf(novo_arq, "%0*d|%-50s|%s|%s|%s|%s|%s|%d|%.2f|%d\n", TAM_ID,
                  v.id, v.marca, v.modelo, v.ano, v.cor, v.combustivel,
                  v.cambio, v.portas, v.preco, v.km);
          raiz->offsets[i] = novo_offset;
        }
      }
    }
    compactar_banco_recursivo(raiz->filhos[i], novo_arq);
  }
}

void desfragmentar_dados(NoB **raiz) {
  printf("\nIniciando compactacao e limpeza do banco de dados...\n");
  FILE *novo_arq = fopen("veiculos_tmp.dat", "w");
  if (!novo_arq) {
    printf("Erro ao criar arquivo temporario!\n");
    return;
  }
  compactar_banco_recursivo(*raiz, novo_arq);
  fclose(novo_arq);

  remove(ARQUIVO_DADOS);
  rename("veiculos_tmp.dat", ARQUIVO_DADOS);

  finalizar_indices(*raiz);
  printf("Banco de dados otimizado com sucesso! Registros desatualizados do "
         "veiculos.dat foram fisicamente excluidos.\n");
}
