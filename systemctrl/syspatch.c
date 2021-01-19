#include <pspsdk.h>
#include <pspkernel.h>
#include <pspinit.h>
#include <psploadexec_kernel.h>
#include <psputilsforkernel.h>
#include <psppower.h>
#include <pspreg.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "rebootex_config.h"

#include "iso/isofs_driver.h"

#if PSP_MODEL == 0
#include "../rebootex/rebootex_01g.h"
#elif PSP_MODEL == 1
#include "../rebootex/rebootex_02g.h"
#elif ( PSP_MODEL == 2 || PSP_MODEL== 3 || PSP_MODEL== 6 || PSP_MODEL== 8 )
#include "../rebootex/rebootex_02g.h"
//#include "../rebootex/rebootex_03g.h"
#elif PSP_MODEL == 4
#include "../rebootex/rebootex_05g.h"
#endif

SEConfig config;

static int reboot_index = 0;
static STMOD_HANDLER previous = NULL;

extern u32 size_systemctrl;
extern char *systemctrl;

extern int k150_flag;

extern u32 memp2;
extern u32 memp8;

extern char *on_reboot_after;
extern void *on_reboot_buf;
extern int on_reboot_size;
extern int on_reboot_flag;


int WaitDevice( const char *device, const char *fat, const char *text_path );

void ms_patch();
void PatchMesgLed();
void DoNoUmdPatches();
int SystemCtrlForKernel_B86E36D1();


/*
void SetConfig(SEConfig *newconfig)
{
	memcpy(&config, newconfig, sizeof(SEConfig));
}*/

//SystemCtrlForKernel_2F157BAF
void sctrlSEApplyConfig(SEConfig *add)
{
	memcpy(&config, add , 112 /*sizeof(SEConfig) - 4*/ );
}

void sctrlSESetBootConfFileIndex(int type)
{
	reboot_index = type;
}

static inline int CheckIsoPath(const char *path)
{
	return ( path && ( strncmp( path, "ms0:/", 5) == 0
#if PSP_MODEL == 4
		|| strncmp( path, "ef0:/", 5) == 0
#endif
		));
}


int (* DecompressReboot)(u8 *dest, u32 destSize, const u8 *src, void *unk);
int DecompressRebootPatched(u8 *dest, u32 destSize, const u8 *src, void *unk)
{
	struct RebootexParam *rebootex_param = (void *)REBOOTEX_PARAM_OFFSET;

	//char *theiso = sctrlSEGetUmdFile();
	char *theiso = sctrlSEGetUmdFileEx( NULL );
	
	rebootex_param->file[0] = '\0';
//	*(char *)0x88FB0000 = 0;

	if (theiso)
	{	
//		strcpy( (void *)0x88FB0000 , theiso );
		strncpy( rebootex_param->file , theiso, REBOOTEX_FILELEN_MAX );
	}

//	memcpy((void *)0x88fb0050, &config,112/* sizeof(SEConfig)*/);
	memcpy( rebootex_param->config, &config, 0x70 );

	if(size_systemctrl)
		memcpy((void *)0x88FB0100 , systemctrl  , size_systemctrl);

	*(u32 *)0x88FB00F0 = size_systemctrl;

	rebootex_param->reboot_index	= reboot_index;
	rebootex_param->mem2			= memp2;
	rebootex_param->mem8			= memp8;
	rebootex_param->k150_flag		= k150_flag;
	rebootex_param->on_reboot_after	= on_reboot_after;
	rebootex_param->on_reboot_buf	= on_reboot_buf;
	rebootex_param->on_reboot_size	= on_reboot_size;
	rebootex_param->on_reboot_flag	= on_reboot_flag;

	
	//memcpy((void *)0x88FC0000, rebootex , size_rebootex );
	sceKernelGzipDecompress((void *)0x88FC0000, 0x4000 , rebootex ,0);

	return DecompressReboot(dest, destSize, src, unk);
}

#if _PSP_FW_VERSION == 639
#if PSP_MODEL != 4
#define LOADEXEC_JUMP_PATCH		0x00002DA8
#define LOADEXEC_REBOOT_PATCH_ADDR	0x00002D5C
#define ExitVSHVSH_Patch_Addr1	0x000016A4
#define ExitVSHVSH_Patch_Addr2	0x000016D8
#define LoadExecVSH_Patch_Addr1	0x000023D0
#define LoadExecVSH_Patch_Addr2	0x00002414
#else
#define LOADEXEC_JUMP_PATCH		0x00002FF4
#define LOADEXEC_REBOOT_PATCH_ADDR	0x00002FA8
#define ExitVSHVSH_Patch_Addr1	0x000016A4
#define ExitVSHVSH_Patch_Addr2	0x000016D8
#define LoadExecVSH_Patch_Addr1	0x00002624
#define LoadExecVSH_Patch_Addr2	0x00002668
#endif /* PSP_MODEL */

