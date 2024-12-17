#
# make file
#
DIR_CUR := $(shell pwd)
CC = gcc
CFLAGS = -Wall -Wno-error -g -O2
LDFLAGS = -ldl -L. -ldltest

SRCS = dl_main.c

OBJS = $(patsubst %.c, %.o, $(SRCS))

TARGET = main
TARGET_LIB = libdltest.so

all:$(TARGET_LIB) $(TARGET) ECHO_OK

$(TARGET):$(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(TARGET_LIB):
	$(CC) -fPIC -shared dl_lib.c -o $@

ECHO_OK:
	@echo "#"
	@echo "# make successfully"
	@echo "#"

clean:
	-rm -rf $(OBJS) $(TARGET) $(TARGET_LIB)


.PHONY:all clean $(TARGET) ECHO_OK
