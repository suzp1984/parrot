
#ifndef _FT_LIST_H_
#define _FT_LIST_H_

typedef struct _FTList FTList;

struct _FTList
{
    FTList *prev;
    FTList *next;
    void   *data;
};

FTList *ft_list_first(FTList *list);

FTList *ft_list_last(FTList *list);

FTList *ft_list_append(FTList *list, void *data);

FTList *ft_list_prepend(FTList *list, void *data);

FTList *ft_list_insert(FTList *list, void *data, int position);

FTList *ft_list_delete(FTList *list, void *data);

FTList *ft_list_delete_link(FTList *list, FTList *link);

FTList *ft_list_nth(FTList *list, int nth);

FTList *ft_list_find(FTList *list, void *data);

int ft_list_position(FTList *list, FTList *link);

int ft_list_index(FTList *list, void *data);

int ft_list_length(FTList *list);

void *ft_list_nth_data(FTList *list, int nth);

void  ft_list_free(FTList *list, int free_nodes);

#endif/*_FT_LIST_H_*/
