#
# make file
#
DIR_CUR := $(shell pwd)
CC = gcc
CFLAGS = -Wall -Wno-error -g -O2
LDFLAGS = -lexpect -ltcl

SRCS = sock_cli.c

OBJS = $(patsubst %.c, %.o, $(SRCS))

TARGET = cli

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
