/*
 * 
 *  rebootex.bin
 *
 */

#include <pspsdk.h>
#include "main.h"

#include "../rebootex/rebootex_patch_addr.h"

static int Main(void *, void *, void *, void *, void *, void *, void *);
int Reboot_Entry(void *a0, void *a1, void *a2, void *a3, void *t0, void *t1, void *t2) __attribute__ ((section (".text.startup")));
int Reboot_Entry(void *a0, void *a1, void *a2, void *a3, void *t0, void *t1, void *t2)
{
	return Main(a0, a1, a2, a3, t0, t1, t2);
}

int (* Real_Reboot)(void *a0, void *a1, void *a2, void *a3, void *t0, void *t1, void *t2) = (void *)0x88600000;
int recovery_flag = 0;
int model=0;

u32 loadcore_text_addr = 0;

int elf_load_flag;
int btcnf_load_flag;
int rtm_flag;

int (* memlmd_E42AFE2E)(void *buf ,void *check , void *s) = NULL;
int (* memlmd_3F2AC9C6)(void *a0,void *a1) = NULL;


static void ClearCaches()
{
	int (* DcacheClear)(void) = (void *)rebootex_patch_list.function_list.DcacheClearAddr;
	int (* IcacheClear)(void) = (void *)rebootex_patch_list.function_list.IcacheClearAddr;

	DcacheClear();
	IcacheClear();
}

static int sceBootLfatOpen(const char *path)
{
	int (* sceBootLfatOpen_k)(const char *)	= (void *)rebootex_patch_list.function_list.BootLfatOpen;
	return sceBootLfatOpen_k( path );
}
static int sceBootLfatRead(void *buffer, int maxsize)
{
	int (* sceBootLfatRead_k)(void* , int)	= (void *)rebootex_patch_list.function_list.BootLfatRead;
	return sceBootLfatRead_k( buffer , maxsize);
}
static int sceBootLfatClose(void)
{
	int (* sceBootLfatClose_k )(void)		= (void *)rebootex_patch_list.function_list.BootLfatClose;
	return sceBootLfatClose_k();
}

int memcpy(u8 *dst,u8 *src,int size)
{
	u8 *p1 = (u8 *)dst;
	u8 *p2 = (u8 *)src;
	while(size--)
	{
		*p1++ = *p2++;
	}
	return p1;
}

void memcpy_b(u8 *d, u8 *s, int len)
{
	while(len--) 
	{
		d[len] = s[len];
	} 
}

int memcmp(u8 *m1, u8 *m2, int size)
{
	int i;

	for (i = 0; i < size; i++)
	{
		if (m1[i] != m2[i])
			return m2[i] - m1[i];
	}

	return 0;
}

int strcmp(char *s, char *t)
{
	while( *s == *t )
	{
		if( *s == '\0' )
			return 0;

		s++;t++;
	}
	return (*s - *t);
}

int memset(u8 *dst,u8 code,int size)
{
	u8 *p1 = (u8 *)dst;
	while(size--)
	{
		*p1++ = code;
	}
	return p1;
}

int strlen(char *str)
{
	char *p;
	for(p=str; *p; ++p)
		;

	return (p - str);
}


int sceBootLfatOpenPatched(char *path)
{

	if( memcmp( path + 4 , "pspbtcnf" , sizeof( "pspbtcnf" )-1 ) == 0)
	{
		path[9] = 'j';
	}

	return sceBootLfatOpen( path );
}

/*
int sceBootLfatReadPatched(void *buff , int max_size )
{
	int ret = sceBootLfatRead( buff , max_size );
	return ret;
}

int sceBootLfatClosePatched(void)//close
{
	if(elf_load_flag)
	{
		elf_load_flag = 0;
		return 0;
	}
	else if(rtm_flag)
	{
		rtm_flag = 0;
		return 0;
	}

	return sceBootLfatClose();
}
*/

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

