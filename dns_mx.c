#include "dns_mx.h"


int getMx(char *address)
{
	char *mx;
	char *prev;
    u_char nsbuf[N];
    char dispbuf[N];
    ns_msg msg;
    ns_rr rr;
    int i, l;
	l = res_query(address, ns_c_any, ns_t_mx, nsbuf, sizeof(nsbuf));
	if (l < 0)
	{
		perror(address);
	}
	else
	{
		ns_initparse(nsbuf, l, &msg);
		l = ns_msg_count(msg, ns_s_an);
		for (i = 0; i < l; i++)
		{
			ns_parserr(&msg, ns_s_an, i, &rr);
			ns_sprintrr(&msg, &rr, NULL, NULL, dispbuf, sizeof(dispbuf));
			mx = strtok(dispbuf, "\t");
			while( mx != NULL )
			{
				prev = mx;
			    mx = strtok(NULL, "\t");
			}
			prev = strrchr(prev, ' ');
			printf( "%s\n", prev );
		}
	}
	return(0);
}

