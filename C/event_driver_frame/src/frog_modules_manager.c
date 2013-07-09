#include "frog_modules_manager.h"
#include "dlist.h"

#include <stdlib.h>

struct _FrogModulesManager 
{
    // maybe this container should be a hash table.
    Dlist* modules;
};

static void _frog_modules_destroy_node(void* ctx, void* data)
{
    frog_module_destroy((FrogModule*)data);
    return;
}

FrogModulesManager* frog_modules_manager_create()
{
    FrogModulesManager* thiz = (FrogModulesManager*)malloc(sizeof(FrogModulesManager));
    
    if (thiz != NULL) {
        thiz->modules = dlist_create(_frog_modules_destroy_node, NULL, NULL);
    }

    return thiz;
}

Ret frog_modules_manager_add(FrogModulesManager* thiz, FrogModule* m)
{
    return_val_if_fail(thiz!= NULL && m != NULL, RET_FAIL);
    
    if (thiz->modules != NULL)
    {
        return dlist_append(thiz->modules, (void*)m);
    }
    return RET_FAIL;
}

static int _frog_modules_cmp(void* ctx, void* data)
{
    return (ctx - data);
}

Ret frog_modules_manager_remove(FrogModulesManager* thiz, FrogModule* m)
{
    return_val_if_fail(thiz != NULL && thiz->modules != NULL && m != NULL,
                       RET_FAIL);
    
    int i = 0;
    i = dlist_find(thiz->modules, _frog_modules_cmp, m);
    return dlist_delete(thiz->modules, i);
}

static int _frog_modules_cmp_by_name(void* ctx, void* data)
{
    char* name = (char*)ctx;
    FrogModule* m = (FrogModule*)data;
    
    return strcmp(name, frog_module_get_name(m));
}

Ret frog_modules_manager_remove_by_name(FrogModulesManager* thiz, const char* name)
{
    return_val_if_fail(thiz != NULL && thiz->modules != NULL
                       && name != NULL, RET_FAIL);
    int i = 0;

    i = dlist_find(thiz->modules, _frog_modules_cmp_by_name, name);
    return dlist_delete(thiz->modules, i);
}

int frog_modules_manager_get_count(FrogModulesManager* thiz)
{
    return_val_if_fail(thiz != NULL && thiz->modules != NULL, 0);

    return dlist_length(thiz->modules);
}

void frog_modules_manager_destroy(FrogModulesManager* thiz)
{
    return_if_fail(thiz != NULL && thiz->modules != NULL);
    
    dlist_destroy(thiz->modules);
    SAFE_FREE(thiz);
}
