#include <stdio.h>
#include <sys/mman.h>
#include "v4l.h"

int v4l_open(char* dev, v4l_device* vd) 
{
	if(!dev)
		dev = DEFAULT_DEVICE;

	if((vd->fd = open(dev, O_RDWR)) < 0) 
	{
		perror("v4l_open: ");
		return -1;
	}

	if(v4l_get_capability(vd)) return -1;
	if(v4l_get_picture(vd)) return -1;

	return 0;
}

int v4l_close(v4l_device* vd)
{
	close(vd->fd);
	return 0;
}

int v4l_get_capability(v4l_device* vd)
{
	if(ioctl(vd->fd, VIDIOCGCAP, &(vd->capability)) < 0)
	{
		perror("v4l_get_capability: ");
		return -1;
	}

	return 0;
}

int v4l_get_picture(v4l_device* vd)
{
	if(ioctl(vd->fd, VIDIOCGPICT, &(vd->picture)) < 0) 
	{
		perror("v4l_get_picture: ");
		return -1;
	}

	return 0;
}

int v4l_get_mbuf(v4l_device* vd)
{
	if(ioctl(vd->fd, VIDIOCGMBUF, &(vd->mbuf)) < 0) 
	{
		perror("v4l_get_mbuf: ");
		return -1;
	}

	return 0;
}

int v4l_set_picture(v4l_device* vd, int br, int hue, int col, int cont, int white)
{
	if(br)
	{
		vd->picture.brightness = br;
	}

	if(hue)
	{
		vd->picture.hue = hue;
	}

	if(col)
	{
		vd->picture.color = col;
	}

	if(cont)
	{
		vd->picture.contrast = cont;
	}

	if(white)
	{
		vd->picture.whiteness = white;
	}

	if(ioctl(vd->fd, VIDIOCSPICT, &(vd->picture)) < 0)
	{
		perror("v4l_set_picture: ");
		return -1;
	}

	return 0;
}

int v4l_grab_picture(v4l_device* vd, unsigned int size)
{
	if(read(vd->fd, &(vd->map), size) == 0)
	{
		return -1;
	}

	return 0;
}


int v4l_mmap_init(v4l_device* vd)
{
	if(v4l_get_mbuf(vd) < 0)
	{
		return -1;
	}

	if((vd->map = mmap(0, vd->mbuf.size, PROT_READ|PROT_WRITE, MAP_SHARED, vd->fd, 0)) < 0) 
	{
		perror("v4l_mmap_init: mmap");
		return -1;
	}

	return 0;
}

int v4l_grab_init(v4l_device* vd, int width, int height)
{
	vd->mmap.width = width;
	vd->mmap.height = height;
	vd->mmap.format = vd->picture.palette;
	vd->frame_current = 0;
	vd->frame_using[0] = FALSE;
	vd->frame_using[1] = FALSE;

	return v4l_grab_frame(vd, 0);
}

int v4l_grab_frame(v4l_device* vd, int frame)
{
	if(vd->frame_using[frame]) 
	{
		fprintf(stderr, "v4l_grab_frame: frame %d is already used. \n", frame);
		return -1;
	}

	vd->mmap.frame = frame;
	if(ioctl(vd->fd, VIDIOCMCAPTURE, &(vd->mmap)) < 0) 
	{
		perror("v4l_grab_frame");
		return -1;
	}

	vd->frame_using[frame] = TRUE;
	vd->frame_current = frame;

	retrun 0;
}

int v4l_grab_sync(v4l_device* vd)
{
	if(ioctl(vd->fd, VIDIOCSYNC, &(vd->frame_current)) < 0) 
	{
		perror("v4l_grab_sync");
	}

	vd->frame_using[vd->frame_current] = FALSE;
	return 0;
}
