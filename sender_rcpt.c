#include "sender_rcpt.h"

int isLocalRecipient(char *address, char* myDomains){
	char *domain;
	domainFromAddress(address, &domain);
	fprintf(stdout, "myDomains: %s\n", myDomains);
	if (strstr(myDomains, domain) != NULL){
		return(1);
	}
	else{
		return(0);
	}
}

int domainFromAddress(char *address, char **domain){
	*domain = strtok(address, "@");
	*domain = strtok(NULL, "@");
	fprintf(stdout, "domain: %s\n", *domain);
	return(0);	
}

int senderFromMail(char *line, char **sender){
	*sender = strtok(line, "<");
	*sender = strtok(NULL, ">");
//	TODO: mail syntax validation
//	fprintf(stdout, "sender: %s\n", *sender);
	return(0);
}

int recipientFromRcpt(char *line, char **recipient, char* myDomains){
	*recipient = strtok(line, "<");
	*recipient = strtok(NULL, ">");
	if(isLocalRecipient(*recipient, myDomains)){
		return(0);
	}
	else{
		return(-1);
	}
//	TODO: mail syntax validation
//	fprintf(stdout, "recipients: %s\n", *recipients);
}

int readFrom(int cSocket, char *line, struct sMail *mail){
	char *sender;
	if (senderFromMail(line, &sender) == 0){
		fprintf(stdout, "sender: %s\n", sender);
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
	if (recipientFromRcpt(line, &recipient, myDomains) == 0){
		fprintf(stdout, "recipient: %s\n", recipient);
		write(cSocket, "250 OK\r\n", 8);
		(*mail).recipients = malloc(sizeof(char) * strlen(recipient));
		strcpy((*mail).recipients, recipient);
		return(0);
	}
	else{
		return(-1);
	}
}