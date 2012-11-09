#ifndef TEST_HARDWARE_MODULE_H
#define TEST_HARDWARE_MODULE_H

#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include "hardware.h"

__BEGIN_DECLS

#define TEST_MODULE_ID "test"

#define TEST_HELLO "hello"

struct test_module_t {
	struct hw_module_t common;

	int (*get_version)(struct test_module_t* module, char** version);
};

struct test_device_t {
	struct hw_device_t common;

	int (*hello)(struct test_device_t* dev, const char* message);
};

__END_DECLS

#endif // TEST_HARDWARE_MODULE_H
