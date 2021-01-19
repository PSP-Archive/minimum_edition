LCFW 6.39LME installer for OFW6.39

-- What is this? -- 
This is LCFW for OFW6.39.

The degree of stability is still unknown. 
Please test it on your own and refer to different people's test reports.


-- How to use --
Copy "installer" folder and "launcher" folder at "ms0:/PSP/GAME/"

-- Preparation --
First. You need to install LCFW modules in your PSP.

1.Execute "LME Installer for 6.39" from XMB.
2.You can select these action:
Press x to install LCFW.
Press [] to uninstall LCFW.
Press R to exit.

After the action, PSP will rboot.

-- Start LCFW --
1.Execute "LME Launcher for 6.39" from XMB.
2. enjoy :)

-- How to enter RecoveryMenu? --
From XMB :Open VshMenu and select "Enter Recovery ->"
From LCFW:Execute Launcher again.
From OFW :Execute Launcher with hold "R".

-- How to mount UmdVideo? --
0. Enable UmdVideo option at RecoveryMenu->config->Use UmdVideo Patch (Go only).
1. copy Iso file at ms(ef)0:/ISO/VIDEO/ .
2. Open VshMenu and select Iso file name.

-- Credit --
some1: For his sceHttpStorageOpen kxploit.
liquidzigong: For his 639kxploit POC.
bbtgp: For his PrxEncrypter.
Draan: For his kirk-engine code.
J416: For his rebootex sample.
N-zaki : For providing PSPGo from him.

BombingBasta: For his translated language file in French.
Rapper_skull: For his translated language file in Italian.
The Z : For his translated language file in German.
largeroliker : For his translated language file in Spanish.
Moz	    : For his translated language file in Italian
Tipped OuT	: For his ICON for EBOOT.PBP.

Thanks for beta tester
@tablet6883
@Zooner_MT
@sayane_0032
@apolo1192
@PopStatue
@dsk_ro_ra
@potechihime
@Mr__Nerve
@wakatake0507
@tsumurin21
@altheme0r
@gluishia
@__taba__
@g_thail
@Satan_Rage
@teck4
@SnyFbSx


-- About translating the recovery menu --
To translate the recovery ,create a text file in ms0:/seplugins/xx_recovery.txt or flash1:/xx_recoery.txt.
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
-- Fixed a license error when execute resumed POPS game.(05g only)
-- Fixed a bug in POPS plugin when M2 card is not inserted.
-- Added a 166/83 CPU clock.
v9.3
-- Fixed a license error when execute resumed game.(05g only)
-- Update version.txt loading. Now you can load txt from ms0:/seplugins/version.txt.
-- Update NidResolver for FuSa_SD.prx.
v9.2
-- Fixed a bug when ISO open in XMB.
-- Added a PBOOT.PBP execute option.
-- Fixed a UMDdrive noise when enter sleep mode.
-- Update Plugin loading. Both ms0 and ef0 plugins are loaded in XMB.(05g only)
v9.1
-- Fixed a bug in sctrlKernelSetUserLevel.
-- Added a patch for leda which allow to use it from ef0:/.
v9
-- Fixed a bug in Official Static ELF loading.
-- Fixed a CPU clock in GAME mode.
-- Added a patch for 0x80020148 error.
-- Fixed a UmdMountPatch (beta4. 05g only)
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
v6 fix2
-- Fixed Launcher bug.
v6 fix
-- Fixed PSN game bug.
v6
-- Added RecoveryMode translate and custom font feature.
-- Added Gameboot Skip option.
v5
-- Added RecoveryMenu option in vshmenu.
-- Fixed a bug in OE driver.
v4
-- Fixed a bug when enter suspend with VshMenu opened.
-- Fixed MEdriver bug.
-- Added BatteryConfig submenu in RecoveryMenu.
-- Added Speed up MS option(beta).
v3
-- Update 639Launcher. Now you can switch between PRO-B6 and ME-3 :)


