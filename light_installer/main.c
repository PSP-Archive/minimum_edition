/*************************************/
/*									*/
/*************************************/

#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdebug.h>
#include <psppower.h>
#include <pspsysmem_kernel.h>

#include <psploadexec_kernel.h>
#include <kubridge.h>
#include <systemctrl_me.h>
//#include <pspvshbridge.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "cfw_modules.h"

#define printf pspDebugScreenPrintf

#if _PSP_FW_VERSION == 639
#define DEVKIT_VER	0x06030910
#define VER_STR	"6.39"
#elif _PSP_FW_VERSION == 660
#define DEVKIT_VER	0x06060010
#define VER_STR	"6.60"
#endif

PSP_MODULE_INFO("plutonium_updater", 0x800, 0, 1);

int WriteFile(char *file, void *buf, int size)
{
	SceUID fd = sceIoOpen(file, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	if (fd < 0)	return -1;

	int written = sceIoWrite(fd, buf, size);
	if (sceIoClose(fd) < 0) return -1;

	return written;
}

void ErrorExit(int milisecs, char *fmt, ...)
{
	va_list list;
	char msg[256];

	va_start(list, fmt);
	vsprintf(msg, fmt, list);
	va_end(list);

	pspDebugScreenPrintf(msg);

	sceKernelDelayThread(milisecs*1000);
	sceKernelExitGame();
}
/*
int StopUnloadModule(SceUID id)
{
	SceUID r = sceKernelStopModule(id ,0 ,NULL , NULL, NULL);
	if(r<0) return r;

	return sceKernelUnloadModule(id);
}

int LoadStartModuleBuffer(char *buff,int size , int flag)
{
	SceUID mod = vshKernelLoadModuleBufferVSH(size ,buff, 0, NULL);
	if (mod < 0) return mod;

	return sceKernelStartModule(mod, (flag == 0)? 0 : 4 , (void *)flag , NULL, NULL);
}

int LoadStartModule(void *path)
{
	SceUID mod = vshKernelLoadModuleVSH(path, 0, NULL);
	if (mod < 0) return mod;

	return sceKernelStartModule(mod,0, NULL, NULL, NULL);
}
*/
void Agreement()
{
	SceCtrlData pad;

	while (1) {
		sceCtrlReadBufferPositive(&pad, 1);

		if (pad.Buttons & PSP_CTRL_CROSS) {
			return;			
		}
		else if (pad.Buttons & PSP_CTRL_RTRIGGER) {
			ErrorExit(5000,"Update canceled by user.\n");
		}

		sceKernelDelayThread(10000);
	}
}

int ByPass(u32 mask )
{
	SceCtrlData pad;
	
	sceCtrlReadBufferPositive(&pad, 1);

	if ((pad.Buttons & mask) == mask)
			return 1;
	
	return 0;
}
/*
int GetMD5Buffer(u8 *buf, u32 size, u8 *md5)
{
	u8 digest[16];
	sceKernelUtilsMd5Digest( buf , size , digest );
	memcpy( md5 , digest , 0x10 );
	return 0;
}
u8 buff[0x1000];

int CheckMD5(char *path ,u8 *hash )
{
	SceUID fd;
	u8 digest[16];
	SceKernelUtilsMd5Context ctx;
	int size;

	printf("Verifying %s... ",path);

	if((fd = sceIoOpen(path , PSP_O_RDONLY, 0777)) < 0) {
		printf("Cannot open file (0x%08X).\n",fd);
		return -1;
	}

	sceKernelUtilsMd5BlockInit(&ctx);

	while((size = sceIoRead(fd , buff , 0x1000)) > 0) {
		sceKernelUtilsMd5BlockUpdate(&ctx, buff , size);
	}

	sceIoClose(fd);
	sceKernelUtilsMd5BlockResult(&ctx, digest);

	if ( memcmp( hash , digest , 16) ) {
		printf("Incorrect file.\n");
		return -1;
	}

	printf("OK\n");

	return 0;
}
*/
void reassign_flash()
{
	if(sceIoUnassign("flash0:") < 0)
		ErrorExit(5000,"Error in unassign.\n");

	if (sceIoAssign("flash0:", "lflash0:0,0", "flashfat0:", IOASSIGN_RDWR, NULL, 0) < 0)
		ErrorExit(5000,"Error in assign.\n");
}
/*
u8 rebootex_buff[0x4000];
static int (*func_rebootex)(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);

int rebootex_cp(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5){

	memcpy((void *) 0x88FC0000, rebootex_buff, 0x4000 );
	memset((void *) 0x88FB0000, 0, 256);

//		_sw(model, 0x88FB0000);     
	return func_rebootex(a1, a2, a3, a4, a5);
}
*/
/*
void start_launch()
{
	_sw( rebootex_cp, (void *)0x08fb0000 );
	int ret = LoadStartModule("flash0:/kd/reboot63x.prx");
	if( ret < 0 )
	{
		ErrorExit( 5000 , "Load error -> 0x%08X\n", ret);
	}

	memcpy( rebootex_buff , (u8 *)0x08fc0000 , 0x4000 );
}
*/
int main(int argc, char** argv)
{
	int i;
	pspDebugScreenInit();

	if(sceKernelDevkitVersion() != DEVKIT_VER )
	{
		ErrorExit(5000,"This program require "VER_STR"\n");
	}

	int model = kuKernelGetModel();
/*	if( !( model == 0 || model == 1 || model == 2) )
	{
		ErrorExit(5000,"This installer does not support this model.\n");
	}
*/
	/*
	if( ByPass(PSP_CTRL_START | PSP_CTRL_SELECT ))
	{
		goto JUMP;
	}
	*/
	u32 se_ver = sctrlSEGetVersion();
	if( se_ver < 0)
		se_ver = 0;

	/*
	if( se_ver >= 0x20001)
		ErrorExit(5000, "This update or a higher one was already applied.\n");
	*/

	printf("\nLCFW "VER_STR" installer by neur0n.\n\n");

	Module *modules = NULL;

	if( model == 0 )
		modules = (Module *)modules_01g;
	else if( model == 1)
		modules = (Module *)modules_02g;
	else if( model == 2)
		modules = (Module *)modules_03g;
	else if( model == 3)
		modules = (Module *)modules_04g;
	else if( model == 4)
		modules = (Module *)modules_05g;
	else if( model == 6)
		modules = (Module *)modules_07g;
	else if( model == 8)
		modules = (Module *)modules_09g;

#if _PSP_FW_VERSION == 660
	else if( model == 10)
		modules = (Module *)modules_11g;
#endif
	else
		ErrorExit(5000,"This installer does not support this model.\n");

	printf("Changes:\n\n");
	
	switch( se_ver & 0xFF ) {
#include "../installer/change_log.h"
	}
	printf("\n");


	printf("Press X to install modules\n"
		"Press [] to uninstall modules\n"
/*		"Press O to start LCFW\n"
*/		"R to exit.\n\n");


	char name_buff[256];
	SceCtrlData pad;
	while (1)
	{
		sceCtrlReadBufferPositive(&pad, 1);

		if (pad.Buttons & PSP_CTRL_CROSS)
		{
			reassign_flash();
/*
			sprintf( name_buff , "flash0:/kd/pspbtknf_0%dg.bin", model + 1);
			sceIoRemove( name_buff );
			sprintf( name_buff , "flash0:/kd/pspbtlnf_0%dg.bin", model + 1);
			sceIoRemove( name_buff );
*/
			sceIoRemove("ms0:/PSP/SYSTEM/isocaches.bin" );
//			sceIoRemove("flash0:/vsh/module/satelite.prx");


			for(i = 0; i < COMMON_MODULES_COUNT; i++)	
			{
				printf("Flashing %s (%d)... ",common_modules[i].dst, common_modules[i].size);
				WriteFile(common_modules[i].dst, common_modules[i].buffer, common_modules[i].size);
				printf("OK\n");
			}

			for(i = 0; i < MODULES_COUNT; i++)
			{
				printf("Flashing %s (%d)... ",modules[i].dst, modules[i].size);
				WriteFile(modules[i].dst, modules[i].buffer, modules[i].size);
				printf("OK\n");
			}

			ErrorExit(6000,"\nUpdate complete. Restarting in 6 seconds...\n");
///			printf("\nUpdate complete. Do you want to launch LCFW? (yes = X , no = R)\n");
//			Agreement();
//			start_launch();
		}
		else if (pad.Buttons & PSP_CTRL_SQUARE )
		{
			reassign_flash();
			for(i = 0; i < COMMON_MODULES_COUNT; i++)	
			{
				printf("Removing %s ... ",common_modules[i].dst );
				sceIoRemove(common_modules[i].dst );
				printf("OK\n");
			}

			for(i = 0; i < MODULES_COUNT; i++)
			{
				printf("Removing %s ... ",modules[i].dst );
				sceIoRemove(modules[i].dst );
				printf("OK\n");
			}

			sprintf( name_buff , "flash0:/kd/pspbtknf_0%dg.bin", model + 1);
			sceIoRemove( name_buff );
			sprintf( name_buff , "flash0:/kd/pspbtlnf_0%dg.bin", model + 1);
			sceIoRemove( name_buff );

			sceIoRemove("flash0:/vsh/module/satelite.prx");

			const char *setting[] = {
				"flash1:/config.me",
				"flash1:/vsh.me",
			};

			for(i=0;i<(sizeof(setting)/sizeof(char*));i++)
			{
				printf("Removing %s ... ",setting[i] );
				sceIoRemove(setting[i]);
				printf("OK\n");
			}

			ErrorExit(6000,"\n uninstall complete. Restarting in 6 seconds...\n");
		}
		else if (pad.Buttons & PSP_CTRL_RTRIGGER)
		{
			ErrorExit(5000,"Update canceled by user.\n");
		}
		sceKernelDelayThread(10000);
	}


	return 0;
}

