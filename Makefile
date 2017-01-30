CC=gcc
CFLAGS=-Wall

all: my_smtp_server.out

my_smtp_server.out: client.o data_rset.o mail.o \
	ses_client_init.o sender_rcpt.o main.o
		$(CC) $(CFLAGS) -pthread client.o data_rset.o\
		       	ses_client_init.o mail.o sender_rcpt.o main.o\
		       	-o my_smtp_server.out -lresolv
main.o: main.c
		$(CC) $(CFLAGS) -c main.c -o main.o 
client.o: client.c
		$(CC) $(CFLAGS) -c client.c -o client.o
data_rset.o: data_rset.c
		$(CC) $(CFLAGS) -c data_rset.c -o data_rset.o
dns_mx.o: dns_mx.c
		$(CC) $(CFLAGS) -c dns_mx.c -o dns_mx.o -lresolv
mail.o: mail.c
		$(CC) $(CFLAGS) -c mail.c -o mail.o
ses_client_init.o: ses_client_init.c
		$(CC) $(CFLAGS) -c ses_client_init.c -o ses_client_init.o
sender_rcpt.o: sender_rcpt.c
		$(CC) $(CFLAGS) -c sender_rcpt.c -o sender_rcpt.o

clean:
		rm *.o my_smtp_server.out
