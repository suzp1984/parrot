#undef LOG_TAG
#define LOG_TAG "Factory"

#include "ft_led.h"
#include "ft_config.h"
#include "ft_textpad.h"
#include "gui/ft_button.h"
#include "hw/hw_led.h"
#include "messages.h"
#include "hw/hw_comm.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <cutils/log.h>
#include <cutils/properties.h>

extern FTColor ft_color_r;
extern FTColor ft_color_g;
extern FTColor ft_color_w;

static FTWindow *ft_lcm_window_new();

typedef struct _LedItem LedItem;

struct _LedItem
{
    int         desc_id;
    FBHandler   handler;
};

static void on_all_led_handler(FTButton *button, void *data)
{
    if (strcmp(button->text, "All LEDs") == 0)
    {
        int max = 0;

        ft_button_set_text(button, "All LEDs ");
        ft_button_set_color(button, &ft_color_g);

        hw_led_get_range(HL_DEVICE_KEYBOARD, NULL, &max);
        hw_led_set(HL_DEVICE_KEYBOARD, max ? max : 1);
        hw_led_set(HL_DEVICE_TRACKBALL, HL_DEV_LED_FULL);
        hw_led_set(HL_DEVICE_FLASH_LIGHT, -1);

        hw_led_set(HL_DEVICE_LED_R, HL_DEV_LED_FULL);
        usleep(200000);
        hw_led_set(HL_DEVICE_LED_G, HL_DEV_LED_FULL);
        usleep(200000);
        hw_led_set(HL_DEVICE_LED_B, HL_DEV_LED_FULL);
        usleep(200000);
    }
    else
    {
        ft_button_set_text(button, "All LEDs");
        ft_button_set_color(button, &ft_color_w);

        hw_led_set(HL_DEVICE_KEYBOARD, 0);
        hw_led_set(HL_DEVICE_TRACKBALL, 0);
        hw_led_set(HL_DEVICE_FLASH_LIGHT, 0);
        hw_led_set(HL_DEVICE_LED_R, 0);
        hw_led_set(HL_DEVICE_LED_G, 0);
        hw_led_set(HL_DEVICE_LED_B, 0);
    }
}

static void on_keyboard_led_handler(FTButton *button, void *data)
{
    int max = 0;

    hw_led_get_range(HL_DEVICE_KEYBOARD, NULL, &max);
    hw_led_set(HL_DEVICE_KEYBOARD, max ? max : 1);
    ft_button_set_color(button, &ft_color_g);

    usleep(200000);

    hw_led_set(HL_DEVICE_KEYBOARD, 0);
    ft_button_set_color(button, &ft_color_w);
}

static void on_trackball_handler(FTButton *button, void *data)
{
    hw_led_set(HL_DEVICE_TRACKBALL, HL_DEV_LED_FULL);
    ft_button_set_color(button, &ft_color_g);

    usleep(200000);

    hw_led_set(HL_DEVICE_TRACKBALL, 0);
    ft_button_set_color(button, &ft_color_w);
}

static void on_flash_light_handler(FTButton *button, void *data)
{
    hw_led_set(HL_DEVICE_FLASH_LIGHT, -1);
    ft_button_set_color(button, &ft_color_g);

    usleep(200000);

    hw_led_set(HL_DEVICE_FLASH_LIGHT, 0);
    ft_button_set_color(button, &ft_color_w);
}

static void on_rgb_handler(FTButton *button, void *data)
{
    ft_button_set_color(button, &ft_color_g);

    hw_led_set(HL_DEVICE_LED_R, HL_DEV_LED_FULL);
    usleep(200000);

    hw_led_set(HL_DEVICE_LED_G, HL_DEV_LED_FULL);
    usleep(200000);

    hw_led_set(HL_DEVICE_LED_B, HL_DEV_LED_FULL);
    usleep(200000);

    hw_led_set(HL_DEVICE_LED_R, 0);
    hw_led_set(HL_DEVICE_LED_G, 0);
    hw_led_set(HL_DEVICE_LED_B, 0);

    ft_button_set_color(button, &ft_color_w);
}

