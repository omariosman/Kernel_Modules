obj-m += Hello.o

KDIR = /usr/src/linux-headers-5.4.0-26-generic

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order
