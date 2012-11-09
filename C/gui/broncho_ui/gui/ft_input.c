#undef LOG_TAG
#define LOG_TAG "Factory"

#include "ft_event.h"
#include "ft_type.h"
#include "ft_framebuffer.h"

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <linux/input.h>
#include <cutils/log.h>

#define FT_INPUT_DEVICE     "/dev/input/event"
#define FT_INPUT_MAX        8
#define BUFFER_SIZE         64
#define EVENT_SIZE          sizeof(struct input_event)

#ifndef ABS_MT_TOUCH_MAJOR
#define ABS_MT_TOUCH_MAJOR      0x30    /* Major axis of touching ellipse */
#endif

#ifndef ABS_MT_POSITION_X
#define ABS_MT_POSITION_X       0x35    /* Center X ellipse position */
#endif

#ifndef ABS_MT_POSITION_Y
#define ABS_MT_POSITION_Y       0x36    /* Center Y ellipse position */
#endif

/* this macro is used to tell if "bit" is set in "array"
 * it selects a byte from the array, and does a boolean AND
 * operation with a byte that only has the relevant bit set.
 * eg. to check for the 12th bit, we do (array[1] & 1<<4)
 */
#define test_bit(bit, array)    (array[bit/8] & (1<<(bit%8)))

typedef struct _FIContext FIContext;

struct _FIContext
{
    int mt_touch;
    int x_max;
    int x_min;
    int y_max;
    int y_min;
    int x_disp;
    int y_disp;
};

static FIContext fi_context;

static int get_abs_info(int fd)
{
    struct input_absinfo info;
    if(ioctl(fd, EVIOCGABS(ABS_MT_POSITION_X), &info))
    {
        LOGE("Error reading absolute controller %d for device fd %d\n",
                ABS_MT_POSITION_X, fd);
        return -1;
    }

    fi_context.x_max = info.maximum;
    fi_context.x_min = info.minimum;

    if(ioctl(fd, EVIOCGABS(ABS_MT_POSITION_Y), &info))
    {
        LOGE("Error reading absolute controller %d for device fd %d\n",
                ABS_MT_POSITION_Y, fd);
        return -1;
    }

    fi_context.y_max = info.maximum;
    fi_context.y_min = info.minimum;

    FBSurface *s = ft_frame_buffer_get_default();
    fi_context.x_disp = s->width;
    fi_context.y_disp = s->height;
    
    return 0;
}

static int get_disp_x(int x)
{
    int min = fi_context.x_min;
    int max = fi_context.x_max;

    if (!min && !max) {
        return x;
    }

    return ((x - min) * fi_context.x_disp) / (max - min);
}

static int get_disp_y(int y)
{
    int min = fi_context.y_min;
    int max = fi_context.y_max;

    if (!min && !max) {
        return y;
    }

    return ((y - min) * fi_context.y_disp) / (max - min);
}

static int *open_all_inputs(int *maxfd)
{
    static int fds[FT_INPUT_MAX+1] = {0};

    uint8_t ev_bitmask[(EV_MAX+7)/8];
    uint8_t abs_bitmask[(ABS_MAX+7)/8];
    char input[BUFFER_SIZE] = {0};
    int fd, max = 0, i = 0, n = 0;

    for (i = 0; i < FT_INPUT_MAX; i++)
    {
        snprintf(input, BUFFER_SIZE, "%s%d", FT_INPUT_DEVICE, i);

        fd = open(input, O_RDONLY);
        if (fd < 0)
            break;

        memset(ev_bitmask, 0, sizeof(ev_bitmask));
        ioctl(fd, EVIOCGBIT(0, sizeof(ev_bitmask)), ev_bitmask);

        if (!test_bit(EV_KEY, ev_bitmask))
            continue;

        memset(abs_bitmask, 0, sizeof(abs_bitmask));
        ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(abs_bitmask)), abs_bitmask);
        
        // Is this a new modern multi-touch driver?
        if (test_bit(ABS_MT_TOUCH_MAJOR, abs_bitmask)
                && test_bit(ABS_MT_POSITION_X, abs_bitmask)
                && test_bit(ABS_MT_POSITION_Y, abs_bitmask))
        {
            get_abs_info(fd);
            fi_context.mt_touch = 1;
        }

        fds[n++] = fd;

        if (fd > max)
            max = fd;
    }

    *maxfd = max;
    return fds;
}

