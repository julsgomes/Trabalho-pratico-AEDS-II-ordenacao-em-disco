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


TJog *leRegistroJogador(FILE *arquivo){
  TJog *jogador = (TJog *)malloc(sizeof(TJog));
  if (fread(&jogador->camisa, sizeof(int), 1, arquivo) <= 0 ||
      fread(jogador->nome, sizeof(char), sizeof(jogador->nome), arquivo) != sizeof(jogador->nome) ||
      fread(&jogador->idade, sizeof(int), 1, arquivo) != 1 ||
      fread(&jogador->salario, sizeof(double), 1, arquivo) != 1 ||
      fread(&jogador->altura, sizeof(double), 1, arquivo) != 1) {
    free(jogador);
    return NULL;
  }
  return jogador;
}


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
    double salario_formatado = salario_aleatorio / 1.2;
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
  while ((jogador = leRegistroJogador(in)) != NULL){
    printf("Camisa: %d\n", jogador->camisa);
    printf("Nome: %s\n", jogador->nome);
    printf("Idade: %d\n", jogador->idade);
    printf("Altura: %.2lf m\n", jogador->altura);
    printf("Salario: US$ %.2lf M\n\n", jogador->salario);
    free(jogador);
  }
}

int tamanho_arquivo(FILE *file, int contSizeFile) {
    int bytesAUX = 0;
    while(!feof(file)) {
        fseek(file, bytesAUX * sizeof(TJog), SEEK_SET);
        TJog *aux = le(file);
        if(aux != NULL) {
            contSizeFile++;
        }
        bytesAUX++;
    }
    return contSizeFile;
}

void printPartition(FILE *file, char partitionName[]) {

    printf("\nIDs employee of partition %s: \n --->  ", partitionName);

    for (int i = 0; i < tamanho_arquivo(file, 0); ++i) {

        fseek(file, i * sizeof(TJog), SEEK_SET) ;
        TJog *aux = le(file);

        printf(" %i ", aux->camisa);
    }

    printf("\n");
}

/*Deve fazer o método de seleçao por substituição, ou seja, pegar um arquivo grande, ler seus 6 primeiros
  jogadores e inserir eles no buffer, após isso ordenar o buffer e pegar o primeiro elemento (o menor) e verificar se ele é maior que o último inserido no arquivo,
  se ele for insere ele no arquivo e depois preenche o espaço do buffer que ficou vazio e ordena novamente refazendo o processo, se não for, congela ele no buffer (no caso do código vai ser buffer[freeze], supondo que o valor de freeze esteja em 3, as 
  posições 0, 1, 2 estão congeladas e não podem ser acessadas), continua o processo até o arquivo acabar ou o buffer ficar congelado, quando o buffer ficar todo congelado, o arquivo que está sendo salvo os jogadores é fechado e um novo arquivo é aberto, 
  o índice do buffer (freeze) é zerado e o processo se repete até o arquivo (nome_arquivo_entrada) acabar. O arquivo de saída deve ser nomeado como "particao1.dat", "particao2.dat" e assim por diante. 
  */
int vetor_congelado(int vet[6])
{
    int cont = 0;
    for (int i = 0; i < 6; ++i){
        if (vet[i] == 1){
            cont++;
        }
    }
    if (cont == 6){
        return 1;
    } else{
        return 0;
    }
}

void create_partition_name(char *partitionName, const char *nameFilePartition, int partitionNumber) {
  char str1[MAX_NAME_LENGTH];
  char str2[] = ".dat";
  itoa(partitionNumber, str1, 10);
  strcat(strcpy(partitionName, nameFilePartition), str1);
  strcat(strcpy(partitionName, partitionName), str2);
}

