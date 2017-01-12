#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "dns_mx.h"
#include "ses_client_init.h"

#define BUF_SIZE 1024
#define BACKLOG 10

int senderFromMail(char *line, char **sender){
	*sender = strtok(line, "<");
	*sender = strtok(NULL, ">");
//	TODO: mail syntax validation
//	fprintf(stdout, "sender: %s\n", *sender);
	return(0);
}

int recipientsFromRcpt(char *line, char **recipients){
	*recipients = strtok(line, "<");
	*recipients = strtok(NULL, ">");
//	TODO: mail syntax validation
//	fprintf(stdout, "recipients: %s\n", *recipients);
	return(0);
}

int readFrom(int cSocket){
	char line[128];
	char *sender;
	int count;
	count = 0;
	do
	{
		read(cSocket, line, 128);
		if (strncmp (line, "MAIL FROM:", 10) == 0 || strncmp (line, "mail from:", 10) == 0){
			// TODO
			if (senderFromMail(line, &sender) == 0){
				fprintf(stdout, "sender: %s\n", sender);
				write(cSocket, "250 OK\r\n", 8);
				return(0);
			}
			else{
				return(-1);
			}
		}
		else if (strncmp (line, "quit\r\n", 6) == 0 || strncmp (line, "QUIT\r\n", 6) == 0){
			//TODO
			exit(-1);
		}
		else{
			//TODO
			write(cSocket, "500 unrecognized command\r\n", 27);
			count++;
		}
	} while ( count < 3);
	return(-1);
}

int readTo(int cSocket){
	char line[128];
	char *recipients;
	int count;
	count = 0;
	do
	{
		read(cSocket, line, 128);
		if (strncmp (line, "RCPT TO:", 8) == 0 || strncmp (line, "rcpt to:", 8) == 0){
			// TODO
			if (recipientsFromRcpt(line, &recipients) == 0){
				fprintf(stdout, "recipients: %s\n", recipients);
				write(cSocket, "250 OK\r\n", 8);
				return(0);
			}
			else{
				return(-1);
			}
		}
		else if (strncmp (line, "quit\r\n", 6) == 0 || strncmp (line, "QUIT\r\n", 6) == 0){
			//TODO
			exit(-1);
		}
		else{
			//TODO
			write(cSocket, "500 unrecognized command\r\n", 27);
			count++;
		}
	} while ( count < 3);
	return(-1);
}

int main(int argc, char* argv[])
{
	int nSocket;
	int nClientSocket;
	//int nConnect;
	//int nBytes;
	int nFoo = 1, nTmp;
	struct sockaddr_in stServerAddr, stClientAddr;
	struct hostent* lpstServerEnt;
	//char cbBuf[BUF_SIZE];

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
		sesInit(nClientSocket);
		clientInit(nClientSocket);
		readFrom(nClientSocket);
		readTo(nClientSocket);
		close(nClientSocket); 
	}

	close(nSocket); 
	return 0;
}
