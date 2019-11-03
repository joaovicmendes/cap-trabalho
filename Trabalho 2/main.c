// Trabalho 1: Caça-Palavras Simples
// Trabalho 2: Caça-Palavras Simples + Arquivo
// Aluno: João Victor Mendes Freire
// RA: 758943
// Turma: CAP - A
// Prof. Ednaldo Brigante Pizzolato

// ENTRADA: vazio
// SAÍDA: imprime a matriz caça-palavras, as palavras a serem procuradas, as encontradas e as não encontradas, e o status (valido/invalido) da matriz (transforma todas as letras em minusculas)

// ENTRADA trabalho2: passagem de três arquivos por parametro (matriz do caça palavras, matriz das palavras, arquivo para armazenar as respostas)
// SAÍDA trabalho2: imprimi o status (1/0) no terceiro arquivo que é passado por parametro

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define LIN_MAT 20    // Quantidade de linhas da matriz com as letras, > 0
#define COL_MAT 20    // Quantidade de colunas da matriz com as letras, > 0
#define TAM_MAX 50    // Tamanho máximo de cada palavra, > 0
#define QTD_PAL 25    // Quantidade máxima de palavras, > 0
#define TRUE 1
#define FALSE 0

// Protótipo das funções e procedimentos
int procuraPalavra(char matriz[LIN_MAT][COL_MAT], int numeroPal, char palavra[numeroPal][TAM_MAX]);

void imprimirMatriz(char matriz[LIN_MAT][COL_MAT], int linhas, int colunas);

void imprimirPal (char palavras[QTD_PAL][TAM_MAX], int qtdPal, int *vetor, int campo);

void cabecalho(char matriz[LIN_MAT][COL_MAT], int linhas, int colunas, char palavras[QTD_PAL][TAM_MAX], int qtdPal);

int main(int argc, char * argv[]) {
    setlocale(LC_ALL, "");

    // Letras serão transformadas em minúsculas para facilitar na busca, tanto no caça-palavras quanto na palavra em sí

    // matriz que deve conter a matriz de teste com valor maximo de linha/colunas de 50
    char matriz[LIN_MAT][COL_MAT];

    // matriz que deve conter as palvras de teste
    char palavras[QTD_PAL][TAM_MAX];

    int i = 0, j = 0;
    int status[TAM_MAX];                 // Guarda a condição de existencia da palavra na matriz [TRUE/FALSE]
    int numPal;                          // Índice da palavra (número da linha na matriz 'palavras[][]') que esta sendo procurada (0 até (QTD_PAL - 1))
    int achou;                           // Controla se a palavra que está sendo buscada foi achada
    int valida;                          // Guarda se a matriz é válida (todas as palavras nela) ou não
    int caracter;
    int linhas = 10, colunas = 10, qtdPal = 10;
    int esperado;
    FILE *fp;

    // INÍCIO ABERTURA E LEITURA ARQUIVOS --
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Erro no primeiro arquivo");
        exit(1);
    }

    // Leitura dos valores do arquivo para a matriz caça-palavras
    caracter = fgetc(fp);
    while (!feof(fp)) {
      if (caracter != ' ') {
        if (caracter != '\n') {
          matriz[i][j] = caracter;
          j++;
          colunas = j;
        }
        else {
          i++;
          j = 0;
        }
      }
      caracter = fgetc(fp);
    }
    linhas = i;
    fclose(fp);


    i = 0;
    j = 0;

    fp = fopen(argv[2], "r");
    if (fp == NULL) {
        printf("Erro no segundo arquivo");
        exit(1);
    }

    // Leitura dos valores do arquivo para a matriz palavras
    caracter = fgetc(fp);
    while (!feof(fp)) {
      if (caracter != ' ') {
        if (caracter != '\n') {
          palavras[i][j] = caracter;
          j++;
        }
        else {
          palavras[i][j] = '\0';
          i++;
          j = 0;
        }
      }
      caracter = fgetc(fp);
    }
    qtdPal = i;
    fclose(fp);

    // -- FIM ABERTURA E LEITURA ARQUIVOS


    // IMPRIMIR MATRIZ E PALAVRA PARA CONFERIR:
    // cabecalho(matriz, linhas, colunas, palavras, qtdPal);

    valida = TRUE;                                    // Caça palavras é considerado válido inicalmente
    for (numPal = 0; numPal < qtdPal; numPal++) {     // Para cada palavra, a busca deve ser realizada

        achou = procuraPalavra(matriz, numPal, palavras);
        status[numPal] = achou;

        if (!achou)                                   // Se não encontrou pelo menos uma palavra, o caça palavras é inválido
            valida = FALSE;
    }

    // IMPRIMIR PALAVRAS ENCONTRADAS E NAO ENCONTRADAS PARA CONFERIR:
    //imprimirPal(palavras, qtdPal, status, TRUE);
    //imprimirPal(palavras, qtdPal, status, FALSE);


    fp = fopen(argv[3], "a+");

    if (valida)
        fprintf(fp, "1\n");
    else
        fprintf(fp, "0\n");

    fclose(fp);

}

