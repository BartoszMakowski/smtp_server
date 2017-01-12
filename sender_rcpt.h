#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * return: exit status
 * params:
 *   IN:
 *     - line - line to get sender from
 *   OUT:
 *     - sender - address of sender
 */
int senderFromMail(char *line, char **sender);

/*
 * return: exit status
 * params:
 *   IN:
 *     - line - line to get recipients from
 *   OUT:
 *     - recipients - addresses of recipients delimited by ','
 */
int recipientsFromRcpt(char *line, char **recipients);

/*
 * return: exit status
 * params:
 *   IN:
 *     - cSocket - file descriptor of socket
 */
int readFrom(int cSocket);

/*
 * return: exit status
 * params:
 *   IN:
 *     - cSocket - file descriptor of socket
 */
int readTo(int cSocket);