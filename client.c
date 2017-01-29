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
	fprintf(stdout, "STATUS: %d", nConnect);
	return(desc);
}

int sendMail(struct sMail *mail, int desc){
	char *message;
	char inMessage[1000];
	fprintf(stdout, "MALLOC\n");
	message = malloc(15 + strlen((*mail).sender));
	strcat(message, "mail from: <");
	strcat(message, (*mail).sender);
	strcat(message, ">\r\n");
	fprintf(stdout, "WRITE\n");
	read(desc, inMessage, 1000);
	fprintf(stdout, "READ: %s\nWRITE: %s", inMessage, message);
	write(desc, message, strlen(message));
	close(desc);
	return(0);
}