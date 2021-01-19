PSPDEV=$(shell psp-config --pspsdk-path)

#MODULE_PATH = ../../installer/modules
MODULE_PATH = ..

all:	a.bin

a.bin: 
	psp-build-exports -k ../../systemctrl/exports.exp
	psp-build-exports -k ../../vshctrl/exports.exp
	mv KUBridge.S kubridge/KUBridge.S
	mv SystemCtrlForKernel.S systemctrl_kernel/SystemCtrlForKernel.S
	mv SystemCtrlForUser.S systemctrl_user/SystemCtrlForUser.S
	mv VshCtrlLib.S vshctrl/VshCtrlLib.S
	"./make_lib.py" 

clean:


#all:
#	psp-build-exports -k ../../systemctrl/exports.exp
#	mv KUBridge.S kubridge/KUBridge.S
#	mv SystemCtrlForKernel.S systemctrl_kernel/SystemCtrlForKernel.S
#	mv SystemCtrlForUser.S systemctrl_user/SystemCtrlForUser.S
#	"./test.py"


