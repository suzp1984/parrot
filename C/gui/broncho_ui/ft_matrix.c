#undef LOG_TAG
#define LOG_TAG "Factory"

#include "ft_matrix.h"
#include "ft_config.h"
#include <stdio.h>
#include <string.h>
#include <cutils/log.h>

#define FM_DEFAULT_ROW      4
#define FM_DEFAULT_COL      3
#define FM_DEFAULT_SIZE     50
//#define FM_DEFAULT_SIZE     30

#define TP_POS      0
//#define TP_POS      15
#define LINE_LEFT 0
#define LINE_RIGHT 1

typedef struct _TPContext TPContext;

static FTWidget *a_widget;

struct _TPContext
{
    FTPoint *pointers;
    int status;
    int start_point;
};

static TPContext tp_context;

//static FTRect ft_matrix_get_rect(FTWidget *widget, int col, int row)
static FTRect ft_matrix_get_rect(FTWidget *widget, FTPoint point)
{
    FBSurface *s = widget->surface;

    FTRect rect = {.width = FM_DEFAULT_SIZE, .height = FM_DEFAULT_SIZE};

    //int w_margin = (s->width  / FM_DEFAULT_COL - FM_DEFAULT_SIZE) / 2;
    //int h_margin = (s->height / FM_DEFAULT_ROW - FM_DEFAULT_SIZE) / 2;

    //rect.x = (s->width  / FM_DEFAULT_COL) * col + w_margin;
    //rect.y = (s->height / FM_DEFAULT_ROW) * row + h_margin;
    rect.x = point.x;
    rect.y = point.y;

    return rect;
}

void get_point_of_long_line(int line, FTPoint *from, FTPoint *to)
{
    FTPoint *pointers = tp_context.pointers;

    if (tp_context.start_point == 0)
    {
        if (line == LINE_LEFT)
        {
            from->x = pointers[0].x + (FM_DEFAULT_SIZE << 1);
            from->y = pointers[0].y;
            to->x = pointers[1].x + FM_DEFAULT_SIZE - 1;
            to->y = pointers[1].y - FM_DEFAULT_SIZE;
        }
        else //line == LINE_RIGHT
        {
            from->x = pointers[0].x;
            from->y = pointers[0].y + (FM_DEFAULT_SIZE << 1);
            to->x = pointers[1].x - FM_DEFAULT_SIZE;
            to->y = pointers[1].y + FM_DEFAULT_SIZE - 1;
        }
    }
    else
    {
        if (line == LINE_LEFT)
        {
            from->x = pointers[2].x;
            from->y = pointers[2].y - FM_DEFAULT_SIZE;
            to->x = pointers[3].x - FM_DEFAULT_SIZE;
            to->y = pointers[3].y;
        }
        else //line == LINE_RIGHT
        {
            from->x = pointers[2].x + (FM_DEFAULT_SIZE << 1);
            from->y = pointers[2].y + FM_DEFAULT_SIZE - 1;
            to->x = pointers[3].x + FM_DEFAULT_SIZE - 1;
            to->y = pointers[3].y + (FM_DEFAULT_SIZE << 1);
        }
    }
}

static float get_y_in_line(int line, int x)
{
    float k, b, y, x1, y1, x2, y2;
    FTPoint from, to;

    get_point_of_long_line(line, &from, &to);

    x1 = from.x;
    y1 = from.y;
    x2 = to.x;
    y2 = to.y;

    k = (y2 - y1) / (x2 - x1);
    b = (x2 * y1 - x1 * y2) / (x2 - x1);

    return (k * x + b);
}

static int ft_point_within_line(FBSurface *s, FTPoint *point)
{
    FTPoint from, to;
    float y;

    y = get_y_in_line(LINE_LEFT, point->x);

    if (point->y < y)
        return 0;

    y = get_y_in_line(LINE_RIGHT, point->x);

    if (point->y > y)
        return 0;

    return 1;
}

static void ft_matrix_draw(FTWidget *widget)
{
    FBSurface *s = widget->surface;
    FTRect rect;
    FTPoint from, to;
    int i, j;

    FTPoint *pointers = tp_context.pointers;

    memset(s->buffer, 0, s->size);

    for (i = tp_context.start_point; i < tp_context.start_point + 2; i++)
    {
        from.x = pointers[i].x;
        from.y = pointers[i].y;
        to.x = pointers[i].x + FM_DEFAULT_SIZE - 1; 
        to.y = pointers[i].y + FM_DEFAULT_SIZE - 1;
        ft_bresenham_draw_line(s, &from, &to, &widget->gc);

        from.x = pointers[i].x;
        from.y = pointers[i].y + FM_DEFAULT_SIZE - 1; 
        to.x = pointers[i].x + FM_DEFAULT_SIZE - 1; 
        to.y = pointers[i].y;
        ft_bresenham_draw_line(s, &from, &to, &widget->gc);
    }

    get_point_of_long_line(LINE_LEFT, &from, &to);
    ft_bresenham_draw_line(s, &from, &to, &widget->gc);

    get_point_of_long_line(LINE_RIGHT, &from, &to);
    ft_bresenham_draw_line(s, &from, &to, &widget->gc);
    /*
    for (i = 0; i < FM_DEFAULT_COL; i++)
    {
        for (j = 0; j < FM_DEFAULT_ROW; j++)
        {
            rect = ft_matrix_get_rect(widget, i, j);

            ft_draw_box(s, &rect, &widget->gc, 0);
        }
    }
    */
}

