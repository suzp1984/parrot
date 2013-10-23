/*
 * File:    command_interface.h
 * Author:  zxsu <suzp1984@gmail.com>
 * Brief:   command interface
 *
 * Copyright (c) zxsu
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2013-10-19 11:08 zxsu <suzp1984@gmail.com> created.
 */

#ifndef _COMMAND_INTERFACE_H
#define _COMMAND_INTERFACE_H

#include "typedef.h"

struct _CmdInterface;
typedef struct _CmdInterface CmdInterface;

typedef void (*CmdFunc)(CmdInterface* thiz, const char* arg);
typedef void (*CmdDestroy)(CmdInterface* thiz);

struct _CmdInterface {
    char* cmd;
    char* arg;
    char* desc;
    CmdFunc func;
    CmdDestroy destroy;
    
    char priv[1];
};

static inline void cmd_get_name(CmdInterface* thiz, char** cmd)
{
    return_if_fail(thiz != NULL && cmd != NULL);

    *cmd = thiz->cmd;
}

static inline void cmd_get_arg(CmdInterface* thiz, char** arg)
{
    return_if_fail(thiz != NULL && arg != NULL);

    *arg = thiz->arg;
}

static inline void cmd_get_desc(CmdInterface* thiz, char** desc)
{
    return_if_fail(thiz != NULL && desc != NULL);

    *desc = thiz->desc;
}

static inline void cmd_run(CmdInterface* thiz)
{
    return_if_fail(thiz != NULL && thiz->cmd != NULL);

    thiz->func(thiz, thiz->arg);
}

static inline void cmd_destroy(CmdInterface* thiz)
{
    return_if_fail(thiz != NULL && thiz->destroy != NULL);

    thiz->destroy(thiz);
}

#endif /* _COMMAND_INTERFACE_H */
