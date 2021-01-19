release: all
	psp-packer $(TARGET).prx
	bin2c $(TARGET).prx $(TARGET)_bin.h $(TARGET)
	rm *.elf *.o sfile/*.o

ifndef $(TARGET_FW)
TARGET_FW = 150
endif

PSP_FW_VERSION=$(TARGET_FW)

TARGET = sysmini
OBJS = main.o exports.o
OBJS += sfile/loadcore635.o sfile/sysmem635.o
# sfile/threadman635.o

INCDIR = 
CFLAGS = -O2 -G0 -Wall -fshort-wchar -fno-pic -mno-check-zero-division
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS) -D_PSP_FW_VERSION=$(TARGET_FW)

BUILD_PRX = 1
PRX_EXPORTS = exports.exp

USE_KERNEL_LIBC = 1
USE_KERNEL_LIBS = 1


LIBDIR =
LIBS = 
LDFLAGS = -nostdlib -nodefaultlibs -e module_bootstart

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak


#all:
#	psp-build-exports -s $(PRX_EXPORTS)