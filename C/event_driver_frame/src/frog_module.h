#ifndef FROG_MODULE_H
#define FROG_MODULE_H

#include "typedef.h"

DECLES_BEGIN

struct _FrogModule;
typedef struct _FrogModule FrogModule;

FrogModule* frog_module_create(const char* name, const char* arguments, void* ctx);

int frog_module_init(FrogModule* thiz, void* ctx);
int frog_module_start(FrogModule* thiz, void* ctx);
void frog_module_done(FrogModule* thiz, void* ctx);

Ret frog_module_set_name(FrogModule* thiz, const char* name);
const char* frog_module_get_name(FrogModule* thiz);
void frog_module_destroy(FrogModule* thiz);

DECLES_END

#endif
