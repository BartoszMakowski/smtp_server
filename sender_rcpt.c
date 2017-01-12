#include "sender_rcpt.h"

int senderFromMail(char *line, char **sender){
	*sender = strtok(line, "<");
	*sender = strtok(NULL, ">");
//	TODO: mail syntax validation
//	fprintf(stdout, "sender: %s\n", *sender);
	return(0);
}

int recipientsFromRcpt(char *line, char **recipients){
	*recipients = strtok(line, "<");
	*recipients = strtok(NULL, ">");
//	TODO: mail syntax validation
//	fprintf(stdout, "recipients: %s\n", *recipients);
	return(0);
}

int readFrom(int cSocket){
	char line[128];
	char *sender;
	int count;
	count = 0;
	do
	{
		read(cSocket, line, 128);
		if (strncmp (line, "MAIL FROM:", 10) == 0 || strncmp (line, "mail from:", 10) == 0){
			// TODO
			if (senderFromMail(line, &sender) == 0){
				fprintf(stdout, "sender: %s\n", sender);
				write(cSocket, "250 OK\r\n", 8);
				return(0);
			}
			else{
				return(-1);
			}
		}
		else if (strncmp (line, "quit\r\n", 6) == 0 || strncmp (line, "QUIT\r\n", 6) == 0){
			//TODO
			exit(-1);
		}
		else{
			//TODO
			write(cSocket, "500 unrecognized command\r\n", 27);
			count++;
		}
	} while ( count < 3);
	return(-1);
}

int readTo(int cSocket){
	char line[128];
	char *recipients;
	int count;
	count = 0;
	do
	{
		read(cSocket, line, 128);
		if (strncmp (line, "RCPT TO:", 8) == 0 || strncmp (line, "rcpt to:", 8) == 0){
			// TODO
			if (recipientsFromRcpt(line, &recipients) == 0){
				fprintf(stdout, "recipients: %s\n", recipients);
				write(cSocket, "250 OK\r\n", 8);
				return(0);
			}
			else{
				return(-1);
			}
		}
		else if (strncmp (line, "quit\r\n", 6) == 0 || strncmp (line, "QUIT\r\n", 6) == 0){
			//TODO
			exit(-1);
		}
		else{
			//TODO
			write(cSocket, "500 unrecognized command\r\n", 27);
			count++;
		}
	} while ( count < 3);
	return(-1);
}