/*
 FEITO POR ANDRÉ LUCAS SANTOS E JÚLIA ROBERTA GOMES MIGUEL
 21/08/2023
*/

#include "basket.h"
#include "basket.c"

int main(){

  FILE *arquivo_jogadores, *equipe1, *equipe2, *metrics;
  equipe1 = fopen("equipe1.dat", "w+b");
  equipe2 = fopen("equipe2.dat", "w+b");
  metrics = fopen("metrics.txt", "a");
  int num_particao = 0; // Tamanho da tabela hash


  int opcao, num, tamEquipe, num_camisa;

  if (equipe1 == NULL || equipe2 == NULL || metrics == NULL) {
      printf("Erro ao abrir os arquivos de equipes!\n");
      return 1; // ou exit(1); se estiver dentro de main()
  }
  
  printf("Deseja carregar o arquivo de jogadores(1) ou criar um novo(0)? ");
  scanf("%d", &opcao);
  if(opcao == 1){
    arquivo_jogadores = fopen("jogadores.dat", "r+b");
    if(arquivo_jogadores == NULL){
      printf("Erro ao abrir o arquivo de jogadores!\n");
      return 1;
    }
    printf("Arquivo carregado com sucesso!\n");
    system("cls");
  }else{
    arquivo_jogadores = fopen("jogadores.dat", "w+b"); // Modificado para "jogadores.txt"
    if(arquivo_jogadores == NULL){
      printf("Erro ao abrir o arquivo de jogadores!\n");
      return 1;
    }
    printf("Quantos jogadores deseja inserir? ");
    scanf("%d", &num);
    printf("Inserindo %d jogadores no arquivo...\n", num);
    insere_jogadores(arquivo_jogadores, num);
    printf("Jogadores inseridos com sucesso!\n");
    system("cls");
  }
  printf("Quantas particoes de hash? ");
  scanf("%d", &num_particao);
  Node *partitions[num_particao]; // Declaração do array de partições
  memset(partitions, 0, sizeof(partitions)); // Inicialização do array de partições
  system("cls");

  do{
    printf("============================================================\n");
    printf("                      MENU DE OPCOES:\n");
    printf("============================================================\n");
    printf("1. Ler jogadores do arquivo.\n");
    printf("2. Ordenar jogadores por numero de camisa (insertionSort).\n");
    printf("3. Buscar jogador por numero de camisa (busca sequencial).\n");
    printf("4. Buscar jogador por numero de camisa (busca binaria).\n");
    printf("5. Ordenar com a selecao natural e intercalacao otima.\n");
    printf("============================================================\n");
    printf("                       TABELA HASH:\n");
    printf("============================================================\n");
    printf("6. Criar tabela hash.\n");
    printf("7. Imprimir tabela hash.\n");
    printf("8. Inserir jogador na tabela hash.\n");
    printf("9. Buscar jogador na tabela hash.\n");
    printf("10. Remover jogador na tabela hash.\n");
    printf("============================================================\n");
    printf("                     MENU DE EQUIPES:\n");
    printf("============================================================\n");
    printf("11. Criar equipes aleatorias.\n");
    printf("12. Comparar altura dos jogadores das equipes.\n");
    printf("13. Imprimir equipes.\n");
    printf("============================================================\n");
    printf("                     SAIR DO PROGRAMA:\n");
    printf("============================================================\n");
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
      insertion_sort_disco(arquivo_jogadores, num, metrics);

      break;
    case 3:{
      printf("Digite o numero de camisa: ");
      scanf("%d", &num_camisa);
      buscar_jogador_sequencial(arquivo_jogadores, num_camisa, metrics);
      break;
    }
    case 4:{
      printf("Digite o numero de camisa: ");
      scanf("%d", &num_camisa);
      printf("\n");
      int comparacoes = 0, encontrado;
      clock_t inicio = clock();
      encontrado = buscar_jogador_binario(arquivo_jogadores, num_camisa);
      clock_t fim = clock();
      double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

      if (encontrado) {
        printf("Tempo de busca: %.6lf segundos.\n", tempo);
        fprintf(metrics, "Busca Binaria (encontrado):\n");
        fprintf(metrics, "Tempo de ordenacao: %.6f segundos\n", tempo);
        fprintf(metrics, "Total de comparacoes: %d\n\n", comparacoes);
        fflush(metrics);
      } else{
        printf("Jogador com camisa %d nao encontrado! Realizou %d comparacoes.\n", num_camisa, comparacoes);
        printf("Tempo de busca: %.6lf segundos.\n", tempo);
        fprintf(metrics, "Busca Binaria (nao encontrado):\n");
        fprintf(metrics, "Tempo de ordenacao: %.6f segundos\n", tempo);
        fprintf(metrics, "Total de comparacoes: %d\n\n", comparacoes);
        fflush(metrics);
      }
      break;
    }
    case 5:{
      char nome_arquivo_saida[] = "ordenado.dat"; // Nome do arquivo de saída final
      int M = BUFFER_SIZE; // Número de jogadores que cabem na memória
      int F = 6;
      clock_t inicio = clock();
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
      printf("\nOrdenacao concluida. Resultado salvo em %s.\n", nome_arquivo_saida);
      clock_t fim = clock();

      double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

      le_jogadores(out);

      printf("Selecao por substituicao e intercalacao otima.\n");
      printf("Tempo da operacao: %.6f segundos\n", tempo);
      fprintf(metrics, "Selecao por substituicao e intercalacao otima.\n");
      fprintf(metrics, "Tempo da operacao: %.6f segundos\n\n", tempo);
      fflush(metrics);
      fclose(out);
      break;
    }
    case 6: // Criar tabela hash
      faz_particao_hash(num_particao); // Crie os arquivos de partição
      tabela_hash(arquivo_jogadores, num_particao, num); // Insira os registros nas partições
      printf("Tabela hash criada com sucesso!\n");
      break;

    case 7: // Imprimir tabela hash
      for (int i = 0; i < num_particao; ++i) {
        char partitionName[100];
        sprintf(partitionName, "particao_hash%d.dat", i);
        FILE *filePartition = fopen(partitionName, "rb");
        imprime_particao(filePartition, partitionName);
        fclose(filePartition);
      }
      break;

    case 8: { // Inserir na tabela hash
      int camisa;
      int partitionID;
      TJog jogador;
      printf("Digite o numero da camisa do jogador: ");
      scanf("%d", &camisa);
      jogador.camisa = camisa;
      printf("Digite o nome do jogador: ");
      scanf("%s", jogador.nome);
      printf("Digite a idade do jogador: ");
      scanf("%d", &jogador.idade);
      printf("Digite a altura do jogador: ");
      scanf("%lf", &jogador.altura);
      printf("Digite o salario do jogador: ");
      scanf("%lf", &jogador.salario);
      partitionID = hashFunction(camisa, num_particao);
      insere_tabela(&jogador, partitionID);
      break;
    }

    case 9: { // Buscar na tabela hash
      int camisa;
      printf("Digite o numero da camisa do jogador que deseja buscar: ");
      scanf("%d", &camisa);

      TJog *jogador = procura_jogador_hash(camisa, num_particao);
      if (jogador != NULL) {
        printf("Jogador encontrado:\n");
        printf("Numero da camisa: %d\n", jogador->camisa);
        printf("Nome: %s\n", jogador->nome);
        printf("Idade: %d\n", jogador->idade);
        printf("Altura: %.2f\n", jogador->altura);
        printf("Salario: %.2f M\n", jogador->salario);
        free(jogador);
      } else {
        printf("Jogador nao encontrado.\n");
      }
    }

    case 10: {
      int camisa = 0;
      printf("Digite o numero da camisa do jogador que deseja remover: ");
      scanf("%d", &camisa);
      int partitionID = hashFunction(camisa, num_particao);
      deletar_jogador(camisa, partitionID);
      break;
    }
    case 11:
      do{
        printf("Quantos jogadores deseja inserir em cada time? (no maximo metade da quantidade de jogadores total): ");
        scanf("%d", &tamEquipe);
        if(tamEquipe > num/2)
          printf("Quantidade invalida! Tente novamente.\n");
      } while (tamEquipe > num/2);
      criarEquipesAleatorias(arquivo_jogadores, equipe1, equipe2, num, tamEquipe);
      printf("Equipes criadas com sucesso!\n");
      break;
    case 12:
      compararEquipes(equipe1, equipe2);
      break;
    case 13:
      printf("Imprimindo equipes...\n");
      imprimirEquipes(equipe1, equipe2);
      break;
    case 0:
      printf("Encerrando o programa.\n");
      break;
    default:
      printf("Opcao invalida.\n");
      break;
    }    printf("\n");
  } while (opcao != 0);

  fflush(arquivo_jogadores);
  fflush(equipe1);
  fflush(equipe2);

  fclose(arquivo_jogadores);
  fclose(equipe1);
  fclose(equipe2);
  fclose(metrics);

  return 0;
}
