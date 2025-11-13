#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==================== ESTRUTURAS ====================

// Estrutura da sala da mansão
typedef struct Sala {
    char nome[50];
    char pista[100]; // Pista associada à sala (pode estar vazia)
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura da árvore de pistas)
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;


// ==================== FUNÇÕES DA MANSÃO ====================

Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para sala.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}


// ==================== FUNÇÕES DA ÁRVORE DE PISTAS (BST) ====================

PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (pista == NULL || strlen(pista) == 0) return raiz;

    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }

    return raiz;
}

// Exibe as pistas em ordem alfabética
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// Libera memória da árvore de pistas
void liberarPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}


// ==================== FUNÇÃO DE EXPLORAÇÃO ====================

void explorarSalasComPistas(Sala *salaAtual, PistaNode **arvorePistas) {
    char opcao;

    while (1) {
        printf("\nVocê está em: %s\n", salaAtual->nome);

        // Se há uma pista na sala, coleta
        if (strlen(salaAtual->pista) > 0) {
            printf("Você encontrou uma pista: \"%s\"\n", salaAtual->pista);
            *arvorePistas = inserirPista(*arvorePistas, salaAtual->pista);
        } else {
            printf("Nenhuma pista neste cômodo.\n");
        }

        // Menu de opções
        printf("\nEscolha um caminho:\n");
        if (salaAtual->esquerda != NULL)
            printf(" - (e) Ir para %s\n", salaAtual->esquerda->nome);
        if (salaAtual->direita != NULL)
            printf(" - (d) Ir para %s\n", salaAtual->direita->nome);
        printf(" - (s) Sair da mansão\n> ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && salaAtual->esquerda != NULL) {
            salaAtual = salaAtual->esquerda;
        } else if (opcao == 'd' && salaAtual->direita != NULL) {
            salaAtual = salaAtual->direita;
        } else if (opcao == 's') {
            printf("\nExploração encerrada.\n");
            return;
        } else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }
}


// ==================== FUNÇÃO PRINCIPAL ====================

int main() {
    // Criação das salas
    Sala *hall = criarSala("Hall de Entrada", "Chave dourada no tapete");
    Sala *salaEstar = criarSala("Sala de Estar", "");
    Sala *cozinha = criarSala("Cozinha", "Pegada de lama próxima à pia");
    Sala *biblioteca = criarSala("Biblioteca", "Livro antigo com páginas rasgadas");
    Sala *jardim = criarSala("Jardim", "Flor rara caída no chão");
    Sala *porao = criarSala("Porão", "Diário misterioso parcialmente queimado");

    // Estrutura da mansão
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = porao;

    // Árvore de pistas inicialmente vazia
    PistaNode *arvorePistas = NULL;

    printf("=== DETECTIVE QUEST: COLETA DE PISTAS ===\n");
    explorarSalasComPistas(hall, &arvorePistas);

    printf("\nPistas coletadas (em ordem alfabética):\n");
    if (arvorePistas == NULL)
        printf("Nenhuma pista coletada.\n");
    else
        exibirPistas(arvorePistas);

    // Limpar memória
    liberarPistas(arvorePistas);
    free(biblioteca);
    free(jardim);
    free(salaEstar);
    free(porao);
    free(cozinha);
    free(hall);

    printf("\nFim do jogo. Até a próxima, detetive!\n");
    return 0;
}
