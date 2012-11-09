#ifndef FROG_SOURCES_MANAGER_H
#define FROG_SOURCES_MANAGER_H

#include "frog_source.h"
#include "typedef.h"

DECLES_BEGIN

struct _FrogSourcesManager;
typedef struct _FrogSourcesManager FrogSourcesManager;

FrogSourcesManager* frog_sources_manager_create(int max_sources);
Ret frog_sources_manager_add(FrogSourcesManager* thiz, FrogSource* source);
Ret frog_sources_manager_remove(FrogSourcesManager* thiz, FrogSource* source);

int frog_sources_manager_get_count(FrogSourcesManager* thiz);
FrogSource* frog_sources_manager_get(FrogSourcesManager* thiz, int i);

/* Tell the main-loop break current loop and reselect */
int frog_sources_manager_need_refresh(FrogSourcesManager* thiz);
Ret frog_sources_manager_set_need_refresh(FrogSourcesManager* thiz);
FrogSource* frog_sources_manager_get_primary_source(FrogSourcesManager* thiz);

void frog_sources_manager_destroy(FrogSourcesManager* thiz);


DECLES_END

#endif
