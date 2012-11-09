#include "ft_list.h"
#include "ft_event.h"
#include "ft_widget.h"

#ifndef _FT_WINDOW_H_
#define _FT_WINDOW_H_

#define FT_WIDGET_HEIGHT    42
#define FT_WIDGET_SPACING   5

typedef struct _FTWindow FTWindow;

struct _FTWindow
{
    FTWidget    widget;
    FTWidget   *focus;
    FTList     *children;
    char       *buffer;
    int         sensitive;
};

FTWindow *ft_window_new();

FTWindow *ft_window_get_top();

FTWidget *ft_window_get_focus(FTWindow *window);

int ft_window_get_sensitive(FTWindow *window);

int ft_window_add_child(FTWindow *window, FTWidget *widget);

int ft_window_add(FTWindow *window, FTWidget *widget, int position);

int ft_window_is_active(FTWindow *window);

void ft_window_layout(FTWindow *window);

void ft_window_show(FTWindow *window);

void ft_window_draw(FTWidget *widget);

void ft_window_event_handler(FTEvent *event, void *data);

void ft_window_move_focus(FTWindow *window, int orient);

void ft_window_set_sensitive(FTWindow *window, int sensitive);

void ft_window_close(FTWindow *window);

void ft_window_destroy(FTWidget *widget);

#endif/*_FT_WINDOW_H_*/
