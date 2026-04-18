#include "global.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void ler_string_seguro(char *dest, int max) {
  fgets(dest, max, stdin);
  int len = strlen(dest);
  if (len > 0 && dest[len - 1] == '\n') {
    dest[len - 1] = '\0';
  } else {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
  }
}

void ler_somente_letras(char *dest, int max) {
  while (1) {
    ler_string_seguro(dest, max);
    int valido = 1;
    for (int i = 0; dest[i] != '\0'; i++) {
      if (!isalpha(dest[i]) && dest[i] != ' ') {
        valido = 0;
        break;
      }
    }
    if (valido && strlen(dest) > 0)
      break;
    printf("Entrada invalida! Digite apenas letras/palavras: ");
  }
}

void ler_alfanumerico(char *dest, int max) {
  while (1) {
    ler_string_seguro(dest, max);
    int valido = 1;
    for (int i = 0; dest[i] != '\0'; i++) {
      if (!isalnum(dest[i]) && dest[i] != ' ' && dest[i] != '-') {
        valido = 0;
        break;
      }
    }
    if (valido && strlen(dest) > 0)
      break;
    printf("Entrada invalida! Digite letras ou numeros permitidos: ");
  }
}

void ler_ano_texto(char *dest, int max) {
  while (1) {
    ler_string_seguro(dest, max);
    if (strlen(dest) >= 4)
      break;
    printf("Entrada invalida! O ano deve ter no minimo 4 caracteres: ");
  }
}

int ler_inteiro() {
  char buffer[256];
  while (1) {
    ler_string_seguro(buffer, sizeof(buffer));
    int valido = 1;
    for (int i = 0; buffer[i] != '\0'; i++) {
      if (!isdigit(buffer[i]) && buffer[i] != '-') {
        valido = 0;
        break;
      }
    }
    if (valido && strlen(buffer) > 0)
      return atoi(buffer);
    printf("Entrada invalida! Digite apenas numeros inteiros: ");
  }
}

float ler_float() {
  char buffer[256];
  while (1) {
    ler_string_seguro(buffer, sizeof(buffer));
    int valido = 1;
    int pontos = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
      if (buffer[i] == '.')
        pontos++;
      else if (!isdigit(buffer[i])) {
        valido = 0;
        break;
      }
    }
    if (valido && pontos <= 1 && strlen(buffer) > 0 &&
        strcmp(buffer, ".") != 0) {
      return atof(buffer);
    }
    printf("Entrada invalida! Digite um valor numerico (ex: 35000.50): ");
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
      inserir_arvore(raiz, id_lido, offset_lido);
      if (id_lido >= *proximo_id) {
        *proximo_id = id_lido + 1;
      }
    }
  }

  fclose(arq);
  printf("Arvore reconstruida com sucesso a partir de %s!\n", ARQUIVO_INDICE);
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
             "R$%-9.2f |\n",
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

int main() {
  NoB *raiz = NULL;
  int proximo_id = 1;

  carregar_arvore_do_indice(&raiz, &proximo_id);
  printf("Sistema iniciado. Proximo ID disponivel: %0*d\n", TAM_ID, proximo_id);

  int opcao_menu = -1;
  while (opcao_menu != 0) {
    printf("\n--- MENU DE VEICULOS ---\n");
    printf("1. Inserir Veiculo\n");
    printf("2. Buscar Veiculo\n");
    printf("3. Listar Todos (por ID)\n");
    printf("0. Sair\n");
    printf("========================\n");
    printf("Escolha uma opcao: ");

    opcao_menu = ler_inteiro();

    switch (opcao_menu) {
    case 1: {
      Veiculo v;
      v.id = proximo_id++;
      v.status = 1;

      printf("\n--- Inserir Novo Veiculo (ID %0*d) ---\n", TAM_ID, v.id);

      printf("Marca: ");
      ler_alfanumerico(v.marca, MAX_MARCA);

      printf("Modelo: ");
      ler_alfanumerico(
          v.modelo, MAX_MODELO); // Permite letras e numeros ex: C3, 208, HB20

      printf("Ano: ");
      ler_ano_texto(v.ano, MAX_ANO);

      printf("Cor: ");
      ler_somente_letras(v.cor, MAX_COR);

      printf("Combustivel: ");
      ler_somente_letras(v.combustivel, MAX_COMBUSTIVEL);

      printf("Cambio: ");
      ler_somente_letras(v.cambio, MAX_CAMBIO);

      printf("Portas: ");
      v.portas = ler_inteiro();

      printf("Preco: ");
      v.preco = ler_float();

      printf("Quilometragem: ");
      v.km = ler_inteiro();

      long pos = salvar_veiculo_arquivo(v);

      if (pos != -1) {
        inserir_arvore(&raiz, v.id, pos);
        printf("\n=> Veiculo ID %0*d salvo e indexado com sucesso no offset "
               "%ld!\n",
               TAM_ID, v.id, pos);
        finalizar_indices(raiz);
      } else {
        printf("\n=> Erro ao salvar veiculo no arquivo de dados.\n");
        proximo_id--;
      }
      break;
    }
    case 2: {
      printf("\n--- Buscar Veiculo ---\n");
      printf("Digite o ID do veiculo: ");
      int id_busca = ler_inteiro();

      long offset_achado = buscar_arvore(raiz, id_busca);

      if (offset_achado != -1) {
        printf("\n=> Encontrado no indice! Buscando dados...\n");

        Veiculo v = ler_veiculo_arquivo(offset_achado);
        if (v.id != -1) {
          printf("===================================\n");
          printf(" ID:          %0*d\n", TAM_ID, v.id);
          printf(" Marca:       %s\n", v.marca);
          printf(" Modelo:      %s\n", v.modelo);
          printf(" Ano:         %s\n", v.ano);
          printf(" Cor:         %s\n", v.cor);
          printf(" Combustivel: %s\n", v.combustivel);
          printf(" Cambio:      %s\n", v.cambio);
          printf(" Portas:      %d\n", v.portas);
          printf(" Preco:       R$%.2f\n", v.preco);
          printf(" Km:          %d\n", v.km);
          printf(" Status:      %d\n", v.status);
          printf("===================================\n");
        } else {
          printf("Erro ao decodificar os dados do disco.\n");
        }
      } else {
        printf("\n=> Veiculo ID %0*d nao encontrado.\n", TAM_ID, id_busca);
      }
      break;
    }
    case 3:
      printf("\n--- Listagem de Veiculos Cadastrados (Por ID) ---\n");
      if (raiz == NULL || raiz->total_ids == 0) {
        printf("A arvore esta vazia!\n");
      } else {
        listar_todos_em_ordem(raiz);
      }
      printf("-------------------------------------------------\n");
      break;
    case 0:
      finalizar_indices(raiz);
      liberar_arvore(raiz);
      printf("\nSaindo e memoria da Arvore-B liberada com sucesso.\n");
      break;
    default:
      printf("Opcao invalida. Tente novamente.\n");
    }
  }

  return 0;
}