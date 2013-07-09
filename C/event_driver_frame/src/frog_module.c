#include "frog_module.h"
#include "frog_module_sym.h"

#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <limits.h>
#include <string.h>

/*
#define FROG_MODULE_INIT "module_init"
#define FROG_MODULE_START "module_start"
#define FROG_MODULE_DONE "module_done"
#define FROG_MODULE_DESCRIPTION "module_description"
*/

#define FROG_MODULE_PATH "./"
#define FROG_MODULE_PATH2 "~/.frog/modules/"

struct _FrogModule
{
    char* name;
    char* argument;

//    FrogModuleInitFunc init;
//    FrogModuleStartFunc start;
//    FrogModuleDoneFunc done;
    
    struct FrogModuleInfo* minfo;
    void* ctx;
    void* handler;
};

static Ret frog_module__load(FrogModule* thiz, const char* name)
{
    return_val_if_fail(thiz != NULL && name != NULL, RET_FAIL);
    
    char path[PATH_MAX];
    void* handle;
    struct FrogModuleInfo* minfo;
    
    snprintf(path, sizeof(path), "%s/lib%s.so", FROG_MODULE_PATH, name);
    if (access(path, R_OK) != 0) {
        snprintf(path, sizeof(path), "%s/lib%s.so", FROG_MODULE_PATH2, name);
    }
    
    if (access(path, R_OK) != 0) {
        return RET_FAIL;
    }

    handle = dlopen(path, RTLD_NOW);
    if (handle == NULL) {
        return RET_FAIL;
    }
    thiz->handler = handle;

    const char* sym = FROG_MODULE_INFO_AS_STR;
    minfo = (struct FrogModuleInfo*)dlsym(handle, sym);
    if (minfo == NULL) {
        return RET_FAIL;
    }
    
    thiz->minfo = minfo;
    return RET_OK;
}

FrogModule* frog_module_create(const char* name, const char* arguments, void* ctx)
{
    return_val_if_fail(name != NULL, NULL);

    FrogModule* thiz = (FrogModule*)malloc(sizeof(FrogModule));
    
    if (thiz != NULL) {
        thiz->ctx = ctx;
        thiz->name = strdup(name);
        if (arguments) {
            thiz->argument = strdup(arguments);
        }
        if (frog_module__load(thiz, name) != RET_OK) {
            frog_module_destroy(thiz);
            return NULL;
        }
    }
    
    return thiz;
}

int frog_module_init(FrogModule* thiz, void* ctx)
{
    return_val_if_fail(thiz != NULL, -1);
    
    if (thiz->minfo && thiz->minfo->init) {
        return thiz->minfo->init(thiz, ctx);
    }
    return 0;
}

int frog_module_start(FrogModule* thiz, void* ctx)
{
    return_val_if_fail(thiz != NULL, -1);
    
    if (thiz->minfo && thiz->minfo->start) {
        return thiz->minfo->start(thiz, ctx);
    }
    
    return 0;
}

void frog_module_done(FrogModule* thiz, void* ctx)
{
    return_if_fail(thiz != NULL);
    
    if (thiz->minfo && thiz->minfo->done) {
        thiz->minfo->done(thiz, ctx);
    }
    
    return;
}

const char* frog_module_get_name(FrogModule* thiz)
{
    return_val_if_fail(thiz != NULL && thiz->name != NULL, NULL);
    
    return thiz->name;
}

Ret frog_module_set_name(FrogModule* thiz, const char* name)
{
    return_val_if_fail(thiz != NULL && name != NULL, RET_FAIL);
    
    SAFE_FREE(thiz->name);
    thiz->name = strdup(name);
    
    return RET_OK;
}

void frog_module_destroy(FrogModule* thiz)
{
    return_if_fail(thiz != NULL);
    
    if (thiz->handler != NULL) {
        dlclose(thiz->handler);
        thiz->handler = NULL;
    }

    SAFE_FREE(thiz->name);
    SAFE_FREE(thiz->argument);
    SAFE_FREE(thiz);
}

#ifdef FROG_MODULE_TEST

#include <assert.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    FrogModule* hello = frog_module_create("hello", NULL, NULL);
    printf("After frog module init.\n");

    assert(hello != NULL);
    frog_module_init(hello, NULL);
    frog_module_start(hello, NULL);
    frog_module_done(hello, NULL);

    frog_module_destroy(hello);

    return 0;
}
#endif // FROG_MODULE_TEST
