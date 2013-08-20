/*
 * File:    slist.c
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
 * 2013-08-19 22:06 zxsu <suzp1984@gmail.com> created.
 */

#include "slist.h"

#include <stdlib.h>

struct SListNode {
    void* data;
    struct SListNode* next;
};

typedef struct SListNode SListNode;

struct _SList 
{
    SListNode* header;
    SListDataDestroyFunc destroy_func;
    void* ctx;
};

static SListNode* slist_node_create(void* data);
static void slist_node_destroy(SList* thiz, SListNode* node);
static SListNode* _slist_node_reverse_recu(SListNode* node);
static SListNode* _slist_node_append(SListNode* ahead, SListNode* tail);

static SListNode* slist_node_create(void* data)
{
    SListNode* node = (SListNode*)malloc(sizeof(SListNode));
    
    if (node != NULL) {
        node->data = data;
        node->next = NULL;
    }

    return node;
}

static void slist_node_destroy(SList* thiz, SListNode* node)
{
    if (node != NULL) {
        if (thiz != NULL && thiz->destroy_func != NULL) {
            thiz->destroy_func(thiz->ctx, node->data);
        }

        free(node);
    }
}

SList* slist_create(SListDataDestroyFunc func, void* ctx)
{
    SList* thiz = (SList*)malloc(sizeof(SList));
    
    if (thiz != NULL) {
        thiz->header = NULL;
        thiz->destroy_func = func;
        thiz->ctx = ctx;
    }

    return thiz;
}

Ret slist_append(SList* thiz, void* data)
{
    if (thiz == NULL || data == NULL) {
        return RET_FAIL;
    }

    SListNode* node = slist_node_create(data);
    SListNode* tail = thiz->header;
    
    if (tail == NULL) {
        thiz->header = node;
        return RET_OK;
    }

    while(tail->next) {
        tail = tail->next;
    }

    tail->next = node;

    return RET_OK;
}

Ret slist_get_by_index(SList* thiz, size_t index, void** data)
{
    if (thiz == NULL) {
        return RET_FAIL;
    }

    size_t n = slist_get_count(thiz);
    if (index >= n) {
        return RET_INVALID_PARAMS;
    }

    if (data == NULL) {
        return RET_INVALID_PARAMS;
    }

    size_t i = 0;
    SListNode* node = thiz->header;

    while(i < index) {
        node = node->next;
        i++;
    }

    *data = node->data;

    return RET_OK;
}

size_t slist_get_count(SList* thiz)
{
    if (thiz == NULL) {
        return 0;
    }
    size_t n = 0;
    SListNode* node = thiz->header;

    while(node) {
        node = node->next;
        n++;
    }
    
    return n;
}

void slist_destroy(SList* thiz)
{
    if (thiz != NULL) {
        SListNode* node = thiz->header;
        SListNode* next;

        while(node != NULL) {
            next = node->next;
            slist_node_destroy(thiz, node);
            node = next;
        }
        
        free(thiz);
    }
}

static SListNode* _slist_node_append(SListNode* ahead, SListNode* tail)
{
    SListNode* ahead_tail = ahead;
    while (ahead_tail->next) {
        ahead_tail = ahead_tail->next;
    }
    
    ahead_tail->next = tail;

    return ahead;
}

static SListNode* _slist_node_reverse_recu(SListNode* node)
{
    SListNode* cdr = node->next;
    SListNode* tmp = NULL;
    
    if (cdr != NULL) {
        tmp = _slist_node_reverse_recu(cdr);
        node->next = NULL;
        return (_slist_node_append(tmp, node));
    } else {
        return node;
    }
}

Ret slist_reverse_recursion(SList* thiz)
{
    thiz->header = _slist_node_reverse_recu(thiz->header);
    
    return RET_OK;
}

Ret slist_reverse_stack(SList* thiz)
{
    return RET_OK;
}
