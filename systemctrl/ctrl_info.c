#include <pspsdk.h>
#include <pspkernel.h>
#include <pspthreadman_kernel.h>
#include <psploadexec_kernel.h>
#include <stdio.h>
#include <string.h>


int sctrlSEGetVersion()
{
#if _PSP_FW_VERSION == 639
	return 0x00002088;

#elif _PSP_FW_VERSION == 660
	return 0x00010008;

#endif

// 5.00M33
//	return 0x00020002;
}

int	sctrlHENIsSE()
{
	return 1;
}

int	sctrlHENIsDevhook()
{
	return 0;
}

int sctrlHENGetVersion()
{
	return 0x00001000;
}
