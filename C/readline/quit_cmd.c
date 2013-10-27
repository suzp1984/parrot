/*
 * File:    quit_cmd.c
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
 * 2013-10-23 23:11 zxsu <suzp1984@gmail.com> created.
 */

#include "quit_cmd.h"
#include "display.h"

#include <stdlib.h>

#define QUIT_CMD_NAME "quit"
#define QUIT_CMD_DESCRIPTION "quit readline loop"

typedef struct _PrivInfo {
    ReadlineEngine* engine;
} PrivInfo;

static void quit_cmd_run(CmdInterface* thiz, const char* arg)
{
    DECLES_PRIV(priv, thiz);
    readline_engine_quit(priv->engine);
}

static void quit_cmd_destroy(CmdInterface* thiz)
{
    if (thiz != NULL) {
        free(thiz);
    }
}

CmdInterface* quit_cmd_create(ReadlineEngine* engine)
{
    CmdInterface* thiz = (CmdInterface*)malloc(sizeof(CmdInterface) + sizeof(PrivInfo));

    if (thiz != NULL) {
        DECLES_PRIV(priv, thiz);
        thiz->func = quit_cmd_run;
        thiz->destroy = quit_cmd_destroy;
        thiz->cmd = QUIT_CMD_NAME;
        thiz->desc = QUIT_CMD_DESCRIPTION;
        thiz->arg = NULL;
        priv->engine = engine;
    }

    return thiz;
}
