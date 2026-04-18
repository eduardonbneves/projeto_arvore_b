#include "global.h"

int main() {
    NoB *raiz = NULL;
    int proximo_id = 1;

    // Tentamos abrir o arquivo para descobrir qual o maior ID que já existe
    FILE *arq_check = fopen("veiculos.dat", "r");
    if (arq_check != NULL) {
        char linha[1024];
        int id_lido;

        // Lê a linha inteira para o buffer primeiro
        while (fgets(linha, sizeof(linha), arq_check)) {
            // Tenta extrair o ID que está antes do primeiro '|'
            if (sscanf(linha, "%d|", &id_lido) == 1) {
                if (id_lido >= proximo_id) {
                    proximo_id = id_lido + 1;
                }
            }
        }
        fclose(arq_check);
    }

    // 2. Criar a raiz se ela não existir
    if (raiz == NULL) raiz = criar_no(1);

    // 3. Usar o ID incremental
    int id_atual = proximo_id++; // Usa o valor de proximo_id e depois soma +1

    // 4. Simular o veículo com o ID automático
    Veiculo v1 = {id_atual, "Chevrolet", "Onix", "2020", "Azul", "Flex", "Automatico", 4, 35000.0, 50000, 1};

    // 5. Salvar no arquivo (Programador A)
    long pos = salvar_veiculo_arquivo(v1);

    // 6. Colocar na Árvore-B (Programador B)
    // Aqui, no futuro, você usará a função inserir_arvore completa.
    raiz->ids[0] = v1.id;
    raiz->offsets[0] = pos;
    raiz->total_ids = 1;

    // 7. Testar a busca
    printf("Buscando o veiculo recem-criado (ID %d)...\n", v1.id);
    long offset_achado = buscar_arvore(raiz, v1.id);

    if (offset_achado != -1) {
        printf("Sucesso! ID %d encontrado no offset %ld.\n", v1.id, offset_achado);
    } else {
        printf("Erro: ID nao encontrado na arvore.\n");
    }

    return 0;
}