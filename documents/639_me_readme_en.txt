CFW 6.39 Minimum Edition for 01g, 02g model by neur0n

-- What is this?--
This is CFW work on PSP1000 and PSP2000.

The degree of stability is still unknown. 
Please test it on your own and refer to different people's test reports.

From 5.00M33 -> OK
From 5.50GEN -> OK(may need to edit version.txt)
From 5.50Prometheus -> OK(may need to edit version.txt)
From 6.20TN-D -> OK
From 6.37ME -> Error
From 6.38ME -> OK
From 6.20PRO-B4 -> Error
From 6.35PRO-B4 -> Error
From 6.20PRO-B5 -> OK
From 6.35PRO-B5 -> OK
From 6.39PRO-B6 -> OK

--How to Install--
First, you need to install CFW or HEN in your PSP.

1. copy UPDATE folder at ms0:/PSP/GAME/.
2. Put 6.39 official update at ms0:/PSP/GAME/UPDATE/639.PBP .
3. Run installer from xmb.


-- features --
Hold R trigger and turn on the psp, you can enter recovery menu.
Hold Home button and turn on the psp, you can Boot OFW.

-- Credit --
Dark_AleX : For his great CFW. This CFW is based his 5.00M33.
Moz : For his beta test and translated language file in Italian.
BombingBasta: For his translated language file in French.
Rapper_skull: For his translated language file in Italian.
The Z : For his translated language file in German.
largeroliker : For his translated language file in Spanish.
Tipped OuT	: For his ICON for EBOOT.PBP.

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

To use custom font in RecoveryMenu, put font data at ms0:/seplugins/font_recovery.bin.

--History--
v9.8
-- Fixed a freeze bug when enter sleep with SpeedUpMs option is Enabled.
-- Added a Text color option in Recovery menu.
v9.4
-- Added a 166/83 CPU clock.
v9.3
-- Update version.txt loading. Now you can load txt from ms0:/seplugins/version.txt.
v9.2
-- Fixed a bug when ISO open in XMB.
-- Added a PBOOT.PBP execute option.
-- Fixed a UMDdrive noise when enter sleep mode.
v9.1
-- Fixed a bug in sctrlKernelSetUserLevel.
-- Added a patch for leda which allow to use it from ef0:/.
v9
-- Fixed a bug in Official Static ELF loading.
-- Fixed a CPU clock in GAME mode.
-- Added a patch for 0x80020148 error.
v8
-- Fixed a ISO Parental Level.
-- Fixed a bug in OE driver(again).
-- Fixed a bug that failed to sleep when you change the CPU clock.
-- Added a page controll in RecoveryMenu->Plugin.
-- Updated UMD Video mount. Now you can play UMD video/audio without dummy UMD.
v7
-- Fixed a crash when useing Dark_AleX's LEDA.
-- Change the iso Parental Level from 5 to 1.
-- Increase plugin path character buffer in PluginManager from 32 to 64.
v6
-- Added RecoveryMode translate and custom font feature.
-- Added Gameboot Skip option.
v5
-- Added RecoveryMenu option in VshMenu.
-- Fixed OEdriver bug.
v4
-- Fixed a bug when enter suspend with VshMenu opened.
-- Fixed MEdriver bug.
-- Added BatteryConfig submenu in RecoveryMenu.
-- Added Speed up MS option(beta).
v3
-- Added ResetVSH in VshMenu.
-- Skipped sceLogo after exit RecoveryMenu.
-- Added "Make AutoBoot Battery" option in RecoveryMenu.
v2
-- Fixed freeze bug when try to connect to PSN.
v1
-- First release.
-- Fixed RecoveryMenu text.
-- Fixed ExtraMemory bug.
-- Changed NetworkUpdates default value.
-- Source code optimization. 