static void init_fds(int *fds, fd_set *set)
{
    int i = 0;

    FD_ZERO(set);

    for (i = 0; fds[i]; i++)
    {
        FD_SET(fds[i], set);
    }
}

static void dispatch_event(struct input_event *e)
{
    static int x = 0, y = 0, key, code = 0, press = 0;
    static int height = 0;

    FTMouseEvent me;
    FTKeyEvent ke;
    FTEvent *event;
    struct fb_var_screeninfo vinfo;

    // LOGD("EVENT: type=%d, code=%d, value=%d, x=%d, y=%d\n", 
    //         e->type, e->code, e->value, x, y);

    switch (e->type)
    {
        case EV_ABS:
            if (e->code == REL_X || e->code == ABS_MT_POSITION_X)
            {
                x = get_disp_x(e->value);
            }
            else if (e->code == REL_Y || e->code == ABS_MT_POSITION_Y)
            {
                y = get_disp_y(e->value);
            }
            else if (e->code == ABS_MT_TOUCH_MAJOR) // && e->value == 0)
            {
                key = e->value;
            }
            break;

        case EV_SYN:
            if (code == FT_KEY_MOUSE || e->code == 2) // SYN_MT_REPORT)
            {
                event = (FTEvent *)&me;
                event->type = key ? (press ? FE_MOUSE_MOVE : FE_MOUSE_PRESS) : FE_MOUSE_RELEASE;

                me.x = x;
                me.y = y;
                press = key ? 1 : 0;
                ft_event_put(event);
            }
            break;

        case EV_REL:
            event = (FTEvent *)&ke;

            if (e->code == REL_X)
            {
                if (e->value < 0)
                    ke.key = FT_KEY_LEFT;
                else
                    ke.key = FT_KEY_RIGHT;
            }
            else /* REL_Y */
            {
                if (e->value < 0)
                    ke.key = FT_KEY_UP;
                else
                    ke.key = FT_KEY_DOWN;
            }

            event->type = FE_KEY_PRESS;
            ft_event_put(event);

            event->type = FE_KEY_RELEASE;
            ft_event_put(event);
            break;

        case EV_KEY:
            if (e->code == FT_KEY_MOUSE)
            {
                key = e->value;
            }
            else
            {
                event = (FTEvent *)&ke;
                event->type = e->value ? FE_KEY_PRESS : FE_KEY_RELEASE;

                ke.key = e->code;

                ft_event_put(event);
            }

            code = e->code;
            break;
    }
}

void wait_for_events()
{
    struct input_event events[BUFFER_SIZE];
    struct input_event *e;

    fd_set rfds;
    int maxfd, retval, byte = 0;
    unsigned int i;
    int *fds;

    fds = open_all_inputs(&maxfd);

    init_fds(fds, &rfds);

    while ((retval = select(maxfd + 1, &rfds, NULL, NULL, NULL)))
    {
        if (retval < 0)
            continue;

        for (i = 0; fds[i]; i++)
        {
            if (FD_ISSET(fds[i], &rfds))
            {
                byte = read(fds[i], events, EVENT_SIZE * BUFFER_SIZE);
                break;
            }
        }

        init_fds(fds, &rfds);

        if ((unsigned int)byte < EVENT_SIZE)
            continue;

        for (i = 0; i < byte / EVENT_SIZE; i++)
        {
            e = &events[i];

            dispatch_event(e);
        }
    }
}

