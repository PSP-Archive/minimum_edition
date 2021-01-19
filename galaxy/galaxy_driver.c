
#include <pspsdk.h>
#include <pspkernel.h>
#include <pspthreadman_kernel.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "csoread.h"
#include "galaxy_driver.h"

extern SceUID umdfd;				//dataF28
//extern SceInt64 umd_file_len;	//dataF2C

extern int umd_is_cso;				//dataF44
extern int umd_open;				//dataF40

int GetIsoDiscSize()
{
	if(umd_is_cso)//dataF44/cso
		return CisofileGetDiscSize(umdfd);//dataF28


	int ret = sceIoLseek(umdfd, 0, PSP_SEEK_CUR);
	int size = sceIoLseek(umdfd ,0,SEEK_END);//dataF28
	sceIoLseek(umdfd, ret, PSP_SEEK_SET);

	if (size < 0)
		return size;

	return size / SECTOR_SIZE;
}

int  ReadUmdFileRetry(void *buf, int size, int fpointer)
{
	int i, read;
	for(i = 0; i < 16; i++)
	{
		if(sceIoLseek32(umdfd, fpointer, PSP_SEEK_SET) >= 0)
		{
			for(i = 16; i > 0; i--)
			{
				if((read = sceIoRead(umdfd, buf, size)) >= 0)
				{
					return read;
				}

				OpenIso();
			}
			return 0x80010013;
		}
		OpenIso();
	}
	return 0x80010013;

}

int umd9660_read2(void *a0)
{
	NP9660ReadParams *params = (NP9660ReadParams *)a0;

	if(umd_is_cso)//dataF44 if cso
		return CisofileReadSectors( params->buf , params->read_size , params->fpointer );

	return ReadUmdFileRetry( params->buf , params->read_size , params->fpointer );
}



