#include <stdio.h>
#include <string.h>

/*
  Estrutura (struct) que representa um território no "War".
  - nome: nome do território (até 29 caracteres + '\0')
  - cor: cor do exército que domina o território (até 9 caracteres + '\0')
  - tropas: quantidade de tropas estacionadas
*/
typedef struct {
  char nome[30];
  char cor[10];
  int tropas;
} Territorio;

/*
  Descarta caracteres restantes na entrada (stdin) ate '\n' ou EOF.
  Ajuda a recuperar de leituras invalidas com scanf.
*/
static void limpar_entrada(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
    /* descarta */
  }
}

int main(void) {
  /*
    Vetor de structs para armazenar 5 territórios.
    Usamos um laço para cadastrar e outro para exibir.
  */
  Territorio territorios[5];

  printf("=== Cadastro de Territorios (War) ===\n\n");
  printf("Voce vai cadastrar 5 territorios.\n");
  printf("Dica: o nome pode conter espaços.\n\n");

  for (int i = 0; i < 5; i++) {
    printf("--- Territorio %d de 5 ---\n", i + 1);

    /*
      Entrada de dados:
      - nome: scanf com %29[^\n] para permitir espacos e limitar tamanho
      - cor: scanf com %9s para evitar overflow
      - tropas: scanf com %d
    */
    while (1) {
      printf("Nome do territorio: ");
      if (scanf(" %29[^\n]", territorios[i].nome) == 1) {
        break;
      }
      printf("Entrada invalida. Tente novamente.\n");
      limpar_entrada();
    }

    while (1) {
      printf("Cor do exercito (ex: vermelho): ");
      if (scanf(" %9s", territorios[i].cor) == 1) {
        break;
      }
      printf("Entrada invalida. Tente novamente.\n");
      limpar_entrada();
    }

    while (1) {
      printf("Quantidade de tropas: ");
      if (scanf(" %d", &territorios[i].tropas) == 1) {
        break;
      }
      printf("Entrada invalida. Digite um numero inteiro.\n");
      limpar_entrada();
    }

    printf("\n");
  }

  /*
    Exibicao dos dados cadastrados com formatacao clara.
  */
  printf("=== Territorios cadastrados ===\n\n");
  for (int i = 0; i < 5; i++) {
    printf("Territorio %d\n", i + 1);
    printf("  Nome  : %s\n", territorios[i].nome);
    printf("  Cor   : %s\n", territorios[i].cor);
    printf("  Tropas: %d\n", territorios[i].tropas);
    printf("\n");
  }

  return 0;
}
