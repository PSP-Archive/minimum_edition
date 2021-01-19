
#include "../recovery/recovery_bin.h"
#include "../galaxy/pulsar_bin.h"
#include "../march33/isotope_bin.h"
#include "../horoscope/horoscope_bin.h"
#include "../vshctrl/vshctrl_bin.h"
//#include "../vshmenu_src/satellite_bin.h"
#include "../vshmenu_new/satellite_bin.h"
#include "../idcanager/idmanager_bin.h"

//#include "modules/usbdev.h"
#include "../usbdevice/usbdevice_bin.h"

#include "march33.h"

#include "pspbtjnf.h"
//#include "modules/pspbtjnf.h"
//#include "modules/pspbtknf.h"
//#include "modules/pspbtlnf.h"
#include "../systemctrl/systemctrl_01g_bin.h"

#include "pspbtjnf_02g.h"
//#include "modules/pspbtjnf_02g.h"
//#include "modules/pspbtknf_02g.h"
//#include "modules/pspbtlnf_02g.h"
#include "../systemctrl/systemctrl_02g_bin.h"

//#include "modules/ipl_block_large.h"

typedef struct Module
{
	char *dst;
	void* buffer;
	u32 size;
} Module;

#define MODULES_COUNT 2
#define COMMON_MODULES_COUNT 9


//#if _PSP_FW_VERSION == 639

static const Module common_modules[COMMON_MODULES_COUNT] = 
{
	{ "flash0:/kd/usbdev.prx", usbdevice, sizeof(usbdevice) },
	{ "flash0:/vsh/module/satellite.prx", satellite, sizeof(satellite) },
	{ "flash0:/vsh/module/recovery.prx", recovery, sizeof(recovery) },
	{ "flash0:/kd/dax9660.prx", march33, sizeof(march33) },
	{ "flash0:/kd/isotope.prx", isotope, sizeof(isotope) },
	{ "flash0:/kd/idmanager.prx", idmanager, sizeof(idmanager) },
	{ "flash0:/kd/pulsar.prx", pulsar, sizeof(pulsar) },
	{ "flash0:/kd/horoscope.prx", horoscope, sizeof(horoscope) },
//	{ "flash1:/prometheus_key.txt", prometheus_key, sizeof(prometheus_key) }
	{ "flash0:/kd/vshctrl_02g.prx", vshctrl, sizeof(vshctrl) }
};

static const Module modules_01g[MODULES_COUNT] =
{
	{ "flash0:/kd/pspbtjnf.bin", pspbtjnf , sizeof(pspbtjnf) },
//	{ "flash0:/kd/pspbtknf.bin", pspbtknf , sizeof(pspbtknf) },
//	{ "flash0:/kd/pspbtlnf.bin", pspbtlnf, sizeof(pspbtlnf) },
	{ "flash0:/kd/systemctrl_01g.prx", systemctrl_01g, sizeof(systemctrl_01g) }
};

static const Module modules_02g[MODULES_COUNT] =
{
	{ "flash0:/kd/pspbtjnf_02g.bin", pspbtjnf_02g, sizeof(pspbtjnf_02g) },
//	{ "flash0:/kd/pspbtknf_02g.bin", pspbtknf_02g, sizeof(pspbtknf_02g) },
//	{ "flash0:/kd/pspbtlnf_02g.bin", pspbtlnf_02g, sizeof(pspbtlnf_02g) },
	{ "flash0:/kd/systemctrl_02g.prx", systemctrl_02g, sizeof(systemctrl_02g) }
};

//#endif