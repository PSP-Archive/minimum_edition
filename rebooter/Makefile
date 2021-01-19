release: all
	psp-packer -t 0x4C94DAF0 -x $(TARGET).prx
	bin2c $(TARGET).prx $(TARGET)_bin.h $(TARGET)
#	sed -i "/int/d" $(TARGET)_bin.h
	sed -i "/int/c #if _PSP_FW_VERSION == $(TARGET_FW)" $(TARGET)_bin.h
	sed -i "/#endif/i #endif" $(TARGET)_bin.h
	rm -f *.elf *.o

ifndef $(TARGET_FW)
TARGET_FW = 150
endif

PSP_FW_VERSION=$(TARGET_FW)

ifeq ($(PSP_MODEL),0)
TARGET = reboot_01g
endif

ifeq ($(PSP_MODEL),1)
TARGET = reboot_02g
endif

ifeq ($(PSP_MODEL),2)
TARGET = reboot_03g
endif

ifeq ($(PSP_MODEL),4)
TARGET = reboot_05g
endif


#TARGET = reboot63x

OBJS = main.o utils.o exports.o

BUILD_PRX=1
PRX_EXPORTS=exports.exp

USE_KERNEL_LIBC = 1
USE_KERNEL_LIBS = 1

INCDIR = ../include
CFLAGS = -Os -G0 -Wall -DPSP_MODEL=$(PSP_MODEL)
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LIBS =

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak
