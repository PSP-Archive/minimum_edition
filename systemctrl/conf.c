#include <pspsdk.h>
#include <pspkernel.h>

#include <string.h>

#include "main.h"

//#define CONFIG_MAGIC	0x4743454D
#define CONFIG_MAGIC	0x4143454D

#define CONFIG_PATH "flashfat1:/config.me"

int sctrlSEGetConfigEx(SEConfig *config , int size)
{
	int k1 = pspSdkSetK1(0);
	SceUID fd;
	int ret;

	memset(config, 0, size);
	fd = sceIoOpen( CONFIG_PATH , PSP_O_RDONLY, 0644);
	if (fd >= 0)
	{
		ret = sceIoRead(fd, config, size);
		sceIoClose(fd);

		if( config->magic != CONFIG_MAGIC )
		{	
			memset(config, 0, size);
			goto RESET_CONFIG;
		}
	}
	else
	{	
RESET_CONFIG:
		config->magic = CONFIG_MAGIC;
		config->umdmode = ME_MODE_NP9660;
		ret = size;
	}

	pspSdkSetK1(k1);
	return ret;
}

int sctrlSEGetConfig(SEConfig *config)
{
	return sctrlSEGetConfigEx(config , sizeof(SEConfig));
}

int sctrlSESetConfigEx(SEConfig *config , int size)
{
	int k1 = pspSdkSetK1(0);
	int ret = -1;

	SceUID fd = sceIoOpen( CONFIG_PATH , PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	if (fd >= 0)
	{
		config->magic = CONFIG_MAGIC;
		if (sceIoWrite(fd, config, size) == size)
		{
			ret = size;
		}
		sceIoClose(fd);
	}

	pspSdkSetK1(k1);
	return ret;
}

int sctrlSESetConfig(SEConfig *config)
{
	return sctrlSESetConfigEx(config , sizeof(SEConfig));

}