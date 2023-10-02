# Código de Gerenciamento de Equipes de Basquete em C

## Descrição

Este projeto é uma implementação em C de um sistema de gerenciamento de equipes de basquete. O sistema realiza várias funções, incluindo a leitura de jogadores de um arquivo, ordenação, busca, e criação de tabelas hash. Além disso, ele também pode criar equipes aleatórias e comparar suas alturas médias.

## Estruturas de Dados Utilizadas

- `TJog`: Estrutura que define os atributos de um jogador.
- `TEntrada`: Estrutura para gerenciar arquivos e jogadores.
- `TVet`: Estrutura para vetores de jogadores e arquivos.
- `Node`: Estrutura para um nó de lista ligada.

## Funções Principais

### Ordenação e Busca

- `insertion_sort_disco()`: Ordena os jogadores por número da camisa usando o algoritmo de Insertion Sort.
- `buscar_jogador_binario()`: Busca um jogador pelo número da camisa usando busca binária.
- `buscar_jogador_sequencial()`: Busca um jogador pelo número da camisa usando busca sequencial.

### Tabela Hash

- `faz_particao_hash()`: Cria as partições para a tabela hash.
- `tabela_hash()`: Insere os registros nas partições.
- `procura_jogador_hash()`: Procura um jogador na tabela hash.
- `deletar_jogador()`: Deleta um jogador da tabela hash.

### Equipes

- `criarEquipesAleatorias()`: Cria equipes aleatórias a partir do arquivo de jogadores.
- `calcularMediaAltura()`: Calcula a altura média dos jogadores em uma equipe.
- `compararEquipes()`: Compara as alturas médias das equipes.

### Utilitários

- `tamanho_registro()`: Retorna o tamanho de um registro.
- `tamanho_arquivo()`: Retorna o tamanho do arquivo.

## Como Usar

O programa apresenta um menu de opções para o usuário, permitindo-lhe escolher entre diferentes funcionalidades, como ordenar jogadores, buscar jogadores, e muito mais.

## Observações

- O projeto também contém métricas de desempenho, registradas em um arquivo `metrics.txt`.
