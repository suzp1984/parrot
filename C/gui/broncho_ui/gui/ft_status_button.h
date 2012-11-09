#ifndef _FT_STATUS_BUTTON_H_
#define _FT_STATUS_BUTTON_H_

#include "ft_button.h"

struct _FTStatusButton
{
    FTButton    button;
    int         status;
};

typedef struct _FTStatusButton FTStatusButton;

FTStatusButton *ft_status_button_new(const char *text);

void ft_status_button_set_status(FTStatusButton *button, int status);

int  ft_status_button_get_status(FTStatusButton *button);

#endif/*_FT_STATUS_BUTTON_H_*/
