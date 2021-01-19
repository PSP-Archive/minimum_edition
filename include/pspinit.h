#ifndef __PSPINIT_H__

#define __PSPINIT_H__

enum PSPBootFrom
{
	PSP_BOOT_FLASH = 0, /* ? */
	PSP_BOOT_DISC = 0x20,
	PSP_BOOT_USBWLAN = 0x30,
	PSP_BOOT_MS = 0x40,
	PSP_BOOT_EF = 0x50,
	PSP_BOOT_FLASH3 = 0x80,
};

/*
60 CA 00 00 | no
40 CA 00 00 | 1 "game"-disc
0C CB 00 00 | 2 "updater"-disc update
4C CB 00 00 | 3 "updater" -"MSTKUPDATE" PSP_INIT_APITYPE_MS1 - EF1
C0 CB 00 00 | 4 "game" - PSP_INIT_APITYPE_MS2
C0 CB 00 00 | 5  "mlnapp" - PSP_INIT_APITYPE_MS2
18 CC 00 00 | 6 "pops" - PSP_INIT_APITYPE_MS5
70 CC 00 00 | 7 "app" - PSP_INIT_APITYPE_MS4 (comic reader)
C8 CC 00 00 | 8 "licensegame" - PSP_INIT_APITYPE_MS6
20 CD 00 00 | 9 "umdemu"-  PSP_INIT_APITYPE_UMDEMU_MS "disc0:/PSP_GAME/SYSDIR/EBOOT.BIN"
78 CD 00 00 | 10 "mlnapp" - "disc0:/PSP_GAME/SYSDIR/EBOOT.BIN"
D0 CD 00 00 | 11 "game" - "wlan0"
28 CE 00 00 | 12 "game" - "vsh"
3C CE 00 00 | 13 "game" - 0x160 Disc + PBOOT
60 CE 00 00 | 9 & 14 "umdemu" - 
B8 CE 00 00 |15? hibimage
*/

enum PSPInitApitype
{
	PSP_INIT_APITYPE_DISC_ELF = 0x110,
	PSP_INIT_APITYPE_DISC_BOOT_BIN = 0x111,
	PSP_INIT_APITYPE_UMDEMU_ELF_MS = 0x112,
	PSP_INIT_APITYPE_UMDEMU_BOOT_BIN_MS = 0x113,
	PSP_INIT_APITYPE_UMDEMU_ELF_EF = 0x114,
	PSP_INIT_APITYPE_UMDEMU_BOOT_BIN_EF = 0x115,

	PSP_INIT_APITYPE_DISC			= 0x120,
	PSP_INIT_APITYPE_DISC_UPDATER	= 0x121,
	PSP_INIT_APITYPE_UMDEMU_MS		= 0x123,//sceVshBridge_A74FB6AF (np9660 game , x-reader)
	PSP_INIT_APITYPE_PBOOT_MS		= 0x124,//sceVshBridge_8EC3E1EE
	PSP_INIT_APITYPE_UMDEMU_EF		= 0x125,//sceVshBridge_E60C7F79
	PSP_INIT_APITYPE_PBOOT_EF		= 0x126,//sceVshBridge_406BD248
	PSP_INIT_APITYPE_USBWLAN = 0x130, /* Game Shareing */
	PSP_INIT_APITYPE_MS1 = 0x140,
	PSP_INIT_APITYPE_MS2 = 0x141,
	PSP_INIT_APITYPE_MS3 = 0x142,
	PSP_INIT_APITYPE_MS4 = 0x143, /* comic reader */
	PSP_INIT_APITYPE_MS5 = 0x144, /* pops */
	PSP_INIT_APITYPE_MS6 = 0x145,//sceVshBridge_EC974808
	PSP_INIT_APITYPE_EF1 = 0x151,
	PSP_INIT_APITYPE_EF2 = 0x152,
	PSP_INIT_APITYPE_EF3 = 0x153,
	PSP_INIT_APITYPE_EF4 = 0x154,
	PSP_INIT_APITYPE_EF5 = 0x155,
	PSP_INIT_APITYPE_EF6 = 0x156,//sceVshBridge_B337A811
	PSP_INIT_APITYPE_PBOOT_DISC = 0x160,
	PSP_INIT_APITYPE_MLNAPP_MS = 0x170,//sceVshBridge_BE28284D
	PSP_INIT_APITYPE_MLNAPP_EF = 0x171,//sceVshBridge_1498A945
	PSP_INIT_APITYPE_VSH1 = 0x210, /* ExitGame */
	PSP_INIT_APITYPE_VSH2 = 0x220, /* ExitVSH */
};

enum PSPKeyConfig
{
	PSP_INIT_KEYCONFIG_VSH	= 0x100,
	PSP_INIT_KEYCONFIG_UPDATE	= 0x110,
	PSP_INIT_KEYCONFIG_GAME	= 0x200,
	PSP_INIT_KEYCONFIG_POPS	= 0x300,
	PSP_INIT_KEYCONFIG_APP	= 0x400,
};

/**
 * Gets the api type 
 *
 * @returns the api type in which the system has booted
*/
int sceKernelInitApitype();

/**
 * Gets the filename of the executable to be launched after all modules of the api.
 *
 * @returns filename of executable or NULL if no executable found.
*/
char *sceKernelInitFileName();

/**
 *
 * Gets the device in which the application was launched.
 *
 * @returns the device code, one of PSPBootFrom values.
*/
int sceKernelBootFrom();

/**
 * Get the key configuration in which the system has booted.
 *
 * @returns the key configuration code, one of PSPKeyConfig values 
*/
int InitForKernel_7233B5BC();

#define sceKernelInitKeyConfig InitForKernel_7233B5BC
#define sceKernelApplicationType InitForKernel_7233B5BC

#endif

