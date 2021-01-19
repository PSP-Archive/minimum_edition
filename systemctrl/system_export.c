#include <pspsdk.h>
#include <pspkernel.h>
#include <pspthreadman_kernel.h>
#include <psploadexec_kernel.h>
#include <pspmodulemgr_kernel.h>

#include <stdio.h>
#include <string.h>

#include "main.h"

char *on_reboot_after = NULL;
void *on_reboot_buf = NULL;
int on_reboot_size = 0;
int on_reboot_flag = 0;

void sctrlHENLoadModuleOnReboot(char *module_after, void *buf, int size, int flags)
{
	on_reboot_after =module_after;
	on_reboot_buf   =buf;
	on_reboot_size  =size;
	on_reboot_flag  =flags;
	
}

u32 memp2 = 0;
u32 memp8 = 0;

int sctrlHENSetMemory(u32 p2, u32 p8)
{
#if PSP_MODEL == 0
	return 0x80000107;
#else

	if(p2 == 0)
		return 0x80000107;

	if(p2+p8 > 51)
		return 0x80000107;

	int k1 = pspSdkSetK1(0);
	memp2=p2;
	memp8=p8;
	pspSdkSetK1(k1);

	return 0;

#endif
}

int SystemCtrlForKernel_B86E36D1()
{
#if PSP_MODEL == 0
	return 0;
#else

	int p2size = memp2;
	if(p2size)
	{
		if( (memp8 + p2size) <= 51)
		{
			u32 *address =(u32 *)sctrlKernelGetPartition(2);
			u32 *next=(u32 *)(address[16/4]);

			address[8/4]=p2size << 20;
			next[20/4]=(p2size << 21)|0xFC;

			address =(u32 *)sctrlKernelGetPartition(9);
			next=(u32 *)(address[16/4]);

			address[4/4]=(p2size << 20) + 0x88800000;
			address[8/4]= memp8 << 20;
			next[20/4]=(memp8 << 21)|0xFC;

		}
	}

	return 0;
#endif
}

//hook sceKernelLoadModuleMs2
void SystemCtrlForKernel_07232EA5(void *func)
{
	SceModule2 *mod = sceKernelFindModuleByName("sceInit");
	if( mod != NULL)
	{
		u32 data = mod->text_addr;
		mod = sceKernelFindModuleByName("sctrlHoroscope");
		if( mod != NULL )
		{	
			void ( *sctrlHoroscopeHookLoadModuleMs2)(void *, u32 ) = search_module_export( mod, "horoscope_driver", 0x0FB6B018 );
			sctrlHoroscopeHookLoadModuleMs2( func, data );
		}
	}
}

