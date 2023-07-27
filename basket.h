#ifndef BASKET_H_INCLUDED
#define BASKET_H_INCLUDED

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

typedef struct Jogador
{
  int camisa;
  char nome[50];
  int idade;
  double salario;
  double altura;
} TJog;

void salva(TJog *jogador, FILE *out);

int tamanho_registro();

TJog *le(FILE *in);

void insere_jogadores(FILE *out, int num);

void le_jogadores(FILE *in);

void insertion_sort_disco(FILE *arq, int tam);

int buscar_jogador_binario(FILE *in, int num_camisa);

int buscar_jogador_sequencial(FILE *in, int num_camisa);

void criarEquipesAleatorias(FILE *in, FILE *out1, FILE *out2, int totalJogadores, int tamanhoEquipe);

void imprimirEquipes(FILE *in1, FILE *in2);

double calcularMediaAltura(FILE *equipe);

int compararEquipes(FILE *equipe1, FILE *equipe2);

#endif // BASKET_H_INCLUDED