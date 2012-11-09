#include "ft_status_button.h"
#include "ft_window.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

extern FTColor ft_color_r;
extern FTColor ft_color_g;

static void ft_status_button_draw(FTWidget *widget)
{
    FTButton *button = (FTButton *)widget;
    FTStatusButton *status_button = (FTStatusButton *)widget;
    FTWindow *window = (FTWindow *)ft_widget_get_parent(widget);

    if (!widget->visible)
        return;

    if (!ft_window_is_active(window))
        return;

    FTDrawGC gc = widget->gc;
    FTRect *rect = &widget->rect;
    FTPoint point;
    int status = status_button->status;

    const char *status_text[] = {"", "[OK]", "[X]", NULL};

    ft_widget_draw(widget);

    point.x = rect->x + FT_FONT_W;
    point.y = rect->y + (rect->height - FT_FONT_H) / 2;

    gc.foreground = button->color;
    ft_draw_text(widget->surface, &point, button->text, &gc);

    /* draw status text */
    point.x += (strlen(button->text) + 1) * FT_FONT_W;
    gc.foreground = (status == FT_STATUS_OK) ? ft_color_g : ft_color_r;

    ft_draw_text(widget->surface, &point, status_text[status], &gc);
}

static void ft_status_button_event_handler(FTEvent *event, void *data)
{
    FTButton *button = (FTButton *)data;

    if (button->handler)
    {
        button->handler(button, button->data);
    }
}

FTStatusButton *ft_status_button_new(const char *text)
{
    assert(text != NULL);

    FTButton *button = ft_button_new(text);
    FTStatusButton *status_button = calloc(1, sizeof(FTStatusButton));
    FTWidget *widget = (FTWidget *)status_button;

    memcpy(status_button, button, sizeof(FTButton));

    widget->draw = ft_status_button_draw;
    widget->handler = ft_status_button_event_handler;
    widget->data = status_button;

    status_button->status = FT_STATUS_NORMAL;
    free(button);

    return status_button;
}

void ft_status_button_set_status(FTStatusButton *button, int status)
{
    assert(button != NULL);

    button->status = status;

    ft_status_button_draw((FTWidget *)button);
}

int ft_status_button_get_status(FTStatusButton *button)
{
    return button->status;
}