#elif _PSP_FW_VERSION == 660
#if PSP_MODEL != 4
#define LOADEXEC_JUMP_PATCH		0x00002DA8
#define LOADEXEC_REBOOT_PATCH_ADDR	0x00002D5C
#define ExitVSHVSH_Patch_Addr1	0x000016A4
#define ExitVSHVSH_Patch_Addr2	0x000016D8
#define LoadExecVSH_Patch_Addr1	0x000023D0
#define LoadExecVSH_Patch_Addr2	0x00002414
#else
#define LOADEXEC_JUMP_PATCH		0x00002FF4
#define LOADEXEC_REBOOT_PATCH_ADDR	0x00002FA8
#define ExitVSHVSH_Patch_Addr1	0x000016A4
#define ExitVSHVSH_Patch_Addr2	0x000016D8
#define LoadExecVSH_Patch_Addr1	0x00002624
#define LoadExecVSH_Patch_Addr2	0x00002668
#endif

#else
#error loadexec.c
#endif /* _PSP_FW_VERSION */

static void PatchLoadExec(SceModule2 *mod)
{
	u32 text_addr = mod->text_addr;
	//6.38
	
	_sw(0x3C0188FC, text_addr + LOADEXEC_JUMP_PATCH );//Jump to 0x88FC0000
	MAKE_CALL(text_addr + LOADEXEC_REBOOT_PATCH_ADDR , DecompressRebootPatched );

	DecompressReboot = (void *)(text_addr);
	
	// Allow ExitVSHVSH in whatever user level
	_sw( 0x10000008 , text_addr + ExitVSHVSH_Patch_Addr1 );
	_sw( 0			, text_addr + ExitVSHVSH_Patch_Addr2 );

	// Allow LoadExecVSH in whatever user level
	_sw( 0x1000000C , text_addr + LoadExecVSH_Patch_Addr1 );
	_sw( 0			, text_addr + LoadExecVSH_Patch_Addr2 );

	//u32 func_addr = search_module_import( mod, "ThreadManForKernel", 0xF6427665 );//sceKernelGetUserLevel
	// _sw(0x03e00008, func_addr ); _sw(0x24020004, func_addr + 4);

	//	ClearCaches();
}

#if PSP_MODEL != 0

int (* read_sfo)() = NULL;
int PatchMemsize(u32 buff, u32 a1, u32 a2)
{
	typedef struct {
		int signature;
		int version;
		u32 offset_key_table;
		u32 offset_values_table;
		u32 num_entries; 
	}PSFHeader;

	typedef struct {
		u16 label_off;
		char rfu001;
		char data_type;
		int datafield_used;
		int datafield_size;
		int data_off;
	}PSFSection;

	int sp=0;
	PSFHeader *header = (PSFHeader *)buff;
	PSFSection *section = (PSFSection *)(buff + sizeof(PSFHeader));

	if(memp2==0) 
	{
		int i;
		int *out;
		int cnt=header->num_entries;

		for(i=0; i< cnt;i++) 
		{
			if( strcmp((char *)(buff + header->offset_key_table + section->label_off),"MEMSIZE") == 0 ){
				out=(int *)(buff + header->offset_values_table + section->data_off);
				sp=out[0];

				break;
			}
			section += 1;//+16
		}

		if(sp) {
			sctrlHENSetMemory(51 , 0);
			SystemCtrlForKernel_B86E36D1();
		}
	}
	else 
	{
		SystemCtrlForKernel_B86E36D1();
		memp2=0;
	}

	return read_sfo(buff ,a1,a2);
}
#endif

