# TODO: fix this shit
obj-m +=log.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules -lcsys

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
