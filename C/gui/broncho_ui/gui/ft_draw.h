#include "ft_type.h"
#include "ft_framebuffer.h"

#ifndef _FT_DRAW_H_
#define _FT_DRAW_H_

#define FT_FONT_W   8   /*FIXME: support 8 only*/
#define FT_FONT_H   16

typedef struct _FTRect      FTRect;
typedef struct _FTPoint     FTPoint;
typedef struct _FTColor     FTColor;
typedef struct _FTDrawGC    FTDrawGC;

struct _FTRect
{
    int x, y;
    int width, height;
};

struct _FTPoint
{
    int x, y;
};

struct _FTColor
{
    int r, g, b;
    int alpha;
};

struct _FTDrawGC
{
    FTColor foreground;
    FTColor background;
    int     font_size;
};

int ft_draw_box(FBSurface *surface, FTRect *rect, FTDrawGC *gc, int fill);

int ft_draw_line(FBSurface *surface, FTPoint *from, FTPoint *to, FTDrawGC *gc);

void ft_bresenham_draw_line(FBSurface *s, FTPoint *from, FTPoint *to, FTDrawGC *gc);

int ft_draw_char(FBSurface *s, FTPoint *point, char c, FTDrawGC *gc);

int ft_draw_point(FBSurface *surface, FTPoint *point, FTDrawGC *gc);

int ft_draw_bold_point(FBSurface *s, FTPoint *point, FTDrawGC *gc, int bold);

int ft_draw_text(FBSurface *s, FTPoint *point, const char *text, FTDrawGC *gc);

int ft_point_in_box(FTPoint *point, FTRect *rect);

void ft_draw_cleanup(FBSurface *surface);

#endif/*_FT_DRAW_H_*/
