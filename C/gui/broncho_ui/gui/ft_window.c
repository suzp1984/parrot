#undef LOG_TAG
#define LOG_TAG "Factory"

#include "ft_window.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <cutils/log.h>

typedef struct _FWContext FWContext;

struct _FWContext
{
    FTList *windows;
    pthread_mutex_t mutex; 
};

static FWContext fw_context;

FTWindow *ft_window_new()
{
    FTWindow *window = calloc(1, sizeof(FTWindow));
    FTWidget *widget = (FTWidget *)window;

    ft_widget_init_default(widget);

    widget->type = FW_TYPE_WINDOW;
    widget->draw = ft_window_draw;

    widget->rect.x = 0;
    widget->rect.y = 0;
    widget->rect.width = widget->surface->width;
    widget->rect.height = widget->surface->height;

    widget->destroy = ft_window_destroy;
    widget->handler = ft_window_event_handler;
    widget->data = window;
    window->sensitive = 1;

    if (fw_context.windows == NULL)
        pthread_mutex_init(&fw_context.mutex, NULL);

    fw_context.windows = ft_list_append(fw_context.windows, window);

    return window;
}

void ft_window_layout(FTWindow *window)
{
    FTWidget *w = (FTWidget *)window;
    FBSurface *s = w->surface;

    FTRect rect;
    int i = 0;

    FTList *iter = window->children;

    for (; iter; iter = iter->next, i++)
    {
        w = (FTWidget *)iter->data;

        if (!w->rect.width || !w->rect.height)
        {
            w->rect.x = (i % 2) ? (s->width / 2 + FT_WIDGET_SPACING) : FT_WIDGET_SPACING;
            w->rect.y = (i / 2) * (FT_WIDGET_HEIGHT + FT_WIDGET_SPACING) + FT_WIDGET_SPACING;

            w->rect.width = s->width / 2 - FT_WIDGET_SPACING * 2;
            w->rect.height = FT_WIDGET_HEIGHT;

            if (w->rect.y + w->rect.height > s->height)
            {
                memset(&w->rect, 0, sizeof(FTRect));
                w->destroy(w);

                ft_list_delete(iter, iter->data);
            }
        }
    }
}

void ft_window_show(FTWindow *window)
{
    FTWidget *widget = (FTWidget *)window;

    pthread_mutex_lock(&fw_context.mutex);

    widget->draw(widget);
    window->focus = ft_window_get_focus(window);

    ft_event_set_handler(widget->handler, widget->data);

    pthread_mutex_unlock(&fw_context.mutex);
}

void ft_window_draw(FTWidget *widget)
{
    FTWindow *window = (FTWindow *)widget;
    FTWidget *w;
    FBSurface *s = widget->surface;

    if (!widget->visible || !ft_window_is_active(window))
        return;

    memset(s->buffer, 0, s->size);

    FTList *iter = window->children;

    for (; iter; iter = iter->next)
    {
        w = (FTWidget *)iter->data;

        w->draw(w);
    }
}

int ft_window_add_child(FTWindow *window, FTWidget *widget)
{
    assert(widget != NULL);

    window->children = ft_list_append(window->children, widget);
    widget->parent = (FTWidget *)window;

    ft_window_layout(window);

    return FT_SUCCESS;
}

int ft_window_add(FTWindow *window, FTWidget *widget, int position)
{
    assert(widget != NULL);

    if (position < 0)
        return ft_window_add_child(window, widget);

    window->children = ft_list_insert(window->children, widget, position);
    widget->parent = (FTWidget *)window;

    ft_window_layout(window);

    return FT_SUCCESS;
}

int ft_window_is_active(FTWindow *window)
{
    return window == ft_window_get_top();
}

FTWindow *ft_window_get_top()
{
    FTList *iter = ft_list_last(fw_context.windows);

    return iter ? iter->data : NULL;
}

FTWidget *ft_window_get_focus(FTWindow *window)
{
    FTList *iter = window->children;
    FTWidget *widget = NULL;

    if (!iter)
        return NULL;

    for (; iter; iter = iter->next)
    {
        widget = (FTWidget *)iter->data;

        if (widget->focus)
        {
            return widget;
        }
    }

    return NULL;
}

