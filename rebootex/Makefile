PSPDEV=$(shell psp-config --pspsdk-path)
INCLUDES=-I $(PSPDEV)/include -I ../include

ifndef $(TARGET_FW)
TARGET_FW = 150
endif

ifeq ($(PSP_MODEL),0)
TARGET = rebootex_01g
endif

ifeq ($(PSP_MODEL),1)
TARGET = rebootex_02g
endif

ifeq ($(PSP_MODEL),2)
TARGET = rebootex_03g
endif

ifeq ($(PSP_MODEL),4)
TARGET = rebootex_05g
endif

CFLAGS=$(INCLUDES) -W -Wall -G0 -Os -DPSP_MODEL=$(PSP_MODEL) -D_PSP_FW_VERSION=$(TARGET_FW)

all:	a.bin reset

a.bin: 
	psp-gcc $(CFLAGS) -fno-pic -mno-abicalls -S main.c -o main.s
	psp-as main.s -o main.o
	psp-ld -T linkfile.l main.o -o main.elf
	psp-strip -s main.elf
	psp-objcopy -O binary main.elf h.bin
	gzip -f h.bin
	bin2c h.bin.gz $(TARGET).h rebootex
	sed -i "/int/c #if _PSP_FW_VERSION == $(TARGET_FW)" $(TARGET).h
	sed -i "/#endif/i #endif" $(TARGET).h

reset:
	rm -rf *~ *.o *.elf *.s

clean: reset
	rm -f rebootex_0?g.h
	
