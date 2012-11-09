#ifndef COMMAND_H
#define COMMAND_H

#include "typedef.h"

struct _CommandInterface;
typedef struct _CommandInterface CommandInterface;

typedef Ret (*CommandExecute)(CommandInterface* thiz);
typedef void (*CommandDestroy)(CommandInterface* thiz);

struct _CommandInterface {
	CommandExecute execute;
	CommandDestroy destroy;

	char priv[1];
};

static inline Ret command_execute(CommandInterface* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->execute != NULL, RET_FAIL);

	return thiz->execute(thiz);
}

static inline void command_destroy(CommandInterface* thiz)
{
	return_if_fail(thiz != NULL && thiz->destroy != NULL);

	return thiz->destroy(thiz);
}

#endif // COMMAND_H
