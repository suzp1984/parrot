#ifndef FROG_EVENT_H
#define FROG_EVENT_H

#include "typedef.h"
#include <string.h>

DECLES_BEGIN

typedef enum _FrogEventType
{
	FROG_EVT_NOP = 0,
	FROG_EVT_ADD_SOURCE,
	FROG_EVT_REMOVE_SOURCE
} FrogEventType;

typedef struct _FrogEvent
{
	FrogEventType type;

	void* widget;
	size_t time;
	union
	{
		void* extra;
	}u;
} FrogEvent;

typedef Ret (*FrogOnEvent)(void* user_data, FrogEvent* event);

static inline Ret frog_event_init(FrogEvent* event, FrogEventType type)
{
	if (event != NULL) {
		memset(event, 0x00, sizeof(FrogEvent));
		event->type = type;
	}

	return RET_OK;
}

DECLES_END

#endif // FROG_EVENT_H
