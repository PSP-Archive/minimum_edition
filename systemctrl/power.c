#include <pspsdk.h>
#include <pspkernel.h>
#include <pspinit.h>
#include <psppower.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

__attribute__((noinline)) u32 FindPowerFunction(u32 nid)
{
	return FindProc("scePower_Service", "scePower", nid);
}

 __attribute__((noinline)) u32 FindPowerDriverFunction(u32 nid)
{
	return FindProc("scePower_Service", "scePower_driver", nid);
}
		
int (* scePowerSetClockFrequency_k)(int cpufreq, int ramfreq, int busfreq);

int	sctrlHENSetSpeed(int cpu,int bus)
{	
	scePowerSetClockFrequency_k = (void *)FindPowerFunction(0x545A7F3C);
	return scePowerSetClockFrequency_k( cpu, cpu ,bus);
}

static const u32 cpu_nid_list[] = {
	0x545A7F3C,
	0xB8D7B3FB,//scePowerSetBusClockFrequency
	0x843FBF43,//scePowerSetCpuClockFrequency
	0xEBD177D6,
	0xA4E93389,
	0x469989AD,
};

void SetSpeed(int cpu, int bus)
{
	if(cpu==20 || cpu==75 || cpu==100 || cpu==133 || cpu==166 || cpu==200 ||
		cpu==222 || cpu==266 || cpu==300 || cpu==333)
	{
		scePowerSetClockFrequency_k = (void *)FindPowerFunction(0x737486F2);//scePowerSetClockFrequency
		scePowerSetClockFrequency_k(cpu, cpu, bus);

		int intr = sceKernelInitKeyConfig();
		if(intr != PSP_INIT_KEYCONFIG_VSH)
		{
			MAKE_DUMMY_FUNCTION0((u32)scePowerSetClockFrequency_k);

			int i;
			for(i=0;i<sizeof(cpu_nid_list)/sizeof(u32);i++)
			{
				u32 patch_addr = (u32)FindPowerFunction(cpu_nid_list[i]);
				MAKE_DUMMY_FUNCTION0( patch_addr );
			}

			ClearCaches();
		}
	}

}


#if PSP_MODEL != 0
int (* scePowerIsBatteryCharging_k)() = NULL;
int (* scePowerBatteryDisableUsbCharging_k)() = NULL;
int (* scePowerBatteryEnableUsbCharging_k)() = NULL;

u32 UsbChargePatch()
{
	static int connected = 0;
	static int usb_swap = 0;

	int r = scePowerIsBatteryCharging_k();

	//if already charging
	if(r != 0) return 2*1000*1000;

	if(connected == 1)//connected
	{
		if(usb_swap)
		{
			scePowerBatteryDisableUsbCharging_k(0);
		}

		connected = 0;

		usb_swap= (usb_swap > 0)? 0: 1 ;

		return 5*1000*1000;
	}


	if( scePowerBatteryEnableUsbCharging_k(1) >= 0)
		connected = 1;

	return 15*1000*1000;
}

static void initUsbPowerFunc(void)
{
	scePowerIsBatteryCharging_k			= (void *)FindPowerFunction(0x1E490401);
	scePowerBatteryDisableUsbCharging_k	= (void *)FindPowerDriverFunction(0x90285886);
	scePowerBatteryEnableUsbCharging_k	= (void *)FindPowerDriverFunction(0x733F973B);
}

void UsbChargeStart()
{
	SceUID id =sceKernelCreateVTimer("",NULL);
	if(id >= 0) {
		initUsbPowerFunc();

		sceKernelStartVTimer(id);
		sceKernelSetVTimerHandlerWide(id ,5*1000*1000, UsbChargePatch ,NULL);

		SceModule2 *mod =sceKernelFindModuleByName("sceUSB_Driver");
		if(mod)
		{
			u32 text_addr =mod->text_addr;
				
#if _PSP_FW_VERSION == 639
#define UsbChargePatchAddr1	0x00009048
#define UsbChargePatchAddr2	0x00009050

#elif _PSP_FW_VERSION == 660
#define UsbChargePatchAddr1	0x00008FE8
#define UsbChargePatchAddr2	0x00008FF0

#endif

			MAKE_DUMMY_FUNCTION0(text_addr + UsbChargePatchAddr1 );//scePowerBatteryEnableUsbCharging
			MAKE_DUMMY_FUNCTION0(text_addr + UsbChargePatchAddr2 );//scePowerBatteryDisableUsbCharging
		}
	}
}
#endif