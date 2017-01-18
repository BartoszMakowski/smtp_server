#ifndef MAIL_H
#define MAIL_H

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
#endif