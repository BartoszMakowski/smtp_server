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

#define BUF_SIZE 1024
#define BACKLOG 10

char myDomains[] = "foo.com:bar.com";

void *threadBehavior(void *tData){
	struct sCon *conInfo = (struct sCon*) tData;
	struct sMail *mail;
	mail = malloc(sizeof(struct sMail));
	int nClientSocket;
	nClientSocket = (*conInfo).desc;
	char line[128];
	int count;
	count = 0;
	
	sesInit(nClientSocket);
	do
	{
		read(nClientSocket, line, 128);
		if (strncmp (line, "EHLO ", 5) == 0 || strncmp (line, "ehlo ", 5) == 0 || strncmp (line, "HELO ", 5) == 0 || strncmp (line, "helo ", 5) == 0){
			clientInit(nClientSocket, &line, mail);
			fprintf(stdout, "### RCVD FROM: %s ###", (*mail).received_from);
		}
		else if (strncmp (line, "MAIL FROM:", 10) == 0 || strncmp (line, "mail from:", 10) == 0){
			readFrom(nClientSocket, &line, mail);
			fprintf(stdout, "### SENDER: %s ###", (*mail).sender);
		}
		else if (strncmp (line, "RCPT TO:", 8) == 0 || strncmp (line, "rcpt to:", 8) == 0){
			readTo(nClientSocket, &line, mail, &myDomains);
			fprintf(stdout, "### RECIPIENT: %s ###", (*mail).recipients);
		}
		else if (strncmp (line, "DATA\r\n:", 6) == 0 || strncmp (line, "data\r\n", 6) == 0){
			readDataCmd(nClientSocket, &line, mail);
			fprintf(stdout, "### DATA:\n###\n%s ###", (*mail).data);
		}
		else if (strncmp (line, "quit\r\n", 6) == 0 || strncmp (line, "QUIT\r\n", 6) == 0){
			quitSuccess(nClientSocket);
		}
		else{
			write(nClientSocket, "500 unrecognized command\r\n", 27);
			count++;
		}
	} while ( count < 3);
	close(nClientSocket);
	pthread_exit(NULL);
}

int quitSuccess(int cSocket){
	write(cSocket, "inf122518_smtp_server: Service closing transmission channel\r\n", 61);			
	return(0);
}

int readDataCmd(int cSocket, char *line, struct sMail *mail){
	char *data;
	write(cSocket, "354 Send message\r\n", 19);
	if(readData(cSocket, &data) == 0){
		(*mail).data = malloc(sizeof(char) * strlen(data));
		strcpy((*mail).data, data);		
		return(0);
	}
	else{
		return(-1);
	}
}

int readData(int cSocket, char** data){
	char line[128];
	char *sender;
	int n;
	int end;
	end = 0;
	*data = malloc(0);
	do
	{
		n = read(cSocket, line, 1024);
		line[n]='\0';
		if (strncmp (line, ".\r\n", 3) == 0){
			write(cSocket, "250 OK\r\n", 8);
			return(0);
		}
		else{
			fprintf(stdout, "read %d chars\n", n);
			realloc(*data, strlen(*data) + n-2);
			strcat(*data, line);
			fprintf(stdout, "DATA:\n%s\n#####", *data);
			//fprintf(stdout, "LINE:\n#####\n%s\n#####", line);
			//TODO
			//write(cSocket, "read %i chars", n, 27);
		}
	} while ( !end );
	return(-1);
}

int main(int argc, char* argv[])
{
	int nSocket;
	int nClientSocket;
	int nFoo = 1, nTmp;
	struct sockaddr_in stServerAddr, stClientAddr;
	struct hostent* lpstServerEnt;

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

	setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char*)nFoo, sizeof(nFoo) );
	 
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
	
	int i;
	for(i=0; i<10; i++)
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
