#include "global.h"

// Função para reconstruir a Árvore-B a partir do arquivo de dados ao iniciar
void carregar_arvore_do_arquivo(NoB **raiz, int *proximo_id) {
    FILE *arq = fopen(ARQUIVO_DADOS, "r");
    if (arq == NULL) return;

    char linha[1024];
    Veiculo v_temp;
    long offset_atual;

    while (1) {
        offset_atual = ftell(arq); // Guarda a posição exata antes de ler a linha
        if (!fgets(linha, sizeof(linha), arq)) break;

        if (sscanf(linha, "%d|", &v_temp.id) == 1) {
            // Atualiza o ID incremental
            if (v_temp.id >= *proximo_id) {
                *proximo_id = v_temp.id + 1;
            }
            // Insere na Árvore-B usando a função completa do Programador B
            inserir_arvore(raiz, v_temp.id, offset_atual);
        }
    }
    fclose(arq);
}

int main() {
    NoB *raiz = NULL;
    int proximo_id = 1;

    // 1. CARGA INICIAL: Reconstroi a árvore na RAM e ajusta o ID incremental
    carregar_arvore_do_arquivo(&raiz, &proximo_id);
    printf("Sistema iniciado. Proximo ID disponivel: %0*d\n", TAM_ID, proximo_id);

    // 2. SIMULAÇÃO DE CADASTRO
    int id_atual = proximo_id++;
    Veiculo v1 = {id_atual, "Chevrolet", "Onix", "2020", "Azul", "Flex", "Automatico", 4, 35000.0, 50000, 1};

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
        printf("Busca: ID %0*d encontrado no offset %ld.\n", TAM_ID, busca, offset_achado);
    }

    // 4. PERSISTÊNCIA DO ÍNDICE: Gerar o arquivo indices.idx antes de fechar
    // Usando a função salvar_indice_texto_recursivo
    finalizar_indices(raiz);

    return 0;
}