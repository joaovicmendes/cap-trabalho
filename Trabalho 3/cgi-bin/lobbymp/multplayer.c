#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcaux.c"

int getDataCuston(int *option);
void printCriarPage(int salaid, int playerid);
void printEntrarPage(int playerid);
int genSalaid(int *salaid, int *playerid);
void genPlayerid(int *playerid);
int criaSalasLog();
void novaSala(FILE *fp, time_t *time, int *salaid, int *playerid, int *outroplayerid, int lugar);
void criaSalaid(int salaid);


int main(int argc, char** argv) {
	int option;
	int salaid;
	int playerid;

	if (criaSalasLog()) {
		printErroPage(1);
		return 0;
	}
	if(!(getDataCuston(&option))) {
		if (option == 1) {
			// Envia a p?gina de para criar a sala
			genPlayerid(&playerid);// Cria um id para o player
			genSalaid(&salaid, &playerid);// Cria um id para a sala, faz a munuten??o das salas
			printCriarPage(salaid, playerid);
		} else {
			// Envia a p?gina de para entrar na sala
			genPlayerid(&playerid);// Cria um id para o player
			printEntrarPage(playerid);
		}
	} else {
		printErroPage(2);
	}

	return 0;
}

//Salva as vari?veis do form
int getDataCuston(int *option) {
	char *data = getenv("QUERY_STRING");
	char *aux;
	char aux2[16];
	int i;

	if (data == NULL) {
		return 1;
	}

	aux = strstr(data, "opt=");

	if (aux == NULL) {
		return 1;
	} else {
		aux += 4;

		i = 0;
		while(aux[i] != '\0' && aux[i] != '&') {
			aux2[i] = aux[i];
			i++;
		}
		aux2[i] = '\0';

		if (!strcmp(aux2, "Criar")) {
			*option = 1;
		} else if (!strcmp(aux2, "Entrar")) {
			*option = 2;
		} else {
			return 1;
		}
	}
	return 0;
}

// Printa a p?gina para criar a sala
void printCriarPage(int salaid, int playerid) {
	printPageBase("Lobby Multiplayer");
	printPageMid("LOBBY MULTIPLAYER");
	printf("<div class=\"mensagem\">\n");
	printf("<form action=\"espera.cgi\"  method=\"get\">\n");
	printf("<input type=\"hidden\" name=\"salaid\" value=\"%d\">\n", salaid);
	printf("<input type=\"hidden\" name=\"playerid\" value=\"%d\">\n", playerid);
	printf("<p>SalaID: %d</p>\n", salaid);
	printf("<p>PlayerID: %d</p>\n", playerid);
	printf("<input type=\"submit\" value=\"Criar\">\n");
	printf("</form>\n");
	printf("</div>\n");
	printPageEnd();
}

//Printa a p?gina para entrar na sala
void printEntrarPage(int playerid) {
	printPageBase("Lobby Multiplayer");
	printPageMid("LOBBY MULTIPLAYER");
	printf("<div class=\"mensagem\">\n");
	printf("<form action=\"connect.cgi\" method=\"get\">\n");
	printf("<p>PlayerID: %d</p>\n", playerid);
	printf("<input type=\"text\" name=\"salaid\" value=\"\">\n");
	printf("<input type=\"hidden\" name=\"playerid\" value=\"%d\">\n", playerid);
	printf("<input type=\"submit\" value=\"Entrar\">\n");
	printf("</form>\n");
	printf("</div>\n");
	printPageEnd();
}

//Cria um id para sala, faz a manuten??o das salas n?o utilizadas
int genSalaid(int *salaid, int *playerid) {
	FILE *fp = fopen("salas.log", "rb+");
	if (fp == NULL) {
		return 1;
	}
	fseek(fp, 0, SEEK_END);
	int blockSize = sizeof(time_t)+3*sizeof(int);
	int salaDisp = -1;
	int qtSalas = (int)(ftell(fp)/blockSize);
	int i;

	//Manuten??o
	time_t entryTime = time(NULL);
	time_t salaTime;
	fseek(fp, 0, SEEK_SET);
	if (qtSalas > 0) {// Se existem salas
		for (i = 0; i < qtSalas; i++) {// Para cada sala
			fread(&salaTime, sizeof(time_t), 1, fp);
			if ((entryTime - salaTime) >= 300) {// Se a sala n?o ? modificada a mais de 5 minutos
				fseek(fp, -1*sizeof(time_t), SEEK_CUR);
				if (salaDisp == -1) {// Se esse ? a primeira sala vazia, ela ser? usada
					salaDisp = i + 100000;
					novaSala(fp, (time_t[]){time(NULL)}, &salaDisp, playerid, (int []){99999}, SEEK_CUR);
				} else {//Se n?o, ser? limpa
					novaSala(fp, (time_t[]){0}, (int []){99999}, (int []){99999}, (int []){99999}, SEEK_CUR);
				}
			} else {
				fseek(fp, 3*sizeof(int), SEEK_CUR);
			}
		}
	} else {// Se n?o existem salas uma sala ? criada
		salaDisp = 100000;
		novaSala(fp, (time_t[]){time(NULL)}, &salaDisp, playerid, (int []){99999}, SEEK_CUR);
	}
	//Fim manuten??o

	if (salaDisp == -1) {// Se todas as salas est?o sendo utilizadas uma sala ? criada
		salaDisp = qtSalas + 100000;
		novaSala(fp, (time_t[]){time(NULL)}, &salaDisp, playerid, (int []){99999}, SEEK_END);
	}
	*salaid = salaDisp;

	criaSalaid(*salaid);// Cria o arquivo referente a sala

	fclose(fp);
	return 0;
}

//Cria o id do player
void genPlayerid(int *playerid) {
	srand(time(NULL));
	*playerid = rand()%100000+100000;
}

//Cria o arquivo de salas
int criaSalasLog() {
	FILE *fp = fopen("salas.log", "ab");
	if (fp == NULL) {
		fclose(fp);
		return 1;
	} else {
		fclose(fp);
		return 0;
	}
}

//Cria ou sobrescreve uma sala
void novaSala(FILE *fp, time_t *time, int *salaid, int *playerid, int *outroplayerid, int lugar) {
	fseek(fp, 0, lugar);
	fwrite(time, sizeof(time_t), 1, fp);//Tempo
	fwrite(salaid, sizeof(int), 1, fp);//Sala id
	fwrite(playerid, sizeof(int), 1, fp);//Player 1 id
	fwrite(outroplayerid, sizeof(int), 1, fp);//Player 2 id
}

// Cria o arquivo referente a sala
void criaSalaid(int salaid) {
	char salaidstr[17];
	sprintf(salaidstr, "salas/%d.log", salaid);
	FILE *fp = fopen(salaidstr, "wb");
	fclose(fp);
}
