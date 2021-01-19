
ifndef $(TARGET_FW)
TARGET_FW = 150
endif


all: common cfw
lite: common lcfw
double: common cfw lcfw

build_lib:
	make -C lib/lib_builld

cfw:
	make -C vshctrl 	TARGET_FW=$(TARGET_FW)
	make -C custom_ipl 	TARGET_FW=$(TARGET_FW)
	make -C installer 	TARGET_FW=$(TARGET_FW)
#	cp unpacker/EBOOT.PBP EBOOT.PBP

lcfw:
#	make -C rebootex 	TARGET_FW=$(TARGET_FW) PSP_MODEL=2
	make -C rebootex 	TARGET_FW=$(TARGET_FW) PSP_MODEL=4
	make -C systemctrl 	TARGET_FW=$(TARGET_FW) PSP_MODEL=2
	make -C systemctrl 	TARGET_FW=$(TARGET_FW) PSP_MODEL=4
	make -C vshctrl 	TARGET_FW=$(TARGET_FW) HEN=1
	make -C rebootex_lite	TARGET_FW=$(TARGET_FW) PSP_MODEL=0
	make -C rebootex_lite 	TARGET_FW=$(TARGET_FW) PSP_MODEL=1
#	make -C rebootex_lite 	TARGET_FW=$(TARGET_FW) PSP_MODEL=2
	make -C rebootex_lite 	TARGET_FW=$(TARGET_FW) PSP_MODEL=4
	make -C rebooter 	TARGET_FW=$(TARGET_FW) PSP_MODEL=0
	make -C rebooter 	TARGET_FW=$(TARGET_FW) PSP_MODEL=1
#	make -C rebooter 	TARGET_FW=$(TARGET_FW) PSP_MODEL=2
	make -C rebooter 	TARGET_FW=$(TARGET_FW) PSP_MODEL=4
	make -C light_installer	TARGET_FW=$(TARGET_FW)
	make -C 639kxploit	TARGET_FW=$(TARGET_FW) LITE=0
	make -C 639launch	TARGET_FW=$(TARGET_FW)

common:
	make -C rebootex 	TARGET_FW=$(TARGET_FW) PSP_MODEL=0
	make -C rebootex 	TARGET_FW=$(TARGET_FW) PSP_MODEL=1
	make -C systemctrl 	TARGET_FW=$(TARGET_FW) PSP_MODEL=0
	make -C systemctrl 	TARGET_FW=$(TARGET_FW) PSP_MODEL=1
	make -C horoscope	TARGET_FW=$(TARGET_FW)
	make -C idcanager 	TARGET_FW=$(TARGET_FW)
	make -C galaxy	 	TARGET_FW=$(TARGET_FW)
	make -C march33 	TARGET_FW=$(TARGET_FW)
	make -C recovery	TARGET_FW=$(TARGET_FW)
#	make -C vshmenu_src	TARGET_FW=$(TARGET_FW)
	make -C vshmenu_new	TARGET_FW=$(TARGET_FW)

clean:
	make clean -C rebootex
	make clean -C systemctrl
	make clean -C horoscope
	make clean -C idcanager
	make clean -C galaxy
	make clean -C march33
	make clean -C vshctrl
	make clean -C custom_ipl
	make clean -C installer
	make clean -C rebootex_lite
	make clean -C rebooter
#	@rm -f EBOOT.PBP


reset_dist:
	-mkdir dist
	rm -rf dist/*


distribute_lite:
	mkdir dist/release_$(TARGET_FW)lme
	mkdir -p dist/release_$(TARGET_FW)lme/PSP/GAME/installer
	mkdir -p dist/release_$(TARGET_FW)lme/PSP/GAME/launcher
	cp -r documents/leda dist/release_$(TARGET_FW)lme
	cp -r recovery/translate dist/release_$(TARGET_FW)lme
	cp vshmenu_new/translate/*.txt dist/release_$(TARGET_FW)lme/translate
	cp 639kxploit/EBOOT.PBP dist/release_$(TARGET_FW)lme/PSP/GAME/installer/EBOOT.PBP
	cp 639launch/EBOOT.PBP dist/release_$(TARGET_FW)lme/PSP/GAME/launcher/EBOOT.PBP
	cp documents/$(TARGET_FW)_lme*en.txt dist/release_$(TARGET_FW)lme/Readme.txt
	-cp documents/$(TARGET_FW)_lme*jp.txt dist/release_$(TARGET_FW)lme/Readme_jp.txt
	cp documents/info.txt dist/release_$(TARGET_FW)lme/info.txt
	cp light_installer/ME.PBP dist/lme.dat

distribute: reset_dist
	mkdir dist/release_$(TARGET_FW)me
	mkdir dist/release_$(TARGET_FW)me/UPDATE
	cp -r documents/leda dist/release_$(TARGET_FW)me
	cp -r recovery/translate dist/release_$(TARGET_FW)me
	cp vshmenu_new/translate/*.txt dist/release_$(TARGET_FW)me/translate
	cp installer/EBOOT.PBP dist/release_$(TARGET_FW)me/UPDATE/EBOOT.PBP
	cp documents/$(TARGET_FW)_me*en.txt dist/release_$(TARGET_FW)me/Readme.txt
	-cp documents/$(TARGET_FW)_me*jp.txt dist/release_$(TARGET_FW)me/Readme_jp.txt
	cp documents/info.txt dist/release_$(TARGET_FW)me/info.txt
	cp installer/EBOOT.PBP dist/me.dat

distribute_all: distribute distribute_lite
