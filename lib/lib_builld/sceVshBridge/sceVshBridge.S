	.set noreorder

#include "pspimport.s"

// Build files
// sceVshBridge_0000.o sceVshBridge_0001.o sceVshBridge_0002.o sceVshBridge_0003.o sceVshBridge_0004.o sceVshBridge_0005.o sceVshBridge_0006.o sceVshBridge_0007.o sceVshBridge_0008.o sceVshBridge_0009.o sceVshBridge_0010.o sceVshBridge_0011.o sceVshBridge_0012.o sceVshBridge_0013.o sceVshBridge_0014.o sceVshBridge_0015.o sceVshBridge_0016.o sceVshBridge_0017.o sceVshBridge_0018.o sceVshBridge_0019.o sceVshBridge_0020.o sceVshBridge_0021.o 

#ifdef F_sceVshBridge_0000
	IMPORT_START "sceVshBridge",0x40090011
#endif
#ifdef F_sceVshBridge_0001
	IMPORT_FUNC  "sceVshBridge",0x61001D64,vshChkregGetPsCode
#endif
#ifdef F_sceVshBridge_0002
	IMPORT_FUNC  "sceVshBridge",0xC6395C03,vshCtrlReadBufferPositive
#endif
#ifdef F_sceVshBridge_0003
	IMPORT_FUNC  "sceVshBridge",0x2380DC08,vshIoDevctl
#endif
#ifdef F_sceVshBridge_0004
	IMPORT_FUNC  "sceVshBridge",0x4DB43867,vshIdStorageLookup
#endif
#ifdef F_sceVshBridge_0005
	IMPORT_FUNC  "sceVshBridge",0x74DA9D25,vshLflashFatfmtStartFatfmt
#endif
#ifdef F_sceVshBridge_0006
	IMPORT_FUNC  "sceVshBridge",0xA5628F0D,vshKernelLoadModuleVSH
#endif
#ifdef F_sceVshBridge_0007
	IMPORT_FUNC  "sceVshBridge",0x41C54ADF,vshKernelLoadModuleVSHByID
#endif
#ifdef F_sceVshBridge_0008
	IMPORT_FUNC  "sceVshBridge",0xC9626587,vshKernelLoadModuleBufferVSH
#endif
#ifdef F_sceVshBridge_0009
	IMPORT_FUNC  "sceVshBridge",0x5C2983C2,vshChkregCheckRegion
#endif
#ifdef F_sceVshBridge_0010
	IMPORT_FUNC  "sceVshBridge",0xC949966C,vshPowerSetWakeupCondition
#endif
#ifdef F_sceVshBridge_0011
	IMPORT_FUNC  "sceVshBridge",0xCA719C34,vshImposeGetStatus
#endif
#ifdef F_sceVshBridge_0012
	IMPORT_FUNC  "sceVshBridge",0x4E4E4DA3,vshImposeSetStatus
#endif
#ifdef F_sceVshBridge_0013
	IMPORT_FUNC  "sceVshBridge",0x639C3CB3,vshImposeGetParam
#endif
#ifdef F_sceVshBridge_0014
	IMPORT_FUNC  "sceVshBridge",0x4A596D2D,vshImposeSetParam
#endif
#ifdef F_sceVshBridge_0015
	IMPORT_FUNC  "sceVshBridge",0x5894C339,vshImposeChanges
#endif
#ifdef F_sceVshBridge_0016
	IMPORT_FUNC  "sceVshBridge",0xCE32CBEF,vshMSAudioInit
#endif
#ifdef F_sceVshBridge_0017
	IMPORT_FUNC  "sceVshBridge",0xE5DA5E95,vshMSAudioEnd
#endif
#ifdef F_sceVshBridge_0018
	IMPORT_FUNC  "sceVshBridge",0x6CAEB765,vshMSAudioAuth
#endif
#ifdef F_sceVshBridge_0019
	IMPORT_FUNC  "sceVshBridge",0x53BFD101,vshMSAudioCheckICV
#endif
#ifdef F_sceVshBridge_0020
	IMPORT_FUNC  "sceVshBridge",0xE174218C,vshMSAudioCheckICVn
#endif
#ifdef F_sceVshBridge_0021
	IMPORT_FUNC  "sceVshBridge",0x7EA32357,vshMSAudioDeauth
#endif
