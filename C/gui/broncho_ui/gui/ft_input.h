#include <linux/input.h>

#ifndef _FT_INPUT_H_
#define _FT_INPUT_H_

#define FT_KEY_HOME     KEY_HOME
#define FT_KEY_BACK     KEY_BACK
#define FT_KEY_SEND     KEY_SEND
#define FT_KEY_END      KEY_END
#define FT_KEY_OK       BTN_MOUSE   //KEY_CENTER
#define FT_KEY_MENU     KEY_MENU    //KEY_SOFT1
#define FT_KEY_HOME     KEY_HOME
#define FT_KEY_SEARCH   KEY_SEARCH

#define FT_KEY_VOL_ADD  KEY_VOLUMEUP
#define FT_KEY_VOL_SUB  KEY_VOLUMEDOWN
#define FT_KEY_POWER    KEY_POWER
#define FT_KEY_PHONE    KEY_PHONE

#define FT_KEY_UP       KEY_UP
#define FT_KEY_LEFT     KEY_LEFT
#define FT_KEY_RIGHT    KEY_RIGHT
#define FT_KEY_DOWN     KEY_DOWN 

#define FT_KEY_MOUSE    BTN_TOUCH

void wait_for_events();

#endif/*_FT_INPUT_H_*/
