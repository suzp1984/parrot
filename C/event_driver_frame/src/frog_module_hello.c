#include "frog_module_sym.h"

#include <stdio.h>

#define HELLO_MODULE_NAME "hello"
#define HELLO_MODULE_AUTHOR "zxsu"
#define HELLO_MODULE_VERSION "0.1"
#define HELLO_MODULE_DESCRIPTION "Hello Module"
#define HELLO_MODULE_USAGE "What ever!!"

static int hello_init(FrogModule* thiz, void* ctx)
{
    printf("Init Hello...\n");
    return 0;
}

static int hello_start(FrogModule* thiz, void* ctx)
{
    printf("start Hello...\n");
    return 0;
}

static void hello_done(FrogModule* thiz, void* ctx)
{
    printf("done Hello...\n");
    return;
}

const struct FrogModuleInfo FROG_MODULE_INFO = {
    .author = HELLO_MODULE_AUTHOR,
    .version = HELLO_MODULE_VERSION,
    .descrition = HELLO_MODULE_DESCRIPTION,
    .usage = HELLO_MODULE_USAGE,
    .init = hello_init,
    .start = hello_start,
    .done = hello_done,
};
