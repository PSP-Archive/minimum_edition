#ifndef __ADDR_LIST_H__
#define __ADDR_LIST_H__


struct Galaxy_patch {
	u32 InitFuncPatchAddr;
	u32 IsoOpenPatchAddr;
	u32 IsoReadPatchAddr1;
	u32 IsoReadPatchAddr2;
	u32 IsoClosePatchAddr;
	u32 UmdCheckMediumAddr;
	u32 UmdMutexLockAddr;
	u32 UmdMutexUnlockAddr;
	u32 CreateThreadPatchAddr;
	u32 StartThreadPatchAddr;
	u32 IsoFdPatchAddr;
	u32 IsoInfoPatchAddr;
	u32 CallbackFlagPatchAddr;
};

#if _PSP_FW_VERSION == 639
static const struct Galaxy_patch galaxy_patch_addr = {
	.InitFuncPatchAddr	= 0x00003C5C,
	.IsoOpenPatchAddr	= 0x00003C78,
	.IsoReadPatchAddr1	= 0x00004414,
	.IsoReadPatchAddr2	= 0x0000596C,
	.IsoClosePatchAddr	= 0x00007D68,
	.UmdCheckMediumAddr	= 0x000036A8,
	.UmdMutexLockAddr	= 0x00004FEC,
	.UmdMutexUnlockAddr	= 0x0000505C,
//	.CreateThreadPatchAddr	= 0x000191B4,
//	.StartThreadPatchAddr	= 0x00019358,
	.IsoFdPatchAddr		= 0x00008A88,
	.IsoInfoPatchAddr	= 0x00008A98,
	.CallbackFlagPatchAddr	= 0x00008A14,
};
#elif _PSP_FW_VERSION == 660
static const struct Galaxy_patch galaxy_patch_addr = {
	.InitFuncPatchAddr	= 0x00003C5C,
	.IsoOpenPatchAddr	= 0x00003C78,
	.IsoReadPatchAddr1	= 0x00004414,
	.IsoReadPatchAddr2	= 0x0000596C,
	.IsoClosePatchAddr	= 0x00007D68,
	.UmdCheckMediumAddr	= 0x000036A8,
	.UmdMutexLockAddr	= 0x00004FEC,
	.UmdMutexUnlockAddr	= 0x0000505C,
//	.CreateThreadPatchAddr	= ,
//	.StartThreadPatchAddr	= ,
	.IsoFdPatchAddr		= 0x00008A88,
	.IsoInfoPatchAddr	= 0x00008A98,
	.CallbackFlagPatchAddr	= 0x00008A14,
};
#else
#error FW error
#endif

#endif

