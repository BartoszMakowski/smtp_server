#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * return: 1 if true 
 * params:
 *   IN:
 *     - address - address to check
 *     - myDomains - concatenation of local domains
 */
int isLocalRecipient(char *address, char* myDomains);

/*
 * return: exit status
 * params:
 *   IN:
 *     - address - address to get domain from
 *   OUT:
 *     - domain
 */
int domainFromAddress(char *address, char **domain);

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
int recipientsFromRcpt(char *line, char **recipients, char *myDomains);

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
int readTo(int cSocket, char* myDomains);