#include <stdio.h>
#include <stdlib.h> // Para a função abs()

#define TAMANHO_TABULEIRO 10 // Tamanho do tabuleiro 10x10
#define NAVIO 3           // Valor para representar um navio no tabuleiro
#define AGUA 0            // Valor para representar água no tabuleiro
#define HABILIDADE 5      // Valor para representar a área afetada por uma habilidade
#define TAMANHO_HABILIDADE 5 // Tamanho das matrizes de habilidade (5x5)

// Matriz que representa o tabuleiro do jogo
int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];

// Matrizes que representam as áreas de efeito das habilidades
int habilidadeCone[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];
int habilidadeCruz[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];
int habilidadeOctaedro[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];

// Função para inicializar o tabuleiro com água (valor 0)
void inicializarTabuleiro() {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = AGUA; // Preenche cada célula do tabuleiro com 0 (água)
        }
    }
}

// Função para posicionar um navio no tabuleiro
// Mantida do nível anterior, com verificação de sobreposição
int posicionarNavio(int linhaInicial, int colunaInicial, int tamanho, int direcao) {
    if (direcao == 0) { // Horizontal
        if (colunaInicial + tamanho > TAMANHO_TABULEIRO) return 0; // Verifica se o navio cabe no tabuleiro
        for (int i = 0; i < tamanho; i++) {
            if (tabuleiro[linhaInicial][colunaInicial + i] == NAVIO) return 0; // Verifica sobreposição
        }
        for (int i = 0; i < tamanho; i++) {
            tabuleiro[linhaInicial][colunaInicial + i] = NAVIO; // Posiciona o navio
        }
        return 1;
    } else if (direcao == 1) { // Vertical
        if (linhaInicial + tamanho > TAMANHO_TABULEIRO) return 0; // Verifica se o navio cabe no tabuleiro
        for (int i = 0; i < tamanho; i++) {
            if (tabuleiro[linhaInicial + i][colunaInicial] == NAVIO) return 0; // Verifica sobreposição
        }
        for (int i = 0; i < tamanho; i++) {
            tabuleiro[linhaInicial + i][colunaInicial] = NAVIO; // Posiciona o navio
        }
        return 1;
    }
    return 0; // Se a direção não for horizontal nem vertical, retorna 0 (erro)
}

// Função para criar a matriz de habilidade Cone
void criarHabilidadeCone() {
    int centro = TAMANHO_HABILIDADE / 2; // Calcula o centro da matriz (2 para 5x5)
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            // A área afetada pelo cone aumenta à medida que a linha aumenta
            if (j >= centro - i && j <= centro + i) {
                habilidadeCone[i][j] = 1; // 1 representa a área afetada
            } else {
                habilidadeCone[i][j] = 0; // 0 representa a área não afetada
            }
        }
    }
}

// Função para criar a matriz de habilidade Cruz
void criarHabilidadeCruz() {
    int centro = TAMANHO_HABILIDADE / 2; // Calcula o centro da matriz
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            if (i == centro || j == centro) { // A linha central e a coluna central são afetadas
                habilidadeCruz[i][j] = 1;
            } else {
                habilidadeCruz[i][j] = 0;
            }
        }
    }
}

// Função para criar a matriz de habilidade Octaedro (Losango)
void criarHabilidadeOctaedro() {
    int centro = TAMANHO_HABILIDADE / 2; // Calcula o centro da matriz
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            // Usa a distância de Manhattan para definir a forma do losango
            if (abs(i - centro) + abs(j - centro) <= centro) {
                habilidadeOctaedro[i][j] = 1;
            } else {
                habilidadeOctaedro[i][j] = 0;
            }
        }
    }
}

// Função para aplicar uma habilidade ao tabuleiro
void aplicarHabilidade(int linhaOrigem, int colunaOrigem, int habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    int centroHabilidade = TAMANHO_HABILIDADE / 2; // Centro da matriz de habilidade
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            if (habilidade[i][j] == 1) { // Se a posição na matriz de habilidade estiver ativa
                int linhaTabuleiro = linhaOrigem - centroHabilidade + i; // Calcula a linha no tabuleiro
                int colunaTabuleiro = colunaOrigem - centroHabilidade + j; // Calcula a coluna no tabuleiro
                if (linhaTabuleiro >= 0 && linhaTabuleiro < TAMANHO_TABULEIRO &&
                    colunaTabuleiro >= 0 && colunaTabuleiro < TAMANHO_TABULEIRO) {
                    // Se a posição estiver dentro do tabuleiro, marca como habilidade
                    if(tabuleiro[linhaTabuleiro][colunaTabuleiro] != NAVIO) // Não sobrescreve navios
                        tabuleiro[linhaTabuleiro][colunaTabuleiro] = HABILIDADE;
                }
            }
        }
    }
}

// Função para exibir o tabuleiro
void exibirTabuleiro() {
    printf("  "); // Espaço para alinhar os números das colunas
    for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf("%d ", j); // Imprime os números das colunas
    }
    printf("\n");
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%d ", i); // Imprime os números das linhas
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            if (tabuleiro[i][j] == AGUA) {
                printf("0 "); // Imprime 0 para água
            } else if (tabuleiro[i][j] == NAVIO) {
                printf("3 "); // Imprime 3 para navio
            } else if (tabuleiro[i][j] == HABILIDADE) {
                printf("5 "); // Imprime 5 para habilidade
            }
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    // Inicializa o tabuleiro com água
    inicializarTabuleiro();

    // Posiciona alguns navios para exemplo
    posicionarNavio(2, 2, 3, 0); // Navio horizontal
    posicionarNavio(6, 7, 3, 1); // Navio vertical

    // Exibe o tabuleiro inicial com os navios
    printf("Tabuleiro inicial com navios:\n");
    exibirTabuleiro();

    // Cria as matrizes de habilidade
    criarHabilidadeCone();
    criarHabilidadeCruz();
    criarHabilidadeOctaedro();

    // Exibe as matrizes de habilidade (para depuração e visualização)
    printf("\nMatriz de habilidade Cone:\n");
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            printf("%d ", habilidadeCone[i][j]);
        }
        printf("\n");
    }

    printf("\nMatriz de habilidade Cruz:\n");
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            printf("%d ", habilidadeCruz[i][j]);
        }
        printf("\n");
    }

    printf("\nMatriz de habilidade Octaedro:\n");
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            printf("%d ", habilidadeOctaedro[i][j]);
        }
        printf("\n");
    }

    // Aplica as habilidades ao tabuleiro
    printf("\nAplicando habilidade Cone na posição (1, 2):\n");
    aplicarHabilidade(1, 2, habilidadeCone);
    exibirTabuleiro(); // Exibe o tabuleiro após aplicar o cone

    printf("\nAplicando habilidade Cruz na posição (7, 5):\n");
    aplicarHabilidade(7, 5, habilidadeCruz);
    exibirTabuleiro(); // Exibe o tabuleiro após aplicar a cruz

    printf("\nAplicando habilidade Octaedro na posição (4, 8):\n");
    aplicarHabilidade(4, 8, habilidadeOctaedro);
    exibirTabuleiro(); // Exibe o tabuleiro após aplicar o octaedro

    return 0; // Indica que o programa terminou com sucesso
}


