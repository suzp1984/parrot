
#include <glib.h>


int main(int argc, char* argv[])
{
    GMainLoop *event_loop;
    int sockfd;
    GIOChannel* io;
// unix socket defination here
    
    event_loop = g_main_loop_new(NULL, FALSE);
    io = g_io_channel_unix_new(sockfd);
    g_io_channel_set_close_on_unref(io, TRUE);
    g_io_channel_unref(io);

    g_main_loop_run(event_loop);

    g_main_loop_unref(even_loop);

    return 0;
}
