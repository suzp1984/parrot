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
#include <readline/readline.h>
#include <readline/history.h>
#include "display.h"
#include "cmd_manager.h"

static GMainLoop *main_loop;

typedef struct _PrivInfo {
    GMainLoop *main_loop;
    guint input;
    CmdManager* cmd_manager;
} PrivInfo;

static void internel_help_cmd(const char* arg)
{
    rl_printf("This is the Help cmd from %s\n", __func__);
}

static void internel_quit_cmd(const char* arg)
{
    rl_printf("Quit the main_loop %s", __func__);
	g_main_loop_quit(main_loop);
}

static void glib_readline_init(ReadlineEngine* thiz)
{
    DECLES_PRIV(priv, thiz);
    return_if_fail(priv->main_loop != NULL);

    rl_attempted_completion_function = client_completion;
}

static void glib_readline_run(ReadlineEngine* thiz)
{
    DECLES_PRIV(priv, thiz);

    if (priv->main_loop) {
        g_main_loop_run(priv->main_loop);
    }
}

static Ret glib_readline_add_cmd(ReadlineEngine* thiz, CmdInterface* cmd)
{
    DECLES_PRIV(priv, thiz);

    return cmd_manager_add_cmd(priv->cmd_manager, cmd);
}

static Ret glib_readline_quit(ReadlineEngine* thiz)
{
    DECLES_PRIV(priv, thiz);
    g_main_loop_quit(priv->main_loop);

    return RET_OK;
}

static void glib_readline_destroy(ReadlineEngine* thiz)
{
    DECLES_PRIV(priv, thiz);

    g_main_loop_unref(priv->main_loop);    
    cmd_manager_destroy(priv->cmd_manager);
    free(thiz);
}

ReadlineEngine* glib_readline_engine_create()
{
    ReadlineEngine* thiz = (ReadlineEngine*)malloc(sizeof(ReadlineEngine) + sizeof(PrivInfo));

    if (thiz != NULL) {
        DECLES_PRIV(priv, thiz);
        thiz->init = glib_readline_init;
        thiz->add_cmd = glib_readline_add_cmd;
        thiz->run = glib_readline_run;
        thiz->quit = glib_readline_quit;

        thiz->destroy = glib_readline_destroy;

        priv->main_loop = g_main_loop_new(NULL, FALSE);
        priv->cmd_manager = cmd_manager_create();
        main_loop = priv->main_loop;
    }

    return thiz;
}
