/*
 * File:    readline-engine.h
 * Author:  zxsu <suzp1984@gmail.com>
 * Brief:   brief
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
 * 2013-10-20 12:22 zxsu <suzp1984@gmail.com> created.
 */

#ifndef _READLINE-ENGINE_H
#define _READLINE-ENGINE_H

#include "typedef.h"
#include "command_interface.h"

struct _ReadlineEngine;
typedef struct _ReadlineEngine ReadlineEngine;

typedef void (*RLEInit)(ReadlineEngine* thiz);
typedef void (*RLERun)(ReadlineEngine* thiz);
typedef Ret (*RLEAddCmd)(ReadlineEngine* thiz, CmdInterface* cmd);
typedef void (*RLEDestroy)(ReadlineEngine* thiz);

struct _ReadlineEngine {
    RLEInit init;
    RLERun run;
    RLEAddCmd add_cmd;
    RLEDestroy destroy;

    char priv[1];
};

static inline void readline_engine_init(ReadlineEngine* thiz)
{
    return_if_fail(thiz != NULL && thiz->init != NULL);

    thiz->init(thiz);
}

static inline void readline_engine_run(ReadlineEngine* thiz)
{
    return_if_fail(thiz != NULL && thiz->run != NULL);

    thiz->run(thiz);
}

static inline Ret readline_engine_add_cmd(ReadlineEngine* thiz, CmdInterface* cmd)
{
    return_val_if_fail(thiz != NULL && cmd != NULL && thiz->add_cmd != NULL, RET_INVALIAD_PARAM);

    return thiz->add_cmd(thiz, cmd);
}

static inline void readline_engine_destroy(ReadlineEngine* thiz)
{
    return_if_fail(thiz != NULL && thiz->destroy != NULL);

    thiz->destroy(thiz);
}

#endif /* _READLINE-ENGINE_H */
