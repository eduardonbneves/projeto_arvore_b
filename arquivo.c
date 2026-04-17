#include "global.h"

long salvar_veiculo_arquivo(Veiculo v) {
    FILE *arq = fopen(ARQUIVO_DADOS, "a");
    if (arq == NULL) return -1;

    // Pega a posição atual antes de escrever
    fseek(arq, 0, SEEK_END);
    long offset = ftell(arq);

    // Escreve no formato ASCII (campos fixos e variáveis com delimitador)
    // Exemplo: ID|MARCA (20 chars)|MODELO...
    fprintf(arq, "%04d|%-20s|%s|%s|%.2f|%d\n",
            v.id, v.marca, v.modelo, v.ano, v.preco, v.status);

    fclose(arq);
    return offset;
}
