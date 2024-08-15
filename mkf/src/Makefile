EXTRA_CFLAGS:=
ifneq ($(CONFIG_TEST),)
CFLAGS_main.o:=-DTEST
endif

obj-y += main.o
obj-$(CONFIG_TEST) += test.o

obj-y += mymath/

