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

static guint setup_standard_input(void);
static char** client_completion(const char* text, int start, int end);
static void rl_handler(char* input);
static gboolean input_handler(GIOChannel *channel, GIOCondition condition,
                              gpointer user_data);
static guint setup_standard_input(void);

static ReadlineEngine* default_engine = NULL;
static GMainLoop *main_loop = NULL;

typedef struct _PrivInfo {
    GMainLoop *main_loop;
    guint input;
    CmdManager* cmd_manager;
} PrivInfo;

static char** client_completion(const char* text, int start, int end)
{
    //char** matches = NULL;
    
    if (start > 0) {
        //	int i;
        
        /*
		for (i = 0; cmd_table[i].cmd; i++) {
			if (strncmp(cmd_table[i].cmd,
                        rl_line_buffer, start - 1))
				continue;
            
        }
        */
    }
    
    return NULL;
}

static void rl_handler(char* input)
{
	char *cmd, *arg;
	int i;
    int count;
    CmdInterface* item_cmd;
    char* item_name;
    char* item_desc;

    if (!default_engine) {
        return;
    }

    DECLES_PRIV(priv, default_engine);
    count = cmd_manager_get_count(priv->cmd_manager);

	if (!input) {
		rl_insert_text("quit");
		rl_redisplay();
		rl_crlf();
		g_main_loop_quit(main_loop);
		return;
	}
    
	if (!strlen(input))
		goto done;
    
	add_history(input);
    
	cmd = strtok_r(input, " ", &arg);
	if (!cmd)
		goto done;
    
	if (arg) {
		int len = strlen(arg);
		if (len > 0 && arg[len - 1] == ' ')
			arg[len - 1] = '\0';
	}
    
    for (i = 0; i < count; i++)
    {
        cmd_manager_get(priv->cmd_manager, i, &item_cmd);
        cmd_get_name(item_cmd, &item_name);
        if (strcmp(cmd, item_name))
            continue;

        cmd_run(item_cmd);
        goto done;
    }
/*
	for (i = 0; cmd_table[i].cmd; i++) {
		if (strcmp(cmd, cmd_table[i].cmd))
			continue;
        
		if (cmd_table[i].func) {
			cmd_table[i].func(arg);
			goto done;
		}
	}
*/
  
	printf("Available commands:\n");

    for (i = 0; i < count; i++) {
        item_desc = NULL;
        cmd_manager_get(priv->cmd_manager, i, &item_cmd);
        cmd_get_desc(item_cmd, &item_desc);
        if (item_desc) {
            printf("  desc: %s\n", item_desc);
        }
    }
    
/*
	for (i = 0; cmd_table[i].cmd; i++) {
		if (cmd_table[i].desc)
			printf("  %s %-*s %s\n", cmd_table[i].cmd,
                   (int)(25 - strlen(cmd_table[i].cmd)),
                   cmd_table[i].arg ? : "",
                   cmd_table[i].desc ? : "");
	}
*/  
    
done:
	free(input);
}

static gboolean input_handler(GIOChannel *channel, GIOCondition condition,
                              gpointer user_data)
{
	if (condition & (G_IO_HUP | G_IO_ERR | G_IO_NVAL)) {
		g_main_loop_quit(main_loop);
		return FALSE;
	}
    
	rl_callback_read_char();
	return TRUE;
}

static guint setup_standard_input(void)
{
	GIOChannel *channel;
	guint source;
    
	channel = g_io_channel_unix_new(fileno(stdin));
    
	source = g_io_add_watch(channel,
                            G_IO_IN | G_IO_HUP | G_IO_ERR | G_IO_NVAL,
                            input_handler, NULL);
    
	g_io_channel_unref(channel);
    
	return source;
}


static void glib_readline_init(ReadlineEngine* thiz)
{
    DECLES_PRIV(priv, thiz);
    return_if_fail(priv->main_loop != NULL);

    rl_attempted_completion_function = client_completion;
    rl_erase_empty_line = 1;
	rl_callback_handler_install(NULL, rl_handler);
    
	rl_set_prompt("# ");
	rl_redisplay();
    
    priv->input = setup_standard_input();

    
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

    g_source_remove(priv->input);

	rl_message("");
	rl_callback_handler_remove();

//	g_main_loop_unref(priv->main_loop);

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
    if (default_engine != NULL) {
        return default_engine;
    }

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
        default_engine = thiz;
    }
    
    return thiz;
}
