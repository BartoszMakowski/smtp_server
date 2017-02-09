#ifndef MAIL_H
#define MAIL_H

#include <stdlib.h>

struct sCon {
	int desc;
};  

struct sMail {
	char* received_from;
	char* received_by;
	char* date;
	char* sender;
	char* recipients;
	char* data;
};

/*
 * info: free sMail struct's memory
 * return: exit status: 0 - ok
 * params:
 *   - *mail - pointer to sMail struct
 */
int freeMail(struct sMail *mail);

int resetMail(struct sMail *mail);

#endif