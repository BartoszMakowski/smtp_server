#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//#include "dns_mx.h"
#include "mail.h"
#include "ses_client_init.h"
#include "sender_rcpt.h"
#include "client.h"

#define BACKLOG 10
#define DEBUG 0

char myDomains[] = "foo.com:bar.com";

void *threadBehavior(void *tData){
	struct sCon *conInfo = (struct sCon*) tData;
	struct sMail *mail;
	mail = malloc(sizeof(struct sMail));
	int nClientSocket;
	nClientSocket = (*conInfo).desc;
	char line[512];
	char lineChar;
	int status;
	int end;
	int count;
	count = 0;
	int stage;
	stage = 0;
	int lineLen;

	sesInit(nClientSocket);
	do
	{
//		TODO: RSET, NOOP, VRFY commands
		lineLen = 0;
		end = 0;
		while (lineLen <= 512 && read(nClientSocket, &line[lineLen], 1) == 1)
		{
			if ( lineLen > 0 && line[lineLen-1] == '\r' && line[lineLen] == '\n')
			{
				break;
			}
			lineLen++;
		}

		if (strncmp (line, "EHLO ", 5) == 0 || strncmp (line, "ehlo ", 5) == 0 || strncmp (line, "HELO ", 5) == 0 || strncmp (line, "helo ", 5) == 0){
			status = clientInit(nClientSocket, &line, mail);
			if (status == 0){
				if (DEBUG){
					fprintf(stdout, "### RCVD FROM: %s ###", (*mail).received_from);
				}
				stage = 1;
			}
			else{
				fprintf(stdout, "!!! >>helo/ehlo<< problem !!!");
			}
		}
		else if (strncmp (line, "MAIL FROM:", 10) == 0 || strncmp (line, "mail from:", 10) == 0){
			status = readFrom(nClientSocket, &line, mail);
			if (status == 0){
				if (DEBUG){
					fprintf(stdout, "### SENDER: %s ###", (*mail).sender);
				}
				stage = 2;
			}
			else{
				if (DEBUG){
					fprintf(stdout, "!!! >>mail from<< problem !!!");
				}
			}
		}
		else if (strncmp (line, "RCPT TO:", 8) == 0 || strncmp (line, "rcpt to:", 8) == 0){
			if (stage == 2 || stage == 3 ){
				status = readTo(nClientSocket, &line, mail, &myDomains);
			}
			else {
				write(nClientSocket, "503 sender not yet given\r\n", 27);
				status = -1;
			}
			if (status == 0){
				if (DEBUG){
					fprintf(stdout, "### RECIPIENT: %s ###", (*mail).recipients);
				}
				stage = 4;
			}
			else{
				if (DEBUG){
				fprintf(stdout, "!!! >>rcpt to<< problem !!!");
				}
			}
		}
		else if (strncmp (line, "DATA\r\n:", 6) == 0 || strncmp (line, "data\r\n", 6) == 0 || \
		strncmp (line, "DATA:\r\n", 7) == 0 || strncmp (line, "data:\r\n", 7) == 0){
			if (stage == 4){
				status = readDataCmd(nClientSocket, &line, mail);
			}
			else {
				write(nClientSocket, "503 valid RCPT command must precede DATA\r\n", 42);
				status = -1;
			}
			if (status == 0){
				if (DEBUG){
					fprintf(stdout, "### DATA:\n###\n%s ###", (*mail).data);
				}
			}
			else{
				if (DEBUG){
					fprintf(stdout, "!!! >>data<< problem !!!");
				}
			}
		}
		else if (strncmp (line, "quit\r\n", 6) == 0 || strncmp (line, "QUIT\r\n", 6) == 0){
			status = quitSuccess(nClientSocket);
			if (status == 0){
				end = 1;
			}
			else{
				if (DEBUG){
					fprintf(stdout, "!!! >>quit<< problem !!!");
				}
			}
		}
		else{
			write(nClientSocket, "500 unrecognized command\r\n", 27);
			count++;
		}
		if (status != 0){
			count++;
		}
	} while ( count < 3 && !end);
	close(nClientSocket);
	fprintf(stdout, ">>>FINAL DATA:\n%s", (*mail).data);
	freeMail(mail);
	fprintf(stdout, ">>>FINAL DATA:\n%s", (*mail).data);
	pthread_exit(NULL);
}

int quitSuccess(int cSocket){
	write(cSocket, "inf122518_smtp_server: Service closing transmission channel\r\n", 61);
	return(0);
}

int readDataCmd(int cSocket, char *line, struct sMail *mail){
	char *data;
	write(cSocket, "354 Send message\r\n", 19);
	fprintf(stdout, "354 send message\r\n");
	if(readData(cSocket, &data) == 0){
		(*mail).data = malloc(sizeof(char) * strlen(data));
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
	char *sender;
	int n;
	int end;
	int status;
	end = 0;
	*data = malloc(0);
	do
	{
		n = read(cSocket, line, 1000);
//		fprintf(stdout, "READ LINE:\n%s\r\n$$$", line);
		line[n]='\0';
			if (DEBUG)
			{
				fprintf(stdout, "read %d chars\n", n);
			}
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

int main(int argc, char* argv[])
{
	int nSocket;
	int nClientSocket;
	int nFoo = 1, nTmp;
	struct sockaddr_in stServerAddr, stClientAddr;
	struct hostent* lpstServerEnt;
	int *dsc;
	char test[] = "172.20.10.245";
	createCon(test, 25, dsc);

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s server_name port_number\n", argv[0]);
		exit(1);
	}

	/* look up server's IP address */
	lpstServerEnt = gethostbyname(argv[1]);
	if (! lpstServerEnt)
	{
		fprintf(stderr, "%s: Can't get the server's IP address.\n", argv[0]);
		exit(1);
	}

	/* create a socket */
	nSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (nSocket < 0)
	{
			fprintf(stderr, "%s: Can't create a socket.\n", argv[0]);
			exit(1);
	}

	/* server info */
	memset(&stServerAddr, 0, sizeof(struct sockaddr));
	stServerAddr.sin_family = AF_INET;
	memcpy(&stServerAddr.sin_addr.s_addr, lpstServerEnt->h_addr, lpstServerEnt->h_length);
	stServerAddr.sin_port = htons(atoi(argv[2]));

	setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nFoo, sizeof(nFoo) );
	 
	/* bind address and port */	 
	if(bind(nSocket, (struct sockaddr *) &stServerAddr, sizeof(struct sockaddr_in)))
	{
		fprintf(stderr, "BIND ERROR.\n");
		exit(1);
	}

	/* start listen*/
	if(listen(nSocket, BACKLOG))
	{
		fprintf(stderr, "LISTEN ERROR.\n");
		exit(1);
	}
	
	while(1)
	{
		nClientSocket = accept(nSocket, (struct sockaddr*)&stClientAddr, &nTmp);
		struct sCon tData;
		tData.desc = nClientSocket;
		pthread_t conThread;
		pthread_create(&conThread, NULL, threadBehavior, (void *)&tData);
	}

	close(nSocket); 
	return 0;
}
