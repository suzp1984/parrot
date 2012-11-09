/*
 * File: ftk_log.h    
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   some log functions.
 *
 * Copyright (c) 2009 - 2010  Li XianJing <xianjimli@hotmail.com>
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
 * 2009-10-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#ifndef FTK_LOG_H
#define FTK_LOG_H

//#include "ftk_typedef.h"

#include <stdarg.h>
//FTK_BEGIN_DECLS

typedef enum _Ret
{
	RET_OK,
	RET_FAIL,
	RET_REMOVE,
	RET_CONTINUE,
	RET_FOUND,
	RET_EOF,
	RET_NEXT,
	RET_QUIT,
	RET_EXIST,
	RET_AGAIN,
	RET_IGNORED,
	RET_NO_TARGET,
	RET_NOT_FOUND,
	RET_OUT_OF_SPACE
}Ret;

typedef enum _FtkLogLevel
{   
	FTK_LOG_V = 0,
	FTK_LOG_D = 1,
	FTK_LOG_I = 2,
	FTK_LOG_W = 3,
	FTK_LOG_E = 4
}FtkLogLevel;

static FtkLogLevel log_level = FTK_LOG_V;

void ftk_set_log_level(FtkLogLevel level);
FtkLogLevel ftk_default_log_level();

#if defined(__GNUC__)
Ret ftk_logv(const char* format, ...) __attribute__((format (printf, 1, 2)));
Ret ftk_logd(const char* format, ...) __attribute__((format (printf, 1, 2)));
Ret ftk_logi(const char* format, ...) __attribute__((format (printf, 1, 2)));
Ret ftk_logw(const char* format, ...) __attribute__((format (printf, 1, 2)));
Ret ftk_loge(const char* format, ...) __attribute__((format (printf, 1, 2)));
#else
Ret ftk_logv(const char* format, ...);
Ret ftk_logd(const char* format, ...);
Ret ftk_logi(const char* format, ...);
Ret ftk_logw(const char* format, ...);
Ret ftk_loge(const char* format, ...);
#endif /* __GNUC__ */

//FTK_END_DECLS

#endif/*FTK_LOG_H*/

