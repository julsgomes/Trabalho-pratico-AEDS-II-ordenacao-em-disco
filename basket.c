#include "basket.h"

// FUNÇÕES BÁSICAS DO TRABALHO

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
  while ((jogador = le(in)) != NULL){
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

void criarEquipesAleatorias(FILE *in, FILE *out1, FILE *out2, int totalJogadores, int tamanhoEquipe){
  rewind(in);
  TJog *jogador;
  TJog *jogadores = (TJog *)malloc(sizeof(TJog) * totalJogadores);

  // Ler todos os jogadores do arquivo e armazená-los em um vetor
  int i = 0;
  while ((jogador = le(in)) != NULL){
    jogadores[i] = *jogador;
    free(jogador);
    i++;
  }

  // Verificar se é possível dividir igualmente entre as equipes
  if (totalJogadores % 2 != 0){
    printf("Não é possível dividir igualmente entre as equipes.\n");
    free(jogadores);
    return;
  }

  // Embaralhar aleatoriamente os jogadores
  srand(time(NULL));
  for (int j = totalJogadores - 1; j > 0; j--){
    int k = rand() % (j + 1);
    TJog temp = jogadores[j];
    jogadores[j] = jogadores[k];
    jogadores[k] = temp;
  }
  int tam2 = tamanhoEquipe + tamanhoEquipe;
  // Distribuir os jogadores igualmente entre as equipes
  for (int j = 0; j < tam2; j++){
    if (j < tamanhoEquipe){
      salva(&jogadores[j], out1); // Equipe 1
    } else{
      salva(&jogadores[j], out2); // Equipe 2
    }
  }
  free(jogadores);
}

void imprimirEquipes(FILE *in1, FILE *in2){
  printf("================================:\n");
  printf("          EQUIPE 1:\n");
  printf("================================:\n");
  rewind(in1);
  TJog *jogador;
  while ((jogador = le(in1)) != NULL){
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
  while ((jogador = le(in2)) != NULL){
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

  while ((jogador = le(equipe)) != NULL)
{
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
  } else if (mediaAlturaEquipe1 < mediaAlturaEquipe2){
    printf("A media de altura da equipe 1 e: %.2lf, a media de altura da equipe 2 e: %.2lf\n", mediaAlturaEquipe1, mediaAlturaEquipe2);
    printf("Equipe 2 tem a maior media de altura.\n");
  }
  else
    printf("As duas equipes tem a mesma media de altura.\n");
  return 0;
}

// MÉTODO DE ORDENAÇÃO EM DISCO: INSERTION SORT
void insertion_sort_disco(FILE *arq, int tam, FILE *metrics){
  int i;
  int comparacoes = 0;
  clock_t inicio = clock();

  printf("Ordenando jogadores por numero de camisa...\n");

  for (int j = 2; j <= tam; j++){
    fseek(arq, (j - 1) * tamanho_registro(), SEEK_SET);
    TJog *fj = le(arq);
    // printf("\n********* Jogador atual: %d\n", fj->camisa);
    i = j - 1;
    fseek(arq, (i - 1) * tamanho_registro(), SEEK_SET);
    TJog *fi = le(arq);
    // printf("fi = %d\n", fi->camisa);
    while ((i > 0) && (fi->camisa > fj->camisa)){
      fseek(arq, i * tamanho_registro(), SEEK_SET);
      // printf("Salvando jogador de camisa %d na posicao %d\n", fi->camisa, i + 1);
      salva(fi, arq);
      i = i - 1;
      fseek(arq, (i - 1) * tamanho_registro(), SEEK_SET);
      fi = le(arq);
      // printf("fi = %d; i = %d\n", fi->camisa, i);
      comparacoes++;
    }
    fseek(arq, (i)*tamanho_registro(), SEEK_SET);
    // printf("*** Salvando jogador de camisa %d na posicao %d\n", fj->camisa, i + 1);
    salva(fj, arq);
  }
  fflush(arq);

  clock_t fim = clock();
  double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

  fprintf(metrics, "Insertion Sort:\n");
  fprintf(metrics, "Tempo de ordenacao: %.6f segundos\n", tempo);
  fprintf(metrics, "Total de comparacoes: %d\n\n", comparacoes);
  fflush(metrics);
}

// MÉTODOS DE BUSCA
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

int buscar_jogador_sequencial(FILE *in, int num_camisa, FILE *metrics){
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
    printf("Salario: %.2f\n M", jogador->salario);
    printf("Tempo de busca: %.6lf segundos.\n", tempo);
    fprintf(metrics, "Busca Sequencial (encontrou):\n");
    fprintf(metrics, "Comparacoes feitas: %d\n", comparacoes);
    fprintf(metrics, "Tempo de busca: %.6lf segundos.\n\n", tempo);
    fflush(metrics);
  } else{
    printf("Jogador com camisa %d nao encontrado! Realizou %d comparacoes.\n", num_camisa, comparacoes);
    printf("Tempo de busca: %.6lf segundos.\n", tempo);
    fprintf(metrics, "Busca Sequencial (nao encontrou):\n");
    fprintf(metrics, "Comparacoes feitas: %d\n", comparacoes);
    fprintf(metrics, "Tempo de busca: %.6lf segundos.\n\n", tempo);
    fflush(metrics);
  }
  free(jogador); // Libera a memória do jogador encontrado ou o último jogador lido (NULL)
  return encontrou;
}

// FUNÇÕES DE PARTIÇÕES (SELEÇÃO POR SUBSTITUIÇÃO E INTERCALAÇÃO ÓTIMA)

// Função que imprime os elementos de uma partição
void imprime_particao(FILE *file, char partitionName[]) {
    printf("\n%s:", partitionName);
    for (int i = 0; i < tamanho_arquivo(file, 0); ++i) {
        fseek(file, i * sizeof(TJog), SEEK_SET) ;
        TJog *aux = le(file);
        printf(" %i ", aux->camisa);
    }
    printf("\n");
}

void imprime_particao_tudo(FILE *file, char partitionName[]) {
  printf("Particao: %s\n", partitionName);
  rewind(file);
  TJog *jogador;
  while ((jogador = le(file)) != NULL) {
    printf("Camisa: %d\n", jogador->camisa);
    printf("Nome: %s\n", jogador->nome);
    printf("Idade: %d\n", jogador->idade);
    printf("Salario: %.2f\n", jogador->salario);
    printf("Altura: %.2f\n", jogador->altura);
    printf("-----------------\n");
    free(jogador);
  }
}

// Função que verifica se todos os elementos do vetor estão congelados
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

// Função que concatena o nome da partição com o número da partição e a extensão .dat
void N_particao(char *partitionName, const char *nameFilePartition, int partitionNumber) {
  char str1[MAX_NAME_LENGTH];
  char str2[] = ".dat";
  itoa(partitionNumber, str1, 10);
  strcat(strcpy(partitionName, nameFilePartition), str1);
  strcat(strcpy(partitionName, partitionName), str2);
}

// Função que realiza a seleção com substituição
int selecao_com_substituicao(FILE *file, char nameFilePartition[], int M) {
  int nome_particao = 0, posicao = M, menor_posicao = 0, smallElement = MAX_INT_VALUE, sizeFileAux = 0;
  TJog jogadores[M]; // Array para armazenar jogadores
  int auxVetJogadores[M]; // Array auxiliar para armazenar números de camisa
  memset(auxVetJogadores, 0, sizeof(auxVetJogadores)); // Inicializa o array auxiliar com zeros

  rewind(file); // Retorna ao início do arquivo
  sizeFileAux = tamanho_arquivo(file, 0); // Obtém o tamanho do arquivo

  // Lê os primeiros M jogadores do arquivo e os armazena no array jogadores
  for (int i = 0; i < M; ++i) {
    fseek(file, i * sizeof(TJog), SEEK_SET);
    TJog *aux = le(file);
    jogadores[i] = *aux;
    auxVetJogadores[i] = jogadores[i].camisa;
  }

  // Loop principal para criar partições
  while (posicao != sizeFileAux) {
    char partitionName[MAX_NAME_LENGTH];
    N_particao(partitionName, nameFilePartition, nome_particao); // Gera o nome da partição. No primeiro loop o nome_particao tem valor 0
    FILE *filePartition = fopen(partitionName, "wb+"); // Abre a partição para gravação
    int auxVetFrozen[M]; // Array para controlar elementos congelados
    memset(auxVetFrozen, 0, sizeof(auxVetFrozen)); // Inicializa o array com zeros

    // Loop para preencher uma partição
    while (posicao != sizeFileAux) {
      smallElement = MAX_INT_VALUE;
      // Encontra o menor elemento não congelado no array auxiliar
      for (int i = 0; i < M; ++i) {
        if (smallElement > auxVetJogadores[i] && auxVetFrozen[i] != 1) {
          smallElement = auxVetJogadores[i];
          menor_posicao = i;
        }
      }

      // Salva o menor elemento na partição
      salva(&jogadores[menor_posicao], filePartition);

      // Lê o próximo jogador do arquivo e atualiza o array auxiliar
      fseek(file, posicao * sizeof(TJog), SEEK_SET);
      TJog *aux = le(file);
      posicao++;
      auxVetJogadores[menor_posicao] = aux->camisa;
      jogadores[menor_posicao] = *aux;
      
      // Se o novo elemento é menor que o salvo, congela a posição
      if (aux->camisa < smallElement) {
        auxVetFrozen[menor_posicao] = 1;
      }

      // Se todos os elementos estão congelados, incrementa o contador de partições e encerra o loop
      if (vetor_congelado(auxVetFrozen) == 1) {
        nome_particao++;
        break;
      }
    }
    fclose(filePartition); // Fecha a partição
    // Se a posição atual for maior ou igual ao tamanho do arquivo, encerra o loop
    if (posicao >= sizeFileAux) {
      break;
    }
  }

  // Restante do código manipula a última partição e imprime as partições
  char partitionName[MAX_NAME_LENGTH];
  N_particao(partitionName, nameFilePartition, nome_particao);
  FILE *filePartitionFinal = fopen(partitionName, "ab+");

  // Ordena o array de jogadores e salva na última partição
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
    N_particao(partitionName, nameFilePartition, i);
    FILE *filePartition = fopen(partitionName, "rb+");
    imprime_particao(filePartition, partitionName);
    fclose(filePartition);
  }
  return nome_particao; // Retorna o número total de partições criadas
}

// Função que realiza a intercalação de arquivos
void intercalar_arquivos(FILE *arquivos_entrada[], int num_arquivos, FILE *arquivo_saida) {
  TJog *jogadores[num_arquivos]; // Array para armazenar um jogador de cada arquivo de entrada
  int fim[num_arquivos]; // Array para marcar arquivos que chegaram ao fim

  // Inicializar jogadores e fim
  for (int i = 0; i < num_arquivos; i++) {
    jogadores[i] = le(arquivos_entrada[i]); // Ler o primeiro jogador de cada arquivo
    fim[i] = (jogadores[i] == NULL); // Marcar como fim se o arquivo estiver vazio
  }

  // Loop principal para intercalar os arquivos
  while (1) {
    int menor_posicao = -1;
    TJog *menor_jogador = NULL;

    // Encontrar o menor jogador
    for (int i = 0; i < num_arquivos; i++) {
      if (!fim[i] && (menor_jogador == NULL || jogadores[i]->camisa < menor_jogador->camisa)) {
        menor_posicao = i; // Armazenar a posição do menor jogador
        menor_jogador = jogadores[i]; // Armazenar o menor jogador
      }
    }

    if (menor_posicao == -1)
      break; // Todos os arquivos estão no fim, sair do loop

    // Salvar o menor jogador no arquivo de saída
    salva(menor_jogador, arquivo_saida);
    free(menor_jogador); // Liberar a memória do jogador intercalado

    // Ler o próximo jogador da posição do menor jogador
    jogadores[menor_posicao] = le(arquivos_entrada[menor_posicao]);
    if (jogadores[menor_posicao] == NULL) {
      fim[menor_posicao] = 1; // Marcar como fim se não houver mais jogadores no arquivo
    }
  }
}

// Função que realiza a intercalação ótima
void intercalacao_otima(FILE *out, int num_p, int F) {
  int num_fases = (num_p + F - 2) / (F - 1); // Cálculo do número de fases
  char numero[MAX_NAME_LENGTH];
  char extensao[] = ".dat";
  int total_intermediarios = 0; // Contador de arquivos intermediários

  // Loop através das fases de intercalação
  for (int fase = 0; fase < num_fases; fase++) {
    FILE *arquivos_entrada[F - 1]; // Arquivos de entrada para esta fase
    char nome[MAX_NAME_LENGTH];

    // Abrir arquivos de entrada para esta fase
    int num_arquivos_abertos = 0;
    for (int i = 0; i < F - 1; i++) {
      int indice_particao = fase * (F - 1) + i;
      if (indice_particao >= num_p)
        break; // Não abrir mais arquivos se todos forem abertos

      strcpy(nome, "particao");
      itoa(indice_particao, numero, 10);
      strcat(nome, numero);
      strcat(nome, extensao);
      arquivos_entrada[i] = fopen(nome, "rb"); // Abrir arquivo em modo de leitura
      num_arquivos_abertos++;
    }

    // Abrir arquivo de saída para esta fase
    strcpy(nome, "intermediario");
    itoa(total_intermediarios++, numero, 10);
    strcat(nome, numero);
    strcat(nome, extensao);
    FILE *arquivo_saida = fopen(nome, "wb"); // Abrir arquivo em modo de escrita

    // Intercalar arquivos de entrada e escrever no arquivo de saída
    intercalar_arquivos(arquivos_entrada, num_arquivos_abertos, arquivo_saida);

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
    arquivos_intermediarios[i] = fopen(nome, "rb"); // Abrir arquivos intermediários
  }
  intercalar_arquivos(arquivos_intermediarios, total_intermediarios, out); // Intercalar arquivos intermediários

  // Fechar arquivos intermediários
  for (int i = 0; i < total_intermediarios; i++) {
    fclose(arquivos_intermediarios[i]);
  }
}


// TABELA HASH

void faz_particao_hash (int num_particao) {
    // Loop para criar 'num_particao' partições
    for (int i = 0; i < num_particao; ++i) {
        char partitionName[100];  // Nome da partição
        char str1[100];           // String temporária para armazenar a representação da parte numérica do nome
        char str2[100] = ".dat";  // Extensão do arquivo
        itoa(i, str1, 10); // Converte o índice da partição para string em base 10
        // Constrói a parte inicial do nome do arquivo da partição, ex: "particao_hash0"
        strcat(strcpy(partitionName, "particao_hash"), str1);
        // Adiciona a extensão ao nome do arquivo, ex: "particao_hash0.dat"
        strcat(strcpy(partitionName, partitionName), str2);
        // Abre o arquivo no modo de leitura/escrita binária, criando o arquivo se ele não existir
        FILE *file = fopen(partitionName,"wb+");
        // Fecha o arquivo, finalizando a criação
        fclose(file);
    }
}

// função hash que trabalha com a técnica de divisão (módulo) e utiliza o particionamento em arquivos separados.
// pega a camisa do jogador e e retorna o resto da divisão dessa chave pelo número total de partições.
int hashFunction(int key, int num_particao) {
    // Calcula o valor de hash para a chave fornecida
    // A chave é, normalmente, o atributo que você quer usar para particionar os dados, como o número da camisa de um jogador
    // Usar o módulo (%) assegura que o valor de hash estará no intervalo [0, num_particao - 1]
    // Isso significa que cada chave será mapeada para uma partição específica neste intervalo
    return key % num_particao; // Retorna o valor de hash
}

// Calcula o valor de hash de uma chave usando o método de divisão (módulo). O valor retornado indica em qual partição um registro deve ser inserido.
void tabela_hash(FILE *file, int num_particao, int sizeFile) {
    rewind(file);
    // Cria um array de ponteiros para os nós, um para cada partição
    Node *partitions[num_particao];
    // Inicializa todas as partições vazias
    memset(partitions, 0, sizeof(partitions));

    // Itera através de todos os registros no arquivo
    for (int i = 0; i < sizeFile; ++i) {
        TJog *auxFunc = le(file);
        // Calcula a partição correspondente usando o módulo do número da camisa pelo número de partições
        int selectedParticipation = auxFunc->camisa % num_particao;

        // Cria um novo nó para este registro
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->jogador = *auxFunc;
        newNode->next = NULL;

        // Se a partição correspondente estiver vazia, simplesmente insere o novo nó
        if (partitions[selectedParticipation] == NULL) {
            partitions[selectedParticipation] = newNode;
        } else {
            // Se a partição já contiver registros, encontra o final da lista ligada
            Node *current = partitions[selectedParticipation];
            while (current->next != NULL) {
                current = current->next;
            }
            // Insere o novo nó no final da lista ligada
            current->next = newNode;
        }
    }

    // Salva as partições em arquivos separados
    for (int i = 0; i < num_particao; ++i) {
        char partitionName[100];
        sprintf(partitionName, "particao_hash%d.dat", i);
        FILE *filePartition = fopen(partitionName, "wb+");
        Node *current = partitions[i];
        while (current != NULL) {
            salva(&(current->jogador), filePartition);
            current = current->next;
        }
        fclose(filePartition);
    }
}

int buscar_posicao_jogador(FILE *in, int num_camisa) {
  rewind(in);
  TJog *jogador;
  int posicao = 0;

  while ((jogador = le(in)) != NULL) {
    if (jogador->camisa == num_camisa) {
      free(jogador);
      return posicao;
    }
    free(jogador);
    posicao++;
  }

  return -1; // Não encontrado
}

void insere_tabela(TJog *funcHashTableInsert, int particao_camisa) {
  char partitionName[100];
  sprintf(partitionName, "particao_hash%d.dat", particao_camisa);

  FILE *filePartition = fopen(partitionName, "r+b");
  int sizefilePartition = tamanho_arquivo(filePartition, 0);
  rewind(filePartition);

  int posicaoExistente = buscar_posicao_jogador(filePartition, funcHashTableInsert->camisa);
  rewind(filePartition);

  int posicaoExcluida = buscar_posicao_jogador(filePartition, 999999999); // Procura por um registro excluído
  rewind(filePartition);

  if (posicaoExistente == -1 && posicaoExcluida == -1) {
    fseek(filePartition, sizefilePartition * sizeof(TJog), SEEK_SET);
    salva(funcHashTableInsert, filePartition);
    printf("\nJogador inserido.");
  }
  else if (posicaoExistente == -1 && posicaoExcluida != -1) {
    fseek(filePartition, posicaoExcluida * sizeof(TJog), SEEK_SET);
    salva(funcHashTableInsert, filePartition);
    printf("\nJogador inserido e o anterior deletado.");
  } else {
    printf("\nJogador ja existe na tabela.");
  }
  fclose(filePartition);
}

TJog* procura_jogador_hash(int camisa, int num_particao) {
    int particao_camisa = hashFunction(camisa, num_particao);
    char partitionName[100];
    sprintf(partitionName, "particao_hash%d.dat", particao_camisa);

    printf("\nSe encontra na %s...\n", partitionName);

    FILE *filePartition = fopen(partitionName, "rb+");
    if (filePartition == NULL) {
        printf("Erro ao abrir a particao %d.\n", particao_camisa);
        return NULL;
    }

    TJog *jogador = (TJog *)malloc(sizeof(TJog));
    while (fread(jogador, sizeof(TJog), 1, filePartition) == 1) {
        if (jogador->camisa == camisa) {
            fclose(filePartition);
            return jogador; // Jogador encontrado
        }
    }

    fclose(filePartition);
    free(jogador);
    return NULL; // Jogador não encontrado
}

void deletar_jogador(int camisa, int particao_camisa) {
    char partitionName[100];
    sprintf(partitionName, "particao_hash%d.dat", particao_camisa);

    FILE *filePartition = fopen(partitionName, "r+b");
    if (filePartition == NULL) {
        printf("\nErro ao abrir a partição %d.\n", particao_camisa);
        return;
    }

    TJog jogador;
    int found = 0;
    while (fread(&jogador, sizeof(TJog), 1, filePartition) == 1) {
        if (jogador.camisa == camisa) {
            // Encontrou o jogador, agora vamos marcar como excluído
            fseek(filePartition, -sizeof(TJog), SEEK_CUR); // Volta para o início do registro
            jogador.camisa = -1; // Marca como excluído
            fwrite(&jogador, sizeof(TJog), 1, filePartition);
            printf("\nJogador deletado com sucesso.");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("\nNúmero da camisa não existe na tabela hash.");
    }

    fclose(filePartition);
}