# perf tool for emc project (aarch64/a57)

## Build

1, Build kernel module (change Makefile and cross compiler path)

```bash
cd kernel_module
ARCH=arm64 CROSS_COMPILE=/home/celery/yocto_sdk/ISSD-SDK-20160701-yocto/LS2085A-SDK-20160304-yocto/build_ls2085aissd_release/tmp/sysroots/x86_64-linux/usr/bin/aarch64-fsl-linux/aarch64-fsl-linux- make
```

2, Build user space perf tool (change cross compiler path)

``` bash
/home/grads/c/celery1124/yocto-install_v3/LS2085A-SDK-20160304-yocto/build_ls2085aissd_release/tmp/sysroots/x86_64-linux/usr/bin/aarch64-fsl-linux/aarch64-fsl-linux-g++ --sysroot=/home/grads/c/celery1124/yocto-install_v3/LS2085A-SDK-20160304-yocto/build_ls2085aissd_release/tmp/sysroots/ls2085aissd perf.c -o perf
```

## Run

```bash
insmod enable_arm_pmu.ko
./perf 24 program
```

Note: The perf event number is commented on perf_lib.h

# Reference
http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0488d/BIIFDEEJ.html(http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0488d/BIIFDEEJ.html)

https://static.docs.arm.com/ddi0487/fa/DDI0487F_a_armv8_arm.pdf?_ga=2.142050290.130834191.1584475779-1440830053.1584475779(https://static.docs.arm.com/ddi0487/fa/DDI0487F_a_armv8_arm.pdf?_ga=2.142050290.130834191.1584475779-1440830053.1584475779)