#include "global.h"

long salvar_veiculo_arquivo(Veiculo v) {
  FILE *arq = fopen(ARQUIVO_DADOS, "a");
  if (arq == NULL)
    return -1;

  // Pega a posi��o atual antes de escrever
  fseek(arq, 0, SEEK_END);
  long offset = ftell(arq);

  // Escreve no formato ASCII (campos fixos e variveis com delimitador)
  // Exemplo: ID|MARCA (20 chars)|MODELO...
  fprintf(arq, "%0*d|%-50s|%s|%s|%s|%s|%s|%d|%.2f|%d|%d\n", TAM_ID, v.id,
          v.marca, v.modelo, v.ano, v.cor, v.combustivel, v.cambio, v.portas,
          v.preco, v.km, v.status);

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
      // Limpa trailing spaces gerados pelo %-20s
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

    token = strsep_custom(&linha_ptr, "|");
    if (token)
      v.status = atoi(token);
  }

  fclose(arq);
  return v;
}
