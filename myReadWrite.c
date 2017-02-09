#include "myReadWrite.h"

int myWrite(int desc, char *text, int n){
	int wSuccess = 0;
	int status = 0;
	do{
		wSuccess += write(desc, text, n);
		if (wSuccess != n){
			memmove(text, text+n, strlen(text) - n);
		}
//		fprintf(stdout, "WRITE: %d\n", wSuccess);
	}
	while (n != wSuccess &&  wSuccess >= 0);
	if(wSuccess < 0 ){
		status = -1;
	}
	return(status);
}

int myRead(int desc, char *text, int n){
	return(0);
}