static void PatchMediaSync(u32 text_addr )
{	
//6.xx
#define MediaSyncBlacklistAddr	0x000000C8
	//kill blacklist
	//addu       $v0, $zr, $zr
	_sw( 0x00001021 , text_addr + MediaSyncBlacklistAddr );

//	if ( sceKernelInitKeyConfig() == PSP_INIT_KEYCONFIG_GAME)
	char *filename = sceKernelInitFileName();
	if (filename)
	{
#define MediaSyncSfoErrorPatch1	0x00000864
#define MediaSyncSfoErrorPatch2	0x00000988
		// Patch mediasync (avoid error 0x80120005 sfo error) 
		//addu       $s1, $zr, $zr
		_sw(0x00008821, text_addr + MediaSyncSfoErrorPatch1 );//allow to load ELF
		_sw(0x00008821, text_addr + MediaSyncSfoErrorPatch2 );//allow to load Custom SFO.

#if PSP_MODEL != 0
		int api = sceKernelInitApitype();
		if( api == 0x141 || api == 0x152 )
//		if (strstr(filename, ".PBP"))
		{
#define MediaSyncExtraMemoryPatchAddr	0x0000097C
#define MediaSyncExtraMemoryPatchFunc	0x00000F40
			MAKE_CALL(text_addr + MediaSyncExtraMemoryPatchAddr, PatchMemsize);
			read_sfo = (void *)(text_addr + MediaSyncExtraMemoryPatchFunc );

		}
		else
#endif
		//else if (strstr(filename, "disc") == filename)
		 if( sceKernelBootFrom() == PSP_BOOT_DISC )
		{
			char *theiso = sctrlSEGetUmdFile();
			if( CheckIsoPath( theiso ) )
			{

#define MediaSyncDiscSfoPatchAddr	0x000003C4
				// Patch the error of diferent sfo
				_sw( 0x5000001D , text_addr+ MediaSyncDiscSfoPatchAddr );

				if (config.umdmode == 0)
				{
					DoAnyUmd(); 
				}
				else if(config.umdmode == 1)//
				{
#define MediaSyncDiscAssignPatchAddr	0x000001C0
					_sw(0x34020000, text_addr+ MediaSyncDiscAssignPatchAddr );
						
					sceIoDelDrv("isofs");
					sceIoAddDrv(getisofs_driver());

					DoNoUmdPatches();
					sceIoAssign("disc0:", "umd0:", "isofs0:", IOASSIGN_RDONLY, NULL, 0);
				}
			}
		}
	}

	ClearCaches();
}


//loc_0000183C
int sceChkregGetPsCodePatched(u8 *pscode)
{
	pscode[0] = 0x01;
	pscode[1] = 0x00;

	u8 reg = config.fakeregion & 0xFF;

	// 12 -> DebugI
	// 13 -> DebugII

	if(reg < 12)
	{
		reg += 2;
	}
	else
	{
		reg -= 11;
	}

	if (reg == 2)
		reg = 3;

	pscode[2] = reg;
	pscode[3] = 0x00;
	pscode[4] = 0x01;
	pscode[5] = 0x00;
	pscode[6] = 0x01;
	pscode[7] = 0x00;

	return 0;
}

static void PatchChkreg()
{
	u32 pscode = (u32)FindProc("sceChkreg", "sceChkreg_driver", 0x59F8491D);//sceChkregGetPsCode
	if (pscode) {
		if (config.fakeregion)
		{
			REDIRECT_FUNCTION(pscode, sceChkregGetPsCodePatched);
//			ClearCaches();
		}
	}

}


void UsbChargeStart(void);

static void OnImposeLoad()
{
	sctrlSEGetConfig(&config);	
	PatchChkreg();


//	if ( sceKernelInitKeyConfig() == PSP_INIT_KEYCONFIG_GAME)
	if( sceKernelBootFrom() == PSP_BOOT_DISC )
	{
		char *theiso = sctrlSEGetUmdFile();
		if( CheckIsoPath( theiso ) )
		{
			if (config.umdmode == 1)
			{
				sceIoDelDrv("umd");
				sceIoAddDrv(getumd9660_driver());
			}
		}	
	}
	else
	{
		sctrlSESetUmdFile("");
		reboot_index = 0;
	}

#if PSP_MODEL != 0
	if(config.usbcharge)
	{
		UsbChargeStart();
	}
#endif	

	SceModule2 *mod = sceKernelFindModuleByName("sceLoadExec");
	if (mod)
	{
		PatchLoadExec(mod);
		PatchMesgLed();
		
		mod = sceKernelFindModuleByName("sceSYSCON_Driver");
		if (mod) {
			create_DNR_list("sceSyscon_driver", mod->text_addr );
		}	

	}

	ClearCaches();

/*
#if PSP_MODEL == 0
	if( k150_flag )
	{
		//500 = 0x777FF2D9
		void (*clockgen_k)() = sctrlHENFindFunction( "sceClockgen_Driver", "sceClockgen_driver" , 0x59E7DFE6 );
		clockgen_k();
	}
#endif
*/
}

