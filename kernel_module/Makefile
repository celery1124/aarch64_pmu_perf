obj-m	:= enable_arm_pmu.o
KDIR	:= /home/grads/c/celery1124/yocto-install_v3/LS2085A-SDK-20160304-yocto/build_ls2085aissd_release/tmp/work/ls2085aissd-fsl-linux/linux-ls2-sdk/4.1-r0/build/
PWD	:= $(shell pwd)

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