int selecao_com_substituicao(FILE *file, char nameFilePartition[], int M) {
  int nome_particao = 0, posicao = M, menor_posicao = 0, smallElement = MAX_INT_VALUE, sizeFileAux = 0;
  TJog jogadores[M];
  int auxVetJogadores[M];
  memset(auxVetJogadores, 0, sizeof(auxVetJogadores));

  rewind(file);
  sizeFileAux = tamanho_arquivo(file, 0);

  for (int i = 0; i < M; ++i) {
    fseek(file, i * sizeof(TJog), SEEK_SET);
    TJog *aux = le(file);
    jogadores[i] = *aux;
    auxVetJogadores[i] = jogadores[i].camisa;
  }

  while (posicao != sizeFileAux) {
    char partitionName[MAX_NAME_LENGTH];
    create_partition_name(partitionName, nameFilePartition, nome_particao);
    FILE *filePartition = fopen(partitionName, "wb+");
    int auxVetFrozen[M];
    memset(auxVetFrozen, 0, sizeof(auxVetFrozen));

    while (posicao != sizeFileAux) {
      smallElement = MAX_INT_VALUE;
      for (int i = 0; i < M; ++i) {
        if (smallElement > auxVetJogadores[i] && auxVetFrozen[i] != 1) {
          smallElement = auxVetJogadores[i];
          menor_posicao = i;
        }
      }
      
      if (smallElement == MAX_INT_VALUE) break;

      salva(&jogadores[menor_posicao], filePartition);
      fseek(file, posicao * sizeof(TJog), SEEK_SET);
      TJog *aux = le(file);
      posicao++;
      auxVetJogadores[menor_posicao] = aux->camisa;
      jogadores[menor_posicao] = *aux;
      if (aux->camisa < smallElement) {
        auxVetFrozen[menor_posicao] = 1;
      }
      if (vetor_congelado(auxVetFrozen) == 1) {
        nome_particao++;
        break;
      }
    }
    fclose(filePartition);
    if (posicao >= sizeFileAux) {
      break;
    }
  }
  char partitionName[MAX_NAME_LENGTH];
  create_partition_name(partitionName, nameFilePartition, nome_particao);
  FILE *filePartitionFinal = fopen(partitionName, "ab+");
  TJog jogAux;
  for (int k = 1; k < M; k++) {
    for (int j = 0; j < M - 1; j++) {
      if (jogadores[j].camisa > jogadores[j + 1].camisa) {
        jogAux = jogadores[j];
        jogadores[j] = jogadores[j + 1];
        jogadores[j + 1] = jogAux;
      }
    }
  }
  for (int i = 0; i < M; ++i) {
    salva(&jogadores[i], filePartitionFinal);
  }
  fclose(filePartitionFinal);

  // Impressão das partições
  for (int i = 0; i <= nome_particao; ++i) {
    create_partition_name(partitionName, nameFilePartition, i);
    FILE *filePartition = fopen(partitionName, "rb+");
    printPartition(filePartition, partitionName);
    fclose(filePartition);
  }
  return nome_particao;
}

void intercalar_arquivos(FILE *arquivos_entrada[], int num_arquivos, FILE *arquivo_saida) {
    TJog *jogadores[num_arquivos];
    int fim[num_arquivos];

    // Inicializar jogadores e fim
    for (int i = 0; i < num_arquivos; i++) {
        jogadores[i] = le(arquivos_entrada[i]);
        fim[i] = (jogadores[i] == NULL);
    }

    while (1) {
        int menor_posicao = -1;
        TJog *menor_jogador = NULL;

        // Encontrar o menor jogador
        for (int i = 0; i < num_arquivos; i++) {
            if (!fim[i] && (menor_jogador == NULL || jogadores[i]->camisa < menor_jogador->camisa)) {
                menor_posicao = i;
                menor_jogador = jogadores[i];
            }
        }

        if (menor_posicao == -1) break; // Todos os arquivos estão no fim

        // Salvar o menor jogador no arquivo de saída
        salva(menor_jogador, arquivo_saida);
        free(menor_jogador); // Liberar a memória do jogador intercalado

        // Ler o próximo jogador da posição do menor jogador
        jogadores[menor_posicao] = le(arquivos_entrada[menor_posicao]);
        if (jogadores[menor_posicao] == NULL) {
            fim[menor_posicao] = 1;
        }
    }
}

