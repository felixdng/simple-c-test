#
# make file
#
DIR_CUR := $(shell pwd)
CC = gcc
CFLAGS = -Wall -Wno-error -g -O2
LDFLAGS = -lm

SRCS = main.c rsa.c

OBJS = $(patsubst %.c, %.o, $(SRCS))

TARGET = rsa_test

all:$(TARGET) ECHO_OK

$(TARGET):$(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

ECHO_OK:
	@echo "#"
	@echo "# make successfully"
	@echo "#"

clean:
	-rm -rf $(OBJS) $(TARGET)


.PHONY:all clean $(TARGET) ECHO_OK
