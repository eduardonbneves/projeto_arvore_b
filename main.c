#include "global.h"

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

// Funcao para listar veiculos em ordem crescente de ID varrendo a Arvore-B
// (In-Order Traversal)
void listar_todos_em_ordem(NoB *raiz) {
  if (raiz == NULL)
    return;

  int i;
  for (i = 0; i < raiz->total_ids; i++) {
    // Primeiro desce para o filho à esquerda do índice atual
    if (!raiz->eh_folha) {
      listar_todos_em_ordem(raiz->filhos[i]);
    }

    // Em seguida, extrai e printa a chave/dado do nó avaliado
    Veiculo v = ler_veiculo_arquivo(raiz->offsets[i]);
    if (v.id != -1) {
      printf("| ID: %0*d | Marca: %-15s | Modelo: %-15s | Ano: %-4s | Preco: "
             "R$%-9.2f |\n",
             TAM_ID, v.id, v.marca, v.modelo, v.ano, v.preco);
    }
  }

  // Por último, desce para o filho mais à direita (que é maior que todos os ids
  // correntes do nó)
  if (!raiz->eh_folha) {
    listar_todos_em_ordem(raiz->filhos[i]);
  }
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

    if (scanf("%d", &opcao_menu) != 1) {
      printf("Entrada invalida. Tente novamente.\n");
      while (getchar() != '\n')
        ;
      continue;
    }

    int c; // Auxiliar para limpeza de buffer

    switch (opcao_menu) {
    case 1: {
      Veiculo v;
      v.id = proximo_id++;
      v.status = 1;

      while ((c = getchar()) != '\n' && c != EOF)
        ; // Pula o newline restante do scanf

      printf("\n--- Inserir Novo Veiculo (ID %0*d) ---\n", TAM_ID, v.id);

      printf("Marca: ");
      fgets(v.marca, MAX_MARCA, stdin);
      v.marca[strcspn(v.marca, "\n")] = 0;

      printf("Modelo: ");
      fgets(v.modelo, MAX_MODELO, stdin);
      v.modelo[strcspn(v.modelo, "\n")] = 0;

      printf("Ano: ");
      fgets(v.ano, MAX_ANO, stdin);
      v.ano[strcspn(v.ano, "\n")] = 0;

      printf("Cor: ");
      fgets(v.cor, MAX_COR, stdin);
      v.cor[strcspn(v.cor, "\n")] = 0;

      printf("Combustivel: ");
      fgets(v.combustivel, MAX_COMBUSTIVEL, stdin);
      v.combustivel[strcspn(v.combustivel, "\n")] = 0;

      printf("Cambio: ");
      fgets(v.cambio, MAX_CAMBIO, stdin);
      v.cambio[strcspn(v.cambio, "\n")] = 0;

      printf("Portas: ");
      scanf("%d", &v.portas);

      printf("Preco: ");
      scanf("%f", &v.preco);

      printf("Quilometragem: ");
      scanf("%d", &v.km);

      long pos = salvar_veiculo_arquivo(v);

      if (pos != -1) {
        inserir_arvore(&raiz, v.id, pos);
        printf("\n=> Veiculo ID %0*d salvo e indexado com sucesso no offset "
               "%ld!\n",
               TAM_ID, v.id, pos);

        // Salva o índice em disco imediatamente para evitar perda por crashes
        // ou Ctrl+C
        finalizar_indices(raiz);
      } else {
        printf("\n=> Erro ao salvar veiculo no arquivo de dados.\n");
        proximo_id--;
      }
      break;
    }
    case 2: {
      int id_busca;
      printf("\n--- Buscar Veiculo ---\n");
      printf("Digite o ID do veiculo: ");
      if (scanf("%d", &id_busca) != 1) {
        printf("Entrada invalida.\n");
        while (getchar() != '\n')
          ;
        break;
      }

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
      break;
    default:
      printf("Opcao invalida. Tente novamente.\n");
    }
  }

  return 0;
}