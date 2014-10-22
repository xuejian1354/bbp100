FILES_PRE = kdrv_test

ifeq ($(KERNELRELEASE),)


KERNELDIR := $(ANDROID_BUILD_TOP)/kernel_imx
PWD := $(shell pwd)

target:modules clean

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	rm -rf *.o *~ core .depend .*.cmd Mo* *.mod.c *order .tmp_versions

distclean:
	rm -rf *.o *~ core .depend .*.cmd Mo* *.mod.c *order .tmp_versions $(FILES_PRE).ko

.PHONY: modules modules_install clean

else
    obj-m := $(FILES_PRE).o
endif
