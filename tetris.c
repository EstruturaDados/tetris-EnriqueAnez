#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Definições Globais ---

// Tamanho máximo da fila de peças futuras (capacidade fixa)
#define CAPACIDADE_FILA 5 

// Contador global para gerar IDs únicos para as peças
// Começa em 0 para a primeira peça
int proximo_id_peca = 0;

// --- Estrutura da Peça ---

/**
 * @brief Representa uma peça do jogo Tetris Stack.
 * * Cada peça tem um tipo (caractere) e um identificador único (id).
 */
typedef struct {
    char tipo;
    int id;
} Peca;

// --- Estrutura da Fila Circular ---

/**
 * @brief Estrutura que gerencia a fila circular de peças.
 * * A fila é implementada como um array estático com ponteiros para o início (frente) 
 * e o final (tras) da fila.
 */
typedef struct {
    Peca elementos[CAPACIDADE_FILA];
    int frente; // Índice do primeiro elemento (próximo a ser removido)
    int tras;   // Índice do último elemento inserido
    int tamanho; // Número atual de elementos na fila
} FilaPecas;

// --- Funções Auxiliares (Modularização) ---

/**
 * @brief Inicializa a fila de peças, definindo seus parâmetros iniciais.
 * * @param fila O ponteiro para a estrutura da fila a ser inicializada.
 */
void inicializarFila(FilaPecas* fila) {
    fila->frente = 0;
    fila->tras = -1; // -1 indica que a fila está vazia
    fila->tamanho = 0;
    // O array é preenchido na função que inicializa as peças
}

/**
 * @brief Gera automaticamente uma nova peça com um tipo aleatório e um ID único.
 * * @return Uma nova estrutura Peca.
 */
Peca gerarPeca() {
    // Tipos de peças disponíveis: 'I', 'O', 'T', 'L' (simplificado para 4 tipos)
    char tipos[] = {'I', 'O', 'T', 'L'};
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);

    // Gera um índice aleatório entre 0 e (num_tipos - 1)
    int indice_aleatorio = rand() % num_tipos;

    Peca nova_peca;
    nova_peca.tipo = tipos[indice_aleatorio];
    // Atribui o ID único e incrementa o contador global para a próxima peça
    nova_peca.id = proximo_id_peca++; 
    
    return nova_peca;
}

/**
 * @brief Preenche a fila inicialmente com peças geradas automaticamente.
 * * @param fila O ponteiro para a estrutura da fila.
 * @param num_pecas O número de peças a serem geradas e inseridas.
 */
void preencherFilaInicial(FilaPecas* fila, int num_pecas) {
    if (num_pecas > CAPACIDADE_FILA) {
        num_pecas = CAPACIDADE_FILA;
    }
    
    for (int i = 0; i < num_pecas; i++) {
        // Enqueue: insere a peça no final ('tras')
        fila->tras = (fila->tras + 1) % CAPACIDADE_FILA; // Circularidade
        fila->elementos[fila->tras] = gerarPeca();
        fila->tamanho++;
    }
}

/**
 * @brief Adiciona uma nova peça ao final da fila (Enqueue).
 * * A operação só é válida se a fila não estiver cheia.
 * * @param fila O ponteiro para a estrutura da fila.
 * @return 1 se a peça foi inserida com sucesso, 0 caso contrário (fila cheia).
 */
int inserirPeca(FilaPecas* fila) {
    // Condicional: verifica se há espaço na fila
    if (fila->tamanho == CAPACIDADE_FILA) {
        printf("\n🛑 Fila cheia! Nao e possivel inserir mais pecas. Maximo: %d.\n", CAPACIDADE_FILA);
        return 0;
    }

    // Move o ponteiro 'tras' para a próxima posição (circular)
    fila->tras = (fila->tras + 1) % CAPACIDADE_FILA;
    
    // Gera e insere a nova peça
    Peca nova_peca = gerarPeca();
    fila->elementos[fila->tras] = nova_peca;
    fila->tamanho++;
    
    printf("\n✅ PECA INSERIDA: [%c %d] adicionada ao final da fila.\n", nova_peca.tipo, nova_peca.id);
    return 1;
}

