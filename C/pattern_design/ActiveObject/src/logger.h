#ifndef H_LOGGER_H
#define H_LOGGER_H

#ifdef USE_LOG4C

#include <log4c.h>

extern int init_userloc_formatters();
static log4c_category_t* mycat = NULL;

#define LOG_INIT(category) \
{ \
	init_userloc_formatters(); \
	log4c_init(); \
	mycat = log4c_category_get(category); \
}

#define LOG_INFO(msg, args...) \
{ \
	const log4c_location_info_t locinfo = \
		LOG4C_LOCATION_INFO_INITIALIZER(NULL); \
	log4c_category_log_locinfo(mycat, &locinfo, \
			LOG4C_PRIORITY_INFO, msg, ##args); \
}

#define LOG_DEBUG(msg, args...) \
{ \
	const log4c_location_info_t locinfo = \
		LOG4C_LOCATION_INFO_INITIALIZER(NULL); \
	log4c_category_log_locinfo(mycat, &locinfo, \
			LOG4C_PRIORITY_DEBUG, msg, ##args); \
}

#define LOG_WARN(msg, args...) \
{ \
	const log4c_location_info_t locinfo = \
		LOG4C_LOCATION_INFO_INITIALIZER(NULL); \
	log4c_category_log_locinfo(mycat, &locinfo, \
			LOG4C_PRIORITY_WARN, msg, ##args); \
}

#define LOG_ERROR(msg, args...) \
{ \
	const log4c_location_info_t locinfo = \
		LOG4C_LOCATION_INFO_INITIALIZER(NULL); \
	log4c_category_log_locinfo(mycat, &locinfo, \
			LOG4C_PRIORITY_ERROR, msg, ##args); \
}

#define LOG_FINISH() \
{ \
	log4c_fini(); \
}

#else 

#include <stdio.h>

static char* mycat = NULL;

#define LOG_INIT(category) { mycat = category; }

#define LOG_INFO(msg, args...) \
{ \
	printf("[INFO] %s, "msg"\n", mycat, ##args); \
} 

#define LOG_DEBUG(msg, args...) \
{ \
	printf("[DEBUG] %s, "msg"\n", mycat, ##args); \
}

#define LOG_WARN(msg, args...) \
{ \
	printf("[WARN] %s, "msg"\n", mycat, ##args); \
}

#define LOG_ERROR(msg, args...) \
{ \
	printf("[ERROR] %s, "msg"\n", mycat, ##args); \
} 

#define LOG_FINISH()

#endif // USE_LOG4C

#endif // H_LOGGER_H
