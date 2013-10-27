#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <glib.h>

#include "display.h"

static GMainLoop *main_loop;

static void help_cmd(const char* arg)
{
    rl_printf("This is the Help cmd from %s\n", __func__);
}

static void quit_cmd(const char* arg)
{
    rl_printf("Quit the main_loop %s", __func__);
	g_main_loop_quit(main_loop);
}

static const struct {
    const char* cmd;
    const char* arg;
    void (*func)(const char* arg);
    const char* desc;
} cmd_table[] = {
    {"help", NULL, help_cmd, "Show help usage func"},
    {"quit", NULL, quit_cmd, "Quit the mainloop"},
    {}
};

static char** client_completion(const char* text, int start, int end)
{
    char** matches = NULL;

    if (start > 0) {
		int i;

		for (i = 0; cmd_table[i].cmd; i++) {
			if (strncmp(cmd_table[i].cmd,
					rl_line_buffer, start - 1))
				continue;
            
        }
    }
}

static void rl_handler(char* input)
{
	char *cmd, *arg;
	int i;

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

	for (i = 0; cmd_table[i].cmd; i++) {
		if (strcmp(cmd, cmd_table[i].cmd))
			continue;

		if (cmd_table[i].func) {
			cmd_table[i].func(arg);
			goto done;
		}
	}

	printf("Available commands:\n");

	for (i = 0; cmd_table[i].cmd; i++) {
		if (cmd_table[i].desc)
			printf("  %s %-*s %s\n", cmd_table[i].cmd,
					(int)(25 - strlen(cmd_table[i].cmd)),
					cmd_table[i].arg ? : "",
					cmd_table[i].desc ? : "");
	}

    
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
