#ifndef BASKET_H_INCLUDED
#define BASKET_H_INCLUDED

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define BUFFER_SIZE 6
#define MAX_NAME_LENGTH 50
#define MAX_INT_VALUE 9999999

#pragma pack(1)
typedef struct Jogador{
  int camisa;
  char nome[50];
  int idade;
  double salario;
  double altura;
} TJog;
#pragma pack()

typedef struct {
  FILE *file;
  TJog *jogador;
} TEntrada;

typedef struct vetor
{
    TJog *cli;
    FILE *f;
} TVet;


typedef struct Node {
    TJog jogador;
    struct Node *next;
} Node;

void salva(TJog *jogador, FILE *out);

int tamanho_registro();

TJog *le(FILE *in);

void insere_jogadores(FILE *out, int num);

void le_jogadores(FILE *in);

void insertion_sort_disco(FILE *arq, int tam, FILE *metrics);

int buscar_jogador_binario(FILE *in, int num_camisa);

int buscar_jogador_sequencial(FILE *in, int num_camisa, FILE *metrics);

void criarEquipesAleatorias(FILE *in, FILE *out1, FILE *out2, int totalJogadores, int tamanhoEquipe);

void imprimirEquipes(FILE *in1, FILE *in2);

double calcularMediaAltura(FILE *equipe);

int compararEquipes(FILE *equipe1, FILE *equipe2);

int tamanho_arquivo(FILE *file, int contSizeFile);

void imprime_particao(FILE *file, char partitionName[]);

int vetores_congelados(int vet[6]);

void N_particao(char *partitionName, const char *nameFilePartition, int partitionNumber);

int selecao_com_substituicao(FILE *file, char nameFilePartition[], int M);

void intercalar_arquivos(FILE *arquivos_entrada[], int num_arquivos, FILE *arquivo_saida);

void intercalacao_otima(FILE *out, int num_p, int F);


// TABELA HASH

void faz_particao_hash (int num_particao);

int hashFunction(int key, int num_particao);

void tabela_hash(FILE *file, int num_particao, int sizeFile);

int buscar_posicao_jogador(FILE *in, int num_camisa);

void insere_tabela(TJog *funcHashTableInsert, int particao_camisa);

TJog* procura_jogador_hash(int camisa, int num_particao);

void deletar_jogador(int camisa, int particao_camisa);



#endif