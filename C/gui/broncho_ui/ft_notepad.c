#undef LOG_TAG
#define LOG_TAG "Factory"

#include "ft_notepad.h"
#include "gui/ft_list.h"

#include <string.h>
#include <assert.h>
#include <cutils/log.h>

#define FN_LINE_SPACING     4

typedef struct _FNLine FNLine;

struct _FNLine
{
    char   *text;
    int     row, col; 
    FTColor color;
};

typedef struct _FNContext FNContext;

struct _FNContext
{
    int     row, col; 
    int     max_row, max_col;
    FTList *lines;
};

static FNContext fn_context;

extern FTColor ft_color_r;
extern FTColor ft_color_g;
extern FTColor ft_color_w;

static void fn_line_free(FNLine *line)
{
    if (line == NULL)
        return;

    free(line->text);
    free(line);
}

static void ft_notepad_draw(FTWidget *widget)
{
    FTWindow *window = (FTWindow *)widget;
    FBSurface *s = widget->surface;
    FTDrawGC gc = widget->gc;
    FTPoint point;

    if (!widget->visible || !ft_window_is_active(window))
        return;

    memset(s->buffer, 0, s->size);

    int i = 0;
    for (; i < ft_list_length(fn_context.lines); i++)
    {
        FNLine *line = ft_list_nth_data(fn_context.lines, i);

        point.x = line->col * FT_FONT_W;
        point.y = line->row * (FT_FONT_H + FN_LINE_SPACING);
        gc.foreground = line->color;

        ft_draw_text(s, &point, line->text, &gc);
    }
}

FTWindow *ft_notepad_new()
{
    FTWindow *window;
    FTWidget *widget;
    FTList *iter;

    window = ft_window_new();
    widget = (FTWidget *)window;
    widget->draw = ft_notepad_draw;

    FBSurface *s = widget->surface;

    memset(&fn_context, 0, sizeof(FNContext));
    fn_context.max_col = s->width / FT_FONT_W;
    fn_context.max_row = s->height / (FT_FONT_H + FN_LINE_SPACING);

    ft_notepad_clear(window);

    return window;
}

void ft_notepad_append(FTWindow *notepad, const char *text, FTColor *color)
{
    assert(notepad != NULL && text != NULL);

    FTWidget *widget = (FTWidget *)notepad;
    FTPoint point;

    FNLine *line = malloc(sizeof(FNLine));

    line->col = fn_context.col;
    line->row = fn_context.row;
    line->color = color ? *color : widget->gc.foreground;
    line->text = strdup(text);

    fn_context.lines = ft_list_append(fn_context.lines, line);
    fn_context.col += strlen(text);

    ft_notepad_draw((FTWidget *)notepad);
}

void ft_notepad_append_line(FTWindow *notepad, const char *text, FTColor *color)
{
    assert(notepad != NULL && text != NULL);

    if (fn_context.row >= fn_context.max_row - 1)
        return;

    fn_context.row++;
    fn_context.col = 0;

    ft_notepad_append(notepad, text, color);
}

void ft_notepad_clear(FTWindow *notepad)
{
    FTList *iter = fn_context.lines;

    for (; iter; iter = iter->next)
        fn_line_free(iter->data);

    ft_list_free(fn_context.lines, 0);
    fn_context.lines = NULL;

    ft_notepad_draw((FTWidget *)notepad);
}

