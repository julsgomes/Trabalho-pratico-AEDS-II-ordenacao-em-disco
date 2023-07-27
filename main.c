#include "basket.h"
#include "basket.c"
int main(){

  FILE *arquivo_jogadores, *equipe1, *equipe2;
  arquivo_jogadores = fopen("jogadores.dat", "w+b"); // Modificado para "jogadores.txt"
  equipe1 = fopen("equipe1.dat", "w+b"); // Modificado para "equipe1.txt"
  equipe2 = fopen("equipe2.dat", "w+b");

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
    printf("============================================================\n");
    printf("                     MENU DE EQUIPES:\n");
    printf("============================================================\n");
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
      fseek(arquivo_jogadores, 0, SEEK_SET);
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

  fflush(arquivo_jogadores);
  fflush(equipe1);
  fflush(equipe2);

  fclose(arquivo_jogadores);
  fclose(equipe1);
  fclose(equipe2);

  return 0;
}
