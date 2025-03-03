#include <pspsdk.h>
#include <pspkernel.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <malloc.h>

#include "virtualpbpmgr.h"
#include "systemctrl_me.h"

#define MAX_FILES	128

#define oe_malloc sctrlKernelMalloc
#define oe_free sctrlKernelFree

static VirtualPbp *vpbps;
static InternalState *states;
static SceUID vpsema;
static int g_index;

static char psp_system_version[] = "1.00";
static int version_str_flag = 0;


extern SEConfig config;

/*
u8 virtualsfo[408] =
{
	0x00, 0x50, 0x53, 0x46, 0x01, 0x01, 0x00, 0x00,
	0x94, 0x00, 0x00, 0x00, 0xE8, 0x00, 0x00, 0x00, 
	0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 
	0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x04, 0x02, 
	0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
	0x04, 0x00, 0x00, 0x00, 0x12, 0x00, 0x04, 0x02, 
	0x0A, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 
	0x08, 0x00, 0x00, 0x00, 0x1A, 0x00, 0x04, 0x02, 
	0x05, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 
	0x18, 0x00, 0x00, 0x00, 0x27, 0x00, 0x04, 0x04, 
	0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 
	0x20, 0x00, 0x00, 0x00, 0x36, 0x00, 0x04, 0x02,
	0x05, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 
	0x24, 0x00, 0x00, 0x00, 0x45, 0x00, 0x04, 0x04, 
	0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 
	0x2C, 0x00, 0x00, 0x00, 0x4C, 0x00, 0x04, 0x02, 
	0x40, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 
	0x30, 0x00, 0x00, 0x00, 0x42, 0x4F, 0x4F, 0x54, 
	0x41, 0x42, 0x4C, 0x45, 0x00, 0x43, 0x41, 0x54, 
	0x45, 0x47, 0x4F, 0x52, 0x59, 0x00, 0x44, 0x49, 
	0x53, 0x43, 0x5F, 0x49, 0x44, 0x00, 0x44, 0x49, 
	0x53, 0x43, 0x5F, 0x56, 0x45, 0x52, 0x53, 0x49,
	0x4F, 0x4E, 0x00, 0x50, 0x41, 0x52, 0x45, 0x4E, 
	0x54, 0x41, 0x4C, 0x5F, 0x4C, 0x45, 0x56, 0x45, 
	0x4C, 0x00, 0x50, 0x53, 0x50, 0x5F, 0x53, 0x59, 
	0x53, 0x54, 0x45, 0x4D, 0x5F, 0x56, 0x45, 0x52, 
	0x00, 0x52, 0x45, 0x47, 0x49, 0x4F, 0x4E, 0x00, 
	0x54, 0x49, 0x54, 0x4C, 0x45, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x4D, 0x47, 0x00, 0x00, 
	0x55, 0x43, 0x4A, 0x53, 0x31, 0x30, 0x30, 0x34, 
	0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x31, 0x2E, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x31, 0x2E, 0x30, 0x30, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 
	0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 
	0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 
	0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 
	0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 
	0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 
	0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

*/

