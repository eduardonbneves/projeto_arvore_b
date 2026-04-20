#include "global.h"

int ler_string_seguro(char *dest, int max) {
  if (fgets(dest, max, stdin) == NULL)
    exit(1);
  int len = strlen(dest);
  if (len > 0 && dest[len - 1] == '\n') {
    dest[len - 1] = '\0';
    return 1;
  } else {
    // Se não encontrou o \n na string lida, verifica se ele é o próximo
    // imediato no buffer
    int c = getchar();
    if (c == '\n' || c == EOF) {
      return 1; // Texto coube matematicamente exato no buffer!
    } else {
      // Estourou! Precisa limpar os lixos restantes
      while (c != '\n' && c != EOF) {
        c = getchar();
      }
      return 0;
    }
  }
}

void ler_somente_letras(char *dest, int max) {
  while (1) {
    if (!ler_string_seguro(dest, max)) {
      printf("Entrada invalida! Limite de %d caracteres excedido. Digite "
             "novamente: ",
             max - 1);
      continue;
    }
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
    if (!ler_string_seguro(dest, max)) {
      printf("Entrada invalida! Limite de %d caracteres excedido. Digite "
             "novamente: ",
             max - 1);
      continue;
    }
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
    if (!ler_string_seguro(dest, max)) {
      printf("Entrada invalida! Limite de %d caracteres excedido. Digite "
             "novamente: ",
             max - 1);
      continue;
    }
    if (strlen(dest) >= 4 && strchr(dest, '|') == NULL)
      break;
    printf("Entrada invalida! O ano deve ter no minimo 4 caracteres e nao pode "
           "conter '|': ");
  }
}

int ler_inteiro() {
  char buffer[12]; // Espaço até aprox ~2 Bilhões (limite int) + sinal negativo
  while (1) {
    if (!ler_string_seguro(buffer, sizeof(buffer))) {
      printf("Entrada invalida! Numero muito grande. Digite novamente: ");
      continue;
    }
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
  char buffer[16]; // Margem até 15 digitos textuais
  while (1) {
    if (!ler_string_seguro(buffer, sizeof(buffer))) {
      printf("Entrada invalida! Numero muito grande. Digite novamente: ");
      continue;
    }
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
