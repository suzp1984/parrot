#include "frog_main_loop.h"

#include <sys/select.h>
#include <stdlib.h>

#include "frog_event.h"
#include "frog_source_primary.h"

struct _FrogMainLoop {
	int running;
	fd_set fdset;
	fd_set err_fdset;
	FrogSourcesManager* manager;
};

FrogMainLoop* frog_main_loop_create(FrogSourcesManager* sources_manager)
{
	return_val_if_fail(sources_manager != NULL, NULL);
	FrogMainLoop* thiz = (FrogMainLoop*) malloc(sizeof(FrogMainLoop));

	if (thiz != NULL) {
		FD_ZERO(&thiz->fdset);
		FD_ZERO(&thiz->err_fdset);
		thiz->running = 0;
		thiz->manager = sources_manager;
	}

	return thiz;
}

Ret frog_main_loop_run(FrogMainLoop* thiz)
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	int i = 0;
	int n = 0;
	int fd = -1;
	int mfd = -1;
	int wait_time = 3600 * 3000;
	int source_wait_time = 0;
	int ret = 0;
	struct timeval tv = {0};
	FrogSource* source = NULL;

	thiz->running = 1;
	while (thiz->running) {
		FD_ZERO(&thiz->fdset);
		FD_ZERO(&thiz->err_fdset);

		for (i = 0; i < frog_sources_manager_get_count(thiz->manager); i++) {
			source = frog_sources_manager_get(thiz->manager, i);
			if ((fd = frog_source_get_fd(source)) >= 0) {
				FD_SET(fd, &thiz->fdset);
				FD_SET(fd, &thiz->err_fdset);
				if (fd > mfd) mfd = fd;
				++n;
			}

			source_wait_time = frog_source_check(source);
			if (source_wait_time >= 0 && source_wait_time < wait_time) {
				wait_time = source_wait_time;
			}
		}

		tv.tv_sec = wait_time/1000;
		tv.tv_usec = (wait_time%1000)*1000;

		ret = select(mfd + 1, &thiz->fdset, NULL, &thiz->err_fdset,	&tv);

		for (i = 0; i < frog_sources_manager_get_count(thiz->manager); ) {
			if (frog_sources_manager_need_refresh(thiz->manager)) {
				break;
			}

			source = frog_sources_manager_get(thiz->manager, i);

			if (source->disable > 0) {
				frog_sources_manager_remove(thiz->manager, source);
				continue;
			}

			if ((fd = frog_source_get_fd(source)) >= 0 && ret != 0) {
				if (ret > 0 && FD_ISSET(fd, &thiz->fdset)) {
					if (frog_source_dispatch(source) != RET_OK) {
						frog_sources_manager_remove(thiz->manager, source);
					} else {
						++i;
					}

					continue;
				} else if (FD_ISSET(fd, &thiz->err_fdset)) {
					frog_sources_manager_remove(thiz->manager, source);
					continue;
				}
			}

			if ((source_wait_time = frog_source_check(source)) == 0) {
				if (frog_source_dispatch(source) != RET_OK) {
					frog_sources_manager_remove(thiz->manager, source);
				} else {
					++i;
				}
				continue;
			}

			++i;
		}
	}

	return RET_OK;
}

Ret frog_main_loop_quit(FrogMainLoop* thiz)
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	thiz->running = 0;
	return RET_OK;
}

Ret frog_main_loop_add_source(FrogMainLoop* thiz, FrogSource* source)
{
	return_val_if_fail(thiz != NULL && source != NULL, RET_INVALID_PARAMS);
	FrogEvent event;

	frog_event_init(&event, FROG_EVT_ADD_SOURCE);
	event.u.extra = source;

	source->active = 1;
	frog_source_enable(source);

	return frog_source_queue_event(
			frog_sources_manager_get_primary_source(thiz->manager), &event);
}

Ret frog_main_loop_remove_source(FrogMainLoop* thiz, FrogSource* source)
{
	return_val_if_fail(thiz != NULL && source != NULL, RET_INVALID_PARAMS);
	FrogEvent event;

	frog_event_init(&event, FROG_EVT_REMOVE_SOURCE);
	event.u.extra = source;

	frog_source_disable(source);
	return frog_source_queue_event(
			frog_sources_manager_get_primary_source(thiz->manager), &event);
}

void frog_main_loop_destroy(FrogMainLoop* thiz)
{
	SAFE_FREE(thiz);
}
