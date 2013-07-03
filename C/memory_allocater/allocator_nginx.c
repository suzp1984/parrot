/*
 * File:    allocator_nginx.c
 * Author:  zxsu <suzp1984@gmail.com>
 * Brief:   memeory allocator inspired by ngnix
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
 * 2013-07-03 12:00 zxsu <suzp1984@gmail.com> created.
 */

#include <stdlib.h>
#include <string.h>

#include "allocator_nginx.h"

struct _ngx_pool_data_t;
typedef struct _ngx_pool_data_t ngx_pool_data_t;

struct _ngx_pool_data_t {
//  u_char* alloc;
    u_char* last;
    u_char* end;
    ngx_pool_data_t* next;
    int            failed;
};

struct _ngx_pool_large_t;
typedef struct _ngx_pool_large_t ngx_pool_large_t;

struct _ngx_pool_large_t {
    ngx_pool_large_t     *next;
    void                 *alloc;
};

typedef struct {
    ngx_pool_data_t*       d;
    size_t                max;
    ngx_pool_data_t      *current;
    ngx_pool_large_t     *large;
} ngx_pool_t;

typedef struct _PrivInfo {
    ngx_pool_t* pool;
} PrivInfo;

static void* allocator_nginx_alloc_block(Allocator* thiz, size_t size)
{
    PrivInfo* priv = (PrivInfo*)thiz->priv;
    ngx_pool_t* pool = priv->pool;
    ngx_pool_data_t* pd;
    ngx_pool_data_t* p;
    ngx_pool_data_t* current;
    char* m;
    
    return_val_if_fail(size <= pool->max, NULL);
    
    pd = (ngx_pool_data_t*)malloc(sizeof(ngx_pool_data_t) + pool->max);
    
    if (pd != NULL) {
        m = (void*)pd + sizeof(ngx_pool_data_t);
        pd->next = NULL;
        pd->last = (void*)m + size;
        pd->end = (void*)m + pool->max;
        pd->failed = 0;
    } else {
        return NULL;
    }
    
    current = pool->current;
    
    for(p = pool->current; p->next; p = p->next) {
        if (p->failed++ > 4) {
            current = p->next;
        }
    }
    p->next = pd;
    pool->current = current ? current : pd;
    
    return m;
}

static void* allocator_nginx_alloc_large(Allocator* thiz, size_t size)
{
    PrivInfo* priv = (PrivInfo*)thiz->priv;
    ngx_pool_t* pool = priv->pool;
    void* p;
    ngx_pool_large_t* large;
    int n = 0;

    p = malloc(size);
    if (p == NULL) {
        return NULL;
    }

    for(large = pool->large; large; large = large->next) {
        if (large->alloc == NULL) {
            large->alloc = p;
            return p;
        }
        
        if(n++ > 3) {
            break;
        }
    }
    
    large = allocator_alloc(thiz, sizeof(ngx_pool_large_t));
    if (large == NULL) {
        free(p);
        return NULL;
    }
    
    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}

static void* allocator_nginx_alloc(Allocator* thiz, size_t size)
{
    PrivInfo* priv = (PrivInfo*)thiz->priv;
    ngx_pool_t* pool = priv->pool;
    ngx_pool_data_t* pd;
    u_char* m;

    if (size <= pool->max) {
        pd = pool->current;
        do {
            m = pd->last;
            if ((size_t)(pd->end - m) >= size) {
                pd->last = m + size;
                return m;
            }
            pd = pd->next;
        } while(pd);
        
        return allocator_nginx_alloc_block(thiz, size);
    }

    return allocator_nginx_alloc_large(thiz, size);
}

static void* allocator_nginx_calloc(Allocator* thiz, size_t nmemb, size_t size)
{
    void* p = allocator_alloc(thiz, (nmemb * size));
    if (p) {
        memset(p, 0, (nmemb * size));
    }
    
    return p;
}

static void allocator_nginx_free(Allocator* thiz, void* ptr)
{
    PrivInfo* priv = (PrivInfo*)thiz->priv;
    ngx_pool_large_t* large = priv->pool->large;
    
    return_if_fail(ptr != NULL);

    for (large = priv->pool->large; large; large = large->next) {
        if (ptr == large->alloc) {
            SAFE_FREE(ptr);
            large->alloc = NULL;
            return;
        }
    }
}

static void* allocator_nginx_realloc(Allocator* thiz, void* ptr, size_t size)
{
    return NULL;
}

static void allocator_nginx_destroy(Allocator* thiz)
{
    PrivInfo* priv = (PrivInfo*)thiz->priv;
    ngx_pool_data_t* p;
    ngx_pool_data_t* n;
    ngx_pool_large_t* l;

    for (l = priv->pool->large; l; l = l->next) {
        if (l->alloc) {
            SAFE_FREE(l->alloc);
            l->alloc = NULL;
        }
    }
    
    for (p = priv->pool->d, n = p->next; ; p = n, n = n->next) {
        SAFE_FREE(p);
        if (n == NULL) {
            break;
        }
    }
    
    SAFE_FREE(priv->pool);
    SAFE_FREE(thiz);
}

Allocator* allocator_nginx_create(size_t size)
{
    return_val_if_fail(size >= 0, NULL);

    Allocator* thiz = (Allocator*)malloc(sizeof(Allocator) + sizeof(PrivInfo));
    
    if (thiz != NULL) {
        PrivInfo* priv = (PrivInfo*)thiz->priv;
        priv->pool = (ngx_pool_t*)malloc(sizeof(ngx_pool_t));
        
        priv->pool->d = (ngx_pool_data_t*) malloc(sizeof(ngx_pool_data_t) + size);

        priv->pool->current = priv->pool->d;
        priv->pool->max = size;
        priv->pool->large = NULL;
        priv->pool->d->last = (u_char*)priv->pool->d + sizeof(ngx_pool_data_t);
        priv->pool->d->end = (u_char*)priv->pool->d->last + size;
        priv->pool->d->next = NULL;
        priv->pool->d->failed = 0;
                
        thiz->alloc = allocator_nginx_alloc;
        thiz->calloc = allocator_nginx_calloc;
        thiz->realloc = allocator_nginx_realloc;
        thiz->free = allocator_nginx_free;
        thiz->destroy = allocator_nginx_destroy;
    }

    return thiz;
}

#ifdef ALLOCATOR_NGINX_TEST

int main(int argc, char* argv[])
{
    Allocator* pool;
    void* p1;
    void* p2;
    void* p3;

    pool = allocator_nginx_create(512);
    
    p1 = allocator_alloc(pool, 416);
    p2 = allocator_alloc(pool, 216);
    p3 = allocator_alloc(pool, 624);

    allocator_free(pool, p1);
    allocator_free(pool, p2);
    allocator_free(pool, p3);

    allocator_destroy(pool);

    return 0;
}
#endif // ALLOCATOR_NGINX_TEST
