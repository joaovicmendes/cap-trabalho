#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcaux.c"

int getData(int *salaid, int *playerid);
int logar(int * salaid, int *playerid, int *outroplayerid);

int main(int argc, char** argv) {
	int salaid;
	int playerid;
	int outroplayerid;

	if (!(getData(&salaid, &playerid))) {
		if (logar(&salaid, &playerid, &outroplayerid)) {
			//Se o P2 consegue entrar na sala, envia a p?gina de escolha de jogo
			printPageBase("Lobby Multiplayer");
			printPageMid("LOBBY MULTIPLAYER");
			printf("<div class=\"mensagem\">\n");
			printBasicInfo(salaid, playerid, outroplayerid);
			printf("<p>Escolha um jogo:</p>\n");
			printBtnJogo("jogovelha", "Jogo da Velha", salaid, playerid, outroplayerid);
			printBtnJogo("xadrez", "Xadrez", salaid, playerid, outroplayerid);
			printf("</div>\n");
			printPageEnd();
		} else {
			printErroPage(4);
		}
	} else {
		printErroPage(2);
	}

	return 0;
}

//Salva as vari?veis do form
int getData(int *salaid, int *playerid) {
	char *data = getenv("QUERY_STRING");
	char *aux;
	char aux2[7];
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

//Salva o P2 em uma sala, se n?o houver P2 na sala
int logar(int *salaid, int *playerid, int *outroplayerid) {
	salvaTime(salaid);
	int p2 = getP2(salaid);
	*outroplayerid = getP1(salaid);
	if (p2 == 99999) {
		setP2(salaid, playerid);
		return 1;
	} else {
		return 0;
	}
}
