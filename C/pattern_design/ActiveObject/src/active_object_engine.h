#ifndef ACTIVE_OBJECT_ENGINE_H
#define ACTIVE_OBJECT_ENGINE_H

#include "command_interface.h"

struct _ActiveObjectEngine;
typedef struct _ActiveObjectEngine ActiveObjectEngine;

ActiveObjectEngine* active_object_engine_create();
void active_object_engine_add_command(ActiveObjectEngine* thiz, CommandInterface* command);
void active_object_engine_run(ActiveObjectEngine* thiz);
void active_object_engine_destroy(ActiveObjectEngine* thiz);

#endif //ACTIVE_OBJECT_ENGINE_H
