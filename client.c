#include "client.h"

int createCon(char *server_name, int port){
	int desc;
	struct hostent* server_host_entity;
	struct sockaddr_in server_address;
	server_host_entity = gethostbyname(server_name);
	desc = socket(PF_INET, SOCK_STREAM, 0);
	memset(&server_address, 0, sizeof(struct sockaddr));
	server_address.sin_family = AF_INET;
	memcpy(&server_address.sin_addr.s_addr, server_host_entity->h_addr, server_host_entity->h_length);
	server_address.sin_port = htons(port);
	int nConnect = connect(desc, (struct sockaddr*)&server_address, sizeof(struct sockaddr));
	fprintf(stderr, "STATUS: %d", nConnect);
	return(desc);
}

int sendMail(struct sMail *mail, int desc){
//	fprintf(stdout, "SENDING MAIL\n");
	int k;
	char *message;
	char inMessage[1000];
//	fprintf(stdout, "MALLOC: %d\n", strlen((*mail).sender) );
	message = malloc(15 + strlen((*mail).sender));
//	bzero(message, 15 + strlen((*mail).sender));
//	message = '\0';

	strcpy(message, "mail from: <");
	strcat(message, (*mail).sender);
	strcat(message, ">\r\n\0");

	fprintf(stdout, "WRITE\n");
	k = read(desc, inMessage, 1000);
	inMessage[k]='\0';
	fprintf(stdout, "READ: %s\nWRITE: %s", inMessage, message);
	write(desc, message, strlen(message));

	message = realloc(message, (13 + strlen((*mail).recipients)) * sizeof(char));
//	bzero(message, 13 + strlen((*mail).sender));
	strcpy(message, "rcpt to: <");
	strcat(message, (*mail).recipients);
	strcat(message, ">\r\n");

	k = read(desc, inMessage, 1000);
	inMessage[k]='\0';
	fprintf(stdout, "READ: %s\nWRITE: %s", inMessage, message);
	write(desc, message, strlen(message));

	message = realloc(message, 6 * sizeof(char));
	strcpy(message, "data\r\n");
	k = read(desc, inMessage, 1000);
	inMessage[k]='\0';
	fprintf(stdout, "READ: %s\nWRITE: %s", inMessage, message);
	write(desc, message, strlen(message));

	message = realloc(message, 1000 * sizeof(char));
	strcpy(message, (*mail).data);
	k = read(desc, inMessage, 1000);
	inMessage[k]='\0';
	fprintf(stdout, "READ: %s\nWRITE: %s", inMessage, message);
	write(desc, (*mail).data, strlen((*mail).data));
	
	message = realloc(message, 7 * sizeof(char));
	strcpy(message, "quit\r\n");
	k = read(desc, inMessage, 1000);
	inMessage[k]='\0';
	fprintf(stdout, "READ: %s\nWRITE: %s", inMessage, message);
	write(desc, message, strlen(message));
	
	free(message);
	k = read(desc, inMessage, 1000);
	inMessage[k]='\0';
//	fprintf(stdout, "READ: %s", inMessage);

	close(desc);
	return(0);
}