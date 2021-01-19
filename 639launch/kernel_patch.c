
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspsdk.h>
#include <pspiofilemgr.h>
#include <psputility.h>
#include <psputility_htmlviewer.h>
#include <psploadexec.h>
#include <psploadexec_kernel.h>
#include <psploadcore.h>
#include <psputils.h>
#include <psputilsforkernel.h>
#include <pspsysmem.h>
#include <psppower.h>
#include <string.h>

#include "main.h"


#if _PSP_FW_VERSION == 639
int MemlmdPatchAddr_01g[] = { 0x00000F88, 0x00001150, 0x000011A4, 0x00000134, 0x00000E88, 0x00000EEC };
int MemlmdPatchAddr_02g[] = { 0x00001078, 0x00001240, 0x00001294, 0x134		, 0x00000F78, 0x00000FDC };
#elif _PSP_FW_VERSION == 660
int MemlmdPatchAddr_01g[] = { 0x00001070, 0x00001238, 0x0000128C, 0x0000020C, 0x00000F70, 0x00000FD4 };
int MemlmdPatchAddr_02g[] = { 0x000010F8, 0x000012C0, 0x00001314, 0x0000020C, 0x00000FF8, 0x0000105C };
#endif

/*
int MemlmdPatchAddr_01g[] = { 0x00001308, 0x000011D0, 0x00000F88, 0x00001150, 0x000011A4, 0x00000134, 0x00000E88, 0x00000EEC };
int MemlmdPatchAddr_02g[] = { 0x00001430, 0x000012C0, 0x00001078, 0x00001240, 0x00001294, 0x134		, 0x00000F78, 0x00000FDC };
*/

u32 loadexec_patch_list[] =		{ 0x00002D5C, 0x00002DA8, 0x000023D0, 0x00002414, };
u32 loadexec_patch_list_05g[] = { 0x00002FA8, 0x00002FF4, 0x00002624, 0x00002668, };

/** Recovery the instruction we smashed */
void recovery_sysmem(void)
{
#if _PSP_FW_VERSION == 639
	_sw(0x3C058801, 0x8800CC34); // lui $a1, 0x8801
#elif _PSP_FW_VERSION == 660
	_sw(0x3C058801, 0x8800CBB8); // lui $a1, 0x8801
#endif

}

static int (*func_rebootex)(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);

extern u8 rebootex_buff[];
extern int is_exploited;
extern int model;


static int rebootex_cp(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5){

	memcpy((void *) 0x88FC0000, rebootex_buff, 0x4000 );
	memset((void *) 0x88FB0000, 0, 256);

	_sw( is_exploited , 0x88FB0008);
	_sw( model , 0x88FB000C);

	return func_rebootex(a1, a2, a3, a4, a5);
}


int (* MemlmdSigcheck)() = NULL;
int (* MemlmdDecrypt)(u8 *buf, int size, int *ret, int u) = NULL;

int MemlmdSigcheckPatch(void *buf,int u,int flag)
{
	PSP_Header *head=(PSP_Header *)buf;
	int i;

	if(head->signature == 0x5053507E)//PSP~
	{
		for(i=0;i<0x30;i++) {
			if(head->scheck[i] != '\0') {
				if(head->reserved2[0] !=0 && head->reserved2[1]!=0 )
					return MemlmdSigcheck(buf, u ,flag);
				break;
			}
		}
	}

	return 0;
}


static void scramble(unsigned char *target, int size, unsigned char *seed, int seed_size)
{
	int seed_offset = 0;
	unsigned char *end_buffer = target + size;

	while( target < end_buffer )
	{
		if( seed_offset >= seed_size )
			seed_offset = 0;

		*target ^= seed[seed_offset];
		seed_offset++;
		target++;
		
	}

}

static void scramble_simple(unsigned int *target, unsigned int *seed, int size )
{
	unsigned int *end_buffer = target + size/sizeof(int);

	while( target < end_buffer )
	{
		*target ^= *seed;

		seed++;
		target++;
	}
}

