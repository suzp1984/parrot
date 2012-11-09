#include "hardware.h"
#include "test.h"
#include <stdio.h>

#define MESSAGE "hello world"
int main(int argc, char* argv[])
{
	struct test_module_t* module;
	struct test_device_t* device;
	int err = 0;
	char* version;

	err = hw_get_module(TEST_MODULE_ID, (const struct hw_module_t**)&module);

	module->get_version(module, &version);

	printf("get version: %s\n", version);

	((struct hw_module_t*)module)->methods->open(module, TEST_HELLO, (struct hw_device_t**)&device);

	device->hello(device, version);

	return 0;
}
