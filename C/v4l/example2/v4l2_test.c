#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

#define DCAM_DEV               "/dev/video0"

#define CAME_WIDTH   640
#define CAME_HEIGHT  480
#define DEFAULT_QERY_BUF 4
#define DEFAULT_IMAGE_FILE "./one.img"

struct buffer {
	void* start;
	unsigned int length;
};

static void process_image(struct buffer buf)
{
	int ret;
	//system("rm ./one.img");
	usleep(100000);
	int fd = open(DEFAULT_IMAGE_FILE, O_RDWR | O_CREAT);

	if (fd < 0) {
		printf("%s: open %s fail\n", __func__, DEFAULT_IMAGE_FILE);
		return;
	}

	ret = write(fd, buf.start, buf.length);

}

int main(int argc, char* argv[])
{
	int i = 0;
	int v4l2_fd = -1;
	struct v4l2_capability cap;
	struct v4l2_cropcap cropcap;
	struct v4l2_crop crop;
	struct v4l2_format fmt;
	struct v4l2_streamparm streamparm;
	struct v4l2_requestbuffers req;
	struct v4l2_fmtdesc fmtdesc;
	struct v4l2_buffer g_buf;
	enum v4l2_buf_type type;

	struct buffer* buffers;

	v4l2_fd = open(DCAM_DEV, O_RDWR, 0);
	if (v4l2_fd < 0) {
		printf("can't open dev: %s\n", DCAM_DEV);
		return -1;
	}

	/* VIDIOC_QUERYCAP */
	memset(&cap, 0, sizeof(cap));
	if (ioctl(v4l2_fd, VIDIOC_QUERYCAP, &cap) < 0) {
		printf("%s: VIDIOC_QUERYCAP fail\n", __func__);
	} else {
		printf("driver: %s\n", cap.driver);
		printf("card: %s\n", cap.card);
		printf("buf_info: %s\n", cap.bus_info);
		printf("version: %d\n", cap.version);
		printf("capbility: 0x%x\n", cap.capabilities);
	}

	/* VIDIOC_ENUM_FMT */
	fmtdesc.index = 0;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	printf("Support format:\n");
	while(ioctl(v4l2_fd, VIDIOC_ENUM_FMT, &fmtdesc) != -1) {
		printf("\t%d.%s\n", fmtdesc.index + 1, fmtdesc.description);
		fmtdesc.index ++;
	}

	/* VIDIOC_S_PARM */
	memset(&streamparm, 0, sizeof(streamparm));
	streamparm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	streamparm.parm.capture.capturemode = 0;

	if (ioctl(v4l2_fd, VIDIOC_S_PARM, &streamparm) < 0) {
		printf("%s: VIDIOC_S_PARM failed\n", __func__);
	}


#if 1
	/* VIDIOC_TRY_FMT*/
	memset(&fmt, 0, sizeof(fmt));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	//fmt.fmt.pix.width = CAME_WIDTH;
	//fmt.fmt.pix.height = CAME_HEIGHT;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	//fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
		
	if (ioctl(v4l2_fd, VIDIOC_TRY_FMT, &fmt) < 0) {
		printf("%s: VIDIOC_S_FMT failed\n", __func__);
	}
#endif 
#if 1

	/* VIDIOC_G_FMT */
	memset(&fmt, 0, sizeof(fmt));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl(v4l2_fd, VIDIOC_G_FMT, &fmt) < 0) {
		printf("%s: VIDIOC_G_FMT failed\n", __func__);
	} else {
		printf("pix.width: %d\n", fmt.fmt.pix.width);
		printf("pix.height: %d\n", fmt.fmt.pix.height);
	} 
#endif 

	/* VIDIOC_CROPCAP */
	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl(v4l2_fd, VIDIOC_CROPCAP, &cropcap) < 0) {
		printf("%s: VIDIOC_CROPCAP failed\n", __func__);
	} else {
		printf("cropcap.bounds.left = %d\n", cropcap.bounds.left);
		printf("cropcap.bounds.top = %d\n", cropcap.bounds.top);
		printf("cropcap.bounds.width = %d\n", cropcap.bounds.width);
		printf("cropcap.bounds.height = %d\n", cropcap.bounds.height);

		printf("cropcap.defrect.left = %d\n", cropcap.defrect.left);
		printf("cropcap.defrect.top = %d\n", cropcap.defrect.top);
		printf("cropcap.defrect.width = %d\n", cropcap.defrect.width);
		printf("cropcap.defrect.height = %d\n", cropcap.defrect.height);
	}

	/* VIDIOC_S_CROP */
	crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	crop.c.left = 0;
	crop.c.top= 0;
	crop.c.width = cropcap.defrect.width;
	crop.c.height = cropcap.defrect.height;

	if (ioctl(v4l2_fd, VIDIOC_S_CROP, &crop) < 0) {
		printf("%s: VIDIOC_S_CROP fail\n", __func__);
	} 

	/* VIDIOC_REQBUFS */
	memset(&req, 0, sizeof(req));
	req.count = DEFAULT_QERY_BUF;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;

	if (ioctl(v4l2_fd, VIDIOC_REQBUFS, &req) < 0) {
		printf("%s: VIDIOC_REQBUFS fail\n", __func__);
	}

	/* mmap buffer */
	buffers = (struct buffer*)calloc(req.count, sizeof(struct buffer));
	
	if (buffers == NULL) {
		printf("%s: calloc failed\n", __func__);
	}

	/* VIDIOC_QBUF*/
	for (i = 0; i < DEFAULT_QERY_BUF; i++) {
		struct v4l2_buffer buf;
		memset(&buf, 0, sizeof(struct v4l2_buffer));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		if (ioctl(v4l2_fd, VIDIOC_QUERYBUF, &buf) < 0) {
			printf("%s: VIDIOC_QUERYBUF fail\n", __func__);
		}

		buffers[i].length = buf.length;
		buffers[i].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, 
				MAP_SHARED, v4l2_fd,  buf.m.offset);
		if (MAP_FAILED == buffers[i].start) {
			printf("%s: mmap v4l2_fd fail\n", __func__);
		}

		if (ioctl(v4l2_fd, VIDIOC_QBUF, &buf) < 0) {
			printf("%s: VIDIOC_QBUF fail\n", __func__);
		}
	}

	/* VIDIOC_STREAMON */
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl(v4l2_fd, VIDIOC_STREAMON, &type) < 0) {
		printf("%s: VIDIOC_STREAMON fail\n", __func__);
	}

	/* get one buf */
	memset(&g_buf, 0, sizeof(struct v4l2_buffer));
	g_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	g_buf.memory = V4L2_MEMORY_MMAP;

	if (ioctl(v4l2_fd, VIDIOC_DQBUF, &g_buf) < 0) {
		printf("%s: VIDIOC_DQBUF fail\n", __func__);
	}

	process_image(buffers[g_buf.index]);

	if (ioctl(v4l2_fd, VIDIOC_QBUF, &g_buf) < 0) {
		printf("%s: VIDIOC_QBUF faile after VIDIOC_DQBUF\n", __func__);
	}

	/* VIDIOC_STREAMOFF */
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl(v4l2_fd, VIDIOC_STREAMOFF, &type) < 0) {
		printf("%s: VIDIOC_STREAMOFF fail\n", __func__);
	}

	for (i = 0; i < DEFAULT_QERY_BUF; i++) {
		munmap(buffers[i].start, buffers[i].length);
	}

	close(v4l2_fd);
	return 0;
}

