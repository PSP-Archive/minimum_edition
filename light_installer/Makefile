release: all
	"../installer/tool/mksfo.py" $(PSP_FW_VERSION) PARAM$(PSP_FW_VERSION).SFO
	pack-pbp ME.PBP PARAM$(PSP_FW_VERSION).SFO ICON0.PNG NULL NULL NULL NULL plutonium_updater.prx NULL
	psp-packer ME.PBP
#	mksfo 'UPDATE 639LME' PARAM.SFO
#	prxEncrypter plutonium_updater.prx
#	pack-pbp EBOOT.PBP PARAM.SFO NULL NULL NULL NULL NULL data.psp NULL
	rm -f *.elf *.prx *.o data.psp

TARGET = plutonium_updater
OBJS = main.o

ifndef $(TARGET_FW)
TARGET_FW = 150
endif

PSP_FW_VERSION=$(TARGET_FW)

INCDIR = ../installer/modules/$(PSP_FW_VERSION)/ ./modules/$(PSP_FW_VERSION)/ ../include
CFLAGS = -Os -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

#EXTRA_TARGETS = ME.PBP
BUILD_PRX = 1

LIBDIR = ../lib
LIBS = -lpsppower -lpspsystemctrl_user -lpspkubridge
#  -lpspvshbridge

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
