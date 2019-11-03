#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcaux.c"

#define XADREZ 2

int getData(int *salaid, int *playerid, int *outroplayerid, int *clicado, int *movendo, int *trocaPeao);// Salva as variùveis do form
void printPageJogo(int salaid, int playerid, int outroplayerid, int turno, int player, int ganhador, int matriz[8][8], int mov[8][8], int movendo, int trocaPeao);// Envia a pùgina do jogo
int lerSalaJogo (int *player, int salaid, int *jogoescolhido, int *turno, int *playerid, int *outroplayerid, int matriz[8][8], int *ganhador, FILE *fp);// Lù o arquivo da sala
int getMovel(int i, int j, int matriz[8][8], int player, FILE *fp);// Define quais peùas podem ser movidas
int getMovelUnico(int i, int j, int matriz[8][8], int player);// Define se uma peùa pode ser movida para uma casa especifica
void getMovelPeca(int i, int j, int matriz[8][8], int player, int mov[8][8], FILE *fp);// Define quais as possiveis posiùùes para uma peùa em todas as direùùes e possibilidades
void getMovelPecaAux(int i, int j, int x, int y, int mov[8][8], int matriz[8][8], int player);// Funùùo auxiliar que salva n possivel posiùùes de uma peùa
void getMovelPecaAux2(int i, int j, int x, int y, int mov[8][8], int matriz[8][8], int player);// Funùùo auxiliar que salva uma possivel posiùùes de uma peùa
void definePecaMovel(int mov[8][8], int matriz[8][8], int player, FILE *fp);// Define quais as possiveis posiùùes para uma peùa em uma reta
void salvaTurnoMatriz(FILE *fp, int *turno, int matriz[8][8]);// Funùùo auxiliar que escreve o turno e a matriz no arquivo da sala
void pecaClicada(int mov[8][8], int matriz[8][8], int *movendo, int clicado, int player, FILE *fp);// Funùùo auxiliar que define quais imagens serùo botùes apùs a escolha de uma peùa
int getTorreEstado(FILE *fp, int torre);// Retorna o estado de uma torre
int getReiEstado(FILE *fp, int rei);// Retorna o estado do rei
int getEnPassant(FILE *fp);// Retorna o ùltimo peùo que se moveu
void setTorreEstado(FILE *fp, int torre, int n);// Define o estado da torre
void setReiEstado(FILE *fp, int rei, int n);// Define o estado do rei
void setEnPassant(FILE *fp, int clicado);// Define o ùltimo peùo que se moveu
void fazRoque(int x1, int x2, int y1, int y2, int p1, int p2, int i1, int i2, int movendo, int clicado, FILE *fp, int *turno, int matriz[8][8]);// Faz as trocas necessùrias para um roque

