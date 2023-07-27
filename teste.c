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

void salva(TJog *jogador, FILE *out){
  fwrite(&jogador->camisa, sizeof(int), 1, out);
  fwrite(jogador->nome, sizeof(char), sizeof(jogador->nome), out);
  fwrite(&jogador->idade, sizeof(int), 1, out);
  fwrite(&jogador->salario, sizeof(double), 1, out);
  fwrite(&jogador->altura, sizeof(double), 1, out);
}

int tamanho_registro(){
  return sizeof(int) + sizeof(char) * 50 + sizeof(int) + sizeof(double) + sizeof(double);
}

TJog *le(FILE *in){
  TJog *jogador = (TJog *)malloc(sizeof(TJog));
  if (fread(&jogador->camisa, sizeof(int), 1, in) <= 0){
    free(jogador);
    return NULL;
  }
  fread(jogador->nome, sizeof(char), sizeof(jogador->nome), in);
  fread(&jogador->idade, sizeof(int), 1, in);
  fread(&jogador->salario, sizeof(double), 1, in);
  fread(&jogador->altura, sizeof(double), 1, in);
  return jogador;
}

void insere_jogadores(FILE *out, int num){
  srand(time(NULL));
  int camisas[num];
  int num_jogadores = 0;
  double altura_minima = 1.70, altura_maxima = 2.20;
  // Lista de nomes e sobrenomes reais
  char *nomes[] = {"Bam", "Jimmy", "Jamal", "Udonis", "Tyler", "Haywood", "Jaime", "Nikola", "Kevin", "kyle", "Caleb", "Victor", "Duncan", "Orlando", "Max", "Gabe", "Omer", "Cody"};
  char *sobrenomes[] = {"Adebayo", "Butler", "Cain", "Haslem", "Herro", "Highsmith", "Jaquez", "Jovic", "Love", "Lowry", "Martin", "Oladipo", "Robinson", "Strus", "Vincent", "Yurtseven", "Zeller"};
  while (num_jogadores < num){
    TJog jogador;
    jogador.camisa = 1 + rand() % num;
    
    // Seleciona um nome e sobrenome aleatoriamente
    char *nome = nomes[rand() % (sizeof(nomes) / sizeof(nomes[0]))];
    char *sobrenome = sobrenomes[rand() % (sizeof(sobrenomes) / sizeof(sobrenomes[0]))];
    
    // Combina nome e sobrenome para formar o nome completo do jogador
    snprintf(jogador.nome, sizeof(jogador.nome), "%s %s", nome, sobrenome);
    jogador.idade = 18 + rand() % 17;

    // Gera o salário aleatoriamente entre 5 e 50
    int salario_aleatorio = 10 + rand() % 51;
    float salario_formatado = salario_aleatorio / 1.2;
    jogador.salario = salario_formatado;

    // Gera a altura aleatoriamente entre 1.70 e 2.20
    jogador.altura = altura_minima + (altura_maxima - altura_minima) * ((double)rand() / RAND_MAX);

    // Gera o número da camisa único
    int camisa_existe = 0;
    for (int i = 0; i < num_jogadores; i++){
      if (camisas[i] == jogador.camisa){
        camisa_existe = 1;
        break;
      }
    }
    if (!camisa_existe){
      camisas[num_jogadores] = jogador.camisa;
      salva(&jogador, out);
      num_jogadores++;
    }
  }
}

void le_jogadores(FILE *in){
  printf("\nLendo jogadores do arquivo...\n\n");
  rewind(in);
  TJog *jogador;
  while ((jogador = le(in)) != NULL){
    printf("Camisa: %d\n", jogador->camisa);
    printf("Nome: %s\n", jogador->nome);
    printf("Idade: %d\n", jogador->idade);
    printf("Altura: %.2lf m\n", jogador->altura);
    printf("Salario: US$ %.2lf M\n\n", jogador->salario);
    free(jogador);
  }
}

void insertion_sort_disco(FILE *arq, int tam){
  int i;
  int comparacoes = 0;
  clock_t inicio = clock();

  printf("Ordenando jogadores por numero de camisa...\n");

  for (int j = 2; j <= tam; j++){
    fseek(arq, (j - 1) * tamanho_registro(), SEEK_SET);
    TJog *fj = le(arq);
    //printf("\n********* Jogador atual: %d\n", fj->camisa);
    i = j - 1;
    fseek(arq, (i - 1) * tamanho_registro(), SEEK_SET);
    TJog *fi = le(arq);
    //printf("fi = %d\n", fi->camisa);
    while ((i > 0) && (fi->camisa > fj->camisa)){
      fseek(arq, i * tamanho_registro(), SEEK_SET);
      //printf("Salvando jogador de camisa %d na posicao %d\n", fi->camisa, i + 1);
      salva(fi, arq);
      i = i - 1;
      fseek(arq, (i - 1) * tamanho_registro(), SEEK_SET);
      fi = le(arq);
      //printf("fi = %d; i = %d\n", fi->camisa, i);
      comparacoes++;
    }
    fseek(arq, (i)*tamanho_registro(), SEEK_SET);
    //printf("*** Salvando jogador de camisa %d na posicao %d\n", fj->camisa, i + 1);
    salva(fj, arq);
  }
  fflush(arq);

  clock_t fim = clock();
  double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

  printf("\nTempo de ordenacao: %.6f segundos\n", tempo);
  printf("Total de comparacoes: %d\n", comparacoes);
}

