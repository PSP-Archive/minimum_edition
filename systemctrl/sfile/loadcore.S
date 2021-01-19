	.set noreorder


#if _PSP_FW_VERSION == 639
/*
#include "pspstub.s"
	STUB_START "LoadCoreForKernel", 0x00010011, 0x00040005
	STUB_FUNC 0xEF8A0BEA, sceKernelFindModuleByName
	STUB_FUNC 0x312CA47E, sceKernelFindModuleByAddress
	STUB_FUNC 0xED53894F, sceKernelFindModuleByUID
	STUB_FUNC 0x01DB1EB3, sceKernelCheckExecFile
	STUB_END
*/
#include "pspimport.s"

	IMPORT_START	"LoadCoreForKernel",0x00010011
	IMPORT_FUNC	"LoadCoreForKernel",0xEF8A0BEA,sceKernelFindModuleByName
	IMPORT_FUNC	"LoadCoreForKernel",0x312CA47E,sceKernelFindModuleByAddress
	IMPORT_FUNC	"LoadCoreForKernel",0xED53894F,sceKernelFindModuleByUID
//	IMPORT_FUNC	"LoadCoreForKernel",0x01DB1EB3,sceKernelCheckExecFile

#elif _PSP_FW_VERSION == 660
#include "pspimport.s"

	IMPORT_START	"LoadCoreForKernel",0x00010011
	IMPORT_FUNC	"LoadCoreForKernel",0xF6B1BF0F,sceKernelFindModuleByName
	IMPORT_FUNC	"LoadCoreForKernel",0xBC99C625,sceKernelFindModuleByAddress
	IMPORT_FUNC	"LoadCoreForKernel",0x40972E6E,sceKernelFindModuleByUID
//	IMPORT_FUNC	"LoadCoreForKernel",0xD3353EC4,sceKernelCheckExecFile

#else
#error loadcore.s
#endif