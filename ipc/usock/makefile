CC = gcc
CFLAGS = -Wall -Werror -g3 -gdwarf-2 -O0 -fPIC -std=gnu99

all: client server

client: common.c client.c
	$(CC) $(CFLAGS) $^ -o $@

server: common.c server.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	/bin/rm -rf client server
