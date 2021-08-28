#
# make file
#
DIR_CUR := $(shell pwd)
CC = g++ -m32
CFLAGS = -std=gnu++11 -Wall -Wno-error -g -O2
LDFLAGS =

SRCS = $(wildcard *.cpp)

OBJS = $(patsubst %.cpp, %.o, $(SRCS))

TARGET = test

all:$(TARGET) ECHO_OK

$(TARGET):$(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

%.o:%.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

ECHO_OK:
	@echo "#"
	@echo "# make successfully"
	@echo "#"

clean:
	-rm -rf $(OBJS) $(TARGET)


.PHONY:all clean ECHO_OK
