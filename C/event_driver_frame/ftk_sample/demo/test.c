#include "ftk.h"
#include "fbus_service.h"
#include "ftk_source_primary.h"
#include "fbus_source_listen.h"
#include "fbus_service_manager.h"
#include "ftk_allocator_default.h"

Ret ftk_demo_init(int argc, char* argv[])
{
	ftk_platform_init(argc, argv);

#ifndef USE_STD_MALLOC
	ftk_set_allocator((ftk_allocator_default_create()));
#endif  
	ftk_set_sources_manager(ftk_sources_manager_create(64));
	ftk_set_main_loop(ftk_main_loop_create(ftk_default_sources_manager()));
	ftk_set_primary_source(ftk_source_primary_create(NULL, NULL));
	ftk_sources_manager_add(ftk_default_sources_manager(), ftk_primary_source());

	return RET_OK;
}

Ret fbus_service_run(void)
{
	return ftk_run();
}

void fbus_service_quit(void)
{
	ftk_quit();

	return;
}

