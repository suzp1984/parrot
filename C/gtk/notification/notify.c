#include <libnotify/notify.h>
#include <glib.h>
#include <unistd.h>

#define CUSTOME_ICON "./xmpp.png"

int main(int argc, char* argv[])
{
	if (argc == 3)
	{
		NotifyNotification* n;
		notify_init("Test");

		n = notify_notification_new(argv[1], argv[2], CUSTOME_ICON, NULL);
		notify_notification_set_timeout(n, 1000); // 1 second

		if (!notify_notification_show(n, NULL)) {
			g_error("Failed to send notification");
			return 1;
		}

		g_object_unref(G_OBJECT(n));
	} else {
		g_print("Too few arguments (%d), 2 needed.\n", argc - 1);
	}

	return 0;
}
