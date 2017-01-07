#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024
#define BACKLOG 10

int readEhlo(int cSocket){
	char line[32];
	int n;
	int count;
	count = 0;
	do
	{
		if (count++){
			write(cSocket, "500 unrecognized command\r\n", 27);
			if (count == 4){
				return(1);
			}
		}
		read(cSocket, line, 32);
	}
	while (strncmp (line, "EHLO ", 5) != 0 && strncmp (line, "ehlo ", 5) != 0 && strncmp (line, "quit", 4) != 0);
	write(cSocket, "250 inf122518_smtp_server: Hello\r\n", 34);
	return(0);
}

int readFrom(int cSocket){
	char line[64];
	int n;
	int count;
	count = 0;
	do
	{
		if (count++){
			write(cSocket, "500 unrecognized command\r\n", 27);
			if (count == 4){
				return(1);
			}
		}
		read(cSocket, line, 32);
	}
	while (strncmp (line, "mail from: ", 11) != 0 && strncmp (line, "MAIL FROM: ", 11) != 0 && strncmp (line, "quit", 4) != 0);
	write(cSocket, "250 OK\r\n", 8);
	return(0);
}

int readTo(int cSocket){
	char line[64];
	int n;
	int count;
	count = 0;
	do
	{
		if (count++){
			write(cSocket, "500 unrecognized command\r\n", 27);
			if (count == 4){
				return(1);
			}
		}
		read(cSocket, line, 32);
	}
	while (strncmp (line, "rcpt to: ", 9) != 0 && strncmp (line, "RCPT TO: ", 9) != 0 && strncmp (line, "quit", 4) != 0);
	write(cSocket, "250 OK\r\n", 8);
	return(0);
}

int main(int argc, char* argv[])
{
	int nSocket;
	int nClientSocket;
	int nConnect;
	int nBytes;
	int nFoo = 1, nTmp;
	struct sockaddr_in stServerAddr, stClientAddr;
	struct hostent* lpstServerEnt;
	char cbBuf[BUF_SIZE];

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
		write(nClientSocket, "220 inf122518_smtp_server\r\n", 28);
		readEhlo(nClientSocket);
		readFrom(nClientSocket);
		readTo(nClientSocket);
		close(nClientSocket); 
	}

	close(nSocket); 
	return 0;
}
