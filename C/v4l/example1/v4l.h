#ifndef _V4L_H_
#define _V4L_H_

#include <sys/types.h>
#include <linux/videodev.h> 

#define DEFAULT_DEVICE "/dev/video0"

struct _v4l_struct
{
	int fd;
	struct video_capability capability;
	struct video_picture picture;
	struct video_mmap mmap;
	struct video_mbuf mbuf;
	unsigned char* map;
	int frame_current;
	int frame_using[VIDEO_MAXFRAME];
};
     
typedef struct _v4l_struct v4l_device;

int v4l_open(char* dev, v4l_device* vd);
int v4l_close(v4l_device* vd);
int v4l_get_capability(v4l_device* vd);
int v4l_get_picture(v4l_device* vd);
int v4l_get_mbuf(v4l_device* vd);
int v4l_set_picture(v4l_device* vd, int br, int bue, int col, int cont, int white);
int v4l_grab_picture(v4l_device* vd, unsigned int size);
int v4l_mmap_init(v4l_device* vd);
int v4l_grab_init(v4l_device* vd, int width, int height);
int v4l_grab_frame(v4l_device* vd, int frame);
int v4l_grab_sync(v4l_device* vd);
