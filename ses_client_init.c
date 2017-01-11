#include "ses_client_init.h"

int domainFromHelo(char *line, char **domain){
	*domain = strtok(line, " ");
	*domain = strtok(NULL, " ");
	//fprintf(stdout, "domain: %s\n", *domain);
	return(0);	
}

int clientInit(int cSocket){
	char line[32];
	char *domain;
	int count;
	count = 0;
	do
	{
		read(cSocket, line, 32);
		if (strncmp (line, "EHLO ", 5) == 0 || strncmp (line, "ehlo ", 5) == 0){
			// TODO
			write(cSocket, "250 inf122518_smtp_server: Hello\r\n", 34);
			domainFromHelo(line, &domain);
			fprintf(stdout, "domain: %s\n", domain);
		}
		else if (strncmp (line, "HELO ", 5) == 0 || strncmp (line, "helo ", 5) == 0){
			// TODO
			write(cSocket, "250 inf122518_smtp_server: Hello\r\n", 34);
			domainFromHelo(line, domain);
			fprintf(stdout, "domain: %s\n", domain);
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