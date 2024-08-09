CC = gcc
CFLAGS = -Wall -Werror -g3 -gdwarf-2 -O0 -fPIC -std=gnu99

all: read write
read:msg_comm.c msg_comm.h msg_read.c
	$(CC) $(CFLAGS) $^ -o $@

write:msg_comm.c msg_comm.h msg_write.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	/bin/rm -rf write read
