#
# make file
#
DIR_CUR := $(shell pwd)
CC = gcc
CFLAGS = -Wall -Wno-error -g -O2
LDFLAGS =


#target
TARGET =
OBJS =

#service
SRCS_SERVICE = service.c
OBJS_SERVICE = $(patsubst %.c, %.o, $(SRCS_SERVICE))
OBJS += $(OBJS_SERVICE)
TARGET_SERVICE = service
TARGET += $(TARGET_SERVICE)

#client
SRCS_CLIENT = client.c
OBJS_CLIENT = $(patsubst %.c, %.o, $(SRCS_CLIENT))
OBJS += $(OBJS_CLIENT)
TARGET_CLIENT = client
TARGET += $(TARGET_CLIENT)


all:$(TARGET) ECHO_OK

$(TARGET_SERVICE):$(OBJS_SERVICE)
	$(CC) -o $@ $(OBJS_SERVICE) $(LDFLAGS)

$(TARGET_CLIENT):$(OBJS_CLIENT)
	$(CC) -o $@ $(OBJS_CLIENT) $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@


ECHO_OK:
	@echo "#"
	@echo "# make successfully"
	@echo "#"

clean:
	-rm -rf $(OBJS) $(TARGET)


.PHONY:all clean $(TARGET) ECHO_OK
