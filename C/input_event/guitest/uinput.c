/*
 * File:    uinput.c
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

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "uinput.h"
#include <stdlib.h>
#ifdef HAS_LINUX_UINPUT_H
#include <linux/uinput.h>
#else
#include "linux_uinput.h"
#endif


UInput* uinput_create(const char* dev, int xmax, int ymax)
{
	int i = 0;
	int ret = 0;
	UInput* thiz = NULL;
	struct uinput_user_dev uinp = {0};

	dev = dev != NULL ? dev : "/dev/uinput";

	if((thiz = calloc(1, sizeof(UInput))) == NULL)
	{
		return NULL;
	}
	printf("calloc sucess!\n");

	if((thiz->fd = open(dev, O_WRONLY | O_NDELAY)) <= 0)
	{
		free(thiz);
		printf("%s does not exist. run mknod %s c 10 223 to create it.\n", dev, dev);

		return NULL;
	}
	printf("open uinput sucessful\n");

	uinp.id.version = 4;
	uinp.id.bustype = BUS_USB;
	uinp.absmin[ABS_X] = 0;
	uinp.absmax[ABS_X] = xmax;
	uinp.absmin[ABS_Y] = 0;
	uinp.absmax[ABS_Y] = ymax;
	uinp.absmin[ABS_PRESSURE] = 0;
	uinp.absmax[ABS_PRESSURE] = 0xfff;

	strncpy(uinp.name, "vinput", UINPUT_MAX_NAME_SIZE);
	ret = ioctl(thiz->fd, UI_SET_EVBIT, EV_KEY);
	ret = ioctl(thiz->fd, UI_SET_EVBIT, EV_REL);
	ret = ioctl(thiz->fd, UI_SET_RELBIT, REL_X);
	ret = ioctl(thiz->fd, UI_SET_RELBIT, REL_Y);
	
	ret = ioctl(thiz->fd, UI_SET_EVBIT, EV_ABS);
	ret = ioctl(thiz->fd, UI_SET_ABSBIT, ABS_X);
	ret = ioctl(thiz->fd, UI_SET_ABSBIT, ABS_Y);
	ret = ioctl(thiz->fd, UI_SET_ABSBIT, ABS_PRESSURE);
	for (i=0; i < 256; i++) 
	{
		ret = ioctl(thiz->fd, UI_SET_KEYBIT, i);
	}
	ret = ioctl(thiz->fd, UI_SET_KEYBIT, BTN_MOUSE);
	ret = ioctl(thiz->fd, UI_SET_KEYBIT, BTN_TOUCH);
	ret = ioctl(thiz->fd, UI_SET_KEYBIT, BTN_MOUSE);
	ret = ioctl(thiz->fd, UI_SET_KEYBIT, BTN_LEFT);
	ret = ioctl(thiz->fd, UI_SET_KEYBIT, BTN_MIDDLE);
	ret = ioctl(thiz->fd, UI_SET_KEYBIT, BTN_RIGHT);
	ret = ioctl(thiz->fd, UI_SET_KEYBIT, BTN_FORWARD);
	ret = ioctl(thiz->fd, UI_SET_KEYBIT, BTN_BACK);

	ret = write(thiz->fd, &uinp, sizeof(uinp));
	if (ioctl(thiz->fd, UI_DEV_CREATE))
	{
		perror("ioctl");
		close(thiz->fd);
		free(thiz);
		return NULL;
	}

	return thiz;
}

static const int s_key_map[256] = 
{
	['1'] = KEY_1,
	['2'] = KEY_2,
	['3'] = KEY_3,
	['4'] = KEY_4,
	['5'] = KEY_5,
	['6'] = KEY_6,
	['7'] = KEY_7,
	['8'] = KEY_8,
	['9'] = KEY_9,
	['0'] = KEY_0,
	['a'] = KEY_A,
	['b'] = KEY_B,
	['c'] = KEY_C,
	['d'] = KEY_D,
	['e'] = KEY_E,
	['f'] = KEY_F,
	['g'] = KEY_G,
	['h'] = KEY_H,
	['i'] = KEY_I,
	['j'] = KEY_J,
	['k'] = KEY_K,
	['l'] = KEY_L,
	['m'] = KEY_M,
	['n'] = KEY_N,
	['o'] = KEY_O,
	['p'] = KEY_P,
	['q'] = KEY_Q,
	['r'] = KEY_R,
	['s'] = KEY_S,
	['t'] = KEY_T,
	['u'] = KEY_U,
	['v'] = KEY_V,
	['w'] = KEY_W,
	['x'] = KEY_X,
	['y'] = KEY_Y,
	['z'] = KEY_Z,
	['A'] = KEY_A,
	['B'] = KEY_B,
	['C'] = KEY_C,
	['D'] = KEY_D,
	['E'] = KEY_E,
	['F'] = KEY_F,
	['G'] = KEY_G,
	['H'] = KEY_H,
	['I'] = KEY_I,
	['J'] = KEY_J,
	['K'] = KEY_K,
	['L'] = KEY_L,
	['M'] = KEY_M,
	['N'] = KEY_N,
	['O'] = KEY_O,
	['P'] = KEY_P,
	['Q'] = KEY_Q,
	['R'] = KEY_R,
	['S'] = KEY_S,
	['T'] = KEY_T,
	['U'] = KEY_U,
	['V'] = KEY_V,
	['W'] = KEY_W,
	['X'] = KEY_X,
	['Y'] = KEY_Y,
	['Z'] = KEY_Z,
	[','] = KEY_COMMA,
	['.'] = KEY_DOT,
	['/'] = KEY_SLASH,
	[';'] = KEY_SEMICOLON,
	[' '] = KEY_SPACE
};

static const int s_ctl_key_map[256] = 
{
	[0xbe] = 102, /*F1-->HOME*/
	[0xbf] = 59,  /*F2-->MENU*/
	[0xc0] = 158, /*F3-->BACK*/
	[0xc1] = 115, /*VOLUME_UP*/
	[0xc2] = 114, /*VOLUME_DOWN*/
	[0xc3] = KEY_FN_F5,
	[0x0d] = KEY_ENTER,
	[0x08] = KEY_BACKSPACE,
	[0xe1] = KEY_LEFTSHIFT,
	[0xe5] = KEY_CAPSLOCK,
	[0x09] = KEY_TAB,
	[0xe3] = KEY_LEFTCTRL,
	[0x1b] = KEY_ESC,
	[0x51] = KEY_LEFT,
	[0x52] = KEY_UP,
	[0x53] = KEY_RIGHT,
	[0x54] = KEY_DOWN,
	[0x55] = KEY_PAGEUP,
	[0x56] = KEY_PAGEDOWN,
	[0x63] = KEY_INSERT,
	[0xff] = KEY_DELETE,
	[0x50] = KEY_HOME,
	[0x57] = KEY_END,
};

