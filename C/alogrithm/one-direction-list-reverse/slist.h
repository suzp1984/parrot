/*
 * File:    slist.h
 * Author:  zxsu <suzp1984@gmail.com>
 * Brief:   one way direction list
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
 * 2013-08-19 22:05 zxsu <suzp1984@gmail.com> created.
 */

#ifndef _SLIST_H
#define _SLIST_H

#include "typedef.h"

#include <stddef.h>

DECLES_BEGIN

typedef void     (*SListDataDestroyFunc)(void* ctx, void* data);

struct _SList;
typedef struct _SList SList;

SList* slist_create(SListDataDestroyFunc func, void* ctx);
Ret slist_append(SList* thiz, void* data);
Ret slist_get_by_index(SList* thiz, size_t index, void** data);
size_t slist_get_count(SList* thiz);

Ret slist_reverse_recursion(SList* thiz);
Ret slist_reverse_stack(SList* thiz);

void slist_destroy(SList* thiz);

DECLES_END

#endif /* _SLIST_H */
