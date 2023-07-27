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

void salva(TJog *jogador, FILE *out) {
    fprintf(out, "%d\n", jogador->camisa);
    fprintf(out, "%s\n", jogador->nome);
    fprintf(out, "%d\n", jogador->idade);
    fprintf(out, "%.2lf\n", jogador->altura);
    fprintf(out, "%.2lf\n\n", jogador->salario);
}

TJog *le(FILE *in) {
    TJog *jogador = (TJog *)malloc(sizeof(TJog));
    if (fscanf(in, "%d", &jogador->camisa) != 1) {
        free(jogador);
        return NULL;
    }
    fscanf(in, " %[^\n]", jogador->nome); // Leitura de string com espaços em branco
    fscanf(in, "%d", &jogador->idade);
    fscanf(in, "%lf", &jogador->salario);
    fscanf(in, "%lf", &jogador->altura);
    return jogador;
}

size_t tamanho_registro() {
    return sizeof(int) + (sizeof(char) * 50) + sizeof(int) + sizeof(double) + sizeof(double);
}

void insere_jogadores(FILE *out, int num) {
    srand(time(NULL));
    int camisas[num];
    int num_jogadores = 0;
    double altura_minima = 1.70, altura_maxima = 2.20;
    // Lista de nomes e sobrenomes reais
    char *nomes[] = {"Bam", "Jimmy", "Jamal", "Udonis", "Tyler", "Haywood", "Jaime", "Nikola", "Kevin", "kyle", "Caleb", "Victor", "Duncan", "Orlando", "Max", "Gabe", "Omer", "Cody"};
    char *sobrenomes[] = {"Adebayo", "Butler", "Cain", "Haslem", "Herro", "Highsmith", "Jaquez", "Jovic", "Love", "Lowry", "Martin", "Oladipo", "Robinson", "Strus", "Vincent", "Yurtseven", "Zeller"};

    while (num_jogadores < num) {
        TJog jogador;
        jogador.camisa = 1 + rand() % 100; // Alterado para gerar números de camisa entre 1 e 100

        // Seleciona um nome e sobrenome aleatoriamente
        char *nome = nomes[rand() % (sizeof(nomes) / sizeof(nomes[0]))];
        char *sobrenome = sobrenomes[rand() % (sizeof(sobrenomes) / sizeof(sobrenomes[0]))];

        // Combina nome e sobrenome para formar o nome completo do jogador
        snprintf(jogador.nome, sizeof(jogador.nome), "%s %s", nome, sobrenome);
        jogador.idade = 18 + rand() % 17;

        // Gera o salário aleatoriamente entre 5 e 50
        int salario_aleatorio = 10 + rand() % 46; // Alterado para gerar salários entre 10 e 55
        float salario_formatado = salario_aleatorio / 1.2;
        jogador.salario = salario_formatado;

        // Gera a altura aleatoriamente entre 1.70 e 2.20
        jogador.altura = altura_minima + (altura_maxima - altura_minima) * ((double)rand() / RAND_MAX);

        // Gera o número da camisa único
        int camisa_existe = 0;
        for (int i = 0; i < num_jogadores; i++) {
            if (camisas[i] == jogador.camisa) {
                camisa_existe = 1;
                break;
            }
        }

        if (!camisa_existe) {
            camisas[num_jogadores] = jogador.camisa;
            salva(&jogador, out);
            num_jogadores++;
        }
    }
}

void imprime_tudo(FILE *arquivo) {
    char linha[40]; // Defina um tamanho adequado para a linha do arquivo

    // Verifica se o arquivo é válido
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Volta para o início do arquivo
    rewind(arquivo);

    // Lê cada linha do arquivo e imprime
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        printf("%s", linha);
    }

    // Verifica se ocorreu algum erro na leitura do arquivo
    if (ferror(arquivo)) {
        printf("Erro ao ler o arquivo.\n");
        return;
    }
}

void le_jogadores(FILE *in) {
    printf("\nLendo jogadores do arquivo...\n\n");
    rewind(in);
    TJog *jogador;
    while ((jogador = le(in)) != NULL) {
        printf("Camisa: %d\n", jogador->camisa);
        printf("Nome: %s\n", jogador->nome);
        printf("Idade: %d\n", jogador->idade);
        printf("Altura: %.2lf m\n", jogador->altura);
        printf("Salario: US$ %.2lf M\n\n", jogador->salario);
        free(jogador);
    }
}

int main() {

    clock_t inicio, fim;
    double tempo_decorrido;
    FILE *arquivo_jogadores, *equipe1, *equipe2, *tempo;
    arquivo_jogadores = fopen("jogadores.txt", "a+");
    equipe1 = fopen("equipe1.txt", "a+");
    equipe2 = fopen("equipe2.txt", "a+");
    tempo = fopen("tempo.txt", "a+");

    if (arquivo_jogadores == NULL || equipe1 == NULL || equipe2 == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
    int opcao, num;
    printf("Quantos jogadores deseja inserir? ");
    scanf("%d", &num);
    printf("Inserindo %d jogadores no arquivo...\n", num);
    insere_jogadores(arquivo_jogadores, num);
    printf("Jogadores inseridos com sucesso!\n");
    system("cls");

    do {
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
        switch (opcao) {
        case 1:
            inicio = clock(); // Registra o tempo no início da inserção
            le_jogadores(arquivo_jogadores);
            fim = clock(); // Registra o tempo no final da inserção
            tempo_decorrido = (double)(fim - inicio) / CLOCKS_PER_SEC;
            fprintf(tempo, "Tempo decorrido para inserir %d jogadores: %.6lf segundos\n\n", num, tempo_decorrido);
            rewind(tempo);
            break;
        // Implemente os outros casos aqui, se necessário
        }
    } while (opcao != 0);

    fclose(arquivo_jogadores);
    fclose(equipe1);
    fclose(equipe2);
    fclose(tempo);

    return 0;
}
