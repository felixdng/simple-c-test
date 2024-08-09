CC = gcc
CFLAGS = -Wall -Werror -g3 -gdwarf-2 -O0 -fPIC -std=gnu99

all: semp semv
semp:sem_comm.c sem_comm.h sem_p.c
	$(CC) $(CFLAGS) $^ -o $@

semv:sem_comm.c sem_comm.h sem_v.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	/bin/rm -rf semp semv
