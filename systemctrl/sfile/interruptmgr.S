	.set noreorder

#include "pspstub.s"

#if _PSP_FW_VERSION == 639
	STUB_START "InterruptManagerForKernel", 0x00010011, 0x00010005
	STUB_FUNC 0xD88DD3B4, sceKernelCallSubIntrHandler
	STUB_END

#elif _PSP_FW_VERSION == 660
	STUB_START "InterruptManagerForKernel", 0x00010011, 0x00010005
	STUB_FUNC 0x0C5F7AE3, sceKernelCallSubIntrHandler
	STUB_END

#else
#error interept.s
#endif