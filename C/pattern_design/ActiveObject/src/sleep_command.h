#ifndef SLEEP_COMMAND_H
#define SLEEP_COMMAND_H

#include "command_interface.h"
#include "active_object_engine.h"

CommandInterface* sleep_command_create(int milliseconds, ActiveObjectEngine* eng, CommandInterface* wakeupCommand);

#endif //SLEEP_COMMAND_H
