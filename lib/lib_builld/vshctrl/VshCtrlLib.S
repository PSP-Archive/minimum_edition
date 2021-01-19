	.set noreorder

#include "pspimport.s"

// Build files
// VshCtrlLib_0000.o VshCtrlLib_0001.o VshCtrlLib_0002.o VshCtrlLib_0003.o VshCtrlLib_0004.o VshCtrlLib_0005.o VshCtrlLib_0006.o VshCtrlLib_0007.o 

#ifdef F_VshCtrlLib_0000
	IMPORT_START "VshCtrlLib",0x40090000
#endif
#ifdef F_VshCtrlLib_0001
	IMPORT_FUNC  "VshCtrlLib",0xFD26DA72,vctrlVSHRegisterVshMenu
#endif
#ifdef F_VshCtrlLib_0002
	IMPORT_FUNC  "VshCtrlLib",0xCD6B3913,vctrlVSHExitVSHMenu
#endif
#ifdef F_VshCtrlLib_0003
	IMPORT_FUNC  "VshCtrlLib",0xE6A6ACBB,vctrlVSHSetRecovery
#endif
#ifdef F_VshCtrlLib_0004
	IMPORT_FUNC  "VshCtrlLib",0xAEB3290C,vctrlVSHSetSpeed
#endif
#ifdef F_VshCtrlLib_0005
	IMPORT_FUNC  "VshCtrlLib",0x483D8EDE,vctrlVSHGetSpeed
#endif
#ifdef F_VshCtrlLib_0006
	IMPORT_FUNC  "VshCtrlLib",0x41C72DA4,sfo_patch
#endif
#ifdef F_VshCtrlLib_0007
	IMPORT_FUNC  "VshCtrlLib",0x23FE148D,sceUmdRegisterUMDCallBackPatch
#endif
