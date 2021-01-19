/*
	PSP VSH
*/
#include "common.h"

extern SEConfig cnf;
extern int psp_model;
/////////////////////////////////////////////////////////////////////////////
// change CPU clock
/////////////////////////////////////////////////////////////////////////////

const u16 cpu_list[]={0, 20, 75, 100, 133, 166, 199, 222, 266, 300, 333};
//const u16 bus_list[]={0, 10, 37, 50, 66, 111, 133, 150, 166};

#define MAX_CLK_LIST (sizeof(cpu_list)/sizeof(u16))

int cpu2no(int cpu)
{
	int i;

	for(i=0;i< MAX_CLK_LIST;i++)
	{
		if(cpu==cpu_list[i])
			return i;
	
	}
	return 0;
}
/*
int bus2no(int cpu)
{
	int i;

	for(i=0;i< MAX_CLK_LIST;i++)
	{
		if(cpu==bus_list[i])
			return i;
	
	}
	return 0;
}
*/

void change_clock(int dir , int flag)
{
	int sel;
	int *cpu[2];

	if(flag)
	{
		cpu[0]=&(cnf.umdisocpuspeed);
		cpu[1]=&(cnf.umdisobusspeed);
	}
	else		
	{
		cpu[0]=&(cnf.vshcpuspeed);
		cpu[1]=&(cnf.vshbusspeed);
	}

	sel = cpu2no(*cpu[0]);

	// select new
	sel = limit(sel+dir,0,MAX_CLK_LIST-1);

	*cpu[0] = cpu_list[sel];
	*cpu[1] = (cpu_list[sel])/2;//bus_list[sel];

}

void change_usb(int dir )
{
	int sel = cnf.usbdevice;

	// select new
	sel = limit(sel+dir,0, 5);
	
	cnf.usbdevice=sel;

}

void change_umd_mode(int dir )
{
	int sel = cnf.umdmode;

	// select new
	sel = limit(sel+dir, ( psp_model == 4 )?1:0, 4);
	
	cnf.umdmode=sel;

}


void change_plugins(int dir , int flag)
{
	int sel;
	int *plugins;

	if(flag == 0){
		plugins=&(cnf.plugvsh);
	}else if(flag == 1){
		plugins=&(cnf.pluggame);
	}else{
		plugins=&(cnf.plugpop);
	}

	sel = *plugins;

	// select new
	sel = (sel)? 0 : 1 ;

	plugins[0] = sel;
}
