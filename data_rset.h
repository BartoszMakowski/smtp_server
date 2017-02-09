#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "mail.h"

int quitSuccess(int cSocket);

int readDataCmd(int cSocket, char *line, struct sMail *mail);

int readData(int cSocket, char **data);

int reset(int cSocket, struct sMail *mail);