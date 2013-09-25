
#include <glib.h>


int main(int argc, char* argv[])
{
    GMainLoop *event_loop;
    
    event_loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(event_loop);

    g_main_loop_unref(even_loop);

    return 0;
}
