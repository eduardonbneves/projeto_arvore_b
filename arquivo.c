#include "global.h"

long salvar_veiculo_arquivo(Veiculo v) {
    FILE *arq = fopen(ARQUIVO_DADOS, "a");
    if (arq == NULL) return -1;

    // Pega a posi��o atual antes de escrever
    fseek(arq, 0, SEEK_END);
    long offset = ftell(arq);

    // Escreve no formato ASCII (campos fixos e vari�veis com delimitador)
    // Exemplo: ID|MARCA (20 chars)|MODELO...
    fprintf(arq, "%0*d|%-20s|%s|%s|%.2f|%d\n",
            TAM_ID, v.id, v.marca, v.modelo, v.ano, v.preco, v.status);

    fclose(arq);
    return offset;
}
