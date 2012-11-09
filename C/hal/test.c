#include "test.h"
#include "hardware.h"
#include "stdlib.h"
#include "unistd.h"

#define VERSION_MESSAGE "v1.2.1"

static int open_modules(const struct hw_module_t* module, const char* name, 
		struct hw_device_t** device);

static struct hw_module_methods_t test_module_methods = {
	.open = open_modules
};

static const char my_version[12] = "v1.2.1";

static int test_get_version(struct test_module_t* module, char** version)
{
	*version = my_version;

	return 0;
}

static int test_hello(struct test_device_t* device, const char* message)
{
	printf("message: %s\n", message);

	return 0;
}

const struct test_module_t HAL_MODULE_INFO_SYM = {
	.common = {
		.tag = HARDWARE_MODULE_TAG,
		.version_major = 1,
		.version_minor = 0,
		.id = TEST_MODULE_ID,
		.name = "zpcat",
		.author = "zxsu",
		.methods = &test_module_methods,
	},
	.get_version = test_get_version
};


/*******************************************/

static int open_modules(const struct hw_module_t* module, const char* name, 
		struct hw_device_t** device)
{
	if (!strcmp(name, TEST_HELLO)) {
		struct test_device_t* dev = malloc(sizeof(struct test_device_t));
		memset(dev, 0, sizeof(*dev));

		dev->common.tag = HARDWARE_DEVICE_TAG;
		dev->common.version = 0;
		dev->common.module = (struct hw_module_t*)module;
		dev->common.close = NULL;

		dev->hello = test_hello;

		*device = (struct hw_device_t*)dev;
	}

	return 0;
}
