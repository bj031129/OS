ifneq ($(KERNELRELEASE),)
	obj-m := tasklet_interrupt.o
else
	KERNELDIR ?= /usr/lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

# 定义内核模块参数
irq ?= 31
devname ?= "tasklet_dev"

# 安装内核模块，传入参数
install: default
	sudo insmod tasklet_interrupt.ko irq=$(irq) devname=$(devname)

# 卸载内核模块
uninstall:
	sudo rmmod tasklet_interrupt

# 清理编译生成的文件
clean:
	-rm *.mod.c *.o *.order *.symvers *.ko

endif
# 使用 .PHONY 声明 clean、install 和 uninstall 为伪目标
.PHONY: clean install uninstall
