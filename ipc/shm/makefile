CC = gcc
CFLAGS = -Wall -Werror -g3 -gdwarf-2 -O0 -fPIC -std=gnu99

all: get put
get:shm_basic.c shm_basic.h sem_basic.c sem_basic.h get.c
	$(CC) $(CFLAGS) $^ -o $@

put:shm_basic.c shm_basic.h sem_basic.c sem_basic.h put.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	/bin/rm -rf get put
