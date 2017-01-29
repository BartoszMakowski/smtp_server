#include "sender_rcpt.h"

int isLocalRecipient(char *address, char* myDomains){
	char *domain;
	int status;
	status = domainFromAddress(address, &domain);
//	fprintf(stdout, "myDomains: %s\n", myDomains);
	if (status == 0){
		if (strstr(myDomains, domain) != NULL){
			status = 1;
		}
		else{
			status = 0;
		}
	}
	return(status);
}

int domainFromAddress(char *address, char **domain){
	int status;
	*domain = strtok(address, "@");
	*domain = strtok(NULL, "@");
	if (domain == NULL || domain[0] == '\0'){
//		fprintf(stdout, "!!! >>domain<< problem !!!\n");
		status = -1;
	}
	else{
		fprintf(stdout, "domain: %s\n", *domain);
		status = 0;
	}
	return(status);	
}

int senderFromMail(char *line, char **sender){
	int status;
	status = 0;
	*sender = strtok(line, "<");
	*sender = strtok(NULL, ">");
	if (sender == NULL || sender[0] == '\0'){
		status = -1;
	}
	else{
//		TODO: mail syntax validation
	}
//	fprintf(stdout, "sender: %s\n", *sender);
	return(status);
}

int recipientFromRcpt(char *line, char **recipient, char* myDomains){
	int status;
	status = 0;
	*recipient = strtok(line, "<"); // get address from brackets
	*recipient = strtok(NULL, ">");
	if (recipient == NULL || recipient[0] == '\0'){
		status = -1; // no brackets
	}
	else if(isLocalRecipient(*recipient, myDomains) == 1){
		status = 1;
	}
	else{
		status = 0;
	}
	return status;
//	TODO: mail syntax validation
//	fprintf(stdout, "recipients: %s\n", *recipients);
}

int readFrom(int cSocket, char *line, struct sMail *mail){
	char *sender;
	if (senderFromMail(line, &sender) == 0){
//		fprintf(stdout, "sender: %s\n", sender);
		write(cSocket, "250 OK\r\n", 8);
		(*mail).sender = malloc(sizeof(char) * strlen(sender));
		strcpy((*mail).sender, sender);
		return(0);
	}
	else{
		return(-1);
	}
}

int readTo(int cSocket, char *line, struct sMail *mail, char* myDomains){
	char *recipient;
	if (recipientFromRcpt(line, &recipient, myDomains) >= 0){
//		fprintf(stdout, "recipient: %s\n", recipient);
		write(cSocket, "250 OK\r\n", 8);
		(*mail).recipients = malloc(sizeof(char) * strlen(recipient));
		strcpy((*mail).recipients, recipient);
		return(0);
	}
	else{
		return(-1);
	}
}