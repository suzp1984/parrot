/*
 * File:    readline-main.c
 * Author:  zxsu <suzp1984@gmail.com>
 * Brief:   readline framework
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
 * 2013-10-20 12:18 zxsu <suzp1984@gmail.com> created.
 */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <glib.h>

#include "display.h"
#include "readline_engine_glib.h"
#include "help_cmd.h"
#include "quit_cmd.h"

int main(int argc, char* argv[])
{
    ReadlineEngine* rdengine = glib_readline_engine_create();
    readline_engine_init(rdengine);

    CmdInterface* help_cmd = help_cmd_create(rdengine);
    CmdInterface* quit_cmd = quit_cmd_create(rdengine);
  
    readline_engine_add_cmd(rdengine, help_cmd);
    readline_engine_add_cmd(rdengine, quit_cmd);

    readline_engine_run(rdengine);

    //    readline_engine_quit(rdengine);
    readline_engine_destroy(rdengine);

    return 0;
}

/*
int main(int argc, char* argv[])
{
	guint input;
    
	main_loop = g_main_loop_new(NULL, FALSE);
    rl_attempted_completion_function = client_completion;
    rl_erase_empty_line = 1;
	rl_callback_handler_install(NULL, rl_handler);
    
	rl_set_prompt("#");
	rl_redisplay();
    
	input = setup_standard_input();
    
	g_main_loop_run(main_loop);
    
	g_source_remove(input);
    
	rl_message("");
	rl_callback_handler_remove();
    
	g_main_loop_unref(main_loop);
    
    return 0;
}
*/
