#include "type_command.h"
#include <stdlib.h>

typedef struct {
	char* msg;
} PrivInfo;

static Ret type_command_execute(CommandInterface* thiz)
{
	DECLES_PRIV(priv, thiz);
	printf("msg: %s\n", priv->msg);

	return RET_OK;
}

static void type_command_destroy(CommandInterface* thiz)
{
	SAFE_FREE(thiz);
}

CommandInterface* type_command_create(char* msg)
{
	return_val_if_fail(msg != NULL, NULL);

	CommandInterface* thiz = (CommandInterface*) malloc(sizeof(CommandInterface) + sizeof(PrivInfo));

	if (thiz != NULL) {
		DECLES_PRIV(priv, thiz);
		thiz->execute = type_command_execute;
		thiz->destroy = type_command_destroy;

		priv->msg = msg;
	}

	return thiz;
}
