#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "dns_mx.h"
#include "ses_client_init.h"
#include "sender_rcpt.h"

#define BUF_SIZE 1024
#define BACKLOG 10

int quitSuccess(int cSocket){
	char line[128];
	int count;
	count = 0;
	do
	{
		read(cSocket, line, 128);
		if (strncmp (line, "quit\r\n", 6) == 0 || strncmp (line, "QUIT\r\n", 6) == 0){
			write(cSocket, "inf122518_smtp_server: Service closing transmission channel\r\n", 61);			
			return(0);
		}
		else{
			write(cSocket, "500 unrecognized command\r\n", 27);
			count++;
		}
	} while ( count < 3);
	return(-1);
}

int readDataCmd(int cSocket){
	char line[128];
	char *sender;
	char **data;
	int count;
	count = 0;
	do
	{
		read(cSocket, line, 128);
		if (strncmp (line, "DATA\r\n:", 6) == 0 || strncmp (line, "data\r\n", 6) == 0){
			write(cSocket, "354 Send message\r\n", 19);
			readData(cSocket, &data);
			return(0);			
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
		if (strncmp (line, ".\r\n", 3) == 0){
			write(cSocket, "250 OK\r\n", 8);
			return(0);
		}
		else{
			fprintf(stdout, "read %d chars\n", n);
			realloc(*data, strlen(*data) + n-2);
			strcat(*data, line);
			fprintf(stdout, "DATA:\n%s", *data);
			//TODO
			//write(cSocket, "read %i chars", n, 27);
		}
	} while ( !end );
	return(-1);
}

char myDomains[] = "foo.com:bar.com";

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
//		sesInit(nClientSocket);
//		clientInit(nClientSocket);
//		readFrom(nClientSocket);
		readTo(nClientSocket, myDomains);
		readDataCmd(nClientSocket);
		quitSuccess(nClientSocket);
		close(nClientSocket); 
	}

	close(nSocket); 
	return 0;
}
