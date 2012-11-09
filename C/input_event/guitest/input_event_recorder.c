#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/poll.h>
#include <time.h>

#include "dev_val.h"

static void usage(char* argv)
{
	printf("Usage: %s [a310|a1|a810] \n", argv);
}

static char* gen_filename()
{
	char* fmt = "%F-%H-%M-%S";
	char outstr[50];
	char* ret;
	time_t t;
	struct tm* tmp;

	if((ret = malloc(50)) == NULL)
	{
		fprintf(stderr, "error when malloc to ret\n");
		exit(-1);
	}

	t = time(NULL);
	tmp = localtime(&t);

	if(strftime(outstr, sizeof(outstr), fmt, tmp) == 0)
	{
		fprintf(stderr, "strftime returned 0\n");
		exit(EXIT_FAILURE);
	}

	strcpy(ret, outstr);
	
	return ret;
}

static void show_event(struct input_event* event)
{
	printf("%d %d %d %d %d\n", event->time.tv_sec, event->time.tv_usec,  event->type, event->code, event->value);

	return;
}

static void store_event(int fd, struct input_event* event)
{
	if(fd > 0)
	{
		write(fd, event, sizeof(struct input_event));
	}

	return;
}

static int open_dev(const char* device_name)
{
	int fd = -1;
	fd = open(device_name, O_RDWR);
	if(fd < 0) {
		fprintf(stderr, "error opening: %s\n", device_name);
		return -1;
	}

	return fd;
}

int main(int argc, char* argv[])
{
	int i = 0;
	int fd;
	struct pollfd* mfds = (struct pollfd*)calloc(1, sizeof(struct pollfd));
	int mfd_count = 0;
	int nr;
	struct input_event event;
	int store_fd;
	char** dev_name;

	if(argc < 2)
	{
		fprintf(stderr, "error input\n");
		usage(argv[0]);
		exit(-1);
	}

	if(strcmp(argv[1], "a1") == 0)
	{
		dev_name = dev_name_a1;
	}
	else if (strcmp(argv[1], "a310") == 0)
	{
		dev_name = dev_name_a310;
	} 
	else if (strcmp(argv[1], "a810") == 0)
	{
		dev_name = dev_name_a810;
	}
	else 
	{
		fprintf(stderr, "error input argv[1]: %s\n", argv[1]);
		usage(argv[0]);
		exit(-1);
	}
	 
	i = 0;
	while(dev_name[i] != NULL)
	{

		printf("open %s\n", dev_name[i]);
		fd = open_dev(dev_name[i]);
		if(fd > 0)
		{
			struct pollfd* new_mfds = (struct pollfd*)realloc(mfds, sizeof(struct pollfd)*(mfd_count + 1));

			if(new_mfds == NULL)
			{
				fprintf(stderr, "realloc out of memeory\n");
				return -1;
			}

			mfds = new_mfds;

			mfds[i].fd = fd;
			mfds[i].events = POLLIN;
			mfd_count++;
		}

		i++;
	}

	//open store file
	store_fd = open(gen_filename(), O_RDWR|O_CREAT);
	if(store_fd < 0)
	{
		fprintf(stderr, "fail to open record file\n");
		exit(-1);
	}
	
	while(1)
	{

		nr = poll(mfds, mfd_count, 0);
		if(nr <= 0)
			continue;

		for(i = 0; i < mfd_count; i++)
		{
			if(mfds[i].revents == POLLIN)
			{
				int ret = read(mfds[i].fd, &event, sizeof(event));
				if(ret == sizeof(event))
				{
					show_event(&event);
					store_event(store_fd, &event);
				}

			}

		}

	}
	return 0;
}
