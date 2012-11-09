#undef LOG_TAG
#define LOG_TAG "Factory"

#include "gui/ft_input.h"
#include "gui/ft_window.h"
#include "gui/ft_button.h"
#include "gui/ft_status_button.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/reboot.h>
#include <cutils/log.h>
#include <cutils/properties.h>

#define BUF_LEN_MAX             128
#define BLUETOOTH_TEST_NR       1
#define GPS_SCAN_NR             1

#define CALIB_FILE              "/data/etc/pointercal"
#define TEXT_DEVICE_FOUND       "The device detected!"
#define TEXT_DEVICE_NOT_FOUND   "The device is not detected!"
#define TEXT_MODULE_NOT_FOUND   "The module is not detected!"

#define FT_VIBRATOR_TIME 255000

typedef struct _FTContext FTContext;

struct _FTContext
{
    int         autotest;
    int         start;
    int         result;
    int         camera_id;
    int         bl_value;
    int         hdmi_close;
};

static FTContext ft_context;

FTColor ft_color_r = {0xff, 0, 0, 0};
FTColor ft_color_g = {0, 0xff, 0, 0};
FTColor ft_color_b = {0, 0, 0xff, 0};
FTColor ft_color_y = {0xff, 0xff, 0, 0};
FTColor ft_color_w = {0xff, 0xff, 0xff, 0};

int main(int argc, char *argv[])
{
    FTWindow *window;
    char text[64];

	window = ft_notepad_new();
	ft_window_set_sensitive(window, 0);
    ft_window_show(window);

    memset(text, 0, sizeof(text));
	sprintf(text, "======== [hello] ========");
	ft_notepad_append(window, text, NULL); 
    /* event loop */
    wait_for_events();

    /* unmap, close fb device */
    ft_frame_buffer_close();

    return 0;
}

