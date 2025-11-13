#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura das salas da mansão
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Função que cria dinamicamente uma sala
Sala* criarSala(const char *nome) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função para a exploração da mansão
void explorarSalas(Sala *salaAtual) {
    char opcao;

    while (salaAtual != NULL) {
        printf("\nVocê está em: %s\n", salaAtual->nome);

        // Verifica se é um cômodo sem saídas
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Não há mais caminhos a seguir. Fim da exploração!\n");
            return;
        }

        printf("Escolha um caminho:\n");
        if (salaAtual->esquerda != NULL) printf(" - (e) Ir para %s\n", salaAtual->esquerda->nome);
        if (salaAtual->direita != NULL) printf(" - (d) Ir para %s\n", salaAtual->direita->nome);
        printf(" - (s) Sair do jogo\n> ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && salaAtual->esquerda != NULL) {
            salaAtual = salaAtual->esquerda;
        } else if (opcao == 'd' && salaAtual->direita != NULL) {
            salaAtual = salaAtual->direita;
        } else if (opcao == 's') {
            printf("Encerrando a exploração. Até mais, detetive!\n");
            return;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// Monta o inicio da exploração
int main() {
    // Criação das salas (árvore binária)
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim");
    Sala *porao = criarSala("Porão");

    // Montagem da árvore
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = porao;

    printf("=== Detective Quest: Exploração da Mansão ===\n");
    explorarSalas(hall);

    // Limpar memória
    free(biblioteca);
    free(jardim);
    free(salaEstar);
    free(porao);
    free(cozinha);
    free(hall);

    return 0;
}
