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
  Sorteia uma missao do vetor e copia para destino usando strcpy.
  (destino deve ter memoria suficiente alocada previamente)
*/
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
  int idx = rand() % totalMissoes;
  strcpy(destino, missoes[idx]);
}

/*
  Exibe a missao uma unica vez no inicio do jogo.
  (passagem por valor: missao e apenas lida/mostrada)
*/
static void exibirMissao(const char *nomeJogador, const char *missao) {
  printf("Missao de %s: %s\n\n", nomeJogador, missao);
}

/*
  Extrai a cor de uma missao que contenha "cor <X>" ou "(<X>)".
  Retorna 1 se conseguiu, 0 caso contrario.
*/
static int extrairCorDaMissao(const char *missao, char corSaida[10]) {
  const char *p = strstr(missao, "cor ");
  if (p != NULL) {
    p += 4; /* pula "cor " */
    int i = 0;
    while (*p != '\0' && *p != '.' && *p != ')' && *p != '\n' && i < 9) {
      corSaida[i++] = *p++;
    }
    corSaida[i] = '\0';
    return i > 0;
  }

  /* suporte a "Eliminar a cor adversaria (azul)" */
  p = strchr(missao, '(');
  if (p != NULL) {
    p++;
    int i = 0;
    while (*p != '\0' && *p != ')' && i < 9) {
      corSaida[i++] = *p++;
    }
    corSaida[i] = '\0';
    return i > 0;
  }

  return 0;
}

