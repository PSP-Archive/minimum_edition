
ifndef $(TARGET_FW)
TARGET_FW = 150
endif

ifndef $(LITE)
LITE = 0
endif

release:
	make -C sysmini		TARGET_FW=$(TARGET_FW)
	make -C rebootex	TARGET_FW=$(TARGET_FW)
	bin2c ../light_installer/ME.PBP ./pbp.h update_pbp
	make -f Makefile.PSP	TARGET_FW=$(TARGET_FW) LITE=$(LITE)
	-rm *.prx *.SFO *.o *.elf