static int CheckCompress(PSP_Header *header)
{
	u8 *data = ((u8 *)header ) + 336;
	scramble_simple( (u32 *)data , (u32 *)header->key_data1 , 0x10 );
	scramble( data , header->comp_size , &(header->scheck[0x38]) , 0x20 );
	return 1;
}

int MemlmdDecryptPatched(u8 *buf, int size ,int *ret, int flag )
{
	PSP_Header *head=(PSP_Header *)buf;

	if(buf && ret) {
		if(head->oe_tag ==0xC6BA41D3 ) {
			if( CheckCompress( head ) ) {
				*ret=head->comp_size;
				memmove(buf ,buf+336 ,*ret);
				return 0;
			}
		}
	}

	return MemlmdDecrypt(buf, size, ret, flag);
}


int kernel_permission_call(void)
{
	void (* _sceKernelIcacheInvalidateAll)(void)			= (void *)0x88000E98;
	void (* _sceKernelDcacheWritebackInvalidateAll)(void)	= (void *)0x88000744;

#if _PSP_FW_VERSION == 639
	void* (*_sceKernelFindModuleByName)(char *) = (void *) 0x8801E2D8;// 0x00017000 + 0x000072D8
	int (* _sceKernelGetModel)() = (void *)( 0x88000000 + 0x0000A13C );
#elif _PSP_FW_VERSION == 660
	void* (*_sceKernelFindModuleByName)(char *) = (void *) 0x8801DF98;// 0x00017000 + 0x00006F98
	int (* _sceKernelGetModel)() = (void *)( 0x88000000 + 0x0000A0B0 );
#endif

	recovery_sysmem();
	unsigned int addr;
	SceModule2 *mod;

	mod = _sceKernelFindModuleByName("sceLoadExec");
	addr = mod->text_addr;

	model = _sceKernelGetModel();

	const u32 *loadexex_list = (model == 4 )? loadexec_patch_list_05g :loadexec_patch_list;

	MAKE_CALL( addr + loadexex_list[0] ,rebootex_cp);
	_sw( 0x3C0188FC, addr + loadexex_list[1] ); // lui $at, 0x88FC
	
	_sw(0x1000000C, addr + loadexex_list[2] );
	_sw(0, addr + loadexex_list[3] );
	
	func_rebootex = (void *) addr;

	if( _sceKernelFindModuleByName("SystemControl") && _sceKernelFindModuleByName("sctrlHoroscope") )
	{
		is_exploited = 1;
		return 0;
	}

	mod = _sceKernelFindModuleByName("sceModuleManager");
	addr = mod->text_addr;

	_sw(0		, addr + 0x00000760 ); // sceKernelLoadModule (User)
	_sw(0x24020000, addr + 0x000007C0 ); // sceKernelLoadModule (User)

	const int *bridge = NULL;
	if (model == 0)
		bridge = MemlmdPatchAddr_01g;
	else //if( model == 1 || model == 2 || model == 3 || model == 4 || model == 6 || model == 8 )
		bridge = MemlmdPatchAddr_02g;

	mod = _sceKernelFindModuleByName("sceMemlmd");
	u32 text_addr = mod->text_addr;

	//MemlmdSigcheck
	MemlmdSigcheck = (void *)(text_addr+ bridge[0] );
	MAKE_CALL(text_addr + bridge[1] , MemlmdSigcheckPatch );
	MAKE_CALL(text_addr + bridge[2] , MemlmdSigcheckPatch );

	//MemlmdDecrypt(buf, size, ret, flag);
	MemlmdDecrypt = (void *)(text_addr+ bridge[3] );
	MAKE_CALL(text_addr+ bridge[4] , MemlmdDecryptPatched );
	MAKE_CALL(text_addr+ bridge[5] , MemlmdDecryptPatched );	

	_sceKernelIcacheInvalidateAll();
	_sceKernelDcacheWritebackInvalidateAll();
	return 0;
}