static void on_lcm_bl_handler(FTButton *button, void *data)
{
    int max = 0, value, step, i;

    if (!hw_led_get_range(HL_DEVICE_LCD, NULL, &max))
    {
        return;
    }

    ft_button_set_color(button, &ft_color_g);

    value = hw_led_get(HL_DEVICE_LCD);
    step = max / 16;

    for (i = 0; i < max; i += step)
    {
        hw_led_set(HL_DEVICE_LCD, i);

        usleep(100 * 1000);
    }

    hw_led_set(HL_DEVICE_LCD, value);
    ft_button_set_color(button, &ft_color_w);
}

static void ft_lcm_window_draw(FTWidget *widget)
{
}

static void ft_lcm_window_handler(FTEvent *event, void *data)
{
    FTWindow *window = (FTWindow *)data;

    if (event->type == FE_KEY_RELEASE ||
        event->type == FE_MOUSE_RELEASE)
    {
        ft_window_close(window);
    }
}

static const LedItem fk_all_labels_A1[] =
{
    {KB_LED_TEXT, on_keyboard_led_handler},
    {TRACEBALL_TEXT, on_trackball_handler},
    {FLASHLIGHT_TEXT, on_flash_light_handler},
    {RGB_TEXT, on_rgb_handler},
    {ALL_LEDS_TEXT, on_all_led_handler},
    {MAIN_LCM_BL_TEXT, on_lcm_bl_handler},
    {-1, NULL}
};

static const LedItem fk_all_labels_N802[] = 
{
    {KB_LED_TEXT, on_keyboard_led_handler},
    {MAIN_LCM_BL_TEXT, on_lcm_bl_handler},
    {-1, NULL}
};

static const LedItem fk_all_labels_N302[] = 
{
    {MAIN_LCM_BL_TEXT, on_lcm_bl_handler},
    {-1, NULL}
};

static const int hl_device_A1[] = {
    HL_DEVICE_KEYBOARD,
    HL_DEVICE_TRACKBALL,
    HL_DEVICE_FLASH_LIGHT,
    HL_DEVICE_LED_R,
    HL_DEVICE_LED_G,
    HL_DEVICE_LED_B,
    -1
};

int hl_device_N802[] = {
    HL_DEVICE_KEYBOARD,
    -1
};

static FTWindow *ft_lcm_window_new()
{
    FTWindow *window;
    FTWidget *widget;

    window = ft_window_new();
    widget = (FTWidget *)window;

    widget->draw = ft_lcm_window_draw;
    widget->handler = ft_lcm_window_handler;
    widget->data = window;

    return window;
}

void ft_led_destroy(FTWidget *widget)
{
    int i;
    const int *hl_device = hl_device_N802;
    ft_textpad_destroy(widget);

    if (strcmp(product_name_get(), "A1") == 0)
    {
        hl_device = hl_device_A1;
    }
    else
    {
        hl_device = hl_device_N802;
    }

    for (i = 0; hl_device[i] != -1; i++)
    {
         hw_led_set(hl_device[i], 0);
    }
}

void set_lcm_bl_max()
{
    int max = 0;

    if (!hw_led_get_range(HL_DEVICE_LCD, NULL, &max))
    {
        return;
    }

    hw_led_set(HL_DEVICE_LCD, max);
}

FTWindow *ft_led_new()
{
    int i;
    const LedItem *fk_all_labels = fk_all_labels_N302;
    FTWindow *window;
    FTWidget *widget;
    FTButton *button, *all_button;
    char keyboard_light[PROPERTY_VALUE_MAX] = {0};

    property_get("ro.broncho.KBL", keyboard_light, "y");

    window = ft_textpad_new(NULL, 0);
    widget = (FTWidget *)window;

    /*Modified by liuhairong*/
    if (strcmp(product_name_get(), "A1") == 0)
    {
        fk_all_labels = fk_all_labels_A1;
    }
    else if (strcmp(product_name_get(), "N305") == 0)
    {
	 fk_all_labels = fk_all_labels_N302;
    }
    else if(!strcmp(keyboard_light, "y"))
    {
        fk_all_labels = fk_all_labels_N802;
    }

    for (i = 0; fk_all_labels[i].desc_id != -1; i++)
    {
        button = ft_button_new(_(fk_all_labels[i].desc_id));
        ft_button_set_handler(button, fk_all_labels[i].handler, NULL);
        ft_window_add(window, (FTWidget *)button, i);
    }
    /*End by liuhairong*/

    widget->destroy = ft_led_destroy;
    // ft_window_show(window);

    ft_textpad_set_id(window, FT_ITEM_LED);

    return window;
}

