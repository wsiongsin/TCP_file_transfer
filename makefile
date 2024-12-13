CC=gcc
CFLAGS=-Wall

all: server client

server: server.c
	$(CC) $(CFLAGS) -o server server.c

client: client.c
	$(CC) $(CFLAGS) -o sendFile client.c

clean:
	rm -f server sendFile
