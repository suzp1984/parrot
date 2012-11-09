/*
 * File:    uinput.h
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   user input device wrapper.
 *
 * Copyright (c) 2009  Li XianJing <xianjimli@hotmail.com>
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2009-09-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef UINPUT_H
#define UINPUT_H

#ifdef __cplusplus
extern "C" {
#endif

struct _UInput;
typedef struct _UInput UInput;

struct _UInput
{
	int fd;
};

UInput* uinput_create(const char* dev, int xmax, int ymax);
void uinput_report_lbutton_event(UInput* thiz, int press);
void uinput_report_mbutton_event(UInput* thiz, int press);
void uinput_report_rbutton_event(UInput* thiz, int press);
void uinput_report_move_event(UInput* thiz, int x, int y);

void uinput_report_key_event(UInput* thiz, unsigned short key, int press);
void uinput_report_touch_event(UInput* thiz, int x, int y, int press/*1(down) 0(up) -1(move)*/);
void uinput_destroy(UInput* thiz);

#ifdef __cplusplus
}
#endif
#endif/*UINPUT_H*/

