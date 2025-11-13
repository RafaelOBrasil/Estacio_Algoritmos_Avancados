#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// ==================== ESTRUTURAS ====================

typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

typedef struct HashItem {
    char pista[100];
    char suspeito[50];
    struct HashItem *prox;
} HashItem;


// ==================== FUNÇÕES DE HASH ====================

int hash(const char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % TAM_HASH;
}

void inserirNaHash(HashItem *tabela[], const char *pista, const char *suspeito) {
    int indice = hash(pista);
    HashItem *novo = (HashItem*) malloc(sizeof(HashItem));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[indice];
    tabela[indice] = novo;
}

char* encontrarSuspeito(HashItem *tabela[], const char *pista) {
    int indice = hash(pista);
    HashItem *atual = tabela[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}


// ==================== ÁRVORE DE PISTAS (BST) ====================

PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (pista == NULL || strlen(pista) == 0) return raiz;

    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);

    return raiz;
}

void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

void exibirSuspeitos() {
    printf("\nSuspeitos identificados:\n");
    printf("- Rafael\n");
    printf("- Pedro\n");
    printf("- Helena\n");
}

int contarPistasSuspeito(PistaNode *raiz, HashItem *tabela[], const char *suspeito) {
    if (raiz == NULL) return 0;
    int count = 0;
    char *sus = encontrarSuspeito(tabela, raiz->pista);
    if (sus != NULL && strcmp(sus, suspeito) == 0)
        count = 1;
    return count +
           contarPistasSuspeito(raiz->esquerda, tabela, suspeito) +
           contarPistasSuspeito(raiz->direita, tabela, suspeito);
}


// ==================== MANSÃO ====================

Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

void explorarSalas(Sala *salaAtual, PistaNode **arvorePistas, HashItem *tabela[]) {
    char opcao;

    while (1) {
        printf("\nVocê está em: %s\n", salaAtual->nome);

        if (strlen(salaAtual->pista) > 0) {
            char *suspeito = encontrarSuspeito(tabela, salaAtual->pista);
            printf("Você encontrou uma pista: \"%s\"\n", salaAtual->pista);
            if (suspeito != NULL)
                printf("➡ Essa pista está ligada a: %s\n", suspeito);
            *arvorePistas = inserirPista(*arvorePistas, salaAtual->pista);
        } else {
            printf("Nenhuma pista neste cômodo.\n");
        }

        printf("\nEscolha um caminho:\n");
        if (salaAtual->esquerda != NULL)
            printf(" - (e) Ir para %s\n", salaAtual->esquerda->nome);
        if (salaAtual->direita != NULL)
            printf(" - (d) Ir para %s\n", salaAtual->direita->nome);
        printf(" - (s) Sair da mansão e resolver o caso\n> ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && salaAtual->esquerda != NULL)
            salaAtual = salaAtual->esquerda;
        else if (opcao == 'd' && salaAtual->direita != NULL)
            salaAtual = salaAtual->direita;
        else if (opcao == 's')
            break;
        else
            printf("Opção inválida!\n");
    }
}


// ==================== JULGAMENTO FINAL ====================

void verificarSuspeitoFinal(PistaNode *arvorePistas, HashItem *tabela[]) {
    char suspeito[50];

    printf("\nPistas coletadas:\n");
    exibirPistas(arvorePistas);

    exibirSuspeitos();

    printf("\nDigite o nome do suspeito que você acredita ser o culpado:\n> ");
    scanf(" %[^\n]", suspeito);

    int qtd = contarPistasSuspeito(arvorePistas, tabela, suspeito);

    printf("\nAnalisando as pistas...\n");
    if (qtd >= 2)
        printf("✅ Acusação confirmada! %s foi o culpado com %d pistas que o incriminam!\n", suspeito, qtd);
    else if (qtd == 1)
        printf("⚠️ Apenas uma pista liga %s ao crime. Evidências insuficientes.\n", suspeito);
    else
        printf("❌ Nenhuma pista aponta para %s. Acusação incorreta.\n", suspeito);
}


// ==================== MAIN ====================

int main() {
    HashItem *tabela[TAM_HASH] = {NULL};

    inserirNaHash(tabela, "Luvas manchadas de tinta", "Rafael");
    inserirNaHash(tabela, "Caderno com anotações suspeitas", "Pedro");
    inserirNaHash(tabela, "Pegadas de lama no tapete", "Rafael");
    inserirNaHash(tabela, "Colar quebrado", "Helena");
    inserirNaHash(tabela, "Chave do escritório", "Pedro");
    inserirNaHash(tabela, "Copo com marca de batom", "Helena");

    Sala *hall = criarSala("Hall de Entrada", "Pegadas de lama no tapete");
    Sala *salaEstar = criarSala("Sala de Estar", "Luvas manchadas de tinta");
    Sala *cozinha = criarSala("Cozinha", "Copo com marca de batom");
    Sala *biblioteca = criarSala("Biblioteca", "Caderno com anotações suspeitas");
    Sala *escritorio = criarSala("Escritório", "Chave do escritório");
    Sala *jardim = criarSala("Jardim", "Colar quebrado");

    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = escritorio;

    PistaNode *arvorePistas = NULL;

    printf("=== DETECTIVE QUEST: O JULGAMENTO FINAL ===\n");
    explorarSalas(hall, &arvorePistas, tabela);

    verificarSuspeitoFinal(arvorePistas, tabela);

    printf("\nFim do caso. Até a próxima, detetive!\n");
    return 0;
}
