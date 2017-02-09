#include "data_rset.h"

int quitSuccess(int cSocket){
	write(cSocket, "inf122518_smtp_server: Service closing transmission channel\r\n", 61);
	return(0);
}

int readDataCmd(int cSocket, char *line, struct sMail *mail){
	char *data;
	write(cSocket, "354 Send message\r\n", 19);
//	fprintf(stdout, "354 send message\r\n");
	if(readData(cSocket, &data) == 0){
		(*mail).data = malloc(sizeof(char) * (strlen(data)+1));
		strcpy((*mail).data, data);
//		fprintf(stdout, "zwracam zero\r\n");
		return(0);
	}
	else{
		return(-1);
	}
}

int readData(int cSocket, char** data){
	char line[1000];
	int n;
	int end;
	int status;
	end = 0;
	*data = malloc(1);
	do
	{
		n = read(cSocket, line, 1000);
//		fprintf(stdout, "READ LINE:\n%s\r\n$$$", line);
		line[n]='\0';
//			if (DEBUG)
//			{
//				fprintf(stdout, "read %d chars\n", n);
//			}
			*data = realloc(*data, strlen(*data) + n-2);
			strcat(*data, line);
			if (n >= 3 && line[n-3] == '.' && line[n-2] == '\r' && line[n-1] == '\n')
			{
				write(cSocket, "250 OK\r\n", 8);
				status = 0;
				end = 1;
			}
	} while ( !end );
//	fprintf(stdout, "KONIEC\r\n");
	return(status);
}

int reset(int cSocket, struct sMail *mail){
	resetMail(mail);
	write(cSocket, "250 Reset OK\r\n", 15);
	return(0);
}