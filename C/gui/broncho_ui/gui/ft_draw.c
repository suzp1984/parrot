#undef LOG_TAG
#define LOG_TAG "Factory"
#include "ft_draw.h"
#include "libfbx-font-8x16.h"
#include "hzk16-16x16.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <cutils/log.h>
#include <unicode/ucnv.h>

#define MAX(x, y)   ((x)>(y))?(x):(y)
#define MIN(x, y)   ((x)<(y))?(x):(y)

/*Modified by liuhairong*/
#define FB_MAKE_COLOR_16(r, g, b) \
    (((r) >> 3) << 11 | ((g) >> 2) << 5 | ((b) >> 3))
#define FB_MAKE_COLOR_24(r, g, b) ((r) << 16 | (g) << 8 | (b))
/*End by liuhairong*/

int ft_draw_box(FBSurface *s, FTRect *rect, FTDrawGC *gc, int fill)
{
    FTPoint a, b, c, d;

    if (fill)
    {
        a.x = rect->x;
        a.y = rect->y;

        b.x = rect->x + rect->width - 1;
        b.y = rect->y;

        for (; a.y < rect->y + rect->height; a.y++, b.y++)
        {
            ft_draw_line(s, &a, &b, gc);
        }
    }
    else
    {
        a.x = rect->x;
        a.y = rect->y;

        b.x = rect->x + rect->width - 1;
        b.y = rect->y;

        c.x = rect->x + rect->width  - 1;
        c.y = rect->y + rect->height - 1;

        d.x = rect->x;
        d.y = rect->y + rect->height - 1;

        ft_draw_line(s, &a, &b, gc);
        ft_draw_line(s, &b, &c, gc);
        ft_draw_line(s, &c, &d, gc);
        ft_draw_line(s, &d, &a, gc);
    }

    return FT_SUCCESS;
}

int ft_draw_line(FBSurface *s, FTPoint *from, FTPoint *to, FTDrawGC *gc)
{
    assert (from->x == to->x || from->y == to->y);

    FTPoint p;
    int a, b, i;

    if (from->x == to->x)
    {
        a = MIN(from->y, to->y);
        b = MAX(from->y, to->y);
        p.x = from->x;

        for (i = a; i <= b; i++)
        {
            p.y = i;
            ft_draw_point(s, &p, gc);
        }
    }
    else
    {
        a = MIN(from->x, to->x);
        b = MAX(from->x, to->x);
        p.y = from->y;

        for (i = a; i <= b; i++)
        {
            p.x = i;
            ft_draw_point(s, &p, gc);
        }
    }

    return FT_SUCCESS;
}

int ft_draw_point(FBSurface *s, FTPoint *point, FTDrawGC *gc)
{
    FTColor fg = gc->foreground;
    int p, color;
    struct fb_var_screeninfo vinfo;

    ft_frame_buffer_get_vinfo(&vinfo);

    p = (point->x + point->y * s->width) * s->depth;

    /*Modified by liuhairong*/
    if (vinfo.bits_per_pixel == 16)
    {
        color = FB_MAKE_COLOR_16(fg.r, fg.g, fg.b);
        s->buffer[p] = color & 0xFF;
        s->buffer[p+1] = color >> 8;
    }
    else if (vinfo.bits_per_pixel == 24)
    {
        color = FB_MAKE_COLOR_24(fg.r, fg.g, fg.b);
        s->buffer[p] = color >> 16;
        s->buffer[p+1] = (color >> 8) & 0xFF;
        s->buffer[p+2] = color & 0xFF;
    }
    /*End by liuhairong*/

    return FT_SUCCESS;
}

int ft_draw_bold_point(FBSurface *s, FTPoint *point, FTDrawGC *gc, int bold)
{
    int i;
    FTPoint bold_point;

    bold_point.x = point->x;
    bold_point.y = point->y;
    
    if (point->y > s->height - 2 || point->x > s->width - 1)
        return FT_FAIL;

    ft_draw_point(s, &bold_point, gc);

    if (point->x > 0)
    {
        bold_point.x = point->x - 1;
        bold_point.y = point->y;
        ft_draw_point(s, &bold_point, gc);
    }

    bold_point.x = point->x + 1;
    bold_point.y = point->y;
    ft_draw_point(s, &bold_point, gc);

    if (point->y > 0)
    {
        bold_point.x = point->x;
        bold_point.y = point->y - 1;
        ft_draw_point(s, &bold_point, gc);
    }

    bold_point.x = point->x;
    bold_point.y = point->y + 1;
    ft_draw_point(s, &bold_point, gc);

    return FT_SUCCESS;
}