int buscar_jogador_binario(FILE *in, int num_camisa){
  fseek(in, 0, SEEK_END);
  int tam = ftell(in) / tamanho_registro();
  rewind(in);

  int left = 0, right = tam - 1, comparacoes = 0;
  while (left <= right){
    comparacoes++;
    int middle = (left + right) / 2;
    fseek(in, middle * tamanho_registro(), SEEK_SET);
    TJog *func = le(in);
    if (num_camisa == func->camisa){
      printf("Jogador encontrado! Comparacoes feitas: %d\n", comparacoes);
      printf("Camisa: %d\n", func->camisa);
      printf("Nome: %s\n", func->nome);
      printf("Idade: %d\n", func->idade);
      printf("Altura: %.2f m\n", func->altura);
      printf("Salario: US$ %.2f M\n\n", func->salario);
      free(func);
      return 1;
    } else if (func->camisa < num_camisa){
      left = middle + 1;
    } else{
      right = middle - 1;
    }
  }

  return 0;
}

int buscar_jogador_sequencial(FILE *in, int num_camisa){
  rewind(in);
  TJog *jogador;
  int encontrou = 0, comparacoes = 0;

  clock_t inicio = clock();
  while ((jogador = le(in)) != NULL){
    comparacoes++;

    if (jogador->camisa == num_camisa){
      encontrou = 1;
      break;
    }
    free(jogador);
  }
  clock_t fim = clock();
  double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
  if (encontrou){
    printf("\nJogador encontrado! Comparacoes feitas: %d\n", comparacoes);
    printf("Numero da camisa: %d\n", jogador->camisa);
    printf("Nome: %s\n", jogador->nome);
    printf("Idade: %d\n", jogador->idade);
    printf("Altura: %.2f\n", jogador->altura);
    printf("Salario: %.2f\n", jogador->salario);
    printf("Tempo de busca: %.6lf segundos.\n", tempo);
  } else{
    printf("Jogador com camisa %d nao encontrado! Realizou %d comparacoes.\n", num_camisa, comparacoes);
    printf("Tempo de busca: %.6lf segundos.\n", tempo);
  }
  free(jogador); // Libera a memória do jogador encontrado ou o último jogador lido (NULL)
  return encontrou;
}

void criarEquipesAleatorias(FILE *in, FILE *out1, FILE *out2, int totalJogadores, int tamanhoEquipe)
{
  rewind(in);
  TJog *jogador;
  TJog *jogadores = (TJog *)malloc(sizeof(TJog) * totalJogadores);

  // Ler todos os jogadores do arquivo e armazená-los em um vetor
  int i = 0;
  while ((jogador = le(in)) != NULL)
  {
    jogadores[i] = *jogador;
    free(jogador);
    i++;
  }

  // Verificar se é possível dividir igualmente entre as equipes
  if (totalJogadores % 2 != 0)
  {
    printf("Não é possível dividir igualmente entre as equipes.\n");
    free(jogadores);
    return;
  }

  // Embaralhar aleatoriamente os jogadores
  srand(time(NULL));
  for (int j = totalJogadores - 1; j > 0; j--)
  {
    int k = rand() % (j + 1);
    TJog temp = jogadores[j];
    jogadores[j] = jogadores[k];
    jogadores[k] = temp;
  }
  int tam2 = tamanhoEquipe + tamanhoEquipe;
  // Distribuir os jogadores igualmente entre as equipes
  for (int j = 0; j < tam2; j++)
  {
    if (j < tamanhoEquipe)
    {
      salva(&jogadores[j], out1); // Equipe 1
    }
    else
    {
      salva(&jogadores[j], out2); // Equipe 2
    }
  }

  free(jogadores);
}


void imprimirEquipes(FILE *in1, FILE *in2)
{
  printf("================================:\n");
  printf("          EQUIPE 1:\n");
  printf("================================:\n");
  rewind(in1);
  TJog *jogador;
  while ((jogador = le(in1)) != NULL)
  {
    printf("Camisa: %d\n", jogador->camisa);
    printf("Nome: %s\n", jogador->nome);
    printf("Idade: %d\n", jogador->idade);
    printf("Altura: %.2lf\n", jogador->altura);
    printf("Salario: US$ %.2lf M\n\n", jogador->salario);
    free(jogador);
  }
  printf("================================:\n");
  printf("          EQUIPE 2:\n");
  printf("================================:\n");
  rewind(in2);
  while ((jogador = le(in2)) != NULL)
  {
    printf("Camisa: %d\n", jogador->camisa);
    printf("Nome: %s\n", jogador->nome);
    printf("Idade: %d\n", jogador->idade);
    printf("Altura: %.2lf\n", jogador->altura);
    printf("Salario: US$ %.2lf M\n\n", jogador->salario);
    free(jogador);
  }
}

