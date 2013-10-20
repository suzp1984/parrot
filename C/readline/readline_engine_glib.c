/*
 * File:    readline_engine_glib.c
 * Author:  zxsu <suzp1984@gmail.com>
 * Brief:   readline powered by glib implementation
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
 * 2013-10-20 12:53 zxsu <suzp1984@gmail.com> created.
 */

#include "readline_engine_glib.h"
#include <stdlib.h>
#include <glib.h>

typedef struct _PrivInfo {
    GMainLoop *main_loop;
} PrivInfo;

static void glib_readline_init(ReadlineEngine* thiz)
{
}

static void glib_readline_run(ReadlineEngine* thiz)
{
}

static Ret glib_readline_add_cmd(ReadlineEngine* thiz, CmdInterface* cmd)
{
}

static void glib_readline_destroy(ReadlineEngine* thiz)
{
}

ReadlineEngine* glib_readline_engine_create()
{
    ReadlineEngine* thiz = (ReadlineEngine*)malloc(sizeof(ReadlineEngine) + sizeof(PrivInfo));
}