/*
  Verifica se a missao foi cumprida.
  Logica simples inicial baseada em contagem/soma por cor.
*/
int verificarMissao(char *missao, Territorio *mapa, int tamanho) {
  char cor[10];
  if (!extrairCorDaMissao(missao, cor)) {
    return 0;
  }

  if (strstr(missao, "Controlar 3 territorios") != NULL) {
    int controlados = 0;
    for (int i = 0; i < tamanho; i++) {
      if (strcmp(mapa[i].cor, cor) == 0) {
        controlados++;
      }
    }
    return controlados >= 3;
  }

  if (strstr(missao, "Controlar 2 territorios") != NULL) {
    int controlados = 0;
    for (int i = 0; i < tamanho; i++) {
      if (strcmp(mapa[i].cor, cor) == 0) {
        controlados++;
      }
    }
    return controlados >= 2;
  }

  if (strstr(missao, "Somar 20 tropas") != NULL) {
    int soma = 0;
    for (int i = 0; i < tamanho; i++) {
      if (strcmp(mapa[i].cor, cor) == 0) {
        soma += mapa[i].tropas;
      }
    }
    return soma >= 20;
  }

  if (strstr(missao, "Dominar todos os territorios") != NULL) {
    for (int i = 0; i < tamanho; i++) {
      if (strcmp(mapa[i].cor, cor) != 0) {
        return 0;
      }
    }
    return 1;
  }

  if (strstr(missao, "Eliminar a cor adversaria") != NULL) {
    for (int i = 0; i < tamanho; i++) {
      if (strcmp(mapa[i].cor, cor) == 0) {
        return 0;
      }
    }
    return 1;
  }

  /* Missao fallback simples */
  return 0;
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
void liberarMemoria(Territorio *mapa, char *missaoJogador1, char *missaoJogador2) {
  free(missaoJogador1);
  free(missaoJogador2);
  free(mapa);
}

int main(void) {
  int totalTerritorios = 0;
  Territorio *mapa = NULL;
  char nomeJogador1[30];
  char nomeJogador2[30];
  char corJogador1[10];
  char corJogador2[10];
  char *missaoJogador1 = NULL;
  char *missaoJogador2 = NULL;

  srand((unsigned)time(NULL));

  printf("=== War estruturado: missoes estrategicas ===\n\n");

  /* Cadastro basico dos jogadores */
  printf("Nome do Jogador 1: ");
  if (scanf(" %29[^\n]", nomeJogador1) != 1) {
    strcpy(nomeJogador1, "Jogador 1");
    limpar_entrada();
  }
  while (1) {
    printf("Cor do exercito do Jogador 1 (ex: vermelho): ");
    if (ler_cor_exercito(corJogador1)) {
      break;
    }
    printf("Entrada invalida. Tente novamente.\n");
    limpar_entrada();
  }

  printf("Nome do Jogador 2: ");
  if (scanf(" %29[^\n]", nomeJogador2) != 1) {
    strcpy(nomeJogador2, "Jogador 2");
    limpar_entrada();
  }
  while (1) {
    printf("Cor do exercito do Jogador 2 (ex: azul): ");
    if (ler_cor_exercito(corJogador2)) {
      if (strcmp(corJogador2, corJogador1) != 0) {
        break;
      }
      printf("Escolha invalida: a cor do Jogador 2 deve ser diferente da do Jogador 1.\n");
      continue;
    }
    printf("Entrada invalida. Tente novamente.\n");
    limpar_entrada();
  }

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
    Vetores de missoes (5+) e sorteio para cada jogador.
    As descricoes incluem a cor para permitir uma verificacao simples.
  */
  char missoesJogador1[5][120];
  char missoesJogador2[5][120];
  char *listaMissoes1[5];
  char *listaMissoes2[5];

  snprintf(missoesJogador1[0], sizeof(missoesJogador1[0]), "Controlar 3 territorios da cor %s.", corJogador1);
  snprintf(missoesJogador1[1], sizeof(missoesJogador1[1]), "Somar 20 tropas (ou mais) na cor %s.", corJogador1);
  snprintf(missoesJogador1[2], sizeof(missoesJogador1[2]), "Dominar todos os territorios (cor %s).", corJogador1);
  snprintf(missoesJogador1[3], sizeof(missoesJogador1[3]), "Eliminar a cor adversaria (%s).", corJogador2);
  snprintf(missoesJogador1[4], sizeof(missoesJogador1[4]), "Controlar 2 territorios da cor %s.", corJogador1);

  snprintf(missoesJogador2[0], sizeof(missoesJogador2[0]), "Controlar 3 territorios da cor %s.", corJogador2);
  snprintf(missoesJogador2[1], sizeof(missoesJogador2[1]), "Somar 20 tropas (ou mais) na cor %s.", corJogador2);
  snprintf(missoesJogador2[2], sizeof(missoesJogador2[2]), "Dominar todos os territorios (cor %s).", corJogador2);
  snprintf(missoesJogador2[3], sizeof(missoesJogador2[3]), "Eliminar a cor adversaria (%s).", corJogador1);
  snprintf(missoesJogador2[4], sizeof(missoesJogador2[4]), "Controlar 2 territorios da cor %s.", corJogador2);

  for (int i = 0; i < 5; i++) {
    listaMissoes1[i] = missoesJogador1[i];
    listaMissoes2[i] = missoesJogador2[i];
  }

  /* Missao armazenada dinamicamente (malloc) */
  missaoJogador1 = (char *)malloc(120);
  missaoJogador2 = (char *)malloc(120);
  if (missaoJogador1 == NULL || missaoJogador2 == NULL) {
    printf("Erro: nao foi possivel alocar memoria para as missoes.\n");
    liberarMemoria(mapa, missaoJogador1, missaoJogador2);
    return 1;
  }

  atribuirMissao(missaoJogador1, listaMissoes1, 5);
  atribuirMissao(missaoJogador2, listaMissoes2, 5);

  exibirMissao(nomeJogador1, missaoJogador1);
  exibirMissao(nomeJogador2, missaoJogador2);

  /*
    Loop de interatividade: permite executar ataques ate o usuario sair.
  */
  int turnoJogador = 1;
  while (1) {
    int opcao = 0;
    ler_inteiro_faixa("Menu: 1) Atacar  0) Sair -> ", &opcao, 0, 1);
    if (opcao == 0) {
      break;
    }

    exibirTerritorios(mapa, totalTerritorios);

    int idxAtacante = 0;
    int idxDefensor = 0;

    const char *nomeAtual = (turnoJogador == 1) ? nomeJogador1 : nomeJogador2;
    const char *corAtual = (turnoJogador == 1) ? corJogador1 : corJogador2;
    char *missaoAtual = (turnoJogador == 1) ? missaoJogador1 : missaoJogador2;

    printf("Turno de %s (cor %s)\n", nomeAtual, corAtual);
    ler_inteiro_faixa("Escolha o indice do ATACANTE: ", &idxAtacante, 1, totalTerritorios);
    ler_inteiro_faixa("Escolha o indice do DEFENSOR: ", &idxDefensor, 1, totalTerritorios);

    if (idxAtacante == idxDefensor) {
      printf("Escolha invalida: atacante e defensor devem ser diferentes.\n\n");
      continue;
    }

    Territorio *atacante = &mapa[idxAtacante - 1];
    Territorio *defensor = &mapa[idxDefensor - 1];

    if (strcmp(atacante->cor, corAtual) != 0) {
      printf("Escolha invalida: o territorio atacante deve ser da sua cor.\n\n");
      continue;
    }

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

    /* Verificacao silenciosa da missao ao final do turno */
    if (verificarMissao(missaoAtual, mapa, totalTerritorios)) {
      printf("=== VITORIA ===\n");
      printf("%s cumpriu sua missao e venceu o jogo!\n", nomeAtual);
      break;
    }

    turnoJogador = (turnoJogador == 1) ? 2 : 1;
  }

  liberarMemoria(mapa, missaoJogador1, missaoJogador2);
  return 0;
}
