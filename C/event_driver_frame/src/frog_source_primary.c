#include "frog_source_primary.h"

#include <stdlib.h>

#include "frog_pipe.h"
#include "frog_event.h"

typedef struct {
	FrogPipe* pipe;
	FrogOnEvent on_event;
	void* user_data;
} PrivInfo;

static int frog_source_primary_get_fd(FrogSource* thiz)
{
	DECLES_PRIV(priv, thiz);

	return frog_pipe_get_read_handle(priv->pipe);
}

static int frog_source_primary_check(FrogSource* thiz)
{
	DECLES_PRIV(priv, thiz);

	return frog_pipe_check(priv->pipe);
}

static Ret frog_source_primary_dispatch(FrogSource* thiz)
{
	DECLES_PRIV(priv, thiz);

	int ret = 0;
	FrogEvent event;
	ret = frog_pipe_read(priv->pipe, &event, sizeof(FrogEvent));

	return_val_if_fail(ret == sizeof(FrogEvent), RET_FAIL);

	switch(event.type)
	{
		case FROG_EVT_NOP:
			break;
		default:
		{
			if (priv->on_event != NULL)
			{
				priv->on_event(priv->user_data, &event);
			}
		}

	}

	return RET_OK;
}

static void frog_source_primary_destroy(FrogSource* thiz)
{
	if (thiz != NULL) {
		DECLES_PRIV(priv, thiz);
		frog_pipe_destroy(priv->pipe);
		free(thiz);
	}
}

FrogSource* frog_source_primary_create(FrogOnEvent on_event, void* user_data)
{
	FrogSource* thiz = (FrogSource*) malloc(sizeof(FrogSource) + sizeof(PrivInfo));

	if (thiz != NULL) {
		DECLES_PRIV(priv, thiz);
		thiz->getfd = frog_source_primary_get_fd;
		thiz->check = frog_source_primary_check;
		thiz->dispatch = frog_source_primary_dispatch;
		thiz->destroy = frog_source_primary_destroy;

		thiz->ref = 1;
		priv->pipe = frog_pipe_create();
		priv->on_event = on_event;
		priv->user_data = user_data;
	}

	return thiz;
}

Ret frog_source_queue_event(FrogSource* thiz, FrogEvent* event)
{
	return_val_if_fail(thiz != NULL && event != NULL, RET_INVALID_PARAMS);
	DECLES_PRIV(priv, thiz);

	int ret = 0;

	ret = frog_pipe_write(priv->pipe, event, sizeof(FrogEvent));

	return (ret == sizeof(FrogEvent)) ? RET_OK : RET_FAIL;
}
