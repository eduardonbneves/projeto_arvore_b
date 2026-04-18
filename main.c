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

  // Lemos a linha inteira primeiro
  while (fgets(linha, sizeof(linha), arq)) {
    // O sscanf extrai os dados da string 'linha'
    // Usamos %d porque o sscanf ignora os zeros a esquerda do ID
    // automaticamente
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

int main() {
  NoB *raiz = NULL;
  int proximo_id = 1;

  // 1. CARGA INICIAL: Reconstroi a árvore a partir do indice e ajusta o ID
  // incremental
  carregar_arvore_do_indice(&raiz, &proximo_id);
  printf("Sistema iniciado. Proximo ID disponivel: %0*d\n", TAM_ID, proximo_id);

  // 2. SIMULAÇÃO DE CADASTRO
  int id_atual = proximo_id++;
  Veiculo v1 = {id_atual,     "Chevrolet", "Onix",  "2020", "Azul", "Flex",
                "Automatico", 4,           35000.0, 50000,  1};

  // Passo A: Salva no arquivo de dados e obtém o offset
  long pos = salvar_veiculo_arquivo(v1);

  if (pos != -1) {
    // Passo B: Insere na Árvore-B (Função com lógica de Split)
    inserir_arvore(&raiz, v1.id, pos);
    printf("Veiculo ID %0*d salvo e indexado com sucesso!\n", TAM_ID, v1.id);
  }

  // 3. TESTE DE BUSCA
  int busca = id_atual;
  long offset_achado = buscar_arvore(raiz, busca);

  if (offset_achado != -1) {
    printf("Busca: ID %0*d encontrado no offset %ld.\n", TAM_ID, busca,
           offset_achado);
  }

  // 4. PERSISTÊNCIA DO ÍNDICE: Gerar o arquivo indices.idx antes de fechar
  // Usando a função salvar_indice_texto_recursivo
  finalizar_indices(raiz);

  return 0;
}