int main() {
	int salaid;
	int playerid;
	int outroplayerid;

	int player;
	int turno;

	int jogoescolhido;

	int clicado;
	int movendo;
	int movendoAux;
	int trocaPeao;

	int ganhador = 0;

	int matriz[8][8] = {{21, 22, 23, 24, 25, 23, 22, 21}, {20, 20, 20, 20, 20, 20, 20, 20}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {10, 10, 10, 10, 10, 10, 10, 10}, {11, 12, 13, 14, 15, 13, 12, 11}};
	int mov[8][8] = {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}};

	int i, j, k;
	char aux[17];
	FILE *fp;

	if(!getData(&salaid, &playerid, &outroplayerid, &clicado, &movendo, &trocaPeao)) {
		sprintf(aux, "salas/%d.log", salaid);
		fp = fopen(aux, "rb+");
		if (fp == NULL) {
			printErroPage(4);
			return 1;
		}
		if (!lerSalaJogo(&player, salaid, &jogoescolhido, &turno, &playerid, &outroplayerid, matriz, &ganhador, fp)) {
			if (turno == player && ganhador == 0) {
				if (clicado == -1 && movendo == -1 && trocaPeao == -1) {// Comeùo do turno
					definePecaMovel(mov, matriz, player, fp);
				} else if (clicado != -1 && movendo == -1 && trocaPeao == -1) {// Clicou em uma peùa
					pecaClicada(mov, matriz, &movendo, clicado, player, fp);
				} else if (clicado != -1 && movendo != -1 && trocaPeao == -1) {// Clicou em uma peùa e jù tinha clicado em outra
					k = getMovelUnico(clicado/8, clicado%8, matriz, player);
					if (k) {// Peùa do outro player ou vazio
						movendoAux = movendo;
						if  (matriz[clicado/8][clicado%8] == 15 && player == 2) {
							ganhador = 2;
						} else if (matriz[clicado/8][clicado%8] == 25 && player == 1) {// Comeu o rei
							ganhador = 1;
						} else  if (matriz[movendo/8][movendo%8] == 10 && clicado <= 7 && player == 1) {
							trocaPeao = 1;
							movendo = clicado;
						} else  if (matriz[movendo/8][movendo%8] == 20 && clicado >= 56 && player == 2) {
							trocaPeao = 2;
							movendo = clicado;
						} else {
							turno = turno%2+1;
						}

						// Moveu uma torre
						if (movendo == 0 && matriz[movendo/8][movendo%8] == 21) {
							setTorreEstado(fp, 0, 0);
						} else if (movendo == 7 && matriz[movendo/8][movendo%8] == 21) {
							setTorreEstado(fp, 1, 0);
						} else if (movendo == 56 && matriz[movendo/8][movendo%8] == 11) {
							setTorreEstado(fp, 2, 0);
						} else if (movendo == 63 && matriz[movendo/8][movendo%8] == 11) {
							setTorreEstado(fp, 3, 0);
						}

						// Moveu um peùo
						if ((matriz[movendo/8][movendo%8] == 10 && clicado >= 32 && clicado <= 39) || (matriz[movendo/8][movendo%8] == 20 && clicado >= 24 && clicado <= 31)) {
							setEnPassant(fp, clicado);
						}

						// En passant
						if (matriz[movendo/8][movendo%8] == 10 && matriz[(clicado/8)+1][clicado%8] == 20 && getEnPassant(fp) ==  clicado+8) {
							matriz[clicado/8][clicado%8] = matriz[movendo/8][movendo%8];
							matriz[movendo/8][movendo%8] = 0;
							matriz[(clicado/8)+1][clicado%8] = 0;
						} else if (matriz[movendo/8][movendo%8] == 20 && matriz[(clicado/8)-1][clicado%8] == 10 && getEnPassant(fp) ==  clicado-8) {
							matriz[clicado/8][clicado%8] = matriz[movendo/8][movendo%8];
							matriz[movendo/8][movendo%8] = 0;
							matriz[(clicado/8)-1][clicado%8] = 0;
						} else {
							matriz[clicado/8][clicado%8] = matriz[movendoAux/8][movendoAux%8];
							matriz[movendoAux/8][movendoAux%8] = 0;
						}

						salvaTurnoMatriz(fp, &turno, matriz);

						if (ganhador != 0) {
							fwrite(&ganhador, sizeof(int), 1, fp);
						}
					} else {// Sua peùa
						if (movendo == 4 && clicado == 0 && getTorreEstado(fp, 0) && getReiEstado(fp, 0) && player == 2) {
							fazRoque(2, 3, 0, 0, 25, 21, 0, 0, movendo, clicado, fp, &turno, matriz);
						} else if (movendo == 4 && clicado == 7 && getTorreEstado(fp, 1) && getReiEstado(fp, 0) && player == 2) {
							fazRoque(6, 5, 0, 0, 25, 21, 1, 0, movendo, clicado, fp, &turno, matriz);
						} else if (movendo == 60 && clicado == 56 && getTorreEstado(fp, 2) && getReiEstado(fp, 1) && player == 1) {
							fazRoque(2, 3, 7, 7, 15, 11, 2, 1, movendo, clicado, fp, &turno, matriz);
						} else if (movendo == 60 && clicado == 63 && getTorreEstado(fp, 3) && getReiEstado(fp, 1) && player == 1) {
							fazRoque(6, 5, 7, 7, 15, 11, 3, 1, movendo, clicado, fp, &turno, matriz);
						} else {
							pecaClicada(mov, matriz, &movendo, clicado, player, fp);
						}
					}
				} else if (trocaPeao > 0) {// Terminou seu turno mas tem que escolher a troca do peao
					matriz[movendo/8][movendo%8] = trocaPeao;
					trocaPeao = -1;
					turno = turno%2+1;

					salvaTurnoMatriz(fp, &turno, matriz);
				}
			}
			printPageJogo(salaid, playerid, outroplayerid, turno, player, ganhador, matriz, mov, movendo, trocaPeao);
		} else {
			printPageJogoErrado(salaid, playerid, outroplayerid, nomejogo[jogoescolhido-1]);
		}
		fclose(fp);
	} else {
		printErroPage(2);
	}
	return 0;
}
// Salva as variùveis do form
int getData(int *salaid, int *playerid, int *outroplayerid, int *clicado, int *movendo, int *trocaPeao) {
	char *data = getenv("QUERY_STRING");
	char *aux;
	char aux2[7];
	int i;

	if (data == NULL) {
		return 1;
	}

	*salaid = getVar(data, "salaid=", 7);
	*playerid = getVar(data, "playerid=", 9);
	*outroplayerid = getVar(data, "outroplayerid=", 14);
	*clicado = getVar(data, "img=", 4);
	*movendo = getVar(data, "mov=", 4);
	*trocaPeao = getVar(data, "peao=", 5);

	if (*salaid == -1 || *playerid == -1 || *outroplayerid == -1) {
		return 1;
	}

	return 0;
}
// Envia a pùgina do jogo
void printPageJogo(int salaid, int playerid, int outroplayerid, int turno, int player, int ganhador, int matriz[8][8], int mov[8][8], int movendo, int trocaPeao) {
	int i, j;
	printPageBase("Xadrez");
	if (turno != player && ganhador == 0 && trocaPeao == -1) {//Printa a tag de refresh se nùo ù seu turno
		printf("<meta http-equiv=\"refresh\" content=\"%d; url=?salaid=%d&playerid=%d&outroplayerid=%d\">\n", 3, salaid, playerid, outroplayerid);
	}
	printPageMid("XADREZ");

	printf("<div class=\"mensagem\">");

	if (ganhador == 0) {// Informa de quem ù o turno
		if (turno == player) {
			printf("<p>Seu turno</p>\n");
		} else {
			printf("<p>Turno do oponente</p>\n");
		}
	} else {//Se o jogo terminou, informa o ganhador
		if (ganhador == 3) {
			printf("<p>Empate!</p>\n");
		} else if (ganhador == player) {
			printf("<p>Vocù ganhou!</p>\n");
		} else {
			printf("<p>Vocù perdeu!</p>\n");
		}
		printf("<a href=\"http://cap.dc.ufscar.br/~758943/lobbymp.html\">Novo jogo</a>\n");
	}
	//Printa informaùùes sobre a sala
	printf("<form action=\"xadrez.cgi\" method=\"get\">\n");
	printf("<input type=\"hidden\" name=\"salaid\" value=\"%d\">\n", salaid);
	printf("<input type=\"hidden\" name=\"playerid\" value=\"%d\">\n", playerid);
	printf("<input type=\"hidden\" name=\"outroplayerid\" value=\"%d\">\n", outroplayerid);
	if (turno == player && movendo != -1) {
		printf("<input type=\"hidden\" name=\"mov\" value=\"%d\">\n", movendo);
	}
	if (trocaPeao != -1) {// Menu da troca do peùo
		printf("<div>\n");
		printf("<input type=\"submit\" name=\"peao\" value=\"%d\" style=\"background:url(http://cap.dc.ufscar.br/~758943/imagesXadrez/img_%d_0_0.jpg); width: 96px; height: 96px; color: #00000000; border: none\" />\n", trocaPeao*10+1, trocaPeao*10+1);
		printf("<input type=\"submit\" name=\"peao\" value=\"%d\" style=\"background:url(http://cap.dc.ufscar.br/~758943/imagesXadrez/img_%d_0_0.jpg); width: 96px; height: 96px; color: #00000000; border: none\" />\n", trocaPeao*10+2, trocaPeao*10+2);
		printf("<input type=\"submit\" name=\"peao\" value=\"%d\" style=\"background:url(http://cap.dc.ufscar.br/~758943/imagesXadrez/img_%d_0_0.jpg); width: 96px; height: 96px; color: #00000000; border: none\" />\n", trocaPeao*10+3, trocaPeao*10+3);
		printf("<input type=\"submit\" name=\"peao\" value=\"%d\" style=\"background:url(http://cap.dc.ufscar.br/~758943/imagesXadrez/img_%d_0_0.jpg); width: 96px; height: 96px; color: #00000000; border: none\" />\n", trocaPeao*10+4, trocaPeao*10+4);
		printf("</div>\n");
	}
	printf("<div>\n");
	if (player == 1) {
		printf("<table align=\"center\" class=\"jogo-velha\">\n");
		for (i = 0; i < 8; i++) {// Botùes em si
			printf("<tr>\n");
			for (j = 0; j < 8; j++) {
				printf("<td>\n");
				if (mov[i][j]%2 == 1 && turno == player && ganhador == 0) {
					printf("<input type=\"submit\" name=\"img\" value=\"%d\" style=\"background:url(http://cap.dc.ufscar.br/~758943/imagesXadrez/img_%d_%d_%d.jpg); width: 96px; height: 96px; color: #00000000; border: none\" />\n", i*8+j, matriz[i][j], (i+j)%2, mov[i][j]/2);
				} else {
					printf("<input style=\"background:url(http://cap.dc.ufscar.br/~758943/imagesXadrez/img_%d_%d_0.jpg); width: 96px; height: 96px; color: #00000000; border: none\" />\n", matriz[i][j], (i+j)%2);
				}
				printf("</td>\n");
			}
			printf("</tr>\n");
		}
	} else {
		printf("<table align=\"center\" class=\"jogo-velha\">\n");
		for (i = 7; i > -1; i--) {// Botùes em si
			printf("<tr>\n");
			for (j = 7; j > -1; j--) {
				printf("<td>\n");
				if (mov[i][j]%2 == 1 && turno == player && ganhador == 0) {
					printf("<input type=\"submit\" name=\"img\" value=\"%d\" style=\"background:url(http://cap.dc.ufscar.br/~758943/imagesXadrez/img_%d_%d_%d.jpg); width: 96px; height: 96px; color: #00000000; border: none\" />\n", i*8+j, matriz[i][j], (i+j)%2, mov[i][j]/2);
				} else {
					printf("<input style=\"background:url(http://cap.dc.ufscar.br/~758943/imagesXadrez/img_%d_%d_0.jpg); width: 96px; height: 96px; color: #00000000; border: none\" />\n", matriz[i][j], (i+j)%2);
				}
				printf("</td>\n");
			}
			printf("</tr>\n");
		}
	}
	printf("</table>\n");
	printf("</div>\n");
	printf("</form>\n");

	printf("</div>");

	printPageEnd();
}
// Lù o arquivo da sala
int lerSalaJogo (int *player, int salaid, int *jogoescolhido, int *turno, int *playerid, int *outroplayerid, int matriz[8][8], int *ganhador, FILE *fp) {
	fseek(fp, 0, SEEK_SET);
	*jogoescolhido = -1;

	int p1;

	salvaTime(&salaid);// Atualiza o tempo da sala
	p1 = getP1(&salaid);// Lù o P1 da sala
	if (*playerid == p1) {// Identifica qual ù o player
		*player = 1;
	} else {
		*player = 2;
	}

	fread(jogoescolhido, sizeof(int), 1, fp);

	if (*jogoescolhido == -1) {// O primeiro que escolha o jogo o define, nesse caso nada foi lido do arquivo
		srand(time(NULL));
		fseek(fp, 0, SEEK_SET);
		*jogoescolhido = 2;
		*turno = rand()%2+1;
		fwrite(jogoescolhido, sizeof(int), 1, fp);// Salva o jogo no arquivo
		fwrite(turno, sizeof(int), 1, fp);// Salva o turno no arquivo
		if (*player == 1) { //Salva os ids dos players no arquivo
			fwrite(playerid, sizeof(int), 1, fp);
			fwrite(outroplayerid, sizeof(int), 1, fp);
		} else {
			fwrite(outroplayerid, sizeof(int), 1, fp);
			fwrite(playerid, sizeof(int), 1, fp);
		}
		fwrite(matriz, 64*sizeof(int), 1, fp);// Salva a matriz do jogo no arquivo
		fwrite((int []){0, 1, 1, 1, 1, 1, 1, 100}, 8*sizeof(int), 1, fp);// Salva o ganhador, o estado das torres, o estado dos reis, e o estado do en passant no arquivo
		return 0;
	} else {
		if (*jogoescolhido == XADREZ) {// Se o jogo jù foi escolhido, lù as informaùùes do arquivo
			fread(turno, sizeof(int), 1, fp);
			fseek(fp, 2*sizeof(int), SEEK_CUR);
			fread(matriz, 64*sizeof(int), 1, fp);
			fread(ganhador, sizeof(int), 1, fp);
			return 0;
		} else {//Retorna erro se o jogo escolhido pelo segundo player foi diferente do escolhido pelo primeiro
			return 1;
		}
	}
}
// Define quais peùas podem ser movidas
int getMovel(int i, int j, int matriz[8][8], int player, FILE *fp) {
	int k;
	if (matriz[i][j] == 10*player) {// Peùo
		k = (player==1?-1:1);
		if (matriz[i+k][j] == 0 && (i+k) >= 0 && (i+k) <= 7) {
			return 1;
		} else if (((matriz[i+k][j+1] >= 20 && player == 1) || (matriz[i+k][j+1] <= 15 && player == 2)) && (i+k) >= 0 && (i+k) <= 7 && (j+1) <= 7) {
			return 1;
		} else if (((matriz[i+k][j-1] >= 20 && player == 1) || (matriz[i+k][j-1] <= 15 && player == 2)) && (i+k) >= 0 && (i+k) <= 7 && (j-1) >= 0) {
			return 1;
		} else if (((matriz[i][j-1] >= 20 && player == 1 && getEnPassant(fp) == i*8+j-1) || (matriz[i][j-1] <= 15 && player == 2 && getEnPassant(fp) == i*8+j-1)) && (j-1) >= 0) {
			return 1;
		} else if (((matriz[i][j+1] >= 20 && player == 1 && getEnPassant(fp) == i*8+j+1) || (matriz[i][j+1] <= 15 && player == 2 && getEnPassant(fp) == i*8+j+1)) && (j+1) <= 7) {
			return 1;
		}
	} else if (matriz[i][j] == 1+10*player) {// Torre
		if (getMovelUnico(i-1, j, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i+1, j, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i, j-1, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i, j+1, matriz, player)) {
			return 1;
		}
	} else if (matriz[i][j] == 2+10*player) {// Cavalo
		if (getMovelUnico(i-1, j-2, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i+1, j-2, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i-1, j+2, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i+1, j+2, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i-2, j-1, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i+2, j-1, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i-2, j+1, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i+2, j+1, matriz, player)) {
			return 1;
		}
	} else if (matriz[i][j] == 3+10*player) {// Bispo
		if (getMovelUnico(i-1, j-1, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i+1, j+1, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i+1, j-1, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i-1, j+1, matriz, player)) {
			return 1;
		}
	} else if (matriz[i][j] == 4+10*player) {// Rainha
		if (getMovelUnico(i-1, j, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i+1, j, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i, j-1, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i, j+1, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i-1, j-1, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i+1, j+1, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i+1, j-1, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i-1, j+1, matriz, player)) {
			return 1;
		}
	} else if (matriz[i][j] == 5+10*player) {// Rei
		if (getMovelUnico(i-1, j, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i+1, j, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i, j-1, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i, j+1, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i-1, j-1, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i+1, j+1, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i-1, j+1, matriz, player)) {
			return 1;
		} else if (getMovelUnico(i+1, j-1, matriz, player)) {
			return 1;
		}
	}
	return 0;
}
// Define se uma peùa pode ser movida para uma casa especifica
int getMovelUnico(int i, int j, int matriz[8][8], int player) {
	if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
		if (matriz[i][j] == 0) {
			return 1;
		}
		if (player == 1) {
			if (matriz[i][j] >= 20) {
				return 2;
			}
		} else  if (player == 2) {
			if (matriz[i][j] <= 15) {
				return 2;
			}
		}
	}
	return 0;
}
// Define quais as possiveis posiùùes para uma peùa em todas as direùùes e possibilidades
void getMovelPeca(int i, int j, int matriz[8][8], int player, int mov[8][8], FILE *fp) {
	int k, a;
	if (matriz[i][j] == 10*player) {// Peùo
		if (player == 1) {
			if (matriz[i-1][j] == 0 && (i-1) >= 0) {
				mov[i-1][j] = 3;
				if (matriz[i-2][j] == 0 && i == 6) {
					mov[i-2][j] = 3;
				}
			}
			if (matriz[i-1][j+1] >= 20 && (i-1) >= 0 && (j+1) <= 7) {
				mov[i-1][j+1] = 3;
			}
			if (matriz[i-1][j-1] >= 20 && (i-1) >= 0 && (j-1) >= 0) {
				mov[i-1][j-1] = 3;
			}
			if (matriz[i][j-1] >= 20 && (j-1) >= 0 && getEnPassant(fp) == i*8+j-1) {
				mov[i-1][j-1] = 3;
			}
			if (matriz[i][j+1] >= 20 && (j+1) <= 7 && getEnPassant(fp) == i*8+j+1) {
				mov[i-1][j+1] = 3;
			}
		} else {
			if (matriz[i+1][j] == 0 && (i+1) <= 7) {
				mov[i+1][j] = 3;
				if (matriz[i+2][j] == 0 && i == 1) {
					mov[i+2][j] = 3;
				}
			}
			if (matriz[i+1][j+1] <= 15 && matriz[i+1][j+1] != 0 && (i+1) <= 7 && (j+1) <= 7) {
				mov[i+1][j+1] = 3;
			}
			if (matriz[i+1][j-1] <= 15 && matriz[i+1][j-1] != 0 && (i+1) <= 7 && (j-1) >= 0) {
				mov[i+1][j-1] = 3;
			}
			if (matriz[i][j-1] <= 15 && (j-1) >= 0 && getEnPassant(fp) == i*8+j-1) {
				mov[i+1][j-1] = 3;
			}
			if (matriz[i][j+1] <= 15 && (j+1) <= 7 && getEnPassant(fp) == i*8+j+1) {
				mov[i+1][j+1] = 3;
			}
		}
	} else if (matriz[i][j] == 1+10*player) {// Torre
		getMovelPecaAux(i, j, -1, 0, mov, matriz, player);
		getMovelPecaAux(i, j, 1, 0, mov, matriz, player);
		getMovelPecaAux(i, j, 0, -1, mov, matriz, player);
		getMovelPecaAux(i, j, 0, 1, mov, matriz, player);
	} else if (matriz[i][j] == 2+10*player) {// Cavalo
		getMovelPecaAux2(i, j, -1, -2, mov, matriz, player);
		getMovelPecaAux2(i, j, 1, -2, mov, matriz, player);
		getMovelPecaAux2(i, j, -1, 2, mov, matriz, player);
		getMovelPecaAux2(i, j, 1, 2, mov, matriz, player);
		getMovelPecaAux2(i, j, -2, -1, mov, matriz, player);
		getMovelPecaAux2(i, j, 2, -1, mov, matriz, player);
		getMovelPecaAux2(i, j, -2, 1, mov, matriz, player);
		getMovelPecaAux2(i, j, 2, 1, mov, matriz, player);
	} else if (matriz[i][j] == 3+10*player) {// Bispo
		getMovelPecaAux(i, j, -1, -1, mov, matriz, player);
		getMovelPecaAux(i, j, 1, 1, mov, matriz, player);
		getMovelPecaAux(i, j, -1, 1, mov, matriz, player);
		getMovelPecaAux(i, j, 1, -1, mov, matriz, player);
	} else if (matriz[i][j] == 4+10*player) {// Rainha
		getMovelPecaAux(i, j, 1, 0, mov, matriz, player);
		getMovelPecaAux(i, j, -1, 0, mov, matriz, player);
		getMovelPecaAux(i, j, 0, 1, mov, matriz, player);
		getMovelPecaAux(i, j, 0, -1, mov, matriz, player);
		getMovelPecaAux(i, j, 1, 1, mov, matriz, player);
		getMovelPecaAux(i, j, -1, -1, mov, matriz, player);
		getMovelPecaAux(i, j, 1, -1, mov, matriz, player);
		getMovelPecaAux(i, j, -1, 1, mov, matriz, player);
	} else if (matriz[i][j] == 5+10*player) {// Rei
		getMovelPecaAux2(i, j, 0, 1, mov, matriz, player);
		getMovelPecaAux2(i, j, 0, -1, mov, matriz, player);
		getMovelPecaAux2(i, j, 1, 0, mov, matriz, player);
		getMovelPecaAux2(i, j, -1, 0, mov, matriz, player);
		getMovelPecaAux2(i, j, 1, 1, mov, matriz, player);
		getMovelPecaAux2(i, j, -1, -1, mov, matriz, player);
		getMovelPecaAux2(i, j, 1, -1, mov, matriz, player);
		getMovelPecaAux2(i, j, -1, 1, mov, matriz, player);
	}
	mov[i][j] = 0;
}
// Funùùo auxiliar que salva n possivel posiùùes de uma peùa
void getMovelPecaAux(int i, int j, int x, int y, int mov[8][8], int matriz[8][8], int player) {
	int k = 1, a;
	do {
		a = getMovelUnico(i+x*k, j+y*k, matriz, player);
		if (a) {
			mov[i+x*k][j+y*k] = 3;
		}
		k++;
	} while (k <= 7 && a == 1);
}
// Funùùo auxiliar que salva uma possivel posiùùes de uma peùa
void getMovelPecaAux2(int i, int j, int x, int y, int mov[8][8], int matriz[8][8], int player) {
	if (getMovelUnico(i+x, j+y, matriz, player)) {
		mov[i+x][j+y] = 3;
	}
}
// Define quais as possiveis posiùùes para uma peùa em uma reta
void definePecaMovel(int mov[8][8], int matriz[8][8], int player, FILE *fp) {
	int i, j;
	for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				mov[i][j] = getMovel(i, j, matriz, player, fp);
			}
	}
}
// Funùùo auxiliar que escreve o turno e a matriz no arquivo da sala
void salvaTurnoMatriz(FILE *fp, int *turno, int matriz[8][8]) {
	fseek(fp, sizeof(int), SEEK_SET);// Id jogo
	fwrite(turno, sizeof(int), 1, fp);// Turno
	fseek(fp, 2*sizeof(int), SEEK_CUR);// Id player
	fwrite(matriz, 64*sizeof(int), 1, fp);// Matriz
}
// Funùùo auxiliar que define quais imagens serùo botùes apùs a escolha de uma peùa
void pecaClicada(int mov[8][8], int matriz[8][8], int *movendo, int clicado, int player, FILE *fp) {
	definePecaMovel(mov, matriz, player, fp);
	getMovelPeca(clicado/8, clicado%8, matriz, player, mov, fp);
	*movendo = clicado;
}
// Retorna o estado de uma torre
int getTorreEstado(FILE *fp, int torre) {
	int aux;
	fseek(fp, (69+torre)*sizeof(int), SEEK_SET);
	fread(&aux, sizeof(int), 1, fp);
	return aux;
}
// Retorna o estado do rei
int getReiEstado(FILE *fp, int rei) {
	int aux;
	fseek(fp, (73+rei)*sizeof(int), SEEK_SET);
	fread(&aux, sizeof(int), 1, fp);
	return aux;
}
// Retorna o ùltimo peùo que se moveu
int getEnPassant(FILE *fp) {
	int aux;
	fseek(fp, 75*sizeof(int), SEEK_SET);
	fread(&aux, sizeof(int), 1, fp);
	return aux;
}
// Define o estado da torre
void setTorreEstado(FILE *fp, int torre, int n) {
	int aux = n;
	fseek(fp, (69+torre)*sizeof(int), SEEK_SET);
	fwrite(&aux, sizeof(int), 1, fp);
}
// Define o estado do rei
void setReiEstado(FILE *fp, int rei, int n) {
	int aux = n;
	fseek(fp, (73+rei)*sizeof(int), SEEK_SET);
	fwrite(&aux, sizeof(int), 1, fp);
}
// Define o ùltimo peùo que se moveu
void setEnPassant(FILE *fp, int clicado) {
	int aux = clicado;
	fseek(fp, 75*sizeof(int), SEEK_SET);
	fwrite(&aux, sizeof(int), 1, fp);
}
// Faz as trocas necessùrias para um roque
void fazRoque(int x1, int x2, int y1, int y2, int p1, int p2, int i1, int i2, int movendo, int clicado, FILE *fp, int *turno, int matriz[8][8]) {
	matriz[movendo/8][movendo%8] = 0;
	matriz[clicado/8][clicado%8] = 0;
	matriz[y1][x1] = p1;
	matriz[y1][x2] = p2;
	setTorreEstado(fp, i1, 0);
	setReiEstado(fp, i2, 0);
	*turno = (*turno)%2+1;
	salvaTurnoMatriz(fp, turno, matriz);
}
