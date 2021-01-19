	.set noreorder

#include "pspstub.s"

	STUB_START "SystemCtrlForKernel", 0x00090000, 0x00050005
	STUB_FUNC 0x16C3B7EE, sctrlSEGetConfig
	STUB_FUNC 0xAC56B90B, sctrlSEGetUmdFile
	STUB_FUNC 0xF9584CAD, sctrlKernelMalloc
	STUB_FUNC 0xA65E8BC4, sctrlKernelFree
	STUB_FUNC 0x1F3037FB, SystemCtrlForKernel_1F3037FB
	STUB_END

/*
	STUB_START "sceOpenPSID_driver", 0x00010011, 0x00010005
	STUB_FUNC 0xC69BEBCE, sceOpenPSIDGetOpenPSID
	STUB_END
*/
