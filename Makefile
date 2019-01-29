obj-m := hello.c

# 'uname -r' print kernel release
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

# enable macor DEBUG in order to use pr_debug()
ccflags-y += -DDEBUG

# targets
all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm *.o *.ko *.mod.c Modules.symvers modules.order -f

