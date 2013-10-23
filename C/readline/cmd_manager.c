/*
 * File:    cmd_manager.c
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
 * 2013-10-23 22:32 zxsu <suzp1984@gmail.com> created.
 */

#include "cmd_manager.h"

#include <stdlib.h>
#include <dlist.h>

struct _CmdManager {
    DList* cmd_pool;
};

CmdManager* cmd_manager_create()
{
    CmdManager* thiz = (CmdManager*)malloc(sizeof(CmdManager));
    
    if (thiz != NULL) {
        thiz->cmd_pool = dlist_create(NULL, NULL, NULL);
    }

    return thiz;
}

Ret cmd_manager_add_cmd(CmdManager* thiz, CmdInterface* cmd)
{
    return_val_if_fail(thiz != NULL && cmd != NULL, RET_INVALIAD_PARAM);

    dlist_append(thiz->cmd_pool, cmd);

    return RET_OK;
}

size_t cmd_manager_get_count(CmdManager* thiz)
{
    return_val_if_fail(thiz != NULL);

    return dlist_length(thiz->cmd_pool);
}

Ret cmd_manager_get(CmdManager* thiz, size_t index, CmdInterface** cmd)
{
    return_val_if_fail(thiz != NULL && index >= 0 && cmd != NULL, RET_INVALIAD_PARAM);
    
    dlist_get_by_index(thiz->cmd_pool, index, cmd);

    return RET_OK;
}

void cmd_manager_destroy(CmdManager* thiz)
{
    return_if_fail(thiz != NULL);
    
    if (thiz->cmd_pool) {
        dlist_destroy(thiz->cmd_pool);
    }

    free(thiz);
}