static void uinput_input_report_key(UInput* thiz, int key, int press)
{
	struct input_event event = {0};
	gettimeofday(&event.time, NULL);

	event.type  = EV_KEY;
	event.code  = key;
	event.value = press ? 1:0;
	write(thiz->fd, &event, sizeof(event));

	return;
}

static void uinput_input_report_abs(UInput* thiz, int code, int value)
{
	struct input_event event = {0};
	gettimeofday(&event.time, NULL);

	event.type  = EV_ABS;
	event.code  = code ;
	event.value = value;
	write(thiz->fd, &event, sizeof(event));

	return;
}

static void uinput_input_report_sync(UInput* thiz)
{
	struct input_event event = {0};
	gettimeofday(&event.time, NULL);

	event.type  = EV_SYN;
	event.code  = SYN_REPORT;
	event.value = 0;
	write(thiz->fd, &event, sizeof(event));

	return;
}

void uinput_report_key_event(UInput* thiz, unsigned short key, int press)
{
	int code = 0;

  	if(key < 0x100)
  	{
  		code = s_key_map[key];
  	}
  	else
  	{
  		code = s_ctl_key_map[0xff & key];
  	}

	if(code != 0 && thiz != NULL && thiz->fd > 0)
	{
		uinput_input_report_key(thiz, code, press);	
		uinput_input_report_sync(thiz);
	}

	return;
}