static void PatchPower(u32 text_addr)
{
//scePowerGetBacklightMaximum
#if _PSP_FW_VERSION == 639
#define POWER_PATCH_ADDR	0x00000E20
#elif _PSP_FW_VERSION == 660
#define POWER_PATCH_ADDR	0x00000E68
#endif

	_sw(0, text_addr + POWER_PATCH_ADDR );
}

static void PatchWlan( u32 buf)
{
//Patch scePowerWlanActivate
//6.xx
#define WLAN_PATCH_ADDR	0x26C0
	_sw(0, buf + WLAN_PATCH_ADDR );
}

#if ( PSP_MODEL != 0 && PSP_MODEL != 4 )
static void PatchUmdCache(SceModule2 *mod)
{
	if (sceKernelInitKeyConfig() == PSP_INIT_KEYCONFIG_GAME)
	{
		int boot_from = sceKernelBootFrom();
		if ( boot_from == PSP_BOOT_MS) // || boot_from == PSP_BOOT_EF 
		{
//			u32 module_start_addr = (u32)(mod->entry_addr);//search_module_export( mod, NULL, 0xD632ACDB );
			MAKE_DUMMY_FUNCTION1( (u32)( mod->entry_addr) );
			ClearCaches();
		}
	}
}
#endif

#if PSP_MODEL != 0
static void ReleaseExtraMemory()
{		
	//memset((void *)0xBC000040 , 0xFF , 0x40)
	u32 addr = 0xBC000040;
	do{
		_sw( 0xFFFFFFFF , addr );
		addr += 4;
	}while( addr < 0xBC000080 );

	u32 *address =(u32 *)sctrlKernelGetPartition(8);
	u32 *next=(u32 *)(address[16/4]);

	address[4/4]=(51 << 20) + 0x88800000;
	address[8/4]= 1 << 20;
	next[20/4]=(1 << 21)|0xFC;
}
#endif

#if PSP_MODEL != 4
static void PatchUmdMan( u32 text_addr )
{
//6.xx
#define UMDMAN_PATCH_ADDR	0x431C

	if (sceKernelBootFrom() == PSP_BOOT_MS)
	{
		// Replace call to sceKernelBootFrom with return PSP_BOOT_DISC
		//search_module_import(SceModule2 *pMod, "InitForKernel", 0x27932388 );//sceKernelBootFrom
		_sw(0x24020020, text_addr + UMDMAN_PATCH_ADDR );
		ClearCaches();
	}

}

static void PatchIsofsDriver(SceModule2 *mod )
{
	// Patch StopModule to avoid crash at exit...
	char *iso = sctrlSEGetUmdFile();
	if( CheckIsoPath( iso ) )
	{
		if (config.umdmode == 1 )	
		{
			/* make module exit inmediately */
//			u32 module_start_addr = (u32)(mod->entry_addr);//search_module_export( mod, NULL, 0xD632ACDB );
			MAKE_DUMMY_FUNCTION1( (u32)(mod->entry_addr) );

			ClearCaches();
		}
	}
}

#endif

static void NpSignupPluginPatch(u32 text_addr)
{
#if _PSP_FW_VERSION == 639
#define NpSignupPlugin_PatchAddr	0x00037264
#elif _PSP_FW_VERSION == 660
#define NpSignupPlugin_PatchAddr	0x00038CBC
#endif
	
	_sw(0x3C041000,text_addr + NpSignupPlugin_PatchAddr);
}

static void VshNpSigninPatch(u32 text_addr)
{
#if _PSP_FW_VERSION == 639
#define VshNpSignin_PatchAddr1	0x00006CB4
#define VshNpSignin_PatchAddr2	0x00009684
#elif _PSP_FW_VERSION == 660
#define VshNpSignin_PatchAddr1	0x00006CF4
#define VshNpSignin_PatchAddr2	0x000096C4
#endif

	_sw(0x10000008 , text_addr + VshNpSignin_PatchAddr1 );	
	_sw(0x3C041000 , text_addr + VshNpSignin_PatchAddr2 );
}