int memlmd_E42AFE2E_patched(PSP_Header *buf, int *check,int *s)//read & decrypt patch
{
	if(buf->oe_tag == 0xC6BA41D3 )//0x55668D96
	{
		scramble_simple( (u32 *)(&(buf->main_data)) , (u32 *)buf->key_data1 , 0x10 );
		scramble( &(buf->main_data) , buf->comp_size , &(buf->scheck[0x38]) , 0x20 );
		memcpy(buf,&(buf->main_data), buf->comp_size);
		*s = buf->comp_size;

		MAKE_CALL(loadcore_text_addr + rebootex_patch_list.memlmd_list.DecryptPatchAddr , memlmd_E42AFE2E);
		ClearCaches();
		return 0;
	}
	
	return memlmd_E42AFE2E(buf , check , s);	
}

//sub_0x08FC04A4
int memlmd_3F2AC9C6_patched(void *a0,void *a1)
{
	PSP_Header *head=(PSP_Header *)a0;
	int i;

	for(i=0;i<0x30;i++)//0x58
	{
		if(head->scheck[i] != 0)
			return memlmd_3F2AC9C6(a0,a1);
	}

	MAKE_CALL(loadcore_text_addr + rebootex_patch_list.memlmd_list.SigcheckPatchAddr , memlmd_3F2AC9C6 );
	ClearCaches();
	return 0;
}

int PatchLoadCore(void *a0, void *a1, void *a2, int (* module_start)(void *, void *, void *))
{
	u32 text_addr = ((u32)module_start) - rebootex_patch_list.memlmd_list.ModuleOffsetAddr;

	MAKE_CALL(text_addr + rebootex_patch_list.memlmd_list.SigcheckPatchAddr , memlmd_3F2AC9C6_patched );
	MAKE_CALL(text_addr + rebootex_patch_list.memlmd_list.DecryptPatchAddr , memlmd_E42AFE2E_patched );

	loadcore_text_addr = text_addr;
	memlmd_3F2AC9C6 = (void *)(text_addr + rebootex_patch_list.memlmd_list.SigcheckFuncAddr );
	memlmd_E42AFE2E = (void *)(text_addr + rebootex_patch_list.memlmd_list.DecryptFuncAddr );

	ClearCaches();
	return module_start(a0, a1, a2);
}

#include "btcnf.h"

