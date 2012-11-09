#include <errno.h>
#include <dbus/dbus.h>
#include <gdbus.h>

#define MY_SERVER "cn.cat"

int my_server_init() 
{
	DBusConnection* conn;
	DBusError err;

	dbus_error_init(&err);
	conn = g_dbus_setup_bus(DBUS_BUS_SYSTEM, MY_SERVER, &err);
	if (!conn) {
		if (dbus_error_is_set(&err)) {
			dbus_error_free(&err);
			return -EIO;
		}
		return -EALREADY;
	}

	
}

int main(int argc, char* argv[])
{
	DBusConnection* connection = NULL;
	DBusError err;

	dbus_error_init(&err);
	connection = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);

	if(my_server_init() < 0) {
	}

}
