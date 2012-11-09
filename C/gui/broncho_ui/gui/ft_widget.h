#include "ft_type.h"
#include "ft_draw.h"
#include "ft_list.h"
#include "ft_event.h"

#ifndef _FT_WIDGET_H_
#define _FT_WIDGET_H_

typedef int FTWidgetType;

typedef struct _FTWidget FTWidget;

typedef void (*FTWidgetDraw)(FTWidget *widget);
typedef void (*FTWidgetDestroy)(FTWidget *widget);

typedef enum
{
    FW_STATUS_NORMAL = 0,
    FW_STATUS_FOCUSED,
    FW_STATUS_INVISIBLE,
    FW_STATUS_INSENSITIVE,
} FTWidgetStatus;

enum
{
    FW_TYPE_WINDOW = 0,
    FW_TYPE_BUTTON,
};

struct _FTWidget
{
    FTWidget       *parent;
    FTWidgetStatus  status;
    FTWidgetType    type;
    FTWidgetDraw    draw;       /* required */
    FTWidgetDestroy destroy;    /* required */
    FEHandler       handler;
    void           *data;

    FBSurface      *surface;
    FTDrawGC        gc;
    FTRect          rect;
    FTList         *list;
    int             focus;
    int             visible;
};

FTWidget *ft_widget_get_parent(FTWidget *);

void ft_widget_init(FTWidget *widget, FTRect *rect, FTWidgetType type);

void ft_widget_init_default(FTWidget *widget);

void ft_widget_set_visible(FTWidget *widget, int visible); 

int  ft_widget_get_visible(FTWidget *widget);

void ft_widget_set_focus(FTWidget *widget);

void ft_widget_unset_focus(FTWidget *widget);

void ft_widget_draw(FTWidget *widget);

void ft_widget_destroy(FTWidget *widget);

#endif/*_FT_WIDGET_H_*/