//sub_00000FF4:
int sceKernelApplyPspRelSectionPatched(void *buffer)
{
	SceModule2 *mod = buffer;	
	u32 buf = mod->text_addr;
	char *modinfo=mod->modname;

	if (strcmp(modinfo, "sceMediaSync") == 0)
	{
		PatchMediaSync( buf);
	}
	else if (strcmp(modinfo, "sceLowIO_Driver") == 0)	
	{
		mallocinit();
#if PSP_MODEL != 0

		int api = sceKernelInitApitype();
		if( api == 0x141 || api == 0x152 )
		{
			ReleaseExtraMemory();
			ClearCaches();
		}
#endif
	}
	else if (strcmp(modinfo, "sceWlan_Driver") == 0)	
	{
		PatchWlan( buf);	
		ClearCaches();
	}
	else if (strcmp(modinfo, "scePower_Service") == 0)	
	{
		PatchPower( buf );
		ClearCaches();
	}

#if ( PSP_MODEL != 0 && PSP_MODEL != 4 )
	else if(strcmp(modinfo, "sceUmdCache_driver") == 0)
	{
		PatchUmdCache( mod );
	}
#endif

	else if (strcmp(modinfo, "sceNpSignupPlugin_Module") == 0)	
	{
		NpSignupPluginPatch( buf );
		ClearCaches();
	}
	else if (strcmp(modinfo, "sceVshNpSignin_Module") == 0)	
	{
		VshNpSigninPatch( buf );
		ClearCaches();
	}
	else if (strcmp(modinfo, "sceImpose_Driver") == 0)	
	{				
		OnImposeLoad();	
	}
#if PSP_MODEL != 4
	else if (strcmp(modinfo, "sceUmdMan_driver") == 0)
	{	
		PatchUmdMan( buf);		
	}
	else if (strcmp(modinfo, "sceIsofs_driver") == 0)	
	{
//		int api = sceKernelInitApitype();
//		if ( api <= 0x120 && api >= 0x110 )
//		if (sceKernelInitKeyConfig() == PSP_INIT_KEYCONFIG_GAME)
		if(sceKernelBootFrom() == PSP_BOOT_DISC )
		{
			PatchIsofsDriver( mod );
		}
	}
#else
	//05g
	else if( strcmp(modinfo, "pspMarch33_Driver") == 0 )
	{
		_sw( 0, buf + 0x000009E8 );
		ClearCaches();
	}
	else if( strcmp(modinfo, "Legacy_Software_Loader") == 0 )
	{
		if( sceKernelInitApitype() == PSP_INIT_APITYPE_EF2 )
		{
			_sw( 0x10000005, buf + 0x0000014C );	
			ClearCaches();
		}
	}
#endif

	if( previous )
	{
		previous( buffer );
	}
	
	static int clock_fastms_flag = 0;
	if(!clock_fastms_flag)
	{
		if( sceKernelGetSystemStatus() == 0x20000 )
		{
//			sceIoAssign("ms1:", "msstor0p0:"/*"msstor0p1:"*/, "fatms0:", IOASSIGN_RDWR, NULL, 0);

			int key = sceKernelInitKeyConfig();
			if( key == PSP_INIT_KEYCONFIG_GAME
				&& sceKernelBootFrom() == PSP_BOOT_DISC
				&& sceKernelInitApitype() != 0x121)
			{
				SetSpeed(config.umdisocpuspeed, config.umdisobusspeed);
			}

			int msconfig = config.fastms;

			if( ( ( msconfig & 1) && ( key == PSP_INIT_KEYCONFIG_VSH) ) || 
				( ( msconfig & 2) && ( key == PSP_INIT_KEYCONFIG_GAME)) || 
				( ( msconfig & 4) && ( key == PSP_INIT_KEYCONFIG_POPS))
				)
			{
				ms_patch();
			}
			clock_fastms_flag=1;
		}
	}

	return 0;
}

STMOD_HANDLER sctrlHENSetStartModuleHandler(STMOD_HANDLER handler)
{
	STMOD_HANDLER r = previous;
	previous= (STMOD_HANDLER)((u32)handler | 0x80000000 );
	return r;
}

int (*StartModuleEX)(int , SceSize , void *, int *, SceKernelSMOption *) = NULL;
void* sctrlSetStartModuleExtra(int (*func)() )
{
	void * ret = (void *)StartModuleEX;
	StartModuleEX = func;
	return ret;
}

