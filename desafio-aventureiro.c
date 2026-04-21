#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

/*
  Le uma linha (permitindo espacos) com limite de tamanho via scanf.
  Retorna 1 em sucesso, 0 em falha.
*/
static int ler_nome_territorio(char destino[30]) {
  return scanf(" %29[^\n]", destino) == 1;
}

/*
  Le uma palavra (sem espacos) com limite de tamanho via scanf.
  Retorna 1 em sucesso, 0 em falha.
*/
static int ler_cor_exercito(char destino[10]) {
  return scanf(" %9s", destino) == 1;
}

/*
  Le um inteiro via scanf com validacao de faixa.
*/
static int ler_inteiro_faixa(const char *prompt, int *valor, int min, int max) {
  while (1) {
    printf("%s", prompt);
    if (scanf(" %d", valor) == 1) {
      if (*valor >= min && *valor <= max) {
        return 1;
      }
      printf("Valor fora do intervalo (%d a %d). Tente novamente.\n", min, max);
      continue;
    }
    printf("Entrada invalida. Digite um numero inteiro.\n");
    limpar_entrada();
  }
}

/*
  Cadastro (entrada) dos territorios usando ponteiros e vetor dinamico.
*/
static void cadastrarTerritorios(Territorio *mapa, int total) {
  printf("=== Cadastro de Territorios (War) ===\n\n");
  printf("Voce vai cadastrar %d territorio(s).\n", total);
  printf("Dica: o nome pode conter espacos.\n\n");

  for (int i = 0; i < total; i++) {
    Territorio *t = &mapa[i];

    printf("--- Territorio %d de %d ---\n", i + 1, total);

    while (1) {
      printf("Nome do territorio: ");
      if (ler_nome_territorio(t->nome)) {
        break;
      }
      printf("Entrada invalida. Tente novamente.\n");
      limpar_entrada();
    }

    while (1) {
      printf("Cor do exercito (ex: vermelho): ");
      if (ler_cor_exercito(t->cor)) {
        break;
      }
      printf("Entrada invalida. Tente novamente.\n");
      limpar_entrada();
    }

    /* Tropas: no War, normalmente um territorio tem pelo menos 1 tropa. */
    ler_inteiro_faixa("Quantidade de tropas (>= 1): ", &t->tropas, 1, 1000000);

    printf("\n");
  }
}

/*
  Exibe os territorios com indice (para facilitar a escolha no ataque).
*/
static void exibirTerritorios(const Territorio *mapa, int total) {
  printf("=== Mapa / Territorios ===\n\n");
  for (int i = 0; i < total; i++) {
    printf("[%d] %s | Cor: %s | Tropas: %d\n",
           i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
  }
  printf("\n");
}

/*
  Simula um ataque com dados (1 a 6) e atualiza os territorios via ponteiros.
  Regras:
  - Se atacante vencer: defensor muda de cor e recebe metade das tropas do atacante.
  - Se atacante perder: atacante perde 1 tropa.
*/
void atacar(Territorio *atacante, Territorio *defensor) {
  int dadoAtacante = (rand() % 6) + 1;
  int dadoDefensor = (rand() % 6) + 1;

  printf("Ataque: %s (%s, %d tropas) -> %s (%s, %d tropas)\n",
         atacante->nome, atacante->cor, atacante->tropas,
         defensor->nome, defensor->cor, defensor->tropas);
  printf("Dados: atacante=%d, defensor=%d\n", dadoAtacante, dadoDefensor);

  if (dadoAtacante > dadoDefensor) {
    int metade = atacante->tropas / 2;
    if (metade < 1) {
      metade = 1;
    }

    strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
    defensor->cor[sizeof(defensor->cor) - 1] = '\0';

    defensor->tropas = metade;
    atacante->tropas -= metade;

    printf("Resultado: atacante venceu! O defensor mudou de dono e recebeu %d tropa(s).\n\n", metade);
  } else {
    if (atacante->tropas > 0) {
      atacante->tropas -= 1;
    }
    printf("Resultado: atacante perdeu (ou empatou). O atacante perdeu 1 tropa.\n\n");
  }
}

/*
  Libera a memoria alocada dinamicamente.
*/
void liberarMemoria(Territorio *mapa) {
  free(mapa);
}

int main(void) {
  int totalTerritorios = 0;
  Territorio *mapa = NULL;

  srand((unsigned)time(NULL));

  printf("=== War estruturado: cadastro e ataque ===\n\n");
  ler_inteiro_faixa("Informe o numero total de territorios (min 2): ", &totalTerritorios, 2, 1000);

  /*
    Alocacao dinamica: vetor de Territorio com tamanho informado pelo usuario.
    calloc zera a memoria, ajudando a evitar lixo de memoria em strings.
  */
  mapa = (Territorio *)calloc((size_t)totalTerritorios, sizeof(Territorio));
  if (mapa == NULL) {
    printf("Erro: nao foi possivel alocar memoria.\n");
    return 1;
  }

  cadastrarTerritorios(mapa, totalTerritorios);
  exibirTerritorios(mapa, totalTerritorios);

  /*
    Loop de interatividade: permite executar ataques ate o usuario sair.
  */
  while (1) {
    int opcao = 0;
    ler_inteiro_faixa("Menu: 1) Atacar  0) Sair -> ", &opcao, 0, 1);
    if (opcao == 0) {
      break;
    }

    exibirTerritorios(mapa, totalTerritorios);

    int idxAtacante = 0;
    int idxDefensor = 0;

    ler_inteiro_faixa("Escolha o indice do ATACANTE: ", &idxAtacante, 1, totalTerritorios);
    ler_inteiro_faixa("Escolha o indice do DEFENSOR: ", &idxDefensor, 1, totalTerritorios);

    if (idxAtacante == idxDefensor) {
      printf("Escolha invalida: atacante e defensor devem ser diferentes.\n\n");
      continue;
    }

    Territorio *atacante = &mapa[idxAtacante - 1];
    Territorio *defensor = &mapa[idxDefensor - 1];

    if (strcmp(atacante->cor, defensor->cor) == 0) {
      printf("Escolha invalida: nao pode atacar um territorio da mesma cor.\n\n");
      continue;
    }

    if (atacante->tropas < 2) {
      printf("Escolha invalida: o atacante precisa ter pelo menos 2 tropas para atacar.\n\n");
      continue;
    }

    atacar(atacante, defensor);
    exibirTerritorios(mapa, totalTerritorios);
  }

  liberarMemoria(mapa);
  return 0;
}