void ft_bresenham_draw_line(FBSurface *s, FTPoint *from, FTPoint *to, FTDrawGC *gc)
{
    FTPoint point;
    int x1 = from->x;
    int y1 = from->y;
    int x2 = to->x;
    int y2 = to->y;

    int dx,dy,e;
    dx = x2 - x1;
    dy = y2 - y1;

    if (dx >= 0)
    {
        if (dy >= 0) // dy>=0
        {
            if (dx >= dy) // 1/8 octant
            {
                e = dy - dx/2;
                while (x1 <= x2)
                {
                    point.x = x1;
                    point.y = y1;
                    ft_draw_point(s, &point, gc);

                    if (e > 0)
                    { y1 += 1; e -= dx;}
                    x1 += 1;
                    e += dy;
                }
            }
            else        // 2/8 octant
            {
                e = dx - dy/2;
                while (y1 <= y2)
                { 
                    point.x = x1;
                    point.y = y1;
                    ft_draw_point(s, &point, gc);

                    if (e > 0) 
                    { x1 += 1; e -= dy;}
                    y1 += 1;
                    e += dx;
                }
            }
        }
        else           // dy<0
        {
            dy = -dy;   // dy=abs(dy)

            if (dx >= dy) // 8/8 octant
            {
                e = dy - dx/2;
                while (x1 <= x2)
                {
                    point.x = x1;
                    point.y = y1;
                    ft_draw_point(s, &point, gc);

                    if (e > 0)
                    { y1-=1; e-=dx;}
                    x1 += 1;
                    e += dy;
                }
            }
            else        // 7/8 octant
            {
                e = dx - dy/2;
                while (y1 >= y2)
                {
                    point.x = x1;
                    point.y = y1;
                    ft_draw_point(s, &point, gc);

                    if (e > 0)
                    { x1+=1; e-=dy;}
                    y1 -= 1;
                    e += dx;
                }
            }
        }
    }
    else //dx<0
    {
        dx =- dx;        //dx=abs(dx)
        if (dy >= 0) // dy>=0
        {
            if (dx >= dy) // 4/8 octant
            {
                e = dy - dx/2;
                while(x1 >= x2)
                {
                    point.x = x1;
                    point.y = y1;
                    ft_draw_point(s, &point, gc);

                    if( e > 0){ y1 += 1; e -= dx;}
                    x1 -= 1;
                    e += dy;
                }
            }
            else        // 3/8 octant
            {
                e = dx - dy/2;
                while (y1 <= y2)
                {
                    point.x = x1;
                    point.y = y1;
                    ft_draw_point(s, &point, gc);

                    if (e > 0)
                    { x1-=1; e-=dy;}
                    y1 += 1;
                    e += dx;
                }
            }
        }
        else           // dy<0
        {
            dy =- dy;   // dy=abs(dy)

            if (dx >= dy) // 5/8 octant
            {
                e = dy - dx/2;
                while (x1 >= x2)
                {
                    point.x = x1;
                    point.y = y1;
                    ft_draw_point(s, &point, gc);

                    if (e > 0)
                    { y1-=1; e-=dx;}
                    x1 -= 1;
                    e += dy;
                }
            }
            else        // 6/8 octant
            {
                e = dx - dy/2;
                while (y1 >= y2)
                {
                    point.x = x1;
                    point.y = y1;
                    ft_draw_point(s, &point, gc);

                    if (e > 0)
                    { x1 -= 1; e -= dy;}
                    y1 -= 1;
                    e += dx;
                }
            }
        }
    }
}

int ft_draw_char(FBSurface *s, FTPoint *point, char c, FTDrawGC *gc)
{
    static char buffer[FT_FONT_W * FT_FONT_H / 8];

    FTPoint p;
    int i, j;

    for (i = 0; i < FT_FONT_H; i++)
    {
        char ch = fb_fontdata_8x16[c * sizeof(buffer) + i];

        for (j = 0; j < 8; j++)
        {
            if ((ch >> j) & 0x1)
            {
                p.x = point->x + 7 - j;
                p.y = point->y + i;

                ft_draw_point(s, &p, gc);
            }
        }
    }

    return FT_SUCCESS;
}

int ft_draw_hz(FBSurface *s, FTPoint *point, const char *hz, FTDrawGC *gc)
{
    static char buffer[FT_FONT_W * FT_FONT_H / 8][2];

    FTPoint p;
    UErrorCode error = U_ZERO_ERROR;
    unsigned char gb2312[3] = {0};
    int offset, i, j, k;

    ucnv_convert("gb2312", "utf8", (char *)gb2312, 3, hz, 3, &error);
    if (error != U_ZERO_ERROR)
    {
        return FT_FAIL;
    }

    offset = ((gb2312[0] - 0xa1) * 94 + (gb2312[1] - 0xa1)) * 32;
    memcpy(buffer, fb_fontdata_16x16 + offset, sizeof(buffer));

    for (i = 0; i < FT_FONT_H; i++)
    {
        for (j = 0; j < 2; j++)
        {
            for (k = 0; k < 8; k++)
            {
                if (buffer[i][j] & (0x80 >> k))
                {
                    p.x = point->x + j * 8 + k;
                    p.y = point->y + i;

                    ft_draw_point(s, &p, gc);
                }
            }
        }
    }

    return FT_SUCCESS;
}

int ft_draw_text(FBSurface *s, FTPoint *point, const char *text, FTDrawGC *gc)
{
    FTPoint p = *point;
    int len, i, hz = 0;

    if (!text || !*text)
    {
        return FT_SUCCESS;
    }

    len = strlen(text);

    for (i = 0; i < len; i++)
    {
        p.x = point->x + (i - hz) * FT_FONT_W;

        if (text[i] >> 4 == 0x0e)
        {
            ft_draw_hz(s, &p, &text[i], gc);
            i += 2;
            hz++;
            continue;
        }

        ft_draw_char(s, &p, text[i], gc);
    }

    return FT_SUCCESS;
}

int ft_point_in_box(FTPoint *point, FTRect *rect)
{
    if (point->x < rect->x || point->y < rect->y)
    {
        return 0;
    }

    if (point->x > rect->x + rect->width || 
        point->y > rect->y + rect->height)
    {
        return 0;
    }

    return 1;
}

void ft_draw_cleanup(FBSurface *s)
{
    FTDrawGC gc;

    FTRect rect = 
    {
        .x = 0,
        .y = 0,
        .width = s->width,
        .height = s->height,
    };

    memset(&gc, 0, sizeof(FTDrawGC));

    ft_draw_box(s, &rect, &gc, 1);
}

