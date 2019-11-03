#include <stdio.h>
#include <stdlib.h>

int main() {
  int i = 0, j = 0;
  int resp[15], esperados[15];
  double nota;
  FILE * fp, * fp2, * fp3;
  fp = fopen("respostas.txt", "r");
  fp2 = fopen("respostas_esperadas.txt", "r");
  fp3 = fopen("respostas_comparadas.txt", "a+");

  // Verificação se a abertura foi feita corretamente
  if (!fp || !fp2 || !fp3)
  {
    printf("Erro!");
    exit(1);
  }

  // Nota se inicia em 0
  nota = 0;
  for (i = 0; i < 15; i++)
  {
    // Captura resposta gerada e esperada
    fscanf(fp, "%d", &resp[i]);
    fscanf(fp2, "%d", &esperados[i]);

    // Se as duas forem iguais, soma-se 0.5 a nota e imprime 1 no arquivo respostas_comparadas.txt, indicando igualdade
    if (resp[i] == esperados[i])
    {
      nota += 0.5;
      fprintf(fp3, "1\n");
    }
    // Do contrario, não se acrescenta anda a nota e imprime 0 no arquivo respostas_comparadas.txt, indicando diferença
    else
      fprintf(fp3, "0\n");
  }

  fclose(fp);
  fclose(fp2);
  fclose(fp3);

  printf("Nota (sem comentário + identação): %.1lf\n", nota); // Nota dos casos de teste (min: 0/max: 7.5)
  printf("Nota (com comentário + identação): %.1lf\n", nota + 2.5); // Nota dos casos de teste com mais 2.5 da indentação/comentário (min: 2.5/max: 10)

  return 0;
}
