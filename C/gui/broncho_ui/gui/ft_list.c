#include "ft_list.h"
#include <string.h>
#include <unistd.h>

FTList *ft_list_first(FTList *list)
{
    while (list && list->prev) list = list->prev;

    return list;
}

FTList *ft_list_last(FTList *list)
{
    while (list && list->next) list = list->next;

    return list;
}

FTList *ft_list_append(FTList *list, void *data)
{
    FTList *node = calloc(1, sizeof(FTList));

    list = ft_list_last(list);

    if (list)
    {
        list->next = node;
        node->prev = list;
    }

    node->data = data;

    return ft_list_first(node);
}

FTList *ft_list_prepend(FTList *list, void *data)
{
    FTList *node = calloc(1, sizeof(FTList));

    list = ft_list_first(list);

    if (list)
    {
        list->prev = node;
        node->next = list;
    }

    node->data = data;

    return node;
}

FTList *ft_list_insert(FTList *list, void *data, int position)
{
    FTList *node = NULL;

    list = ft_list_nth(list, position);

    if (list)
    {
        node = malloc(sizeof(FTList));

        node->next = list;
        node->prev = list->prev;
        node->data = data;

        if (list->prev)
            list->prev->next = node;

        list->prev = node;
    }

    return ft_list_first(node);
}

FTList *ft_list_delete(FTList *list, void *data)
{
    FTList *iter;

    iter = ft_list_first(list);

    for (; iter; iter = iter->next)
    {
        if (iter->data == data)
        {
            if (iter->prev)
                iter->prev->next = iter->next;

            if (iter->next)
                iter->next->prev = iter->prev;
            
            if (list == iter)
                list = iter->next ? iter->next : iter->prev;

            free(iter);

            break;
        }
    }

    return ft_list_first(list);
}

FTList *ft_list_delete_link(FTList *list, FTList *link)
{
    FTList *iter;

    iter = ft_list_first(list);

    for (; iter; iter = iter->next)
    {
        if (iter == link)
        {
            if (iter->prev)
                iter->prev->next = iter->next;

            if (iter->next)
                iter->next->prev = iter->prev;
            
            if (list == iter)
                list = iter->next ? iter->next : iter->prev;

            free(iter);

            break;
        }
    }

    return ft_list_first(list);
}

FTList *ft_list_nth(FTList *list, int nth)
{
    int i = 0;

    list = ft_list_first(list);

    while (list && list->next && i++ < nth) list = list->next;

    return list;
}

FTList *ft_list_find(FTList *list, void *data)
{
    FTList *iter = ft_list_first(list);

    for (; iter; iter = iter->next)
    {
        if (iter->data == data) 
            break;
    }

    return iter;
}

int ft_list_position(FTList *list, FTList *link)
{
    FTList *iter = ft_list_first(list);
    int index = 0;

    for (; iter; iter = iter->next)
    {
        if (iter == link) 
            return index;

        index++;
    }

    return -1;
}

int ft_list_index(FTList *list, void *data)
{
    FTList *iter = ft_list_first(list);
    int index = 0;

    for (; iter; iter = iter->next)
    {
        if (iter->data == data) 
            return index;

        index++;
    }

    return -1;
}

int ft_list_length(FTList *list)
{
    int len = 0;

    list = ft_list_first(list);

    for(; list; list = list->next) len++;

    return len;
}

void *ft_list_nth_data(FTList *list, int nth)
{
    list = ft_list_nth(list, nth);

    return list ? list->data : NULL;
}

void ft_list_free(FTList *list, int free_nodes)
{
    FTList *temp;

    list = ft_list_first(list);

    while (list)
    {
        if (free_nodes)
            free(list->data);

        temp = list;
        list = list->next;

        free(temp);
    }
}

