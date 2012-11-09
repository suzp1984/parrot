#ifndef CMD_INTERFACE_H
#define CMD_INTERFACE_H

#include "typedef.h"

DECLES_BEGIN

struct _CmdInterface;
typedef  struct _CmdInterface CmdInterface;

typedef int (*CmdInterfaceExecute)(CmdInterface* thiz, void* ctx);
typedef void (*CmdInterfaceDestroy)(CmdInterface* thiz);

struct _CmdInterface {
	CmdInterfaceExecute execute;
	CmdInterfaceDestroy destroy;

	int cmd;
	char priv[1];
};

static inline int cmd_interface_get_cmd(CmdInterface* thiz)
{
	return_val_if_fail(thiz != NULL, -1);

	return thiz->cmd;
}

static inline int cmd_interface_execute(CmdInterface* thiz, void* ctx)
{
	return_val_if_fail(thiz != NULL, -1);

	return thiz->execute(thiz, ctx);
}

static inline void cmd_interface_destroy(CmdInterface* thiz)
{
	return_if_fail(thiz != NULL);

	return thiz->destroy(thiz);
}

DECLES_END

#endif // CMD_INTERFACE_H
