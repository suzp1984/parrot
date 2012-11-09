#include "active_object_engine.h"
#include "command_interface.h"
#include "dlist.h"
#include "logger.h"

#include <stdlib.h>

struct _ActiveObjectEngine {
	DList* commands_list;	
};

static void commands_data_destroy(void* ctx, void* data) {
	return_if_fail(data != NULL);

	CommandInterface* command = (CommandInterface*)data;
	command_destroy(command);
}

ActiveObjectEngine* active_object_engine_create()
{
	ActiveObjectEngine* thiz = (ActiveObjectEngine*)malloc(sizeof(ActiveObjectEngine));

	if (thiz != NULL) {
		//LOG_INIT("user.log");
		thiz->commands_list = dlist_create(NULL, NULL);
		//printf("create commands_list\n");
		//LOG_INFO("create commands_list");
	}

	return thiz;
}

void active_object_engine_add_command(ActiveObjectEngine* thiz, CommandInterface* command)
{
	return_if_fail(thiz != NULL && thiz->commands_list != NULL && command != NULL);

	dlist_append(thiz->commands_list, command);
}

void active_object_engine_run(ActiveObjectEngine* thiz)
{
	return_if_fail(thiz != NULL);

	CommandInterface* command = NULL;
	while (dlist_length(thiz->commands_list) != 0) {
		command = NULL;
		dlist_get_by_index(thiz->commands_list, 0, (void**)&command);
		if (command != NULL) {
			command_execute(command);	
		}
		dlist_delete(thiz->commands_list, 0);
	}
}

void active_object_engine_destroy(ActiveObjectEngine* thiz)
{
	return_if_fail(thiz != NULL && thiz->commands_list != NULL);

	dlist_destroy(thiz->commands_list);
	
	//LOG_FINISH();
	thiz->commands_list = NULL;
	SAFE_FREE(thiz);
}