double calcularMediaAltura(FILE *equipe){
  rewind(equipe);
  TJog *jogador;
  double somaAltura = 0.0;
  int numJogadores = 0;

  while ((jogador = le(equipe)) != NULL){
    somaAltura += jogador->altura;
    numJogadores++;
    free(jogador);
  }
  return somaAltura / numJogadores;
}

int compararEquipes(FILE *equipe1, FILE *equipe2){
  double mediaAlturaEquipe1 = calcularMediaAltura(equipe1);
  double mediaAlturaEquipe2 = calcularMediaAltura(equipe2);
  if (mediaAlturaEquipe1 > mediaAlturaEquipe2){
    printf("A media de altura da equipe 1 e: %.2lf, a media de altura da equipe 2 e: %.2lf\n", mediaAlturaEquipe1, mediaAlturaEquipe2);
    printf("Equipe 1 tem a maior media de altura.\n");
  } else if (mediaAlturaEquipe1 < mediaAlturaEquipe2)
    printf("Equipe 2 tem a maior media de altura.\n");
  else
    printf("As duas equipes tem a mesma media de altura.\n");
  return 0;
}


int main(){

  FILE *arquivo_jogadores, *equipe1, *equipe2;
  arquivo_jogadores = fopen("jogadores.dat", "a+b");
  equipe1 = fopen("equipe1.dat", "a+b");
  equipe2 = fopen("equipe2.dat", "a+b");

  if (arquivo_jogadores == NULL || equipe1 == NULL || equipe2 == NULL){
    printf("Erro ao abrir o arquivo.\n");
    return 1;
  }
  int opcao, num, tamEquipe, num_camisa;
  printf("Quantos jogadores deseja inserir? ");
  scanf("%d", &num);
  printf("Inserindo %d jogadores no arquivo...\n", num);
  insere_jogadores(arquivo_jogadores, num);
  printf("Jogadores inseridos com sucesso!\n");
  sleep(0);
  system("cls");

  do{
    printf("============================================================\n");
    printf("                      MENU DE OPCOES:\n");
    printf("============================================================\n");
    printf("1. Ler jogadores do arquivo.\n");
    printf("2. Ordenar jogadores por numero de camisa (insertionSort).\n");
    printf("3. Buscar jogador por numero de camisa (busca sequencial).\n");
    printf("4. Buscar jogador por numero de camisa (busca binaria).\n\n");
    printf("5. Criar equipes aleatorias.\n");
    printf("6. Comparar altura dos jogadores das equipes.\n");
    printf("7. Imprimir equipes.\n");
    printf("0. Sair.\n");
    printf("============================================================\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    system("cls");
    switch (opcao){
    case 1:
      le_jogadores(arquivo_jogadores);
      break;
    case 2:
      insertion_sort_disco(arquivo_jogadores, num);
      break;
    case 3:{
      printf("Digite o numero de camisa: ");
      scanf("%d", &num_camisa);
      buscar_jogador_sequencial(arquivo_jogadores, num_camisa);

      break;
    }
    case 4:{
      printf("Digite o numero de camisa: ");
      scanf("%d", &num_camisa);
      printf("\n");

      int comparacoes = 0;
      clock_t inicio = clock();
      int encontrado = buscar_jogador_binario(arquivo_jogadores, num_camisa);
      clock_t fim = clock();
      double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

      if (encontrado) {
        printf("Tempo de busca: %.6lf segundos.\n", tempo);
      } else{
        printf("Jogador com camisa %d nao encontrado! Realizou %d comparacoes.\n", num_camisa, comparacoes);
        printf("Tempo de busca: %.6lf segundos.\n", tempo);
      }
      break;
    }
    case 5:
      do{
        printf("Quantos jogadores deseja inserir em cada time? (no maximo metade da quantidade de jogadores total): ");
        scanf("%d", &tamEquipe);
        if(tamEquipe > num/2)
          printf("Quantidade invalida! Tente novamente.\n");
      } while (tamEquipe > num/2);
      criarEquipesAleatorias(arquivo_jogadores, equipe1, equipe2, num, tamEquipe);
      printf("Equipes criadas com sucesso!\n");
      break;
    case 6:
      compararEquipes(equipe1, equipe2);
      break;
    case 7:
      printf("Imprimindo equipes...\n");
      imprimirEquipes(equipe1, equipe2);
      break;
    case 0:
      printf("Encerrando o programa.\n");
      break;
    default:
      printf("Opcao invalida.\n");
      break;
    }
    printf("\n");
  } while (opcao != 0);

  fclose(arquivo_jogadores);
  fclose(equipe1);
  fclose(equipe2);

  return 0;
}
