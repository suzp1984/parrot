/*
 * File:    logger.h<2>
 * Author:  zxsu <suzp1984@gmail.com>
 * Brief:   logger interface
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
 * 2013-07-06 12:00 zxsu <suzp1984@gmail.com> created.
 */

#ifndef _LOGGER_H
#define _LOGGER_H

#include <stdarg.h>

#include "typedef.h"

struct _Logger;
typedef struct _Logger Logger;

typedef void (*LoggerVerbose)(Logger* thiz, const char* format, va_list ap);
typedef void (*LoggerInfo)(Logger* thiz, const char* format, va_list ap);
typedef void (*LoggerDebug)(Logger* thiz, const char* format, va_list ap);
typedef void (*LoggerWarning)(Logger* thiz, const char* format, va_list ap);
typedef void (*LoggerError)(Logger* thiz, const char* format, va_list ap);

typedef void (*LoggerDestroy)(Logger* thiz);

struct _Logger {
   LoggerVerbose verbose;
   LoggerInfo info;
   LoggerDebug debug;
   LoggerWarning warning;
   LoggerError error;

   LoggerDestroy destroy;

   char priv[0];
};

static inline void logger_verbose(Logger* thiz, const char* format, ...)
{
    return_if_fail(thiz != NULL && thiz->verbose != NULL);
    
    va_list ap;
    va_start(ap, format);   

    thiz->verbose(thiz, format, ap);
    va_end(ap);
}

static inline void logger_info(Logger* thiz, const char* format, ...)
{
    return_if_fail(thiz != NULL && thiz->info != NULL);
    
    va_list ap;
    va_start(ap, format);

    thiz->info(thiz, format, ap);
    va_end(ap);
}

static inline void logger_debug(Logger* thiz, const char* format, ...)
{
    return_if_fail(thiz != NULL && thiz->debug != NULL);

    va_list ap;
    va_start(ap, format);

    thiz->debug(thiz, format, ap);
    va_end(ap);
}

static inline void logger_warning(Logger* thiz, const char* format, ...)
{
    return_if_fail(thiz != NULL && thiz->warning != NULL);

    va_list ap;
    va_start(ap, format);

    thiz->warning(thiz, format, ap);
    va_end(ap);
}

static inline void logger_error(Logger* thiz, const char* format, ...)
{
   return_if_fail(thiz != NULL && thiz->error != NULL);

    va_list ap;
    va_start(ap, format);

    thiz->error(thiz, format, ap);
    va_end(ap);
}

static inline void logger_destroy(Logger* thiz)
{
    return_if_fail(thiz != NULL && thiz->destroy != NULL);
    
    thiz->destroy(thiz);
}

#endif /* _LOGGER_H */
