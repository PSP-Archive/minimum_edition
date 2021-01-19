#include <pspkernel.h>
//#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "systemctrl_se.h"
#include "systemctrl_me.h"
#include "kubridge.h"


#include "ui.h"

#include "blit.h"

typedef struct
{
	char isofile[72];//	
	int type;
	ScePspDateTime mtime;//
} ISO_cache;

typedef struct {
	char FileName[13];
	char LongName[256];
}SceFatMsDirentPrivate;

typedef struct {
	SceSize size;
	char FileName[16];
	char LongName[1024];
}SceFatMsDirentPrivateKernel;

int menu_draw(void);
int menu_setup(void);
int menu_ctrl(u32 buttons,u32 button_on);

int menu_plugin(u32 *buttons,u32 *button_on);

char *GetVideoName(void);
int GetVideoType(void);
void GetVideoPath(char *out, const char *input, int dev );
void change_umd_video(int dir );
int init_videoCache(const char *iso_name);


int cpu2no(int cpu);
int bus2no(int cpu);
void change_clock(int dir , int flag);
void change_usb(int dir );
void change_umd_mode(int dir );
void change_plugins(int dir , int flag);


/*
#define scePaf_967A56EF_strlen strlen
#define scePaf_6439FDBC_memset memset
#define scePaf_B6ADE52D_memcmp memcmp
#define scePaf_11EFC5FD_sprintf sprintf
#define scePaf_15AFC8D3_snprintf snprintf
#define scePaf_6BD7452C_memcpy memcpy
#define scePaf_98DE3BA6_strcpy strcpy
*/

#define scePaf_967A56EF_strlen scePaf_strlen
#define scePaf_6439FDBC_memset scePaf_memset
#define scePaf_B6ADE52D_memcmp scePaf_memcmp
#define scePaf_11EFC5FD_sprintf scePaf_sprintf
#define scePaf_15AFC8D3_snprintf scePaf_snprintf
#define scePaf_6BD7452C_memcpy scePaf_memcpy
#define scePaf_98DE3BA6_strcpy scePaf_strcpy


int scePaf_967A56EF_strlen(const char *path);
int scePaf_6439FDBC_memset(void *buff ,int c ,int size);
int scePaf_B6ADE52D_memcmp(const void *path , const void *name , int c);
int scePaf_11EFC5FD_sprintf(char *buffer , const char *format , ...);
int scePaf_15AFC8D3_snprintf(char *buffer,int c , const char *format, ...);
int scePaf_6BD7452C_memcpy(void *path , void *name , int size);
int scePaf_98DE3BA6_strcpy(char *path , const char *name);

void *scePaf_malloc(int size);
void scePaf_free(void *);

