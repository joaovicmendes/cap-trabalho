// Trabalho 1: Caça-Palavras Simples
// Aluno: João Victor Mendes Freire
// RA: 758943
// Turma: CAP - A
// Prof. Ednaldo Brigante Pizzolato

// ENTRADA: vazio
// SAÍDA: imprime a matriz caça-palavras, as palavras a serem procuradas, as encontradas e as não encontradas,
                                                                    //e o status (valido/invalido) da matriz (transforma todas as letras em minusculas)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// ALTERAR TAMANHO DA MATRIZ E DAS PALAVRAS AQUI
#define LIN_MAT 10    // Quantidade de linhas da matriz com as letras, > 0
#define COL_MAT 10    // Quantidade de colunas da matriz com as letras, > 0 
#define TAM_MAX 20    // Tamanho máximo de cada palavra, > 0
#define QTD_PAL 10    // Quantidade de palavras, > 0
#define TRUE 1
#define FALSE 0

// Protótipo das funções e procedimentos
int procuraPalavra(char matriz[LIN_MAT][COL_MAT], int numeroPalavra, char palavra[numeroPalavra][TAM_MAX]);

void imprimirMatriz(char matriz[LIN_MAT][COL_MAT]);

void imprimePalavra (char palavras[QTD_PAL][TAM_MAX], int *vetor, int campo);

void cabecalho(char matriz[LIN_MAT][COL_MAT], char palavras[QTD_PAL][TAM_MAX]);

int main() {
    setlocale(LC_ALL, "");

    // Letras serão transformadas em minúsculas para facilitar na busca, tanto no caça-palavras quanto na palavra em sí

    // Substituir esta matriz, bem como os valores pré-processados, se necessário
    char matriz[LIN_MAT][COL_MAT] =  {{'G','a','D','o','p','a','t','o','g','m'},
                                      {'m','a','r','r','e','c','o','o','a','a'},
                                      {'g','a','L','o','e','f','g','h','n','c'},
                                      {'r','a','t','o','e','f','g','h','S','a'},
                                      {'c','o','e','l','h','o','g','h','o','c'},
                                      {'a','b','c','d','e','f','g','a','n','o'},
                                      {'a','u','r','u','b','u','g','h','i','j'},
                                      {'a','a','c','a','b','r','a','h','i','j'},
                                      {'a','t','m','a','r','r','e','c','o','o'},
                                      {'a','o','g','a','M','o','g','h','i','j'}};

    // Substituir esta matriz, bem como os valores pré-processados, se necessário
    char palavras[QTD_PAL][TAM_MAX] = {"Gato", "pAto", "ganso", "marreco", "galo", "macaco", "rato", "coelho", "urubu","cabra"};

    int status[TAM_MAX];                 // Guarda a condição de existencia da palavra na matriz [TRUE/FALSE]
    int numPal;                          // Índice da palavra (número da linha na matriz 'palavras[][]') que esta sendo procurada (0 até (QTD_PAL - 1))
    int achou;                           // Controla se a palavra que está sendo buscada foi achada
    int valida;                          // Guarda se a matriz é válida (todas as palavras nela) ou não


    printf(">>INÍCIO\n");

    cabecalho(matriz, palavras);


    valida = TRUE;                                    // Caça palavras é considerado válido inicalmente
    for (numPal = 0; numPal < QTD_PAL; numPal++) {    // Para cada palavra, a busca deve ser realizada

        achou = procuraPalavra(matriz, numPal, palavras);
        status[numPal] = achou;

        if (!achou)                                   // Se não encontrou pelo menos uma palavra, o caça palavras é inválido
            valida = FALSE;
    }

    imprimePalavra(palavras, status, TRUE);
    imprimePalavra(palavras, status, FALSE);

    if (valida)
        printf("O caça-palavras é VÁLIDO\n");
    else
        printf("O caça-palavras é INVÁLIDO\n");

    printf(">>FIM\n");

}

// Função que busca a palavra
int procuraPalavra(char matriz[LIN_MAT][COL_MAT], int numeroPalavra, char palavra[QTD_PAL][TAM_MAX]){
    int i, j, k;
    int achou, procurouTudo, diferente;
    unsigned long int tamPalavra;

    i = 0;
    achou = FALSE;
    procurouTudo = FALSE;
    tamPalavra = strlen(palavra[numeroPalavra]);

    // Repetir o loop até encontrar a palavra ou até chegar no fim da matriz
    while (!achou && !procurouTudo) {

        // Loop que controla a linha de busca na matriz
        while (!achou && i < LIN_MAT){
            j = 0;

            // Loop que controla a coluna de busca na matriz
            while(!achou && j < COL_MAT) {
                diferente = FALSE;
                k = -1;

                // Procura de cada letra na horiozntal
                while (!diferente && ++k < tamPalavra) {
                    if (palavra[numeroPalavra][k] == matriz[i][j + k])
                        diferente = FALSE;
                    else
                        diferente = TRUE;
                }


                // Se chegou no fim e nenhuma letra foi diferente, encontrou
                if (k == tamPalavra && !diferente)
                    achou = TRUE;

                // Se não, procura na vertical
                else {
                    diferente = FALSE;
                    k = -1;

                    // Procura de cada letra na vertical
                    while (!diferente && ++k < tamPalavra) {
                        if (palavra[numeroPalavra][k] == matriz[i + k][j])
                            diferente = FALSE;
                        else
                            diferente = TRUE;
                    }
                    if (k == tamPalavra && !diferente)
                        achou = TRUE;
                }
                j++;
            }

            i++;
            if (i == LIN_MAT && j == COL_MAT)
                procurouTudo = TRUE;
        }

    }

    return achou;
}

// Procedimento que imprime o cabeçalho do programa
void cabecalho(char matriz[LIN_MAT][COL_MAT], char palavras[QTD_PAL][TAM_MAX]) {
    int i, j;

    printf("=== CAÇA PALAVRAS ===\n");
    imprimirMatriz(matriz);
    printf("=====================\n");
    printf("Palavras para buscar:\n");
    for (i = 0; i < QTD_PAL; i++) {

        // Converte letras maiusculas em minusculas
        for (j = 0; j < TAM_MAX; j++)
            if (palavras[i][j] >= 'A' && palavras[i][j] <= 'Z')
                palavras[i][j] += 32;

        printf("> %s\n", palavras[i]);

    }
    printf("=====================\n\n");
}

// Procedimento que imprime a matriz original
void imprimirMatriz(char matriz[LIN_MAT][COL_MAT]) {
    int i, j;

    for (i = 0; i < LIN_MAT; i++) {
        for (j = 0; j < COL_MAT; j++) {

            // Converte maiusculas em minusculas
            if (matriz[i][j] >= 'A' && matriz[i][j] <= 'Z')
                matriz[i][j] += 32;

            printf("%2c", matriz[i][j]);
        }
        printf("\n");
    }
}

// Procedimento que imprime as palavras
void imprimePalavra (char palavras[QTD_PAL][TAM_MAX], int *vetor, int campo) {
    int i;

    if (campo == TRUE) {
        printf("Encontrada(s) no caça-palavras:\n");

        for (i = 0; i < QTD_PAL; i++) {
            if (vetor[i] == 1)
                printf("+ %s\n", palavras[i]);
        }
        printf("\n");
    }

    else if (campo == FALSE) {
        printf("Não enocntrada(s) no caça-palavras:\n");

        for (i = 0; i < QTD_PAL; i++) {
            if (vetor[i] == 0)
                printf("- %s\n", palavras[i]);
        }
        printf("\n");
    }
}
