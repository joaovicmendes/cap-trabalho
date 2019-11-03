#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcaux.c"

#define JOGO_VELHA 1

int getData(int *salaid, int *playerid, int *outroplayerid, int *clidado);
int lerSalaJogo (int *player, int salaid, int *jogoescolhido, int *turno, int *playerid, int *outroplayerid, int matriz[3][3], int *ganhador);
void printPageJogo(int salaid, int playerid, int outroplayerid, int turno, int player, int ganhador, int matriz[3][3]);

int main() {
	int salaid;
	int playerid;
	int outroplayerid;

	int player;
	int turno;

	int jogoescolhido;

	int clicado;

	int ganhador = 0;

	int matriz[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
	int trio[8][3][2] = {{{0, 0}, {0, 1}, {0, 2}}, {{0, 0}, {1, 1}, {2, 2}}, {{0, 0}, {1, 0}, {2, 0}}, {{0, 2}, {1, 2}, {2, 2}}, {{0, 2}, {1, 1}, {2, 0}}, {{0, 1}, {1, 1}, {2, 1}}, {{1, 0}, {1, 1}, {1, 2}}, {{2, 0}, {2, 1}, {2, 2}}};

	int i, j;
	char aux[17];
	FILE *fp;

	if(!getData(&salaid, &playerid, &outroplayerid, &clicado)) {
			if (!lerSalaJogo(&player, salaid, &jogoescolhido, &turno, &playerid, &outroplayerid, matriz, &ganhador)) {
				if (clicado != -1 && turno == player && ganhador == 0) {
					sprintf(aux, "salas/%d.log", salaid);
					fp = fopen(aux, "rb+");

					if (fp != NULL) {
						fseek(fp, sizeof(int), SEEK_SET);
						turno = turno%2+1;
						fwrite(&turno, sizeof(int), 1, fp);
					}

					fseek(fp, 2*sizeof(int), SEEK_CUR);
					matriz[clicado/3][clicado%3] = player;

					for (i = 1; i < 3; i++) {
						for (j = 0; j < 8; j++) {
							if (matriz[trio[j][0][0]][trio[j][0][1]] == i && matriz[trio[j][1][0]][trio[j][1][1]] == i && matriz[trio[j][2][0]][trio[j][2][1]] == i) {
								ganhador = i;
								matriz[trio[j][0][0]][trio[j][0][1]] = i+2;
								matriz[trio[j][1][0]][trio[j][1][1]] = i+2;
								matriz[trio[j][2][0]][trio[j][2][1]] = i+2;
							}
						}
					}

					fwrite(matriz, 9*sizeof(int), 1, fp);

					if (ganhador == 0) {
						ganhador = 3;
						for (i = 0; i < 3; i++) {
							for (j = 0; j < 3; j++) {
								if (matriz[i][j] == 0) {
									ganhador = 0;
								}
							}
						}
					}

					if (ganhador != 0) {
						fwrite(&ganhador, sizeof(int), 1, fp);
					}
					fclose(fp);
				}

				printPageJogo(salaid, playerid, outroplayerid, turno, player, ganhador, matriz);
			} else {
				printPageJogoErrado(salaid, playerid, outroplayerid, nomejogo[jogoescolhido-1]);
			}
	} else {
		printErroPage(2);
	}
	return 0;
}

//Salva as variùveis do form
int getData(int *salaid, int *playerid, int *outroplayerid, int *clicado) {
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

	if (*salaid == -1 || *playerid == -1 || *outroplayerid == -1) {
		return 1;
	}

	return 0;
}


//Envia a pùgina do jogo
void printPageJogo(int salaid, int playerid, int outroplayerid, int turno, int player, int ganhador, int matriz[3][3]) {
	int i, j;
	printPageBase("Jogo da Velha");
	if (turno != player && ganhador == 0) {//Printa a tag de refresh se nùo ù seu turno
		printf("<meta http-equiv=\"refresh\" content=\"%d; url=?salaid=%d&playerid=%d&outroplayerid=%d\">\n", 3, salaid, playerid, outroplayerid);
	}
	printPageMid("JOGO DA VELHA");
	printf("<div class=\"mensagem\">\n");
	printf("<p>Jogo da Velha</p>\n");

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
	printf("<form action=\"jogovelha.cgi\" method=\"get\">\n");
	printf("<input type=\"hidden\" name=\"salaid\" value=\"%d\">\n", salaid);
	printf("<input type=\"hidden\" name=\"playerid\" value=\"%d\">\n", playerid);
	printf("<input type=\"hidden\" name=\"outroplayerid\" value=\"%d\">\n", outroplayerid);
	printf("<table align=\"center\" class=\"jogo-velha\">\n");
	for (i = 0; i < 3; i++) {
		printf("<tr>\n");
		for (j = 0; j < 3; j++) {
			printf("<td>");
			if (matriz[i][j] == 0 && turno == player && ganhador == 0) {//Imprime um botùo inùtil, referente a uma casa em branco
				printf("<input type=\"submit\" name=\"img\" value=\"%d\" style=\"background:url(http://cap.dc.ufscar.br/~758943/imagesJogoVelha/img_0.jpg); width: 128px; height: 128px; color: #00000000; border: none\"\n", i*3+j);
			} else {//Imprime um botùo inùtil, referente a uma casa ocupada, equivalente a uma imagem
				printf("<input style=\"background:url(http://cap.dc.ufscar.br/~758943/imagesJogoVelha/img_%d.jpg);width:128px;height:128px;color:#00000000;border:none\"/>\n", matriz[i][j]);
			}
			printf("</td>\n");
		}
		printf("</tr>\n");
	}
	printf("</table>\n");
	printf("</form>\n");
	printf("</div>\n");

	printPageEnd();
}

//Lù o arquivo da sala
int lerSalaJogo (int *player, int salaid, int *jogoescolhido, int *turno, int *playerid, int *outroplayerid, int matriz[3][3], int *ganhador) {
	*jogoescolhido = -1;

	int p1;
	char salaidstr[17];
	sprintf(salaidstr, "salas/%d.log", salaid);
	FILE *fp = fopen(salaidstr, "rb+");

	salvaTime(&salaid);// Atualiza o tempo da sala
	p1 = getP1(&salaid);// Lù o P1 da sala
	if (*playerid == p1) {// Identifica qual ù o player
		*player = 1;
	} else {
		*player = 2;
	}

	if (fp == NULL) {
		fclose(fp);
		return 1;
	} else {
		fread(jogoescolhido, sizeof(int), 1, fp);

		if (*jogoescolhido == -1) {// O primeiro que escolha o jogo o define, nesse caso nada foi lido do arquivo
			srand(time(NULL));
			fseek(fp, 0, SEEK_SET);
			*jogoescolhido = 1;
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

			fwrite(matriz, 9*sizeof(int), 1, fp);// Salva a matriz do jogo no arquivo
			fwrite((int []){0}, sizeof(int), 1, fp);// Salva o ganhador no arquivo

			fclose(fp);
			return 0;
		} else {
			if (*jogoescolhido == JOGO_VELHA) {// Se o jogo ja foi escolhido, lù as informaùùes do arquivo
				fread(turno, sizeof(int), 1, fp);
				fseek(fp, 2*sizeof(int), SEEK_CUR);
				fread(matriz, 9*sizeof(int), 1, fp);
				fread(ganhador, sizeof(int), 1, fp);
				fclose(fp);
				return 0;
			} else {//Retorna erro se o jogo escolhido pelo segundo player foi diferente do escolhido pelo primeiro
				fclose(fp);
				return 1;
			}
		}
	}
}
