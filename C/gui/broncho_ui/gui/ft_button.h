
#include "ft_event.h"
#include "ft_widget.h"

#ifndef _FT_BUTTON_H_
#define _FT_BUTTON_H_

typedef struct _FTButton FTButton;

typedef void (*FBHandler)(FTButton *button, void *data);

struct _FTButton
{
    FTWidget    widget;
    FTColor     color;
    FBHandler   handler;
    void       *data;
    char       *text;
};

FTButton *ft_button_new(const char *text);

void ft_button_set_handler(FTButton *button, FBHandler handler, void *data);

void ft_button_set_text(FTButton *button, const char *text);

void ft_button_set_color(FTButton *button, FTColor *color);

#endif/*_FT_BUTTON_H_*/
