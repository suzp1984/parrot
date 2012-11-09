#undef LOG_TAG
#define LOG_TAG "Factory"

#include "ft_testcase.h"
#include "hw/hw_comm.h"
#include "hw/hw_gps.h"
#include "hw/hw_wifi.h"
#include "hw/hw_bluetooth.h"
#include <cutils/log.h>
#include <string.h>

int ft_testcase_ic_encryption()
{
    int status = 0;
    int result = 0;

    result = hw_ic_challenge(&status);

    if (status == 0 || result <= 0)
        return 0;
    else 
        return 1;
}

int ft_testcase_gsensor()
{
    return hw_detect_g_sensor();
}

int ft_testcase_lsensor()
{
    return hw_detect_l_sensor();
}

int ft_testcase_gps()
{
    //return hw_detect_gps();
    if (hw_gps_open() <= 0)
        return 0;

    if (hw_gps_scan() <= 0)
        return 0;

    hw_gps_close();

    return 1;
}

int ft_testcase_sdcard()
{
    return hw_detect_memory_card();
}

int ft_testcase_usbhost()
{
    return hw_detect_usb_host();
}

int ft_testcase_wifi()
{
    int result = 0;

    while (!hw_wireless_ready()) sleep(1);

    if (!hw_wifi_open())
        return 0;

    char *aps = hw_wifi_scan();

    if (strncmp(aps, "No", 2))
        result = 1;

    hw_wifi_close();
    free(aps);

    return result;
}

int ft_testcase_bluetooth()
{
    while (!hw_wireless_ready()) sleep(1);

    if (!hw_bluetooth_enable())
        return 0;

    if (!hw_bluetooth_disable())
        return 0;

    return 1;
}

