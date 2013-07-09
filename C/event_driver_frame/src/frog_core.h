#ifndef FROG_CORE_H
#define FROG_CORE_H

#include "typedef.h"
#include "frog_modules_manager.h"
#include "frog_main_loop.h"
//#include "frog_module.h"

DECLES_BEGIN

struct _FrogCore;
typedef struct _FrogCore FrogCore;

FrogCore* frog_core_create();

Ret frog_core_set_modules_manager(FrogCore* thiz, FrogModulesManager* manager);
FrogModulesManager* frog_core_get_modules_manager(FrogCore* thiz);

Ret frog_core_set_main_loop(FrogCore* thiz, FrogMainLoop* mainloop);
FrogMainLoop* frog_core_get_main_loop(FrogCore* thiz);

void frog_core_destroy(FrogCore* thiz);

DECLES_END

#endif
