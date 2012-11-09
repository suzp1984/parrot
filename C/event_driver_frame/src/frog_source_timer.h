#ifndef FROG_SOURCE_TIMER_H
#define FROG_SOURCE_TIMER_H

#include "frog_source.h"

DECLES_BEGIN

typedef Ret (*FrogTimer)(void* user_data);

FrogSource* frog_source_timer_create(int interval, FrogTimer action, void* user_data);

Ret frog_source_timer_reset(FrogSource* thiz);
Ret frog_source_timer_modify(FrogSource* thiz, int interval);

DECLES_END

#endif // FROG_SOURCE_TIMER_H