static void ft_matrix_destroy()
{
    if (tp_context.pointers != NULL)
    {
        free(tp_context.pointers);
        tp_context.pointers = NULL;
    }
}

static void ft_matrix_handler(FTEvent *event, void *data)
{
    FTWindow *window = (FTWindow *)data;
    FTWidget *widget = (FTWidget *)data;

    FBSurface *s = widget->surface;

    FTDrawGC gc;
    FTPoint point;
    FTRect rect_start;
    FTRect rect_end;

    FTMouseEvent *me;
    FTKeyEvent *ke;
    char key[32];
    int i, j;
    int draw = 1;

    FTPoint *pointers = tp_context.pointers;

    static int num = FM_DEFAULT_COL * FM_DEFAULT_ROW;

    sprintf(key, "%d", FT_ITEM_MATRIX);

    if (event->type == FE_KEY_RELEASE)
    {
        ke = (FTKeyEvent *)event;

        if (ke->key == FT_KEY_BACK)
        {
            ft_config_set_int(key, FT_STATUS_NORMAL);
            ft_window_close(window);
        }

        return;
    }

    me = (FTMouseEvent *)event;
    point.x = me->x;
    point.y = me->y;
    
    if (event->type == FE_MOUSE_PRESS)
    {
        rect_start = ft_matrix_get_rect(widget, pointers[tp_context.start_point]);
        rect_end = ft_matrix_get_rect(widget, pointers[tp_context.start_point + 1]);

        if (ft_point_in_box(&point, &rect_start))
        {
            tp_context.status = FT_STATUS_OK;
        }
        else if (ft_point_in_box(&point, &rect_end))
        {
            rect_start = ft_matrix_get_rect(widget, pointers[tp_context.start_point + 1]);
            rect_end = ft_matrix_get_rect(widget, pointers[tp_context.start_point]);
            tp_context.status = FT_STATUS_OK;
        }
        else if (!ft_point_within_line(s, &point))
        {
            tp_context.status = FT_STATUS_FAIL;
        }
    }

    if (!ft_point_in_box(&point, &rect_start) && !ft_point_in_box(&point, &rect_end) && !ft_point_within_line(s, &point))
    {
         tp_context.status = FT_STATUS_FAIL;
    }

    if (tp_context.status == FT_STATUS_FAIL)
    {
        widget->draw(a_widget);
        return;
    }


    if (event->type == FE_MOUSE_RELEASE)
    {
        if (!ft_point_in_box(&point, &rect_end))
        {
            tp_context.status = FT_STATUS_FAIL;
        }
        else
        {
            draw = 0;
            tp_context.status = FT_STATUS_OK;
           //ft_config_set_int(key, FT_STATUS_OK);
            ft_window_close(window);

            if (tp_context.start_point == 0)
            {
                window = ft_matrix_page(1);
                ft_window_show(window);
            }
            else
            {
           ft_config_set_int(key, FT_STATUS_OK);
                ft_matrix_destroy();
            }
        }
    }

    if (tp_context.status == FT_STATUS_FAIL)
    {
        widget->draw(a_widget);
    }
    else if(draw)
    {
        ft_draw_bold_point(s, &point, &widget->gc, 0);
    }

    /*
    if (event->type == FE_MOUSE_RELEASE)
    {
        me = (FTMouseEvent *)event;

        gc.foreground = widget->gc.background;
        gc.background = widget->gc.background;

        point.x = me->x;
        point.y = me->y;

        for (i = 0; i < FM_DEFAULT_COL; i++)
        {
            for (j = 0; j < FM_DEFAULT_ROW; j++)
            {
                rect = ft_matrix_get_rect(widget, i, j);

                if (ft_point_in_box(&point, &rect))
                {
                    ft_draw_box(s, &rect, &gc, 0);

                    if (--num <= 0)
                    {
                        num = FM_DEFAULT_COL * FM_DEFAULT_ROW;

                        ft_config_set_int(key, FT_STATUS_OK);
                        ft_window_close(window);
                    }

                    return;
                }
            }
        }
    }
    */
}

FTWindow *ft_matrix_page(int page)
{
    FTWindow *window = ft_window_new();
    FTWidget *widget = (FTWidget *)window;

    tp_context.status = FT_STATUS_OK;
    tp_context.start_point = page * 2;

    widget->draw = ft_matrix_draw;
    widget->handler = ft_matrix_handler;
    widget->data = window;

    a_widget = widget;

    return window;
}

FTWindow *ft_matrix_new()
{
    FTWindow *window = ft_matrix_page(0);
    FTWidget *widget = (FTWidget *)window;
    FBSurface *s = widget->surface;

    tp_context.pointers = (FTPoint *)malloc(sizeof(FTPoint) * 4);
    if (tp_context.pointers == NULL)
        return NULL;

    tp_context.pointers[0].x = TP_POS;
    tp_context.pointers[0].y = TP_POS;
    tp_context.pointers[1].x = s->width - FM_DEFAULT_SIZE - TP_POS;
    tp_context.pointers[1].y = s->height - FM_DEFAULT_SIZE - TP_POS;
    tp_context.pointers[2].x = TP_POS;
    tp_context.pointers[2].y = s->height - FM_DEFAULT_SIZE - TP_POS;
    tp_context.pointers[3].x = s->width - FM_DEFAULT_SIZE - TP_POS;
    tp_context.pointers[3].y = TP_POS;

    return window;
}

