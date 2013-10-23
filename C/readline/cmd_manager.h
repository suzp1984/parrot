/*
 * File:    cmd_manager.h
 * Author:  zxsu <suzp1984@gmail.com>
 * Brief:   cmd manager header file
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

#ifndef _CMD_MANAGER_H
#define _CMD_MANAGER_H

#include "typedef.h"
#include "command_interface.h"

struct _CmdManager;
typedef struct _CmdManager CmdManager;

CmdManager* cmd_manager_create();
Ret cmd_manager_add_cmd(CmdManager* thiz, CmdInterface* cmd);
size_t cmd_manager_get_count(CmdManager* thiz);
Ret cmd_manager_get(CmdManager* thiz, size_t index, CmdInterface** cmd);

void cmd_manager_destroy(CmdManager* thiz);

#endif /* _CMD_MANAGER_H */
