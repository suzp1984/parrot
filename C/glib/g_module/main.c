#include <glib.h>
#include <gmodule.h>
#include "operation.h"

#define PLUGIN_NAME "libplugin.so"

int main(int argc, char* argv[])
{
	GModule* mod = NULL;
	Operation* opt = NULL;

	if (!g_module_supported())
	{
		g_error("g_module_supported fail");
		return -1;
	}

	if((mod = g_module_open(PLUGIN_NAME, G_MODULE_BIND_LAZY)) == NULL) {
		g_error(g_module_error());
		return -1;
	}

	if (!g_module_symbol(mod, "ope", (gpointer*)&opt)) {
		g_error(g_module_error());
		return -1;
	}

	g_print("add 1 , 2 is %d\n", opt->add(1, 2));
	opt->hello();

	g_module_close(mod);

	return 0;
}
