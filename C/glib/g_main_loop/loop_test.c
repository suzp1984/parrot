#include <glib.h>
#include <glib/gprintf.h>

static gboolean print_signal(void* var);

int main(int argc, char** argv)
{
	GMainLoop* loop;

	loop = g_main_loop_new(NULL, FALSE);

	g_timeout_add(1000, (GSourceFunc)print_signal, NULL);
	g_main_loop_run(loop);

	return 0;
}

static gboolean print_signal(void* var)
{
	g_printf("print_signal: \n");
	return TRUE;
}