void intercalacao_otima(FILE *out, int num_p, int F) {
    int num_fases = (num_p + F - 2) / (F - 1); // Cálculo correto do número de fases
    char numero[MAX_NAME_LENGTH];
    char extensao[] = ".dat";
    int total_intermediarios = 0;

    for (int fase = 0; fase < num_fases; fase++) {
        FILE *arquivos_entrada[F - 1];
        char nome[MAX_NAME_LENGTH];

        // Abrir arquivos de entrada para esta fase
        int num_arquivos_abertos = 0;
        for (int i = 0; i < F - 1; i++) {
            int indice_particao = fase * (F - 1) + i;
            if (indice_particao >= num_p) break;

            strcpy(nome, "particao");
            itoa(indice_particao, numero, 10);
            strcat(nome, numero);
            strcat(nome, extensao);
            arquivos_entrada[i] = fopen(nome, "rb");
            num_arquivos_abertos++;
        }

        // Abrir arquivo de saída para esta fase
        strcpy(nome, "intermediario");
        itoa(total_intermediarios++, numero, 10);
        strcat(nome, numero);
        strcat(nome, extensao);
        FILE *arquivo_saida = fopen(nome, "wb");

        // Intercalar arquivos de entrada e escrever no arquivo de saída
        intercalar_arquivos(arquivos_entrada, num_arquivos_abertos, arquivo_saida); // Use o número real de arquivos abertos

        // Fechar arquivos desta fase
        for (int i = 0; i < num_arquivos_abertos; i++) {
            fclose(arquivos_entrada[i]);
        }
        fclose(arquivo_saida);
    }

    // Intercalar arquivos intermediários para formar o arquivo final
    FILE *arquivos_intermediarios[total_intermediarios];
    for (int i = 0; i < total_intermediarios; i++) {
        char nome[MAX_NAME_LENGTH];
        strcpy(nome, "intermediario");
        itoa(i, numero, 10);
        strcat(nome, numero);
        strcat(nome, extensao);
        arquivos_intermediarios[i] = fopen(nome, "rb");
    }
    intercalar_arquivos(arquivos_intermediarios, total_intermediarios, out); // Use o número real de arquivos intermediários

    for (int i = 0; i < total_intermediarios; i++) {
        fclose(arquivos_intermediarios[i]);
    }
}


int main(){
  
  FILE *arquivo_jogadores = fopen("jogadores.dat", "w+b");
  int num;
  char nome_arquivo_saida[] = "ordenado.dat"; // Nome do arquivo de saída final
  int M = BUFFER_SIZE; // Número de jogadores que cabem na memória
  int F = 6;

  printf("Quantos jogadores deseja inserir? ");
  scanf("%d", &num);
  printf("Inserindo %d jogadores no arquivo...\n", num);
  insere_jogadores(arquivo_jogadores, num);
  

  // Cria as partições usando seleção com substituição
  int num_p = selecao_com_substituicao(arquivo_jogadores, "particao", M); // Criar partições
  fclose(arquivo_jogadores);

  // Abrir o arquivo de saída
  FILE *out = fopen("ordenado.dat", "w+b");
  if (out == NULL) {
    printf("Erro ao abrir o arquivo de saída.\n");
    return 1;
  }

  // Executar a intercalação ótima
  intercalacao_otima(out, num_p, F);

  printf("Ordenação concluída. Resultado salvo em %s.\n", nome_arquivo_saida);
  FILE *arquivo_ordenado = fopen("intermediario0.dat", "rb");
  FILE *arquivo_ordenado2 = fopen("intermediario1.dat", "rb");
  le_jogadores(out);
  le_jogadores(arquivo_ordenado);
  le_jogadores(arquivo_ordenado2);

  fclose(arquivo_ordenado);
  fclose(arquivo_ordenado2);
    return 0;
}