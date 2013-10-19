/*
 * File:    help_cmd.c
 * Author:  zxsu <suzp1984@gmail.com>
 * Brief:   help command
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
 * 2013-10-19 11:26 zxsu <suzp1984@gmail.com> created.
 */

#include "help_cmd.h"
#include "display.h"

#include <stdlib.h>

#define HELP_NAME "help"
#define HELP_DESCRIPTION "Show help usage func"

static void help_cmd_run(CmdInterface* thiz, const char* arg)
{
    rl_printf("This is the Help cmd from %s\n", __func__);
}

static void help_cmd_destroy(CmdInterface* thiz)
{
    if (thiz != NULL) {
        free(thiz);
    }

    return;
}

CmdInterface* help_cmd_create()
{
    CmdInterface* thiz = (CmdInterface*)malloc(sizeof(CmdInterface));

    if (thiz != NULL) {
        thiz->cmd = HELP_NAME;
        thiz->desc = HELP_DESCRIPTION;
        thiz->arg = NULL;
        thiz->func = help_cmd_run;
        thiz->destroy = help_cmd_destroy;
    }

    return thiz;
}
