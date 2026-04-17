#include "global.h"

int main() {
    // 1. Criar um nó manual para teste
    NoB *raiz = criar_no(1); // Criamos um nó folha

    // 2. Simular um veículo já salvo
    Veiculo v1 = {10, "Ford", "Ka", "2020", "Azul", "Flex", "Manual", 4, 35000.0, 50000, 1};
    long pos = salvar_veiculo_arquivo(v1);

    // 3. Colocar "na mão" na árvore (só para testar a busca)
    raiz->ids[0] = v1.id;
    raiz->offsets[0] = pos;
    raiz->total_ids = 1;

    // 4. Testar a busca
    int id_procurado = 10;
    long offset_achado = buscar_arvore(raiz, id_procurado);

    if (offset_achado != -1) {
        printf("ID encontrado no offset %ld. Agora o Programador A pode ler o arquivo!\n", offset_achado);
    } else {
        printf("ID nao encontrado.\n");
    }

    return 0;
}