/**
 * @brief Remove a peça da frente da fila (Dequeue) para ser "jogada".
 * * A operação só é válida se a fila não estiver vazia.
 * * @param fila O ponteiro para a estrutura da fila.
 * @return 1 se a peça foi removida com sucesso, 0 caso contrário (fila vazia).
 */
int jogarPeca(FilaPecas* fila) {
    // Condicional: verifica se há peças para jogar
    if (fila->tamanho == 0) {
        printf("\n🛑 Fila vazia! Nao ha pecas para jogar.\n");
        // O ponteiro 'tras' volta para -1 quando o tamanho é 0
        fila->tras = -1; 
        return 0;
    }

    // Pega a peça da frente (FIFO)
    Peca peca_removida = fila->elementos[fila->frente];

    // Move o ponteiro 'frente' para a próxima posição (circular)
    fila->frente = (fila->frente + 1) % CAPACIDADE_FILA;
    fila->tamanho--;

    printf("\n🗑️ PECA JOGADA: [%c %d] removida da frente da fila.\n", peca_removida.tipo, peca_removida.id);
    return 1;
}

/**
 * @brief Exibe o estado atual da fila de peças futuras.
 * * Itera sobre os elementos da fila, começando pela 'frente' e terminando no 'tras'.
 * * @param fila O ponteiro para a estrutura da fila.
 */
void exibirFila(const FilaPecas* fila) {
    printf("\n--- ESTADO ATUAL DA FILA (%d/%d) ---\n", fila->tamanho, CAPACIDADE_FILA);

    if (fila->tamanho == 0) {
        printf("Fila de pecas: [VAZIA]\n");
        return;
    }

    printf("Fila de pecas: ");
    
    int i = fila->frente;
    int count = 0;
    
    // Itera enquanto não percorrer todos os elementos da fila (tamanho)
    while (count < fila->tamanho) {
        printf("[%c %d]", fila->elementos[i].tipo, fila->elementos[i].id);
        
        // Adiciona uma seta para separar os elementos
        if (count < fila->tamanho - 1) {
            printf(" -> ");
        }

        // Move para o próximo índice de forma circular
        i = (i + 1) % CAPACIDADE_FILA;
        count++;
    }
    printf("\n");
    printf("--- FIM DA FILA ---\n");
}


// --- Função Principal (Main) ---

int main() {
    // Inicializa o gerador de números aleatórios para gerar tipos de peças
    srand(time(NULL));

    FilaPecas fila;
    inicializarFila(&fila);

    // Requisito: Inicializar a fila de peças com um número fixo de elementos (5)
    preencherFilaInicial(&fila, CAPACIDADE_FILA);

    int opcao;

    do {
        // Exibe o estado da fila antes da ação (Usabilidade)
        exibirFila(&fila);

        printf("\nOpcoes de acao:\n");
        printf("1. Jogar peca (dequeue)\n");
        printf("2. Inserir nova peca (enqueue)\n");
        printf("0. Sair\n");
        printf("Digite o codigo da acao: ");
        
        // Entrada de dados
        if (scanf("%d", &opcao) != 1) {
            // Tratamento de erro de entrada (ex: digitar uma letra)
            printf("\nOpcao invalida. Por favor, digite um numero.\n");
            // Limpa o buffer de entrada para evitar loop infinito
            while (getchar() != '\n');
            continue;
        }

        // Operadores Lógicos e Condicionais para controle de fluxo
        switch (opcao) {
            case 1:
                jogarPeca(&fila);
                break;
            case 2:
                inserirPeca(&fila);
                break;
            case 0:
                printf("\n👋 Saindo do Tetris Stack Simulator. Ate logo!\n");
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }
        
        // Pausa para visualização (melhora a usabilidade no terminal)
        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            // Limpa o buffer antes de esperar o ENTER
            while (getchar() != '\n'); 
            getchar(); // Espera a tecla ENTER
        }

    } while (opcao != 0);

    return 0;
}