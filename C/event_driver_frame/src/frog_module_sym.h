#ifndef FROG_MODULE_SYM_H
#define FROG_MODULE_SYM_H

#include "typedef.h"
#include "frog_module.h"

DECLES_BEGIN

#define FROG_MODULE_INFO FMI
#define FROG_MODULE_INFO_AS_STR "FMI"

typedef int (*FrogModuleInitFunc)(FrogModule* thiz, void* ctx);
typedef int (*FrogModuleStartFunc)(FrogModule* thiz, void* ctx);
typedef void (*FrogModuleDoneFunc)(FrogModule* thiz, void* ctx);

struct FrogModuleInfo 
{
    const char* author;
    const char* version;
    const char* descrition;
    const char* usage;

    FrogModuleInitFunc init;
    FrogModuleStartFunc start;
    FrogModuleDoneFunc done;
};

DECLES_END

#endif
