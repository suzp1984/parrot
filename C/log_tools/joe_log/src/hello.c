#include "logger.h"

int main(int argc, char* argv[])
{
	LOG_INIT("user.log");

	LOG_DEBUG("hello world, debug");
	LOG_INFO("hello world, info %s", "abc");
	LOG_ERROR("hello world, error");

	LOG_FINISH();
	return 0;
}

