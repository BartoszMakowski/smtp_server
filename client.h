#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "mail.h"

/* info: connect to the next SMTP server
 * return: descriptor's number
 * params:
 *   - next SMTP server's name
 *   - -||- port
 */
int createCon(char *server_name, int port);

/* info: write mail to socket
 * return: exit status
 * params:
 *   - mail to send
 *   - descriptor's number
 */
int sendMail(struct sMail *mail, int desc);