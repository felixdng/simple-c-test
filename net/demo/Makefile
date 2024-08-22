#
# make file
#
DIR_CUR := $(shell pwd)
CC := gcc
CFLAGS := -Wall -fPIC -std=gnu99 -Werror -g3 -O0 -gdwarf-2
LDFLAGS :=

SRCS := $(wildcard *.c)

OBJS := $(patsubst %.c, %.o, $(SRCS))
dep_files := $(wildcard $(foreach v,$(OBJS),.$(v).d))

#$(info "dep_files:$(dep_files)")

TARGET = demo

all:$(TARGET) ECHO_OK

$(TARGET):$(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

ifneq ($(dep_files),)
include $(dep_files)
endif

%.o:%.c
	$(CC) $(CFLAGS) -Wp,-MMD,.$@.d -c $< -o $@

ECHO_OK:
	@echo "#"
	@echo "# make successfully"
	@echo "#"

clean:
	-rm -rf $(OBJS) $(TARGET)
dist-clean:clean
	-rm -rf $(dep_files)

.PHONY:all clean dist-clean $(TARGET) ECHO_OK
