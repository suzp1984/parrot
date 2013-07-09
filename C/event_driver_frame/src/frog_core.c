#include "frog_core.h"

#include <stdlib.h>

struct _FrogCore 
{
    FrogMainLoop* mainloop;
    FrogModulesManager* modules_manager;
};

FrogCore* frog_core_create()
{
    FrogCore* thiz = (FrogCore*)malloc(sizeof(FrogCore));
    
    if (thiz != NULL) {
                
    }

    return thiz;
}

Ret frog_core_set_modules_manager(FrogCore* thiz, FrogModulesManager* manager)
{
    return_val_if_fail(thiz != NULL && manager != NULL, RET_FAIL);
    
    thiz->modules_manager = manager;
    
    return RET_OK;
}

FrogModulesManager* frog_core_get_modules_manager(FrogCore* thiz)
{
    return_val_if_fail(thiz != NULL, NULL);
    
    return thiz->modules_manager;
}

Ret frog_core_set_main_loop(FrogCore* thiz, FrogMainLoop* mainloop)
{
    return_val_if_fail(thiz != NULL && mainloop != NULL, RET_FAIL);

    thiz->mainloop = mainloop;
    return RET_OK;
}

FrogMainLoop* frog_core_get_main_loop(FrogCore* thiz)
{
    return_val_if_fail(thiz != NULL, NULL);
    
    return thiz->mainloop;
}

void frog_core_destroy(FrogCore* thiz)
{
    return_if_fail(thiz != NULL);
    
    frog_modules_manager_destroy(thiz->modules_manager);
    SAFE_FREE(thiz);
}

#ifdef FROG_CORE_TEST

#include "frog_sources_manager.h"
#include "frog_main_loop.h"
#include "frog_modules_manager.h"

int main(int argc, char* argv[])
{
    FrogCore* thiz = frog_core_create();
    FrogSourcesManager* sources_manager = frog_sources_manager_create(64);
    FrogMainLoop* mainloop = frog_main_loop_create(sources_manager);
    FrogModulesManager* modules_manager = frog_modules_manager_create();

    frog_core_set_modules_manager(thiz, modules_manager);
    frog_core_set_main_loop(thiz, mainloop);

    frog_modules_manager_destroy(modules_manager);
    frog_main_loop_destroy(mainloop);
    frog_sources_manager_destroy(sources_manager);
    frog_core_destroy(thiz);
    return 0;
}

#endif // FROG_CORE_TEST
