obj-m :=workqueue_test.o

CURRENT_PATH:=$(shell pwd)
LINUX_PATH:=/usr/lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(LINUX_PATH) M=$(CURRENT_PATH) modules

clean:
	rm *.mod.c *.o *.order *.symvers *.ko
