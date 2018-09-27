# https://www.kernel.org/doc/Documentation/kbuild/modules.txt
obj-m := debugfs_remove.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
WARN_FLAGS += -Wall

.PHONY: default
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	rm -vrf *.o *.ko *.mod.c .debugfs_remove.* .tmp_versions/ modules.order Module.symvers