void uinput_report_move_event(UInput* thiz, int x, int y)
{
	if(thiz != NULL && thiz->fd > 0)
	{
		uinput_input_report_abs(thiz, ABS_X, x);
		uinput_input_report_abs(thiz, ABS_Y, y);
		uinput_input_report_sync(thiz);
	}

	return;
}

static void uinput_report_button_event(UInput* thiz, int button, int press)
{
	if(thiz != NULL && thiz->fd > 0)
	{
		uinput_input_report_key(thiz, button, press);
		uinput_input_report_sync(thiz);
	}

	return;
}

void uinput_report_touch_event(UInput* thiz, int x, int y, int press/*1 0 -1*/)
{
	if(thiz != NULL && thiz->fd > 0)
	{
		switch(press)
		{
			case 1:
			{
				/*press*/
				uinput_input_report_key(thiz, BTN_TOUCH, 1);
				uinput_input_report_abs(thiz, ABS_X, x);
				uinput_input_report_abs(thiz, ABS_Y, y);
				uinput_input_report_abs(thiz, ABS_PRESSURE, 0xff);
				uinput_input_report_sync(thiz);
				break;
			}
			case 0:
			{
				/*release*/
				uinput_input_report_abs(thiz, ABS_PRESSURE, 0);
				uinput_input_report_key(thiz, BTN_TOUCH, 0);
				uinput_input_report_sync(thiz);
				break;
			}
			case -1:
			{
				/*move*/
				uinput_input_report_abs(thiz, ABS_X, x);
				uinput_input_report_abs(thiz, ABS_Y, y);
				uinput_input_report_sync(thiz);
				break;
			}
		}

		printf("%s: x=%d y=%d press=%d\n", __func__, x, y, press);
	}

	return;
}

void uinput_report_lbutton_event(UInput* thiz, int press)
{
	uinput_report_button_event(thiz, BTN_LEFT, press);

	return;
}

void uinput_report_mbutton_event(UInput* thiz, int press)
{
	uinput_report_button_event(thiz, BTN_MIDDLE, press);

	return;
}

void uinput_report_rbutton_event(UInput* thiz, int press)
{
	uinput_report_button_event(thiz, BTN_RIGHT, press);

	return;
}

void uinput_destroy(UInput* thiz)
{
	if(thiz != NULL)
	{
		close(thiz->fd);
		free(thiz);
	}

	return;
}

#ifdef UINPUT_TEST
int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("usage: %s key[char]|move[x,y]|button[0|1|2|3]\n", argv[0]);
		exit(0);
	}

	UInput* dev = uinput_create(NULL, 1000, 1000);
	const char* args = argv[1];
	if(strncmp(args, "key[", 4) == 0)
	{
		unsigned char c = 0;
		sscanf(args, "key[%c]", &c);
		uinput_report_key_event(dev, c, 1);
		uinput_report_key_event(dev, c, 0);
	}
	else if(strncmp(args, "move[", 5) == 0)
	{
		int x = 0;
		int y = 0;
		sscanf(args, "move[%d,%d]", &x, &y);
		uinput_report_move_event(dev, x, y);
	}
	else if(strncmp(args, "button[", 7) == 0)
	{
		int button = 0;
		sscanf(args, "button[%d]", &button);
		if(button == 0)
		{
			uinput_report_lbutton_event(dev, 1);
			uinput_report_lbutton_event(dev, 0);
		}
		else if(button == 1)
		{
			uinput_report_mbutton_event(dev, 1);
			uinput_report_mbutton_event(dev, 0);
		}
		else if(button == 2)
		{
			uinput_report_rbutton_event(dev, 1);
			uinput_report_rbutton_event(dev, 0);
		}
		else
		{
			uinput_report_touch_event(dev, 100, 100, 1);
			uinput_report_touch_event(dev, 200, 200, -1);
			uinput_report_touch_event(dev, 100, 100, 0);
		}
	}

	uinput_destroy(dev);
}
#endif
