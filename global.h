#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ARQUIVO_DADOS "veiculos.dat"
#define ARQUIVO_INDICE "indices.idx"

#define TAM_ID 5

#define MAX_MARCA 50
#define MAX_MODELO 50
#define MAX_ANO 10
#define MAX_COR 20
#define MAX_COMBUSTIVEL 20
#define MAX_CAMBIO 20

typedef struct {
  int id;
  char marca[MAX_MARCA];
  char modelo[MAX_MODELO];
  char ano[MAX_ANO];
  char cor[MAX_COR];
  char combustivel[MAX_COMBUSTIVEL];
  char cambio[MAX_CAMBIO];
  int portas;
  float preco;
  int km;
} Veiculo;

#define ORDEM 6

typedef struct No {
  int total_ids;
  int ids[ORDEM];
  long offsets[ORDEM];
  struct No *filhos[ORDEM + 1];
  int eh_folha;
} NoB;

NoB *criar_no(int eh_folha);
void inserir_arvore(NoB **raiz, int id, long offset);
long buscar_arvore(NoB *raiz, int id);
int atualizar_offset_arvore(NoB *raiz, int id, long novo_offset);
void finalizar_indices(NoB *raiz);
void listar_todos_em_ordem(NoB *raiz);
void liberar_arvore(NoB *raiz);

Veiculo ler_veiculo_arquivo(long offset);
long salvar_veiculo_arquivo(Veiculo v);
void carregar_arvore_do_indice(NoB **raiz, int *proximo_id);
void compactar_banco_recursivo(NoB *raiz, FILE *novo_arq);
void desfragmentar_dados(NoB **raiz);

int ler_string_seguro(char *dest, int max);
void ler_somente_letras(char *dest, int max);
void ler_alfanumerico(char *dest, int max);
void ler_ano_texto(char *dest, int max);
int ler_inteiro();
float ler_float();

#endif
