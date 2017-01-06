#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024
#define BACKLOG 10

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
		write(nClientSocket, "220 inf122518_smtp_server\n", 27);
		close(nClientSocket); 
	}

	close(nSocket); 
	return 0;
}
