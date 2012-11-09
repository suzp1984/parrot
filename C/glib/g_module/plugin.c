#include <glib.h>
#include <gmodule.h>

#include <stdio.h>
#include "operation.h"

/*
void g_module_check_init()
{
	g_print("Plugin works!");
} */

static int plugin_add(int a, int b)
{
	return a + b;
}

static void plugin_hello(void)
{
	printf("Hello world!\n");
}

const Operation ope = {
	.add = plugin_add,
	.hello = plugin_hello,
}; 


