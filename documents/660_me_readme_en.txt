CFW 6.60 Minimum Edition for 01g, 02g model by neur0n

-- What is this?--
This is CFW work on PSP1000 and PSP2000.

The degree of stability is still unknown. 
Please test it on your own and refer to different people's test reports.

From 4.01M33 -> OK
From 5.00M33 -> OK
From 5.50GEN -> OK(may need to edit version.txt)
From 5.50Prometheus -> OK(may need to edit version.txt)
From 6.20TN-D -> OK
From 6.37ME -> Error
From 6.38ME -> OK
From 6.39ME -> OK
From 6.20PRO-B4 -> Error
From 6.35PRO-B4 -> Error
From 6.20PRO-B5 -> OK
From 6.35PRO-B5 -> OK
From 6.39PRO-B6 -> OK

--How to Install--
First, you need to install CFW or HEN in your PSP.

1. copy UPDATE folder at ms0:/PSP/GAME/.
2. Put 6.60 official update at ms0:/PSP/GAME/UPDATE/660.PBP, or let the program download it for you can use a wifi connection.
3. Run installer from xmb.


-- features --
Hold R trigger and turn on the psp, you can enter recovery menu.
Hold Home button and turn on the psp, you can Boot OFW.

-- Credit --
Dark_AleX	: For his great CFW. This CFW is based his 5.00M33.
Moz		: For his beta test and translated language file in Italian.
BombingBasta	: For his translated language file in French.
Rapper_skull	: For his translated language file in Italian.
The Z 		: For his translated language file in German.
largeroliker 	: For his translated language file in Spanish.
bassiw		: For his translated language file in Netherlands.
Publikacii	: For his translated language file in Bulgarian.
Yoti		: For his translated language file in Russian.
ExzoTikFruiT	: For his translated language file in Russian.
ABCanG &Dadrfy &estuibal &plum &SnyFbSx &teck4 &wn : For their work to collect nid list.
Tipped OuT	: For his EBOOT.PBP ICON0.

-- About translating the recovery menu --
To translate the recovery ,create a text file in ms0:/seplugins/xx_recovery.txt or flash1:/xx_recovery.txt .
"xx" is the language code of your language.

es -> spanish
en -> english
fr -> french
de -> german
it -> italian
ja -> japanese
ko -> korean
nl -> netherlands
pt -> portuguese
ru -> russian
ch1 -> chinese simplified
ch2 -> chinese traditional

VshMenu: ms0:/seplugins/xx_vshmnu.txt
To use custom font in RecoveryMenu, put font data at ms0:/seplugins/xx_ftable.bin.("xx" is the language code)

--History--
v1.7
-- Fixed a freeze bug when enter sleep with SpeedUpMs option is Enabled.
-- Added a Text color option in Recovery menu.
-- Refactor code.
v1.6
-- Change CPU clock from 199/99 to 200/100.;
-- Update VshMenu. Now you can use translate text.
-- Fixed a ISO size when dump it through USB in XMB.
v1.5
-- Added a 199/99 CPU clock.
-- Fixed a bug in VshMenu when save config.
-- Optimizing the patch.
v1.4
-- Fixed USB Connect in Recovery menu.(05g only).
-- Update VshMenu.
v1.3
-- Added Backup/Restore Netconfig option in Recovery menu.
-- Fixed a bug when Use VshMenu while using RemotePlay, Skype, 1seg and PSN-store.
v1.2
-- Update RecoveryMenu.
-- Added a 166/83 CPU clock.
v1.1
-- Update version.txt loading. Now you can load txt from ms0:/seplugins/version.txt.
-- Update NidResolver for FuSa_SD.prx.
v1
-- Fixed a bug in sctrlHENPatchSyscall.
beta3
-- Fixed a bug when use Normal_driver.
-- Fixed a bug when selece Video on XMB.
beta2
-- Fixed a bug when execute converted PS1.
beta
-- First release.