// Função que busca a palavra
int procuraPalavra(char matriz[LIN_MAT][COL_MAT], int numeroPal, char palavra[QTD_PAL][TAM_MAX]){
    int i, j, k;
    int achou, procurouTudo, diferente;
    unsigned long int tamPal;

    i = 0;
    achou = FALSE;
    procurouTudo = FALSE;
    tamPal = strlen(palavra[numeroPal]);

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
                while (!diferente && ++k < tamPal) {
                    if (palavra[numeroPal][k] == matriz[i][j + k])
                        diferente = FALSE;
                    else
                        diferente = TRUE;
                }


                // Se chegou no fim e nenhuma letra foi diferente, encontrou
                if (k == tamPal && !diferente)
                    achou = TRUE;

                // Se não, procura na vertical
                else {
                    diferente = FALSE;
                    k = -1;

                    // Procura de cada letra na vertical
                    while (!diferente && ++k < tamPal) {
                        if (palavra[numeroPal][k] == matriz[i + k][j])
                            diferente = FALSE;
                        else
                            diferente = TRUE;
                    }
                    if (k == tamPal && !diferente)
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
void cabecalho(char matriz[LIN_MAT][COL_MAT], int linhas, int colunas, char palavras[QTD_PAL][TAM_MAX], int qtdPal) {
    int i, j;

    printf("=== CAÇA PALAVRAS ===\n");
    imprimirMatriz(matriz, linhas, colunas);
    printf("=====================\n");
    printf("Palavras para buscar:\n");
    for (i = 0; i < qtdPal; i++) {

        // Converte letras maiusculas em minusculas
        for (j = 0; j < TAM_MAX; j++)
            if (palavras[i][j] >= 'A' && palavras[i][j] <= 'Z')
                palavras[i][j] += 32;

        printf("> %s\n", palavras[i]);

    }
    printf("=====================\n\n");
}

// Procedimento que imprime a matriz original
void imprimirMatriz(char matriz[LIN_MAT][COL_MAT], int linhas, int colunas) {
    int i, j;

    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {

            // Converte maiusculas em minusculas
            if (matriz[i][j] >= 'A' && matriz[i][j] <= 'Z')
                matriz[i][j] += 32;

            printf("%2c", matriz[i][j]);
        }
        printf("\n");
    }
}

// Procedimento que imprime as palavras
void imprimirPal (char palavras[QTD_PAL][TAM_MAX], int qtdPal, int *vetor, int campo) {
    int i;

    if (campo == TRUE) {
        printf("Encontrada(s) no caça-palavras:\n");

        for (i = 0; i < qtdPal; i++) {
            if (vetor[i] == 1)
                printf("+ %s\n", palavras[i]);
        }
        printf("\n");
    }

    else if (campo == FALSE) {
        printf("Não enocntrada(s) no caça-palavras:\n");

        for (i = 0; i < qtdPal; i++) {
            if (vetor[i] == 0)
                printf("- %s\n", palavras[i]);
        }
        printf("\n");
    }
}
