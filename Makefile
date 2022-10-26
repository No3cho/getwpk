# obj-m is a list of what kernel modules to build. The .o and other
# objects will be automatically built from the corresponding .c file -
# no need to list the source files explicitly.
obj-m := getwpk.o 

# KDIR is the location of the kernel source. The current standard is
# to link to the associated source tree from the directory containing
# the compiled modules.
KDIR  := /lib/modules/$(shell uname -r)/build

# PWD is the current working directory and the location of our module
# source files.
PWD   := $(shell pwd)

default:
	$(MAKE) build

build:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	rm -fv Module.symvers getwpk.ko getwpk.mod.c getwpk.mod.o getwpk.o modules.order getwpk.mod \
	.getwpk.o.cmd .getwpk.ko.cmd .getwpk.mod.cmd .getwpk.mod.o.cmd .getwpk.o.d

install: build
	sudo insmod getwpk.ko

uninstall: clean
	sudo rmmod getwpk.ko