int sceKernelStartModulePatched(SceUID modid, SceSize argsize, void *argp, int *status, SceKernelSMOption *option)
{
	static int plugin_loaded = 0;
	SceModule2 *mod = sceKernelFindModuleByUID(modid);
	if (mod )
	{
		if(StartModuleEX ) 
		{
			int ret;
			if( ( ret = StartModuleEX(modid, argsize, argp, status, option ) ) >= 0)
			{
				return ret;
			}
		}

		if (strcmp(mod->modname, "vsh_module") == 0)
		{
			if (sceKernelInitKeyConfig() == PSP_INIT_KEYCONFIG_VSH)
			{
				if( config.skiplogo || config.startupprog )
				{
					u32 *vshmain_args = (u32 *)sctrlKernelMalloc(1024);
					memset(vshmain_args, 0, 0x0400);
			
					if ( argsize )
						memcpy( vshmain_args , argp , argsize );
		
					vshmain_args[0/4] = 0x0400;//sizeof(vshmain_args)
					vshmain_args[4/4] = 0x20;
					vshmain_args[0x40/4] = 1;
#if PSP_MODEL != 4
					if( config.startupprog && argsize == 0)
					{
						struct SceKernelLoadExecVSHParam param;
						memset(&param, 0, sizeof(param));
						param.size = sizeof(param);
						param.args = sizeof("ms0:/PSP/GAME/BOOT/EBOOT.PBP");
						param.argp = "ms0:/PSP/GAME/BOOT/EBOOT.PBP";
						param.key = "game";
						param.vshmain_args_size = 0x0400;
						param.vshmain_args = vshmain_args;

						sctrlKernelLoadExecVSHMs2("ms0:/PSP/GAME/BOOT/EBOOT.PBP", &param);
					}
#endif

					int ret = sceKernelStartModule(modid ,1024 ,vshmain_args ,status ,option);
					sctrlKernelFree(vshmain_args);
					return ret;
				}
			}
		}
		else if(!plugin_loaded)
		{
			int keyconfig = sceKernelInitKeyConfig();
			if( sceKernelFindModuleByName( (keyconfig == PSP_INIT_KEYCONFIG_POPS)?"sceIoFilemgrDNAS":"sceMediaSync") != NULL )
			{
				plugin_loaded = 1;				
				char* text_path = NULL;
				int load_flag = 1;

				switch(keyconfig)
				{
				case PSP_INIT_KEYCONFIG_VSH:

					//check updata or not
					if( sceKernelFindModuleByName("scePspNpDrm_Driver") != NULL)
					{
						load_flag = config.plugvsh;
						text_path = "ms0:/seplugins/vsh.txt";
					}
/*					else
					{
						load_flag = config.plugvsh;
						text_path = "ms0:/seplugins/updater.txt";
					}
*/					break;

				case PSP_INIT_KEYCONFIG_GAME:

					load_flag = config.pluggame;
					text_path = "ms0:/seplugins/game.txt";
					break;

				case PSP_INIT_KEYCONFIG_POPS:

					load_flag = config.plugpop;
					text_path = "ms0:/seplugins/pops.txt";
					break;
				case PSP_INIT_KEYCONFIG_APP:
					load_flag = 0;
					text_path = "ms0:/seplugins/app.txt";
					break;
				}	

#if PSP_MODEL == 4
				int ef_flag = sctrlKernelMsIsEf();
#endif

				if(text_path && !load_flag)
				{	
					int ret = 0;

#if PSP_MODEL == 4
					if( ef_flag == 0 )
#endif
					{
						ret = WaitDevice("mscmhc0:", "fatms0:", text_path );
					}

#if PSP_MODEL == 4
					if( ef_flag == 1 || ret < 0 || keyconfig == PSP_INIT_KEYCONFIG_VSH || keyconfig == PSP_INIT_KEYCONFIG_POPS )
					{
						text_path[0] = 'e';
						text_path[1] = 'f';
						WaitDevice( NULL, NULL, text_path );
					}
#endif
				}
			}
		}
	}

	return sceKernelStartModule(modid, argsize, argp, status, option);	
}

#if _PSP_FW_VERSION == 639
#define InitStartAddr		0x00001A4C
#define InitStartModuleAddr	0x00001CBC

#elif _PSP_FW_VERSION == 660
#define InitStartAddr		0x00001A4C
#define InitStartModuleAddr	0x00001C3C

#else
#error init
#endif

int Patch_Init(int (* real_init)(),  u32 a1)//init patch
{
	u32 text_addr = (u32)real_init - InitStartAddr;
	MAKE_JUMP( text_addr + InitStartModuleAddr  , sceKernelStartModulePatched);
	ClearCaches();
	return real_init(4,a1);
}

