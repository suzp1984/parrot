#include "frog_source_timer.h"

#include <sys/time.h>

typedef struct {
	int interval;
	FrogTimer action;
	void* user_data;
	int next_time;
} PrivInfo;

static int frog_get_relative_time(void);

static int frog_get_relative_time(void)
{
	struct timeval now = {0};
	gettimeofday(&now, NULL);

	return now.tv_sec*1000 + now.tv_usec/1000;
}

static int frog_source_timer_get_fd(FrogSource* thiz)
{
	return -1;
}

static int frog_source_timer_check(FrogSource* thiz)
{
	DECLES_PRIV(priv, thiz);

	int t = priv->next_time - frog_get_relative_time();

	t = t < 0 ? 0 : t;

	return t;
}

static Ret frog_source_timer_dispatch(FrogSource* thiz)
{
	DECLES_PRIV(priv, thiz);
	Ret ret = RET_FAIL;

	if (thiz->disable <= 0)
	{
		ret = priv->action(priv->user_data);
	}

	priv->next_time = frog_get_relative_time() + priv->interval;

	return ret;
}

static void frog_source_timer_destroy(FrogSource* thiz)
{
	SAFE_FREE(thiz);
}

FrogSource* frog_source_timer_create(int interval, FrogTimer action, void* user_data)
{
	return_val_if_fail(interval > 0 && action != NULL,  NULL);

	FrogSource* thiz = (FrogSource*) malloc(sizeof(FrogSource) + sizeof(PrivInfo));

	if (thiz != NULL) {
		DECLES_PRIV(priv, thiz);
		thiz->getfd = frog_source_timer_get_fd;
		thiz->check = frog_source_timer_check;
		thiz->dispatch = frog_source_timer_dispatch;
		thiz->destroy = frog_source_timer_destroy;

		priv->interval = interval;
		priv->action = action;
		priv->user_data = user_data;
		priv->next_time = frog_get_relative_time() + priv->interval;
	}

	return thiz;
}

Ret frog_source_timer_reset(FrogSource* thiz)
{
	DECLES_PRIV(priv, thiz);

	thiz->disable = 0;
	priv->next_time = frog_get_relative_time() + priv->interval;

	return RET_OK;
}

Ret frog_source_timer_modify(FrogSource* thiz, int interval)
{
	DECLES_PRIV(priv, thiz);

	priv->interval = interval;
	priv->next_time = frog_get_relative_time() + priv->interval;

	return RET_OK;
}
