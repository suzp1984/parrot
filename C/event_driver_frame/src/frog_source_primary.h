#ifndef FROG_SOURCE_PRIMARY_H
#define FROG_SOURCE_PRIMARY_H

#include "frog_event.h"
#include "frog_source.h"

DECLES_BEGIN

FrogSource* frog_source_primary_create(FrogOnEvent on_event, void* user_data);

Ret frog_source_queue_event(FrogSource* thiz, FrogEvent* event);

DECLES_END

#endif // FROG_SOURCE_PRIMARY_H
