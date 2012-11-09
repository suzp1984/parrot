#include "ft_widget.h"
#include <stdlib.h>

void ft_widget_init(FTWidget *widget, FTRect *rect, FTWidgetType type)
{
    ft_widget_init_default(widget);

    widget->rect = *rect;
    widget->type = type;
}

void ft_widget_init_default(FTWidget *widget)
{
    FBSurface *s = ft_frame_buffer_get_default();

    FTColor fg = {255, 255, 255, 0};
    FTColor bg = {0, 0, 0, 0};

    widget->status = FW_STATUS_NORMAL;
    widget->rect.x = 0;
    widget->rect.y = 0;
    widget->rect.width  = 0;
    widget->rect.height = 0;

    widget->surface = s;
    widget->gc.foreground = fg;
    widget->gc.background = bg;

    widget->draw = ft_widget_draw;
    widget->visible = 1;
}

void ft_widget_set_visible(FTWidget *widget, int visible)
{
    FTDrawGC gc;
   
    if (visible == widget->visible)
        return;

    gc.foreground = widget->gc.background;
    gc.background = widget->gc.background;

    ft_draw_box(widget->surface, &widget->rect, &gc, 1);

    if (visible)
        widget->draw(widget);

    widget->visible = visible;
    widget->focus = visible ? widget->focus : 0;
}

int  ft_widget_get_visible(FTWidget *widget)
{
    return widget->visible;
}

void ft_widget_set_focus(FTWidget *widget)
{
    FTRect rect;

    if (!widget->visible)
        return;

    rect.x      = widget->rect.x + 2;
    rect.y      = widget->rect.y + 2;
    rect.width  = widget->rect.width  - 4;
    rect.height = widget->rect.height - 4;

    ft_draw_box(widget->surface, &rect, &widget->gc, 0);

    widget->focus = 1;
}

void ft_widget_unset_focus(FTWidget *widget)
{
    FTRect rect;
    FTDrawGC gc;

    if (!widget->visible)
        return;

    rect.x      = widget->rect.x + 2;
    rect.y      = widget->rect.y + 2;
    rect.width  = widget->rect.width  - 4;
    rect.height = widget->rect.height - 4;

    gc = widget->gc;
    gc.foreground = widget->gc.background;
    gc.background = widget->gc.foreground;

    ft_draw_box(widget->surface, &rect, &gc, 0);

    widget->focus = 0;
}

void ft_widget_draw(FTWidget *widget)
{
    FTDrawGC gc;
   
    if (!widget->visible)
        return;

    gc.foreground = widget->gc.background;
    gc.background = widget->gc.background;

    ft_draw_box(widget->surface, &widget->rect, &gc, 1);
    ft_draw_box(widget->surface, &widget->rect, &widget->gc, 0);

    if (widget->focus)
    {
        ft_widget_set_focus(widget);
    }
}

void ft_widget_destroy(FTWidget *widget)
{
    if (widget->destroy)
        widget->destroy(widget);
}

FTWidget *ft_widget_get_parent(FTWidget *widget)
{
    return widget->parent;
}

