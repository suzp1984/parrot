#include "frog_sources_manager.h"
#include "frog_source_primary.h"

struct _FrogSourcesManager {
	int source_nr;
	int need_refresh;
	int max_source_nr;
	FrogSource* primary_source;
	FrogSource* sources[1];
};

static Ret frog_source_primary_on_event(void* user_data, FrogEvent* event);

static Ret frog_source_primary_on_event(void* user_data, FrogEvent* event)
{
	return_val_if_fail(user_data != NULL && event != NULL, RET_INVALID_PARAMS);
	FrogSourcesManager* manager = (FrogSourcesManager*)user_data;

	switch(event->type) 
	{
		case FROG_EVT_ADD_SOURCE:
		{
			frog_sources_manager_add(manager, (FrogSource*)(event->u.extra));
			break;
		}
		case FROG_EVT_REMOVE_SOURCE:
		{
			frog_sources_manager_remove(manager, (FrogSource*)(event->u.extra));
			break;
		}
		default:
		{
			break;
		}
	}

	return RET_OK;
}

FrogSourcesManager* frog_sources_manager_create(int max_sources)
{
	return_val_if_fail(max_sources > 0, NULL);

	FrogSourcesManager* thiz = (FrogSourcesManager*) malloc(sizeof(FrogSourcesManager) + 
			max_sources * sizeof(FrogSource));

	if (thiz != NULL) {
		thiz->source_nr = 0;
		thiz->max_source_nr = max_sources;
		FrogSource* primary_source = frog_source_primary_create(frog_source_primary_on_event, (void*)thiz);
		frog_sources_manager_add(thiz, primary_source);
		thiz->primary_source = primary_source;
		frog_source_ref(primary_source);
	}

	return thiz;
}

Ret frog_sources_manager_add(FrogSourcesManager* thiz, FrogSource* source)
{
	return_val_if_fail(thiz != NULL && source != NULL, RET_INVALID_PARAMS);

	if (thiz->source_nr + 1 < thiz->max_source_nr) {
		thiz->sources[thiz->source_nr] = source;
		thiz->source_nr++;
	} else {
		return RET_FAIL;
	}

	frog_sources_manager_set_need_refresh(thiz);
	return RET_OK;
}

Ret frog_sources_manager_remove(FrogSourcesManager* thiz, FrogSource* source)
{
	return_val_if_fail(thiz != NULL && source != NULL, RET_INVALID_PARAMS);

	int i = 0;

	for (i = 0; i < thiz->source_nr; i++) {
		if (thiz->sources[i] == source) {
			break;	
		}
	}

	if (i < thiz->source_nr) {
		for (; (i + 1) < thiz->source_nr; i++) {
			thiz->sources[i] = thiz->sources[i+1];
		}
		thiz->source_nr--;
		thiz->sources[thiz->source_nr] = NULL;
		source->active = 0;
		frog_source_unref(source);
	}
	
	frog_sources_manager_set_need_refresh(thiz);

	return RET_OK;
}

int frog_sources_manager_get_count(FrogSourcesManager* thiz)
{
	return_val_if_fail(thiz != NULL, 0);

	return thiz->source_nr;
}

FrogSource* frog_sources_manager_get(FrogSourcesManager* thiz, int i)
{
	return_val_if_fail(thiz != NULL && i < thiz->source_nr, NULL);

	return thiz->sources[i];
}

int frog_sources_manager_need_refresh(FrogSourcesManager* thiz)
{
	return_val_if_fail(thiz != NULL, 0);
	int ret = 0;

	if (thiz->need_refresh > 0) {
		ret = thiz->need_refresh;
		thiz->need_refresh--;
	}

	return ret;
}

Ret frog_sources_manager_set_need_refresh(FrogSourcesManager* thiz)
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	thiz->need_refresh++;
	return RET_OK;
}

void frog_sources_manager_destroy(FrogSourcesManager* thiz)
{
	int i = 0;

	if (thiz != NULL) {
		for (i = 0; i < thiz->source_nr; i++) {
			frog_source_unref(thiz->sources[i]);
			thiz->sources[i] = NULL;
		}
		
		frog_source_unref(thiz->primary_source);
	}

	SAFE_FREE(thiz);
	return;
}

FrogSource* frog_sources_manager_get_primary_source(FrogSourcesManager* thiz)
{
	return_val_if_fail(thiz != NULL, NULL);

	return thiz->primary_source;
}
