obj-m += SimpleDriver.o

# Current dir
PWD  := $(shell pwd)

# Kernel sources
KDIR := ${Raspberry_Kernel}

all:
	make ARCH=arm CROSS_COMPILE=$(Raspberry)- -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