static int Main(void *a0, void *a1, void *a2, void *a3, void *t0, void *t1, void *t2)
{	
	recovery_flag = *(u32 *)0x88FB0008;
	model = *(int *)0x88FB000C;

	elf_load_flag = 0;//
	btcnf_load_flag = 0;
	rtm_flag = 0;

	MAKE_CALL( rebootex_patch_list.patch_list.BootLfatOpenPatch		, sceBootLfatOpenPatched );//01g
//	MAKE_CALL( rebootex_patch_list.patch_list.BootLfatReadPatch		, sceBootLfatReadPatched );//01g
//	MAKE_CALL( rebootex_patch_list.patch_list.BootLfatClosePatch	, sceBootLfatClosePatched);//01g

	if(recovery_flag )
	{
		MAKE_CALL( rebootex_patch_list.patch_list.CheckPspConfigPatch	, sceKernelCheckPspConfigPatched);//01g
	}

	//kdebug patch
	_sw( 0x03E00008 , rebootex_patch_list.patch_list.KdebugPatchAddr  );
	_sw( 0x24020001 , rebootex_patch_list.patch_list.KdebugPatchAddr + 4 );//addiu  $v0, $zr, 1 

	// Patch ~PSP header check
	_sw(0xafa50000, rebootex_patch_list.patch_list.BtHeaderPatchAddr	 );//sw $a0, 0($sp) -> sw $a1, 0($sp)
	_sw(0x20a30000, rebootex_patch_list.patch_list.BtHeaderPatchAddr + 4 );//addiu v1, zr,-1 -> addi	$v1, $a1, 0

	///patch sceBootLfatfsMount
	_sw(0, rebootex_patch_list.patch_list.LfatMountPatchAddr );

	//patch sceBootLfatSeek size
	_sw(0, rebootex_patch_list.patch_list.LfatSeekPatchAddr1 );
	_sw(0, rebootex_patch_list.patch_list.LfatSeekPatchAddr2 );

	//MIPS_ADDU( 7 , 17 , 0 )//addu $a3,$s1,$zr
	_sw( 0x02203821 ,	rebootex_patch_list.patch_list.LoadCorePatchAddr );
	MAKE_JUMP(			rebootex_patch_list.patch_list.LoadCorePatchAddr + 4 , PatchLoadCore );
	_sw( 0x02A0E821 ,	rebootex_patch_list.patch_list.LoadCorePatchAddr + 8 );//addu  $sp, $s5, $zr

	//patch error
	_sw(0, rebootex_patch_list.patch_list.HashCheckPatchAddr );
	
/*
#if PSP_MODEL == 0
	//fat
	MAKE_CALL( 0x88602768 , sceBootLfatOpenPatched);//01g
//	MAKE_CALL( 0x886027D8 , sceBootLfatReadPatched);//01g
//	MAKE_CALL( 0x88602804 , sceBootLfatClosePatched);//01g

	if(recovery_flag )
	{
		MAKE_CALL( 0x8860711C  , sceKernelCheckPspConfigPatched);//01g
	}

	_sw( 0x03E00008 , 0x88603848  );// -01g
	_sw( 0x24020001 , 0x8860384C );//addiu  $v0, $zr, 1 

	// Patch ~PSP header check - 01g
	_sw(0xafa50000, 0x886056D0 );//sw $a0, 0($sp) -> sw $a1, 0($sp)
	_sw(0x20a30000, 0x886056D4 );//addiu v1, zr,-1 -> addi	$v1, $a1, 0

	///patch sceBootLfatfsMount - 01g
	_sw(0, 0x88602760 );

	//patch sceBootLfatSeek size -01g
	_sw(0, 0x886027B4 );

	//patch buffer size check -01g
	_sw(0, 0x886027CC );

	_sw( MIPS_ADDU( 7 , 17 , 0 ) , 0x88605590 );////addu $a3,$s1,$zr +
	MAKE_JUMP( 0x88605594 , PatchLoadCore );//- 01g
	_sw( 0x02A0E821 , 0x88605598 );//addu  $sp, $s5, $zr +

	//patch decrypt error - 01g
	_sw(0, 0x886073B4 );
	
#elif (PSP_MODEL == 1 || PSP_MODEL == 2 || PSP_MODEL == 4 )


	MAKE_CALL( 0x88602838 , sceBootLfatOpenPatched);//6.39
//	MAKE_CALL( 0x886028A8 , sceBootLfatReadPatched);//6.39
//	MAKE_CALL( 0x886028D4 , sceBootLfatClosePatched);//6.39


	if(recovery_flag )
	{
		MAKE_CALL( 0x886071EC  , sceKernelCheckPspConfigPatched);//6.39
	}

	_sw( 0x03E00008 , 0x88603918  );// -6.39
	_sw( 0x24020001 , 0x8860391C );//addiu  $v0, $zr, 1 

	// Patch ~PSP header check -6.39
	_sw(0xafa50000, 0x886057A0 );//sw $a0, 0($sp) -> sw $a1, 0($sp)
	_sw(0x20a30000, 0x886057A4 );//addiu v1, zr,-1 -> addi	$v1, $a1, 0

	///patch sceBootLfatfsMount -6.39
	_sw(0, 0x88602830 );

	//patch sceBootLfatSeek size -6.39
	_sw(0, 0x88602884 );

	//patch buffer size check -6.39
	_sw(0, 0x8860289C );

	_sw( MIPS_ADDU( 7 , 17 , 0 ) , 0x88605660 );////addu $a3,$s1,$zr +
	MAKE_JUMP( 0x88605664 , PatchLoadCore );//-6.39
	_sw( 0x02A0E821 , 0x88605668 );//addu  $sp, $s5, $zr +

	//patch decrypt error -6.39
	_sw(0, 0x88607484 );
#else
	AAAAA;
#endif
	*/

	ClearCaches();

	return Real_Reboot(a0, a1, a2, a3, t0, t1, t2);	
}