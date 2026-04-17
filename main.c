#include "global.h"

int main() {
    // 1. Criar e preencher um veículo de teste
    Veiculo v1;
    v1.id = 1;
    strcpy(v1.marca, "Fiat");
    strcpy(v1.modelo, "Uno");
    strcpy(v1.ano, "2010");
    v1.preco = 15000.00;
    v1.status = 1;

    // 2. Chamar a função e capturar o offset
    long pos = salvar_veiculo_arquivo(v1);

    // 3. Verificar se deu certo
    if (pos != -1) {
        printf("Veiculo salvo com sucesso no offset: %ld\n", pos);
    } else {
        printf("Erro ao salvar no arquivo.\n");
    }

    return 0;
}
