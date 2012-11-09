#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <unistd.h>

#include "uinput.h"
#include "dev_val.h"


static useconds_t  get_interval(struct timeval last_time, struct timeval current_time)
{
	return ((current_time.tv_sec - last_time.tv_sec)*1000000 + current_time.tv_usec - last_time.tv_usec);
}

static void show_event(struct input_event* current_event)

{
	printf("%d %d %d %d %d\n", current_event->time.tv_sec, current_event->time.tv_usec,  current_event->type, current_event->code, current_event->value);

	return;
}

static void output_event(int fd, struct input_event* last_event, struct input_event* current_event)
{
	struct timeval last_time, current_time;

	if(fd > 0)
	{
		if(last_event != NULL)
		{
			last_time = last_event->time;
		} else {
			last_time = current_event->time;
			usleep(100000);
		}

		current_time = current_event->time;

		usleep(get_interval(last_time, current_time));
		gettimeofday(&current_event->time, NULL);
		write(fd, current_event, sizeof(struct input_event));
		//show_event(current_event);
	}

	return;
}

static void usage(char* argv)
{
	printf("Usage: %s [a310|a1|a810] [event file name]\n", argv);
}

int main(int argc, char* argv[])
{
	int store_fd;
	struct input_event* last_event = NULL;
	struct input_event* current_event = NULL;
	struct stat st;
	void* data;
	UInput* udev;
	char* event_filename;
	int screen_width, screen_heigh;

	if( argc < 3 )
	{
		fprintf(stderr, "error input\n");
		usage(argv[0]);
		exit(-1);
	} 

	if( strcmp("a1", argv[1]) == 0 )
	{
		screen_width = screen_width_a1;
		screen_heigh = screen_heigh_a1;
	}
	else if (strcmp("a310", argv[1]) == 0)
	{
		screen_width = screen_width_a310;
		screen_heigh = screen_heigh_a310;
	}
	else if (strcmp("a810", argv[1]) == 0)
	{
		screen_width = screen_width_a810;
		screen_heigh = screen_heigh_a810;
	}
	else
	{
		fprintf(stderr, "error input argv[1]: %s \n", argv[1]);
		usage(argv[0]);
		exit(-1);
	}
	event_filename = argv[2];
	

	if(stat(event_filename, &st) != 0)
	{
		fprintf(stderr, "file to read file stat of %s \n", event_filename);
		exit(-1);
	}

	udev = uinput_create(NULL, screen_width, screen_heigh);
	if(udev->fd < 0)
	{
		fprintf(stderr, "fail to open /dev/uinput\n");
		exit(-1);
	}

	store_fd = open(event_filename, O_RDONLY);
	if(store_fd < 0)
	{
		fprintf(stderr, "fail to open file %s\n", event_filename);
		exit(-1);
	}
	data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, store_fd, 0);

	int i = 0;
	while(1)
	{
		if(st.st_size > sizeof(struct input_event)*i) 
		{
			last_event = current_event;
			current_event = (struct input_event*)(data + sizeof(struct input_event)*i);
			if(current_event == NULL)
			{
				exit(-1);
			}
			output_event(udev->fd, last_event, current_event);
			show_event(current_event);
		} else {
			return 0;
		}
		i++;
	}

	uinput_destroy(udev);
}
