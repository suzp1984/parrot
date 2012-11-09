#ifndef FROG_MAIN_LOOP_H
#define FROG_MAIN_LOOP_H

#include "frog_sources_manager.h"
#include "typedef.h"

DECLES_BEGIN

struct _FrogMainLoop;
typedef struct _FrogMainLoop FrogMainLoop;

FrogMainLoop* frog_main_loop_create(FrogSourcesManager* sources_manager);

Ret frog_main_loop_run(FrogMainLoop* thiz);
Ret frog_main_loop_quit(FrogMainLoop* thiz);
Ret frog_main_loop_add_source(FrogMainLoop* thiz, FrogSource* source);
Ret frog_main_loop_remove_source(FrogMainLoop* thiz, FrogSource* source);

void frog_main_loop_destroy(FrogMainLoop* thiz);

DECLES_END

#endif // FROG_MAIN_LOOP_H
