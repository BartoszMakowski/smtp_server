#include "mail.h"

int freeMail(struct sMail *mail){
	free((*mail).received_from);
	free((*mail).received_by);
	free((*mail).recipients);
	free((*mail).sender);
	free((*mail).data);
	free((*mail).date);
	free(mail);
	return(0);
}
