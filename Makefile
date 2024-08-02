ifeq ($(KERNELRELEASE),)

KDIR ?= /lib/modules/$(shell uname -r)/build

modules modules_install clean help:
	$(MAKE) -C $(KDIR) M=$(shell pwd) $@

else

obj-m := nyancat.o
nyancat-y := nyan.o

endif
