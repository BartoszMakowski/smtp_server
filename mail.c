#include "mail.h"

int freeMail(struct sMail *mail){
	free((*mail).received_from);
	free((*mail).received_by);
	free((*mail).date);
	resetMail(mail);
	free(mail);
	return(0);
}

int resetMail(struct sMail *mail){
	free((*mail).recipients);
	(*mail).recipients = NULL;
	free((*mail).sender);
	(*mail).sender = NULL;
	free((*mail).data);
	(*mail).data = NULL;
	return(0);	
}