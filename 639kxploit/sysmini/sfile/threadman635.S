	.set noreorder

#include "pspstub.s"

	STUB_START "ThreadManForKernel", 0x00010011, 0x00020005
	STUB_FUNC 0xF475845D, sceKernelStartThread
	STUB_FUNC 0x446D8DE6, sceKernelCreateThread
	STUB_END
	