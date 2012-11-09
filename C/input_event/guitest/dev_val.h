#ifndef DEV_VAL_H
#define DEV_VAL_H

int screen_width_a1 = 320;
int screen_heigh_a1 = 480;

char* dev_name_a1[8] = {
	"/dev/input/event0",
	"/dev/input/event1",
	"/dev/input/event3",
	"/dev/input/mouse0",
	"/dev/input/mouse1",
	"/dev/input/event4",
	"/dev/input/mouse2",
	NULL
};

int screen_width_a310 = 320;
int screen_heigh_a310 = 240;

char* dev_name_a310[10] = {
	"/dev/input/event0",
	"/dev/input/event2",
	"/dev/input/event3",
	"/dev/input/mouse0",
	"/dev/input/mouse1",
	"/dev/input/event4",
	"/dev/input/mouse2",
	"/dev/input/mouse3",
	"/dev/input/event1",
	NULL
};

int screen_width_a810 = 800;
int screen_heigh_a810 = 480;

char* dev_name_a810[8] = {
	"/dev/input/event0",
	"/dev/input/event1",
	"/dev/input/event2",
	"/dev/input/event3",
	"/dev/input/mouse0",
	"/dev/input/mouse1",
	"/dev/input/mouse2",
	NULL
};


#endif
