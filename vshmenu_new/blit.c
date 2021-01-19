/*
	PSP VSH 24bpp text bliter
*/
#include "common.h"

//#define ALPHA_BLEND 1

extern unsigned char msx[];

unsigned char *ftabe = msx;
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int pwidth;
int pheight, bufferwidth, pixelformat;
unsigned int* vram32;

u32 fcolor = 0x00ffffff;
u32 bcolor = 0xff000000;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
static int ReadFile(const char *file, void *buf, int size)
{
	SceUID fd = sceIoOpen(file, PSP_O_RDONLY, 0);
	if (fd < 0)
		return fd;

	int read = sceIoRead(fd, buf, size);
	
	sceIoClose(fd);
	return read;
}

static int memid = -1;
int load_font(const char *path)
{

	SceIoStat stat;
	if(sceIoGetstat ( path , &stat) < 0)
		return -1;

	if( memid > 0)
	{
		sceKernelFreePartitionMemory(memid);
		memid = -1;
	}

	memid = sceKernelAllocPartitionMemory(2, "font", 0, stat.st_size , NULL);
	if( memid > 0 )
	{
		ftabe = (u8 *)sceKernelGetBlockHeadAddr( memid );
		ReadFile( path , ftabe , stat.st_size );
	}

	return 0;
}

void release_font_table()
{
	ftabe = msx;
	if( memid > 0)
	{
		sceKernelFreePartitionMemory(memid);
		memid = -1;
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
static u32 adjust_alpha(u32 col)
{
	u32 alpha = col>>24;
	u8 mul;
	u32 c1,c2;

	if(alpha==0)    return col;
	if(alpha==0xff) return col;

	c1 = col & 0x00ff00ff;
	c2 = col & 0x0000ff00;
	mul = (u8)(255-alpha);
	c1 = ((c1*mul)>>8)&0x00ff00ff;
	c2 = ((c2*mul)>>8)&0x0000ff00;
	return (alpha<<24)|c1|c2;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//int blit_setup(int sx,int sy,const char *msg,int fg_col,int bg_col)
int blit_setup(void)
{
	int unk;
	sceDisplayGetMode(&unk, &pwidth, &pheight);
	sceDisplayGetFrameBuf((void*)&vram32, &bufferwidth, &pixelformat, PSP_DISPLAY_SETBUF_NEXTFRAME);
	if( (bufferwidth==0) || (pixelformat!=3)) return -1;

	fcolor = 0x00ffffff;
	bcolor = 0xff000000;

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// blit text
/////////////////////////////////////////////////////////////////////////////
void blit_set_color(int fg_col,int bg_col)
{
	fcolor = fg_col;
	bcolor = bg_col;
}

/////////////////////////////////////////////////////////////////////////////
// blit text
/////////////////////////////////////////////////////////////////////////////
int blit_string(int sx,int sy,const char *msg)
{
	int x,y,p;
	u32 offset, char_offset;
	unsigned char code;
	u8 font;
	u32 fg_col,bg_col;

	u32 col,c1,c2;
	u32 alpha;

	fg_col = adjust_alpha(fcolor);
	bg_col = adjust_alpha(bcolor);

//Kprintf("MODE %d WIDTH %d\n",pixelformat,bufferwidth);
	if( (bufferwidth==0) || (pixelformat!=3)) return -1;

	u32 base_offset = (sy)*bufferwidth + sx;

//	for(x=0;msg[x] && x<(pwidth/8);x++)
	for(x=0;msg[0] && x<(pwidth/8);x++, msg++, base_offset += 8 )
	{
		code = msg[0]; // 7bit ANK
//		code = msg[0] & 0x7f; // 7bit ANK
//		code = msg[x] & 0x7f; // 7bit ANK

		char_offset = base_offset;

		for(y=0;y<8;y++, char_offset+=bufferwidth)
		{
			offset = char_offset;
//			offset = (sy+y)*bufferwidth + sx+x*8;
//			font = y>=7 ? 0x00 : msx[ code*8 + y ];
			font = ftabe[ code*8 + y ];
			for(p=0;p<8;p++)
			{
				col = (font & 0x80) ? fg_col : bg_col;
				alpha = col>>24;
				if(alpha==0) vram32[offset] = col;
				else if(alpha!=0xff)
				{
					c2 = vram32[offset];
					c1 = c2 & 0x00ff00ff;
					c2 = c2 & 0x0000ff00;
					c1 = ((c1*alpha)>>8)&0x00ff00ff;
					c2 = ((c2*alpha)>>8)&0x0000ff00;
					vram32[offset] = (col&0xffffff) + c1 + c2;
				}

				font <<= 1;
				offset++;
			}
		}
	}
	return x;
}

int blit_string_ctr(int sy,const char *msg)
{
	int sx = (480/2 - 0x10)-scePaf_strlen(msg)*(8/2);
	return blit_string(sx,sy,msg);
}
