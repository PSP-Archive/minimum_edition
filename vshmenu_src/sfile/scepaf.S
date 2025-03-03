	.set noreorder

#include "pspstub.s"

#if _PSP_FW_VERSION == 639
	STUB_START "scePaf",0x00010011,0x00090005 
	STUB_FUNC 0x49A72E5D, scePaf_strlen
	STUB_FUNC 0x43B6A88A, scePaf_memset
	STUB_FUNC 0x861C4627, scePaf_memcmp
	STUB_FUNC 0x4C386F3C, scePaf_sprintf
	STUB_FUNC 0x726776D7, scePaf_snprintf
	STUB_FUNC 0xE281261E, scePaf_memcpy
	STUB_FUNC 0x8F95CC01, scePaf_strcpy
	STUB_FUNC 0x9E9FFBFB, scePaf_malloc
	STUB_FUNC 0xB0363C2E, scePaf_free
	STUB_END

#elif _PSP_FW_VERSION == 660
	STUB_START "scePaf",0x00010011,0x00090005 
	STUB_FUNC 0xD7DCB972, scePaf_strlen
	STUB_FUNC 0xBB89C9EA, scePaf_memset
	STUB_FUNC 0xB05D9677, scePaf_memcmp
	STUB_FUNC 0xA138A376, scePaf_sprintf
	STUB_FUNC 0x7B7133D5, scePaf_snprintf
	STUB_FUNC 0xF7C46E37, scePaf_memcpy
	STUB_FUNC 0x726DFBA9, scePaf_strcpy
	STUB_FUNC 0x5E909060, scePaf_malloc
	STUB_FUNC 0xDB1612F1, scePaf_free
	STUB_END

#else
#error scePaf.S
#endif