static u8 virtualsfo[] = {
	0x00, 0x50, 0x53, 0x46, 0x01, 0x01, 0x00, 0x00, 0x94, 0x00, 0x00, 0x00, 0xe8, 0x00, 0x00, 0x00, 
	0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x04, 0x02, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 
	0x04, 0x00, 0x00, 0x00, 0x12, 0x00, 0x04, 0x02, 0x0a, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 
	0x08, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x04, 0x02, 0x05, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 
	0x18, 0x00, 0x00, 0x00, 0x27, 0x00, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 
	0x20, 0x00, 0x00, 0x00, 0x36, 0x00, 0x04, 0x02, 0x05, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 
	0x24, 0x00, 0x00, 0x00, 0x45, 0x00, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 
	0x2c, 0x00, 0x00, 0x00, 0x4c, 0x00, 0x04, 0x02, 0x49, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 
	0x30, 0x00, 0x00, 0x00, 0x42, 0x4f, 0x4f, 0x54, 0x41, 0x42, 0x4c, 0x45, 0x00, 0x43, 0x41, 0x54, 
	0x45, 0x47, 0x4f, 0x52, 0x59, 0x00, 0x44, 0x49, 0x53, 0x43, 0x5f, 0x49, 0x44, 0x00, 0x44, 0x49, 
	0x53, 0x43, 0x5f, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4f, 0x4e, 0x00, 0x50, 0x41, 0x52, 0x45, 0x4e, 
	0x54, 0x41, 0x4c, 0x5f, 0x4c, 0x45, 0x56, 0x45, 0x4c, 0x00, 0x50, 0x53, 0x50, 0x5f, 0x53, 0x59, 
	0x53, 0x54, 0x45, 0x4d, 0x5f, 0x56, 0x45, 0x52, 0x00, 0x52, 0x45, 0x47, 0x49, 0x4f, 0x4e, 0x00, 
	0x54, 0x49, 0x54, 0x4c, 0x45, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x4d, 0x47, 0x00, 0x00, 
	0x55, 0x43, 0x4a, 0x53, 0x31, 0x30, 0x30, 0x34, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x31, 0x2e, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x31, 0x2e, 0x30, 0x30, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 
	0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 
	0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 
	0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

typedef struct  __attribute__((packed))
{
	u32 signature;//0
	u32 version;//4
	u32 fields_table_offs;//8
	u32 values_table_offs;//12
	int nitems;//16
} SFOHeader;

typedef struct __attribute__((packed))
{
	u16 field_offs;
	u8  unk;
	u8  type; // 0x2 -> string, 0x4 -> number
	u32 unk2;
	u32 unk3;
	u16 val_offs;
	u16 unk4;
} SFODir;


const char *init_version_str()
{
	if( version_str_flag == 0 )
	{
		u32 version = sceKernelDevkitVersion();
		psp_system_version[0] = (version >> 24) + '0';
		psp_system_version[2] = ((version  >> 16)& 0xFF )  + '0';
		psp_system_version[3] = ( (version >> 8 )& 0xFF ) + '0';
		version_str_flag = 1;
	}

	return psp_system_version;
}

static void GetSFOTitle(char *title, int n, char *disc,int m, char *system_version , int *opnssmp, int *parental, char *sfo)
{
	SFOHeader *header = (SFOHeader *)sfo;
	SFODir *entries = (SFODir *)(sfo+0x14);
	int i;
	opnssmp[0] = 0;
	*(u32 *)system_version = 0x30302E31;

	for (i = 0; i < header->nitems; i++)
	{
		char *fields_str = (char *)( sfo+header->fields_table_offs+entries[i].field_offs );
		char *values_str = (char *)( sfo+header->values_table_offs+entries[i].val_offs );

		if (strcmp( fields_str , "TITLE") == 0)
		{
			memset(title, 0, n);
			strncpy(title, values_str , n);
		}
		else if (strcmp( fields_str , "DISC_ID") == 0)
		{
			memset(disc, 0, m);
			strncpy(disc, values_str , m);
		}
		else if (strcmp( fields_str , "HRKGMP_VER") == 0)
		{
			*opnssmp = *(int *)(values_str);
			//strncpy(disc, sfo+header->values_table_offs+entries[i].val_offs, m);
		}
		else if (strcmp( fields_str , "PSP_SYSTEM_VER" ) == 0)
		{
			*(u32 *)system_version = *(u32 *)(values_str);
		}
		else if( strcmp( fields_str ,"PARENTAL_LEVEL") == 0 )
		{
			*parental = *(int *)(values_str);
		}
	}
}

int virtualpbp_init()
{
	vpbps = (VirtualPbp *)oe_malloc(MAX_FILES*sizeof(VirtualPbp));
	memset(vpbps, 0, MAX_FILES * sizeof(VirtualPbp));//MAX_FILES*sizeof(VirtualPbp) = 0x9800

	if (!vpbps)
	{
		return -1;
	}

	states = (InternalState *)oe_malloc(MAX_FILES*sizeof(InternalState));
	memset(states, 0, MAX_FILES * sizeof(InternalState));//MAX_FILES*sizeof(InternalState) = 1536
	if (!states)
	{
		return -1;
	}

	vpsema = sceKernelCreateSema("VirtualPBPMgr", 0, 1, 1, NULL);
	if (vpsema < 0)
	{
		return vpsema;
	}

	memset(vpbps, 0, MAX_FILES*sizeof(VirtualPbp));
	g_index = 0;

	init_version_str();

	if( config.execute_pboot )
		virtualsfo[0xEC] = 'E';

	return 0;
}

int virtualpbp_exit()
{

	sceKernelWaitSema(vpsema, 1, NULL);
	oe_free(vpbps);
	oe_free(states);
	sceKernelDeleteSema(vpsema);

	return 0;
}

int virtualpbp_reset()
{
	sceKernelWaitSema(vpsema, 1, NULL);

	memset(vpbps, 0, MAX_FILES*sizeof(VirtualPbp));
	memset(states, 0, MAX_FILES*sizeof(InternalState));
	g_index = 0;

	sceKernelSignalSema(vpsema, 1);
	return 0;
}

void getlba_andsize(PspIoDrvFileArg *arg, const char *file, int *lba, int *size)
{
	SceIoStat stat;

	memset(&stat, 0, sizeof(SceIoStat));
	if (isofs_getstat(arg, file, &stat) >= 0)
	{
		*lba = stat.st_private[0];
		*size = stat.st_size;
	}
}

int virtualpbp_add(char *isofile, ScePspDateTime *mtime, VirtualPbp *res)
{
	PspIoDrvFileArg arg;//20
	int offset;

	sceKernelWaitSema(vpsema, 1, NULL);

	if (g_index >= MAX_FILES)
	{
		sceKernelSignalSema(vpsema, 1);
		return -1;
	}

	memset(vpbps[g_index].isofile, 0, ISOFILE_MAX );
	strncpy(vpbps[g_index].isofile, isofile, ISOFILE_MAX );
	sctrlSESetUmdFile(isofile);

	memset(&arg, 0, sizeof(arg));
	if (isofs_init(NULL) < 0)
	{
		isofs_exit(NULL);
		sceKernelSignalSema(vpsema, 1);
		return -1;
	}

	if (isofs_open(&arg, "/PSP_GAME/PARAM.SFO", PSP_O_RDONLY, 0) >= 0)
	{
		char *buf = (char *)oe_malloc(1024);

		isofs_read(&arg, buf, 1024);
		isofs_close(&arg);
		GetSFOTitle( vpbps[g_index].sfotitle, 64 + 8, vpbps[g_index].discid, 10, vpbps[g_index].system_ver, 
			&(vpbps[g_index].opnssmp_type), &(vpbps[g_index].parental_level), buf);
		oe_free(buf);
	}
	else
	{
		isofs_exit(NULL);
		sceKernelSignalSema(vpsema, 1);
		return -1;
	}

	getlba_andsize(&arg, "/PSP_GAME/ICON0.PNG", &vpbps[g_index].i0png_lba, &vpbps[g_index].i0png_size);
	getlba_andsize(&arg, "/PSP_GAME/ICON1.PMF", &vpbps[g_index].i1pmf_lba, &vpbps[g_index].i1pmf_size);
	getlba_andsize(&arg, "/PSP_GAME/PIC0.PNG", &vpbps[g_index].p0png_lba, &vpbps[g_index].p0png_size);
	getlba_andsize(&arg, "/PSP_GAME/PIC1.PNG", &vpbps[g_index].p1png_lba, &vpbps[g_index].p1png_size);
	getlba_andsize(&arg, "/PSP_GAME/SND0.AT3", &vpbps[g_index].s0at3_lba, &vpbps[g_index].s0at3_size);
	
	isofs_exit(NULL);

	vpbps[g_index].header[0] = 0x50425000;
	vpbps[g_index].header[1] = 0x10000;

	offset = 0x28;//40
	
	/*vpbps[g_index].header[2] = offset; // SFO 
	offset += vpbps[g_index].psfo_size;*/

	vpbps[g_index].header[2] = offset;
	offset += sizeof(virtualsfo);

	vpbps[g_index].header[3] = offset; // ICON0.PNG 
	offset += vpbps[g_index].i0png_size;

	vpbps[g_index].header[4] = offset; // ICON1.PMF 
	offset += vpbps[g_index].i1pmf_size;

	vpbps[g_index].header[5] = offset; // PIC0.PNG 
	offset += vpbps[g_index].p0png_size;

	vpbps[g_index].header[6] = offset; // PIC1.PNG 
	offset += vpbps[g_index].p1png_size;

	vpbps[g_index].header[7] = offset; // SND0.AT3 
	offset += vpbps[g_index].s0at3_size;

	vpbps[g_index].header[8] = offset; // DATA.PSP 
	vpbps[g_index].header[9] = offset; // DATA.PSAR 

	vpbps[g_index].filesize = offset;

	memcpy(&vpbps[g_index].mtime, mtime, sizeof(ScePspDateTime));

	if (res)
	{
		memcpy(res, &vpbps[g_index], sizeof(VirtualPbp));
	}

	g_index++;

	sceKernelSignalSema(vpsema, 1);
	return 0;
}

int virtualpbp_fastadd(VirtualPbp *pbp)
{
	sceKernelWaitSema(vpsema, 1, NULL);

	if (g_index >= MAX_FILES)
	{
		sceKernelSignalSema(vpsema, 1);
		return -1;
	}

	memcpy(&vpbps[g_index], pbp, sizeof(VirtualPbp));

	g_index++;

	sceKernelSignalSema(vpsema, 1);
	return 0;
}

int virtualpbp_open(int i)
{
	sceKernelWaitSema(vpsema, 1, NULL);

	if (i < 0 || i >= g_index || states[i].deleted)
	{
		sceKernelSignalSema(vpsema, 1);
		return -1;
	}

	vpbps[i].filepointer = 0;

	sceKernelSignalSema(vpsema, 1);
	return 0x7000+i;
}

int virtualpbp_close(SceUID fd)
{
	sceKernelWaitSema(vpsema, 1, NULL);

	fd = fd-0x7000;

	if (fd < 0 || fd >= g_index )
	{
		sceKernelSignalSema(vpsema, 1);
		return -1;
	}

	sceKernelSignalSema(vpsema, 1);
	return 0;
}

int virtualpbp_read(SceUID fd, void *data, SceSize size)
{
	sceKernelWaitSema(vpsema, 1, NULL);

	fd = fd-0x7000;

	if (fd < 0 || fd >= g_index)
	{
		sceKernelSignalSema(vpsema, 1);
		return -1;
	}

	sctrlSESetUmdFile(vpbps[fd].isofile);
	isofs_fastinit();

	PspIoDrvFileArg arg;
	int remaining;
	int n, read, base;
	void *p;
	char filename[32];

	memset(&arg, 0, sizeof(PspIoDrvFileArg));
	remaining = size;
	read = 0;
	p = data;

	while (remaining > 0)
	{
		if (vpbps[fd].filepointer >= 0 && vpbps[fd].filepointer < 0x28)
		{
			u8 *header = (u8 *)&vpbps[fd].header;

			n = 0x28-vpbps[fd].filepointer; 

			if (remaining < n)
			{
				n = remaining;
			}			

			memcpy(p, header+vpbps[fd].filepointer, n);

			remaining -= n;
			p += n;
			vpbps[fd].filepointer += n;
			read += n;
		}

		/*if ((vpbps[fd].filepointer >= vpbps[fd].header[2]) &&
			(vpbps[fd].filepointer < vpbps[fd].header[3]))
		{
			base = vpbps[fd].filepointer - vpbps[fd].header[2];
			
			n = vpbps[fd].psfo_size-(base);
			if (remaining < n)
			{
				n = remaining;
			}

			sprintf(filename, "/sce_lbn0x%x_size0x%x", vpbps[fd].psfo_lba, vpbps[fd].psfo_size);
			
			isofs_open(&arg, filename, PSP_O_RDONLY, 0);
			isofs_lseek(&arg, base, PSP_SEEK_SET);
			isofs_read(&arg, p, n);
			isofs_close(&arg);

			remaining -= n;
			p += n;
			vpbps[fd].filepointer += n;
			read += n;
		}*/

		if ((vpbps[fd].filepointer >= vpbps[fd].header[2]) && 
			(vpbps[fd].filepointer < vpbps[fd].header[3]))
		{
			
			memcpy(virtualsfo+0x118, vpbps[fd].sfotitle,  64  + 8 );//4A90
			
			if(vpbps[fd].discid[0])
			{
				memcpy(virtualsfo+0xF0, vpbps[fd].discid, 10);//4A68
			}
			
			char *ver_str = vpbps[fd].system_ver;
			if( strncmp( psp_system_version , ver_str , 4 ) < 0 )
			{
					ver_str = psp_system_version;
			}
			memcpy(virtualsfo+0x10C ,  ver_str , 4 );

			int parental = vpbps[fd].parental_level;
			if( parental == 0 )
			{
				parental = 1;
			}			
			*(int *)(virtualsfo + 0x108 ) = parental;

			base = vpbps[fd].filepointer - vpbps[fd].header[2];
			
			n = sizeof(virtualsfo)-(base);
			if (remaining < n)
			{
				n = remaining;
			}

			memcpy(p, virtualsfo+base, n);
			remaining -= n;
			p += n;
			vpbps[fd].filepointer += n;
			read += n;
		}

		if ((vpbps[fd].filepointer >= vpbps[fd].header[3]) && 
			(vpbps[fd].filepointer < vpbps[fd].header[4]))
		{
			base = vpbps[fd].filepointer - vpbps[fd].header[3];
			
			n = vpbps[fd].i0png_size-(base);
			if (remaining < n)
			{
				n = remaining;
			}

			sprintf(filename, "/sce_lbn0x%x_size0x%x", vpbps[fd].i0png_lba, vpbps[fd].i0png_size);
			
			isofs_open(&arg, filename, PSP_O_RDONLY, 0);
			isofs_lseek(&arg, base, PSP_SEEK_SET);
			isofs_read(&arg, p, n);
			isofs_close(&arg);

			remaining -= n;
			p += n;
			vpbps[fd].filepointer += n;
			read += n;
		}

		if ((vpbps[fd].filepointer >= vpbps[fd].header[4]) && 
			(vpbps[fd].filepointer < vpbps[fd].header[5]))
		{
			base = vpbps[fd].filepointer - vpbps[fd].header[4];
			
			n = vpbps[fd].i1pmf_size-(base);
			if (remaining < n)
			{
				n = remaining;
			}

			sprintf(filename, "/sce_lbn0x%x_size0x%x", vpbps[fd].i1pmf_lba, vpbps[fd].i1pmf_size);
			
			isofs_open(&arg, filename, PSP_O_RDONLY, 0);
			isofs_lseek(&arg, base, PSP_SEEK_SET);
			isofs_read(&arg, p, n);
			isofs_close(&arg);

			remaining -= n;
			p += n;
			vpbps[fd].filepointer += n;
			read += n;
		}

		if ((vpbps[fd].filepointer >= vpbps[fd].header[5]) && 
			(vpbps[fd].filepointer < vpbps[fd].header[6]))
		{
			base = vpbps[fd].filepointer - vpbps[fd].header[5];
			
			n = vpbps[fd].p0png_size-(base);
			if (remaining < n)
			{
				n = remaining;
			}

			sprintf(filename, "/sce_lbn0x%x_size0x%x", vpbps[fd].p0png_lba, vpbps[fd].p0png_size);
			
			isofs_open(&arg, filename, PSP_O_RDONLY, 0);
			isofs_lseek(&arg, base, PSP_SEEK_SET);
			isofs_read(&arg, p, n);
			isofs_close(&arg);

			remaining -= n;
			p += n;
			vpbps[fd].filepointer += n;
			read += n;
		}

		if ((vpbps[fd].filepointer >= vpbps[fd].header[6]) && 
			(vpbps[fd].filepointer < vpbps[fd].header[7]))
		{
			base = vpbps[fd].filepointer - vpbps[fd].header[6];
			
			n = vpbps[fd].p1png_size-(base);
			if (remaining < n)
			{
				n = remaining;
			}

			sprintf(filename, "/sce_lbn0x%x_size0x%x", vpbps[fd].p1png_lba, vpbps[fd].p1png_size);
			
			isofs_open(&arg, filename, PSP_O_RDONLY, 0);
			isofs_lseek(&arg, base, PSP_SEEK_SET);
			isofs_read(&arg, p, n);
			isofs_close(&arg);

			remaining -= n;
			p += n;
			vpbps[fd].filepointer += n;
			read += n;
		}

		if ((vpbps[fd].filepointer >= vpbps[fd].header[7]) &&
			(vpbps[fd].filepointer < vpbps[fd].header[8]))
		{
			base = vpbps[fd].filepointer - vpbps[fd].header[7];
			
			n = vpbps[fd].s0at3_size-(base);
			if (remaining < n)
			{
				n = remaining;
			}

			sprintf(filename, "/sce_lbn0x%x_size0x%x", vpbps[fd].s0at3_lba, vpbps[fd].s0at3_size);
			
			isofs_open(&arg, filename, PSP_O_RDONLY, 0);
			isofs_lseek(&arg, base, PSP_SEEK_SET);
			isofs_read(&arg, p, n);
			isofs_close(&arg);

			remaining -= n;
			p += n;
			vpbps[fd].filepointer += n;
			read += n;
		}

		if (vpbps[fd].filepointer >= vpbps[fd].filesize)
		{
			break;
		}
	}

	isofs_exit(NULL);

	sceKernelSignalSema(vpsema, 1);
	return read;
}

int virtualpbp_lseek(SceUID fd, SceOff offset, int whence)
{
	sceKernelWaitSema(vpsema, 1, NULL);

	fd = fd - 0x7000;

	if (fd < 0 || fd >= g_index)
	{
		sceKernelSignalSema(vpsema, 1);
		return -1;
	}

	if (whence == PSP_SEEK_SET)
	{
		vpbps[fd].filepointer = (int)offset;
	}
	else if (whence == PSP_SEEK_CUR)
	{
		vpbps[fd].filepointer += (int)offset;
	}
	else if (vpbps[fd].filepointer == PSP_SEEK_END)
	{
		vpbps[fd].filepointer = vpbps[fd].filesize - (int)offset;
	}
	else
	{
		sceKernelSignalSema(vpsema, 1);
		return -1;
	}

	sceKernelSignalSema(vpsema, 1);
	return vpbps[fd].filepointer;
}

int virtualpbp_getstat(int i, SceIoStat *stat)
{
	int res;

	sceKernelWaitSema(vpsema, 1, NULL);

	if (i < 0 || i >= g_index || states[i].deleted)
	{
		sceKernelSignalSema(vpsema, 1);
		return -1;
	}

	res =  sceIoGetstat(vpbps[i].isofile, stat);
	stat->st_size = vpbps[i].filesize;

	memcpy(&stat->st_mtime, &stat->st_ctime, sizeof(ScePspDateTime));

	sceKernelSignalSema(vpsema, 1);
	return res;
}

int virtualpbp_chstat(int i, SceIoStat *stat, int bits)
{
	sceKernelWaitSema(vpsema, 1, NULL);

	if (i < 0 || i >= g_index)
	{
		sceKernelSignalSema(vpsema, 1);
		return -1;
	}

	sceIoChstat(vpbps[i].isofile, stat, bits);

	sceKernelSignalSema(vpsema, 1);
	return 0;
}

int virtualpbp_remove(int i)
{
	sceKernelWaitSema(vpsema, 1, NULL);
	
	if (i < 0 || i >= g_index)
	{
		sceKernelSignalSema(vpsema, 1);
		return -1;
	}

	sctrlSESetUmdFile("");
	int res = sceIoRemove(vpbps[i].isofile);
	if (res >= 0)
	{
		states[i].deleted = 1;
	}
	
	sceKernelSignalSema(vpsema, 1);
	return 0;
}

int virtualpbp_rmdir(int i)
{
	sceKernelWaitSema(vpsema, 1, NULL);

	if (i < 0 || i >= g_index || states[i].psdirdeleted)
	{
		sceKernelSignalSema(vpsema, 1);
		return -1;
	}

	states[i].psdirdeleted = 1;

	sceKernelSignalSema(vpsema, 1);
	return 0;
}

int virtualpbp_dread(SceUID fd, SceIoDirent *dir)
{
	SceFatMsDirentPrivate *private;

	sceKernelWaitSema(vpsema, 1, NULL);

	fd = fd - 0x7000;

	if (fd < 0 || fd >= g_index)
	{
		sceKernelSignalSema(vpsema, 1);
		return -1;
	}

	if (states[fd].dread == 2)
	{
		states[fd].dread = 0;
		sceKernelSignalSema(vpsema, 1);
		return 0;
	}

	sceIoGetstat(vpbps[fd].isofile, &dir->d_stat);
	memcpy(&dir->d_stat.st_mtime, &dir->d_stat.st_ctime, sizeof(ScePspDateTime));
	private = (SceFatMsDirentPrivate *)dir->d_private;

	if (states[fd].dread == 1)
	{
		dir->d_stat.st_size = vpbps[fd].filesize;
		strcpy(dir->d_name, "EBOOT.PBP");
		if (private)
		{
			strcpy(private->FileName, "EBOOT.PBP");
			strcpy(private->LongName, "EBOOT.PBP");
		}
	}
	else
	{
		dir->d_stat.st_size -= vpbps[fd].filesize;
		strcpy(dir->d_name, "IMAGE.ISO");
		if (private)
		{
			strcpy(private->FileName, "IMAGE.ISO");
			strcpy(private->LongName, "IMAGE.ISO");
		}
	}

	states[fd].dread++;
	sceKernelSignalSema(vpsema, 1);
	return 1;
}

char *virtualpbp_getfilename(int i)
{
	sceKernelWaitSema(vpsema, 1, NULL);

	if (i < 0 || i >= g_index)
	{
		sceKernelSignalSema(vpsema, 1);
		return NULL;
	}

	sceKernelSignalSema(vpsema, 1);
	return vpbps[i].isofile;
}

int virtualpbp_get_isotype(int i)
{
	sceKernelWaitSema(vpsema, 1, NULL);

	if (i < 0 || i >= g_index)
	{
		sceKernelSignalSema(vpsema, 1);
		return 0;
	}

	sceKernelSignalSema(vpsema, 1);
	return vpbps[i].opnssmp_type;
}
