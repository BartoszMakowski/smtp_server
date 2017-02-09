#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "myReadWrite.h"
#include "mail.h"

/*
 * return: exit status
 * params:
 *   IN:
 *     - line - line to get domain from
 *   OUT:
 *     - domain
 */
int domainFromHelo(char *line, char **domain);

/*
 * return: exit status
 * params:
 *   IN:
 *     - cSocket - file descriptor of socket
 */
int sesInit(int cSocket);

/*
 * return: exit status
 * params:
 *   IN:
 *     - cSocket - file descriptor of socket
 */
int clientInit(int cSocket, char* line, struct sMail *mail);