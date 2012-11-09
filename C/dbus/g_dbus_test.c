#include <gdbus.h>

int main (int argc, char* argv[])
{
	DBusGConnection* connection;
	GError* error;
	DBusGProxy* proxy;
	char** name_list;
	char** name_list_ptr;

	g_type_init();
	 
	error = NULL;
	connection = dbus_g_bus_get(DBUS_BUS_SESSION,
}
