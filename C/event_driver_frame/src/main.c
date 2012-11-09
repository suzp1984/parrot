#include "frog_main_loop.h"
#include "frog_sources_manager.h"
#include "frog_source_timer.h"
#include "typedef.h"

#include <stdio.h>

static Ret test_timer(void* user_data);

static Ret test_timer(void* user_data)
{
	printf("%s: timer.....\n", __func__);

	return RET_OK;
}

int main(int argc, char* argv[])
{
	FrogSourcesManager* sources_manager = frog_sources_manager_create(64);
	FrogMainLoop* main_loop = frog_main_loop_create(sources_manager);

	// add some sources ...
	FrogSource* timer_source = frog_source_timer_create(2000, test_timer, NULL);
	//frog_sources_manager_add(sources_manager, timer_source);
	frog_main_loop_add_source(main_loop, timer_source);

	// start loop
	frog_main_loop_run(main_loop);

	return 0;
}
