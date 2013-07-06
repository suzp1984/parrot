/*
 * File:    logger_default.c
 * Author:  zxsu <suzp1984@gmail.com>
 * Brief:   brief
 *
 * Copyright (c) zxsu
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2013-07-06 13:57 zxsu <suzp1984@gmail.com> created.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "logger_default.h"

typedef struct _PrivInfo {
    char buffer[1024];
} PrivInfo;

static void logger_default_verbose(Logger* thiz, const char* format, va_list ap)
{
    PrivInfo* priv = (PrivInfo*)thiz->priv;
    
    memset(priv->buffer, 0, sizeof(priv->buffer));
    vsnprintf(priv->buffer, sizeof(priv->buffer), format, ap);

	printf("logger verbose: [%s]\n", priv->buffer);
}

static void logger_default_info(Logger* thiz, const char* format, va_list ap)
{
    PrivInfo* priv = (PrivInfo*)thiz->priv;
    
    memset(priv->buffer, 0, sizeof(priv->buffer));
    vsnprintf(priv->buffer, sizeof(priv->buffer), format, ap);

	printf("logger info: [%s]\n", priv->buffer);
}

static void logger_default_debug(Logger* thiz, const char* format, va_list ap)
{
    PrivInfo* priv = (PrivInfo*)thiz->priv;
    
    memset(priv->buffer, 0, sizeof(priv->buffer));
    vsnprintf(priv->buffer, sizeof(priv->buffer), format, ap);

	printf("logger debug: [%s]\n", priv->buffer);
}

static void logger_default_warning(Logger* thiz, const char* format, va_list ap)
{
    PrivInfo* priv = (PrivInfo*)thiz->priv;
    
    memset(priv->buffer, 0, sizeof(priv->buffer));
    vsnprintf(priv->buffer, sizeof(priv->buffer), format, ap);

	printf("logger warning: [%s]\n", priv->buffer);
}

static void logger_default_error(Logger* thiz, const char* format, va_list ap)
{
    PrivInfo* priv = (PrivInfo*)thiz->priv;
    
    memset(priv->buffer, 0, sizeof(priv->buffer));
    vsnprintf(priv->buffer, sizeof(priv->buffer), format, ap);

	printf("logger error: [%s]\n", priv->buffer);
}

static void logger_default_destroy(Logger* thiz)
{
    if (thiz != NULL) {
        free(thiz);
    }

    thiz = NULL;
}

Logger* logger_default_create()
{
    Logger* thiz = (Logger*)malloc(sizeof(Logger) + sizeof(PrivInfo));

    if (thiz != NULL)
    {
        PrivInfo* priv = (PrivInfo*)thiz->priv;
        memset(priv->buffer, 0, sizeof(priv->buffer));

        thiz->verbose = logger_default_verbose;
        thiz->info = logger_default_info;
        thiz->debug = logger_default_debug;
        thiz->warning = logger_default_warning;
        thiz->error = logger_default_error;
        thiz->logger_level = LOGGER_VERBOSE;

        thiz->destroy = logger_default_destroy;
    }

    return thiz;
}

#ifdef LOGGER_DEFAULT_TEST

#include "logger_default.h"

int main(int argc, char* argv[])
{
    Logger* logger = logger_default_create();
    logger_verbose(logger, "hello world");

    logger_set_level(logger, LOGGER_WARNING);
    logger_info(logger, "logger info");
    logger_error(logger, "logger error");
    logger_warning(logger, "logger warning");

    logger_destroy(logger);

    return 0;
}

#endif // LOGGER_DEFAULT_TEST