int ft_window_get_sensitive(FTWindow *window)
{
    return window->sensitive;
}

void ft_window_move_focus(FTWindow *window, int orient)
{
    FTWidget *widget = NULL;
    FTList *iter = NULL;

    if (window->children == NULL)
        return;

    widget = ft_window_get_focus(window);

    if (widget)
        iter = ft_list_find(window->children, widget);

    if (orient > 0)
    {
        if (iter && iter->next)
            iter = iter->next;
        else
            iter = window->children;
    }
    else
    {
        if (iter && iter->prev)
            iter = iter->prev;
        else
            iter = ft_list_last(window->children);
    }

    window->focus = (FTWidget *)iter->data;

    if (widget)
        ft_widget_unset_focus(widget);

    ft_widget_set_focus(window->focus);
}

void ft_window_set_sensitive(FTWindow *window, int sensitive)
{
    window->sensitive = sensitive;
}

void ft_window_close(FTWindow *window)
{
    FTWidget *widget = (FTWidget *)window;
    FTWindow *w = NULL;

    FTList *last = ft_list_last(fw_context.windows);

    widget->destroy(widget);

    if (window == last->data)
    {
        if (last->prev)
        {
            w = (FTWindow *)last->prev->data;
        }
        else
        {
            pthread_mutex_destroy(&fw_context.mutex);
            exit(FT_SUCCESS);
        }
    }

    fw_context.windows = ft_list_delete(fw_context.windows, window);
    ft_window_show(w);
}

void ft_window_destroy(FTWidget *widget)
{
    FTWindow *window = (FTWindow *)widget;
    FTList *iter = window->children;

    for (; iter; iter = iter->next)
    {
        FTWidget *w = (FTWidget *)iter->data;

        w->destroy(w);
    }

    free(window);
}

static FTWidget *ft_window_find_widget(FTWindow *window, FTPoint *point)
{
    FTWidget *widget = NULL;
    FTList *iter = window->children;

    for (; iter; iter = iter->next)
    {
        widget = (FTWidget *)iter->data;

        if (ft_point_in_box(point, &widget->rect))
        {
            return widget;
        }
    }

    return NULL;
}

void ft_window_event_handler(FTEvent *event, void *data)
{
    FTWindow *window = (FTWindow *)data;

    FTMouseEvent *me;
    FTKeyEvent *ke;
    FTPoint point;
    FTWidget *w;

    if (!window->sensitive)
        return;

    switch (event->type)
    {
        case FE_KEY_RELEASE:
            ke = (FTKeyEvent *)event;

            switch (ke->key)
            {
                case FT_KEY_OK:
                    w = ft_window_get_focus(window);

                    if (w && w->handler)
                    {
                        w->handler(event, w->data);
                    }
                    break;

                case FT_KEY_UP:
                case FT_KEY_LEFT:
                    ft_window_move_focus(window, -1);
                    break;

                case FT_KEY_DOWN:
                case FT_KEY_RIGHT:
                    ft_window_move_focus(window, 1);
                    break;

                case FT_KEY_BACK:
                    ft_window_close(window);
                    break;
            }

            break;

        case FE_MOUSE_PRESS:
            me = (FTMouseEvent *)event;

            point.x = me->x;
            point.y = me->y;

            w = ft_window_find_widget(window, &point);

            if (w && w->handler && w->visible)
            {
                if (w != window->focus)
                {
                    if (window->focus)
                        ft_widget_unset_focus(window->focus);

                    ft_widget_set_focus(w);

                    window->focus = w;
                }
            }

            break;

        case FE_MOUSE_RELEASE:
            me = (FTMouseEvent *)event;

            point.x = me->x;
            point.y = me->y;

            w = ft_window_find_widget(window, &point);

            if (w && w->handler && w->visible)
            {
                w->handler(event, w->data);
            }

            break;

        default: break;
    }
}

