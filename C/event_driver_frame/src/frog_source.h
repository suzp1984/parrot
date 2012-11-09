#ifndef FROG_SOURCE_H
#define FROG_SOURCE_H

#include "typedef.h"

DECLES_BEGIN

struct _FrogSource;
typedef struct _FrogSource FrogSource;

typedef int (*FrogSourceGetFd)(FrogSource* thiz);
typedef int (*FrogSourceCheck)(FrogSource* thiz);
typedef Ret (*FrogSourceDispatch)(FrogSource* thiz);
typedef void (*FrogSourceDestroy)(FrogSource* thiz);

struct _FrogSource {
	FrogSourceGetFd getfd;
	FrogSourceCheck check;
	FrogSourceDispatch dispatch;
	FrogSourceDestroy destroy;

	int ref;
	int active;
	int disable;
	char priv[1];
};

static inline int frog_source_get_fd(FrogSource* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->getfd != NULL, -1);

	return thiz->getfd(thiz);
}

static inline int frog_source_check(FrogSource* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->check != NULL, -1);

	return thiz->check(thiz);
}

static inline Ret frog_source_dispatch(FrogSource* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->dispatch != NULL, RET_FAIL);

	return thiz->dispatch(thiz);
}

static inline void frog_source_destory(FrogSource* thiz)
{
	return_if_fail(thiz != NULL && thiz->destroy != NULL);

	return thiz->destroy(thiz);
}

static inline Ret frog_source_disable(FrogSource* thiz)
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	thiz->disable++;
	return RET_OK;
}

static inline Ret frog_source_enable(FrogSource* thiz)
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	if (thiz->disable > 0) {
		thiz->disable--;
	} else {
		thiz->disable = 0;
	}

	return RET_OK;
}

static inline void frog_source_ref(FrogSource* thiz)
{
	return_if_fail(thiz != NULL);
	
	thiz->ref ++;

	return;
}

static inline void frog_source_unref(FrogSource* thiz)
{
	return_if_fail(thiz != NULL);

	thiz->ref--;
	if (thiz->ref <= 0) {
		frog_source_destory(thiz);
	}

	return;
}

DECLES_END

#endif // FROG_SOURCE_H
