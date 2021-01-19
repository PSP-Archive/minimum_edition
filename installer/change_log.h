

#if _PSP_FW_VERSION == 639	
case 0 ... 0x81:	
//	printf(" - Fixed a freeze bug when try to connect to PSN.\n");
//	printf(" - Fixed a bug when enter suspend with VshMenu opened.\n");		
//	printf(" - Added Speed up MS option(beta).\n");
//	printf(" - Added RecoveryMode option and ResetVSH in VshMenu.\n");
//	printf(" - Added Gameboot Skip option.\n");
//	printf(" - Fixed a ISO Parental Level.\n");
//	printf(" - Fixed a bug in OE driver.(again)\n");
	printf(" - Fixed a bug that failed to sleep when you change the CPU clock.\n");
//	printf(" - Fixed a bug in Official Static ELF loading.\n");
//	printf(" - Added a patch for 0x80020148 error.\n");
//	printf(" - Fixed a bug in sctrlKernelSetUserLevel.\n");
	printf(" - Added a patch for leda which allow to use it from ef0:/.\n");
	printf(" - Added a PBOOT.PBP execute option.\n");
	printf(" - Fixed a UMDdrive noise when enter sleep mode.\n");
	printf(" - Added a 200/100 and 166/83 CPU clock.\n");
//	printf(" - Update version.txt loading.\n");
//	printf(" - Fixed a bug when using POPS pulgin.\n");
case 0x84:	
	printf(" - Fixed USB Connect in Recovery menu.(05g only)\n");
case 0x86:
	printf(" - Update VshMenu. Now you can use translate text.\n");
	printf(" - Fixed a ISO size when dump it through USB in XMB.\n");
	printf(" - Fixed a bug in VshMenu when save config.\n");
case 0x87:
	printf(" - Fixed a freeze bug when enter sleep with SpeedUpMs is Enabled.\n");
	printf(" - Added a Text color option in Recovery menu.\n");


#elif _PSP_FW_VERSION == 660
case 0:	
	printf(" - Update version.txt loading.\n");
	printf(" - Update Nidresolver.\n");	
case 1:	
	printf(" - Update RecoveryMenu.\n");
	printf(" - Added a 166/83 CPU clock.\n");
case 2:	
	printf(" - Added Backup/Restore Netconfig option in Recovery menu.\n");
	printf(" - Block VshMenu while using RemotePlay, Skype, 1seg and PSN-store.\n");
case 3:
	printf(" - Fixed USB Connect in Recovery menu.(05g only)\n");
	printf(" - Update VshMenu.\n");
case 4:
//	printf(" - Added a 199/99 CPU clock.\n");
	printf(" - Fixed a bug in VshMenu when save config.\n");
case 5:
	printf(" - Added a 200/100 CPU clock.\n");
	printf(" - Update VshMenu. Now you can use translate text.\n");
	printf(" - Fixed a ISO size when dump it through USB in XMB.\n");
case 6:
	printf(" - Fixed a freeze bug when enter sleep with SpeedUpMs is Enabled.\n");
	printf(" - Added a Text color option in Recovery menu.\n");
case 7:
	printf(" - Bug fix.\n");
	printf(" - Remove debug option.\n");
#else
#error changelog.h
#endif
