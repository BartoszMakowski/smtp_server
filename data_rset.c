#include "data_rset.h"

int reset(int cSocket, struct sMail *mail){
	resetMail(mail);
	write(cSocket, "250 Reset OK\r\n", 15);
	return(0);
}