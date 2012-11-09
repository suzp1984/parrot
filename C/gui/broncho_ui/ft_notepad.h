#include "gui/ft_window.h"

#ifndef _FT_NOTEPAD_H_
#define _FT_NOTEPAD_H_

FTWindow *ft_notepad_new();

void ft_notepad_append(FTWindow *notepad, const char *text, FTColor *color);

void ft_notepad_append_line(FTWindow *notepad, const char *text, FTColor *color);

void ft_notepad_clear(FTWindow *notepad);

#endif/*_FT_NOTEPAD_H_*/
