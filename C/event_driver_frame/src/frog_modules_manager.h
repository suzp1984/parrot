#ifndef FROG_MODULES_MANAGER_H
#define FROG_MODULES_MANAGER_H

#include "typedef.h"
#include "frog_module.h"

DECLES_BEGIN

struct _FrogModulesManager;
typedef struct _FrogModulesManager FrogModulesManager;

FrogModulesManager* frog_modules_manager_create();
Ret frog_modules_manager_add(FrogModulesManager* thiz, FrogModule* m);
Ret frog_modules_manager_remove(FrogModulesManager* thiz, FrogModule* m);
Ret frog_modules_manager_remove_by_name(FrogModulesManager* thiz, const char* name);

int frog_modules_manager_get_count(FrogModulesManager* thiz);

void frog_modules_manager_destroy(FrogModulesManager* thiz);

DECLES_END

#endif
