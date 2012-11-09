#undef LOG_TAG
#define LOG_TAG "Factory"

#include "ft_framebuffer.h"
#include "ft_type.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <cutils/log.h>

#define FRAMEBUFFER_DEV_NAME    "/dev/graphics/fb0"

struct FBContext
{
    int                         fd;
    int                         screen_size;
    char                       *buffer;
    struct fb_var_screeninfo    vinfo;
    struct fb_fix_screeninfo    finfo;
    FBSurface                   surface;
    int                         hdmi_switch;
};

struct FBContext fb_context;

void ft_frame_hdmi_switch_tag()
{
    fb_context.hdmi_switch = 1;
}

FBSurface *ft_frame_buffer_get_default()
{
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    char *buffer;

    if (fb_context.fd && !fb_context.hdmi_switch)
        return &fb_context.surface;

    int fd = open(FRAMEBUFFER_DEV_NAME, O_RDWR);

    if (fd < 0) 
    {
        perror("open "FRAMEBUFFER_DEV_NAME);
        return NULL;
    }

    if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo)) 
    {
        perror("ioctl FBIOGET_FSCREENINFO");
        return NULL;
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo)) 
    {
        perror("ioctl FBIOGET_VSCREENINFO");
        return NULL;
    }

    int screen_size = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    buffer = (char *)mmap(0, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    memset(buffer, 0, screen_size);

    fb_context.fd = fd;
    fb_context.finfo = finfo;
    fb_context.vinfo = vinfo;
    fb_context.buffer = buffer;
    fb_context.screen_size = screen_size;

    fb_context.surface.width  = vinfo.xres;
    fb_context.surface.height = vinfo.yres;
    fb_context.surface.depth  = vinfo.bits_per_pixel / 8;
    fb_context.surface.buffer = buffer;
    fb_context.surface.size   = screen_size;

    fb_context.hdmi_switch = 0;
    printf("Mode: %dx%d %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    return &fb_context.surface;
}

int ft_frame_buffer_get_vinfo(struct fb_var_screeninfo *vinfo)
{
    if (!fb_context.fd || vinfo == NULL)
    {
        return FT_FAIL;
    }

    *vinfo = fb_context.vinfo;
    
    return FT_SUCCESS;
}

int ft_frame_buffer_get_finfo(struct fb_fix_screeninfo *finfo)
{
    if (!fb_context.fd || finfo == NULL)
    {
        return FT_FAIL;
    }

    *finfo = fb_context.finfo;

    return FT_SUCCESS;
}

void ft_frame_buffer_close()
{
    if (!fb_context.fd)
    {
        return;
    }

    munmap(fb_context.buffer, fb_context.screen_size);
    close(fb_context.fd);

    memset(&fb_context, 0, sizeof(fb_context));
}

/*
int output_mode_switch(enum EMXX_FB_OUTPUT_MODE mode)
{
    int fd;
    enum EMXX_FB_OUTPUT_MODE old_mode;
    struct scrn_modes old_scrn_modes;

    fd = open(FRAMEBUFFER_DEV_NAME, O_RDWR);
    if (fd < 0)     {
        LOGD("can't open %s\n", FRAMEBUFFER_DEV_NAME);
        return -1;
    }

    if (ioctl(fd, EMXX_FB_GET_OUTPUT, &old_mode) < 0)
        goto fail;
    if (old_mode != mode) {
        if (ioctl(fd, EMXX_FB_SET_OUTPUT, &mode) < 0)
            goto fail;
        LOGD("switch fb output mode from %d to %d\n", old_mode, mode);
    }

    if (ioctl(fd, EMXX_FB_GET_MODES, &old_scrn_modes) < 0)
        goto fail;
    LOGD("scrn modes: tc_enable %d t_color 0x%x alpha 0x%x page %d rot %d, update %d\n",
            old_scrn_modes.tc_enable, old_scrn_modes.t_color,
            old_scrn_modes.alpha,
            old_scrn_modes.page, old_scrn_modes.rot, old_scrn_modes.update);
    if (ioctl(fd, EMXX_FB_UPDATE_SCRN, &old_scrn_modes) < 0)
        goto fail;

    close(fd);
    return 0;
fail:
    LOGD("%s is not a framebuffer.\n", FRAMEBUFFER_DEV_NAME);
    close(fd);
    return -1;
}
*/

