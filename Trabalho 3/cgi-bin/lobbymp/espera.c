#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcaux.c"

int getData(int *salaid, int *playerid);
int p2logado(int *salaid, int *outroplayerid);

int main(int argc, char** argv) {
	int salaid;
	int playerid;
	int outroplayerid;

	if(!(getData(&salaid, &playerid))) {
		printPageBase("Lobby Multiplayer");
		if (p2logado(&salaid, &outroplayerid)) {
			//Se o P2 estù na sala, envia a pùgina de escolha de jogo
			printPageMid("LOBBY MULTIPLAYER");
			printf("<div class=\"mensagem\">\n");
			printBasicInfo(salaid, playerid, outroplayerid);
			printf("<p>Escolha um jogo:</p>\n");
			printBtnJogo("jogovelha", "Jogo da Velha", salaid, playerid, outroplayerid);
			printBtnJogo("xadrez", "Xadrez", salaid, playerid, outroplayerid);
			printf("</div>\n");
		} else {
			//Se o P2 nùo estù na sala, envia a pùgina de espera
			printf("<meta http-equiv=\"refresh\" content=\"%d; url=?salaid=%d&playerid=%d\">\n", 1, salaid, playerid);
			printPageMid("LOBBY MULTIPLAYER");
			printf("<div class=\"mensagem\">\n");
			printf("<p>SalaID: %d</p>\n", salaid);
			printf("<p>PlayerID: %d</p>\n", playerid);
			printf("<p>Player 2: off</p>\n");
			printf("</div>\n");
		}
		printPageEnd();
	} else {
		printErroPage(2);
	}

	return 0;
}

//Salva as variùveis do form
int getData(int *salaid, int *playerid) {
	char *data = getenv("QUERY_STRING");
	char *aux;
	char aux2[16];
	int i;

	if (data == NULL) {
		return 1;
	}

	*salaid = getVar(data, "salaid=", 7);
	*playerid = getVar(data, "playerid=", 9);

	if (*salaid == -1 || *playerid == -1) {
		return 1;
	}

	return 0;
}

//Verifica de o P2 estù na sala
int p2logado(int *salaid, int *outroplayerid) {
	int p2 = getP2(salaid);
	if (p2 == 99999) {
		return 0;
	} else {
		*outroplayerid = p2;
		salvaTime(salaid);
		return 1;
	}
}
