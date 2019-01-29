#
# Usage:
#   make o=<source file name with or without extension>
#   make o="<source file names with or without extension>"
#
objects := $(addsuffix .o,$(basename $(strip $(o))))

ifneq ($(filter-out clean, $(MAKECMDGOALS)),)
  ifeq ($(objects),)
    $(error No object to be compiled)
  else
    $(warning Compiling $(objects))
  endif
endif

obj-m := $(objects)

# 'uname -r' print kernel release
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

# enable macor DEBUG in order to use pr_debug()
ccflags-y += -DDEBUG

# targets
all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

