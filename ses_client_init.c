#include "ses_client_init.h"

int domainFromHelo(char *line, char **domain){
	*domain = strtok(line, " ");
	*domain = strtok(NULL, " ");
	fprintf(stdout, "domain: %s\n", *domain);
	return(0);	
}

int clientInit(int cSocket, char *line, struct sMail *mail){
	char *domain;
	write(cSocket, "250 inf122518_smtp_server: Hello\r\n", 34);
	domainFromHelo(line, &domain);
//	TODO: domain validation
	(*mail).received_from = malloc(sizeof(char) * strlen(domain));
	strcpy((*mail).received_from, domain);
	return(0);
}

int sesInit(int cSocket){
	if(1){	
		write(cSocket, "220 inf122518_smtp_server\r\n", 28);
		return(0);
	}
	else{
		write(cSocket, "554 connection rejected\r\n", 28);
		//TODO
		return(-1);
	}
}