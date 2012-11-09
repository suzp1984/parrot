#ifndef DELAYED_TYPER_COMMAND_H
#define DELAYED_TYPER_COMMAND_H

#include "command_interface.h"
#include "active_object_engine.h"

CommandInterface* delayed_typer_command_create(long milliseconds, char c, ActiveObjectEngine* eng);

#endif // DELAYED_TYPER_COMMAND_H
