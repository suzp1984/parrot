#undef LOG_TAG
#define LOG_TAG "Factory"

#include "ft_textpad.h"
#include "ft_config.h"
#include "gui/ft_button.h"
#include "gui/ft_status_button.h"
#include "messages.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cutils/log.h>

#define FT_MAX_LINE         10
#define FT_LINE_SPACING     4

typedef struct _FTContext FTContext;

struct _FTContext
{
    char   *text;
    int     id;
    int     center;
    int     result;
};

static FTContext ft_context;

extern FTColor ft_color_r;
extern FTColor ft_color_g;

static void on_ok_handler(FTButton *button, void *data)
{
    ft_context.result = FT_STATUS_OK;

    ft_window_close(data);
}

static void on_fail_handler(FTButton *button, void *data)
{
    ft_context.result = FT_STATUS_FAIL;

    ft_window_close(data);
}

static char **ft_textpad_split(const char *text, int *len)
{
    char **lines = NULL;
    char *p = NULL;
    int i = 0;

    lines = calloc(FT_MAX_LINE + 1, sizeof(char *));

    p = strchr(text, '\n');

    if (!p)
    {
        *lines = strdup(text);
        *len = 1;

        return lines;
    }

    for (i = 0; i < FT_MAX_LINE; i++)
    {
        if (!p)
        {
            lines[i] = strdup(text);
            break;
        }

        lines[i] = malloc(p - text + 1);

        strncpy(lines[i], text, p - text);
        lines[i][p - text] = '\0';
        text = p + 1;

        p = strchr(text, '\n');
    }

    if (len)
       *len = i + 1;

    return lines;
}

static void ft_textpad_draw(FTWidget *widget)
{
    FBSurface *s = widget->surface;
    FTPoint point;

    char **lines = NULL;
    int len = 0, i;

    if (!ft_window_is_active((FTWindow *)widget))
        return;

    ft_window_draw(widget);

    if (ft_context.text == NULL)
        return;

    point.x = FT_FONT_W;
    point.y = FT_FONT_W;

    lines = ft_textpad_split(ft_context.text, &len);

    if (ft_context.center)
        point.y = (s->height - (FT_FONT_H + FT_LINE_SPACING) * len) / 2;

    for (i = 0; i < len; i++, point.y += FT_FONT_H + FT_LINE_SPACING)
    {
        if (ft_context.center)
            point.x = (s->width - FT_FONT_W * strlen(lines[i])) / 2;

        ft_draw_text(s, &point, lines[i], &widget->gc);
    }

    for (i = 0; lines[i]; i++)
        free(lines[i]);

    free(lines);
}

void ft_textpad_event_handler(FTEvent *event, void *data)
{
    FTWindow *window = (FTWindow *)data;
    FTKeyEvent *ke;

    if (event->type == FE_KEY_RELEASE)
    {
        ke = (FTKeyEvent *)event;

        if (ke->key == FT_KEY_SEND || ke->key == FT_KEY_END)
        {
            ft_context.result = (ke->key == FT_KEY_SEND) ? FT_STATUS_OK : FT_STATUS_FAIL;
            ft_window_close(window);
            return;
        }
    }

    ft_window_event_handler(event, data);
}

void ft_textpad_destroy(FTWidget *widget)
{
    ft_window_destroy(widget);

    if (ft_context.id != -1)
    {
        char key[32];

        sprintf(key, "%d", ft_context.id);
        ft_config_set_int(key, ft_context.result);
    }
}

FTWindow *ft_text_new(const char *text, int center)
{
    FTWindow *window = ft_window_new();
    FTWidget *widget = (FTWidget *)window;

    widget->draw = ft_textpad_draw;
    widget->handler = ft_textpad_event_handler;
    widget->destroy = ft_textpad_destroy;

    free(ft_context.text);

    ft_context.id = -1;
    ft_context.text = text ? strdup(text) : NULL;
    ft_context.center = center;
    ft_context.result = FT_STATUS_NORMAL;

    return window;
}

FTWindow *ft_textpad_new(const char *text, int center)
{
    FTWindow *window = ft_window_new();
    FTWidget *widget = (FTWidget *)window;
    FBSurface *s = widget->surface;
    FTButton *button = NULL;
    FTRect *rect = NULL;

    char str[32];

    memset(str, 0, sizeof(str));
    sprintf(str, "       %s", _(OK_TEXT));
    button = ft_button_new(str);

    rect = &((FTWidget *)button)->rect;
    rect->x = FT_WIDGET_SPACING;
    rect->y = s->height - FT_WIDGET_HEIGHT - FT_WIDGET_SPACING;
    rect->width = s->width / 2 - FT_WIDGET_SPACING * 2;
    rect->height = FT_WIDGET_HEIGHT;

    ft_button_set_color(button, &ft_color_g);
    ft_button_set_handler(button, on_ok_handler, window);
    ft_window_add_child(window, (FTWidget *)button);

    memset(str, 0, sizeof(str));
    sprintf(str, "       %s", _(FAIL_TEXT));
    button = ft_button_new(str);

    rect = &((FTWidget *)button)->rect;
    rect->x = s->width / 2 + FT_WIDGET_SPACING;
    rect->y = s->height - FT_WIDGET_HEIGHT - FT_WIDGET_SPACING;
    rect->width = s->width / 2 - FT_WIDGET_SPACING * 2;
    rect->height = FT_WIDGET_HEIGHT;

    ft_button_set_color(button, &ft_color_r);
    ft_button_set_handler(button, on_fail_handler, window);
    ft_window_add_child(window, (FTWidget *)button);

    widget->draw = ft_textpad_draw;
    widget->handler = ft_textpad_event_handler;
    widget->destroy = ft_textpad_destroy;

    free(ft_context.text);

    ft_context.id = -1;
    ft_context.text = text ? strdup(text) : NULL;
    ft_context.center = center;
    ft_context.result = FT_STATUS_NORMAL;

    return window;
}

void ft_textpad_set_text(FTWindow *textpad, const char *text)
{
    free(ft_context.text);

    ft_context.text = text ? strdup(text) : NULL;

    ft_textpad_draw((FTWidget *)textpad);
}

void ft_textpad_set_color(FTWindow *textpad, FTColor *color)
{
    FTWidget *w = (FTWidget *)textpad;

    w->gc.foreground = *color;

    ft_textpad_draw(w);
}

void ft_textpad_set_id(FTWindow *textpad, int test_id)
{
    ft_context.id = test_id;
}

void ft_textpad_set_result(FTWindow *textpad, int result)
{
    ft_context.result = result;
}

