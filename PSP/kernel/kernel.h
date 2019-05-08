#ifndef KERNEL_H
#define KERNEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pspsdk.h> 
#include <pspkernel.h>
#include <psploadexec.h>
#include <pspsysevent.h>

int kernel_loadExec(const char *file, int argc, char** argv);
int kernel_sceKernelRegisterSysEventHandler(PspSysEventHandler *handler);
int kernel_sceKernelUnregisterSysEventHandler(PspSysEventHandler *handler);
unsigned int kernel_sctrlHENFindFunction(char *modname, char *libname, unsigned int nid);

#ifdef __cplusplus
}
#endif

#endif