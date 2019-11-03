#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Variùveis globais
int blockSize = sizeof(time_t) + 3*sizeof(int);
char nomejogo[2][25] = {"Jogo da Velha", "Xadrez"};

//Lù uma variùvel do form
int getVar(char *data, char *ptr, int n) {
	char *aux = strstr(data, ptr);
	char aux2[7];
	int i;

	if (aux == NULL) {
		return -1;
	} else {
		aux += n;
		i = 0;
		while (aux[i] != '\0' && aux[i] != '&') {
			aux2[i] = aux[i];
			i++;
		}
		aux2[i] = '\0';
		return atoi(aux2);
	}
}

//Printf a parte inicial da pùgina
void printPageBase(char *title) {
	printf("%s%c%c\n", "Content-Type:text/html;charset=iso-8859-1", 13, 10);
	printf("<!DOCTYPE html>\n");
	printf("<html>\n");
	printf("<head>\n");
	printf("<title>%s</title>\n", title);
	printf("<meta charset=\"UTF-8\">\n");
	printf("<link rel=\"stylesheet\" type=\"text/css\" href=\"../../style.css\">\n");
}

//Printa o meio da pùgina
void printPageMid(char *title) {
	printf("</head>\n");
	printf("<body>\n");
	printf("<h2>%s</h2>\n", title);
}

//Printa o fim da pùgina
void printPageEnd() {
	printf("<bottom>\n");
	printf("<a href=\"http://cap.dc.ufscar.br/~758943/lobbymp.html\">Pùgina Inicial</a>\n");
	printf("<p>Desenvolvido por Matheus Victorello e Joùo Victor Mendes</p>\n");
	printf("</bottom>\n");
	printf("</body>\n");
	printf("</html>\n");
}

//Printa a pùgina de erro
void printErroPage(int erro) {
	printPageBase("Lobby Multiplayer");
	printPageMid("LOBBY MULTIPLAYER");
	printf("<p>Erro: %d</p>\n", erro);
	printPageEnd();
}

//Atualiza o tempo em uma sala
void salvaTime(int *salaid) {
	int salaindex = *salaid - 100000;
	int blockSize = sizeof(time_t) + 3*sizeof(int);
	time_t entryTime = time(NULL);
	FILE *salas = fopen("salas.log", "rb+");
	if (salas != NULL) {
		fseek(salas, salaindex * blockSize, SEEK_SET);
		fwrite(&entryTime, sizeof(time_t), 1, salas);
	}
	fclose(salas);
}

//Retorna o P1 de uma sala
int getP1(int *salaid) {
	int salaindex = *salaid - 100000;
	int blockSize = sizeof(time_t) + 3*sizeof(int);
	int pid;
	FILE *salas = fopen("salas.log", "rb+");
	if (salas != NULL) {
		fseek(salas, salaindex * blockSize, SEEK_SET);
		fseek(salas, (sizeof(time_t)+sizeof(int)), SEEK_CUR);
		fread(&pid, sizeof(int), 1, salas);
	}
	fclose(salas);
	return pid;
}

//Retorna o P2 de uma sala
int getP2(int *salaid) {
	int salaindex = *salaid - 100000;
	int blockSize = sizeof(time_t) + 3*sizeof(int);
	int pid;
	FILE *salas = fopen("salas.log", "rb+");
	if (salas != NULL) {
		fseek(salas, salaindex * blockSize, SEEK_SET);
		fseek(salas, (sizeof(time_t)+2*sizeof(int)), SEEK_CUR);
		fread(&pid, sizeof(int), 1, salas);
	}
	fclose(salas);
	return pid;
}

//Defina o P2 de uma sala
void setP2(int *salaid, int *pid) {
	int salaindex = *salaid - 100000;
	int blockSize = sizeof(time_t) + 3*sizeof(int);
	FILE *salas = fopen("salas.log", "rb+");
	if (salas != NULL) {
		fseek(salas, salaindex * blockSize, SEEK_SET);
		fseek(salas, (sizeof(time_t)+2*sizeof(int)), SEEK_CUR);
		fwrite(pid, sizeof(int), 1, salas);
	}
	fclose(salas);
}

//Printa um botùo referente a um jogo
void printBtnJogo(char *cgi, char *nome, int salaid, int playerid, int outroplayerid) {
	printf("<form action=\"%s.cgi\" method=\"get\">\n", cgi);
	printf("<input type=\"hidden\" name=\"salaid\" value=\"%d\">\n", salaid);
	printf("<input type=\"hidden\" name=\"playerid\" value=\"%d\">\n", playerid);
	printf("<input type=\"hidden\" name=\"outroplayerid\" value=\"%d\">\n", outroplayerid);
	printf("<input type=\"submit\" value=\"%s\">\n", nome);
	printf("</form>\n");
}

//Printa informaùùes sobre a sala
void printBasicInfo(int salaid, int playerid, int outroplayerid) {
	printf("<p>SalaID: %d</p>\n", salaid);
	printf("<p>PlayerID: %d</p>\n", playerid);
	printf("<p>Outro PlayerID: %d</p>\n", outroplayerid);
}

//Envia a pùgina referente a escolha do jogo errado
void printPageJogoErrado(int salaid, int playerid, int outroplayerid, char nomejogo[25]) {
	printPageBase("Lobby Multiplayer");
	printPageMid("LOBBY MULTIPLAYER");
	printBasicInfo(salaid, playerid, outroplayerid);
	printf("<p>O outro jogador escolheu: %s</p>\n", nomejogo);
	printf("<p>Escolha um jogo:</p>\n");
	printBtnJogo("jogovelha", "Jogo da Velha", salaid, playerid, outroplayerid);
	printBtnJogo("xadrez", "Xadrez", salaid, playerid, outroplayerid);
	printPageEnd();
}
