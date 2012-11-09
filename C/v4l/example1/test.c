#include "v4l.h"
v4l_device vd;

int main()
{
	v4l_open(DEFAULT_DEVICE, &vd);
	v4l_mmap_init(&vd);
	v4l_grab_init(&vd, 320, 240);
	v4l_grab_sync(&vd);


	return 0;
}
