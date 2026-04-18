#include "global.h"

int main() {
  // Apaga os registros antigos para começar do zero com os exemplos
  remove(ARQUIVO_DADOS);
  remove(ARQUIVO_INDICE);

  NoB *raiz = NULL;

  Veiculo frota[10] = {
      {1, "Toyota", "Corolla", "2021", "Prata", "Flex", "Automatico", 4,
       115000.0, 25000},
      {2, "Honda", "Civic", "2020/2021", "Preto", "Gasolina", "Automatico", 4,
       110000.0, 35000},
      {3, "Chevrolet", "Onix", "2022", "Branco", "Flex", "Manual", 4, 75000.0,
       15000},
      {4, "Volkswagen", "Polo", "2019", "Vermelho", "Flex", "Manual", 4,
       65000.0, 55000},
      {5, "Hyundai", "HB20", "2023", "Prata", "Flex", "Automatico", 4, 85000.0,
       5000},
      {6, "Fiat", "Argo", "2020", "Azul", "Flex", "Manual", 4, 60000.0, 40000},
      {7, "Jeep", "Renegade", "2022", "Preto", "Diesel", "Automatico", 4,
       140000.0, 20000},
      {8, "Renault", "Kwid", "2023", "Laranja", "Flex", "Manual", 4, 50000.0,
       10000},
      {9, "Nissan", "Kicks", "2021", "Cinza", "Flex", "Automatico", 4, 98000.0,
       30000},
      {10, "Ford", "Mustang", "2018", "Amarelo", "Gasolina", "Automatico", 2,
       350000.0, 18000}};

  printf("Gerando banco de dados falso...\n");
  for (int i = 0; i < 10; i++) {
    long pos = salvar_veiculo_arquivo(frota[i]);
    if (pos != -1) {
      inserir_arvore(&raiz, frota[i].id, pos);
      printf("-> Veiculo %s %s inserido.\n", frota[i].marca, frota[i].modelo);
    }
  }

  finalizar_indices(raiz);
  printf("Pronto! 10 veiculos gerados e salvos no disco.\n");
  return 0;
}
