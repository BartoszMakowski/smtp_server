#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mail.h"


/*
 * return: 1 if true  / 0 - false / (-1) - address error
 * params:
 *   IN:
 *     - address - address to check
 *     - myDomains - concatenation of local domains
 */
int isLocalRecipient(char *address, char* myDomains);

/*
 * return: exit status: 0 - ok / (-1) - error
 * params:
 *   IN:
 *     - address - address to get domain from
 *   OUT:
 *     - domain
 */
int domainFromAddress(char *address, char **domain);

/*
 * return: exit status: 0 - ok / (-1) - error
 * params:
 *   IN:
 *     - line - line to get sender from
 *   OUT:
 *     - sender - address of sender
 */
int senderFromMail(char *line, char **sender);

/*
 * return: exit status: 0 - ok / 1 - ok + local recipient / (-1) - error
 * params:
 *   IN:
 *     - line - line to get recipients from
 *   OUT:
 *     - recipient - address of recipient
 */
int recipientFromRcpt(char *line, char **recipient, char *myDomains);

/*
 * params:
 *   IN:
 *     - cSocket - file descriptor of socket
 */
int readFrom(int cSocket, char* line, struct sMail *mail);

/*
 * return: exit status: 0 - ok / (-1) - error
 * params:
 *   IN:
 *     - cSocket - file descriptor of socket
 */
int readTo(int cSocket, char *line, struct sMail *mail, char* myDomains);