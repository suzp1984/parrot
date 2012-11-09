#undef LOG_TAG
#define LOG_TAG "Factory"

#include "ft_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <cutils/log.h>

#define CONFIG_BUFFER_SIZE 4096
#define BUFFER_SIZE 32

typedef struct _FCContext FCContext;

struct _FCContext
{
    int         fd;
    char        buf[CONFIG_BUFFER_SIZE];
    FCHandler   handler;
    void       *data;
};

static FCContext fc_context;

int ft_config_init()
{
    int fd = open(FT_CONFIG_FILE, O_RDWR | O_CREAT);

    if (fd == -1)
    {
        perror("open "FT_CONFIG_FILE);
        return 0;
    }

    read(fd, fc_context.buf, CONFIG_BUFFER_SIZE);
    fc_context.fd = fd;

    return 1;
}

int ft_config_set(const char *key, const char *value)
{
    assert(key != NULL && value != NULL);

    if (!fc_context.fd && !ft_config_init())
    {
        return 0;
    }

    char token[BUFFER_SIZE + 2];
    char *buf, *s, *e;

    buf = fc_context.buf;

    strncpy(token, key, BUFFER_SIZE);
    strcat(token, "=");
    
    for (s = buf; (s = strstr(s, token)); s++)
    {
        if (s == fc_context.buf || *(s - 1) == '\n')
            break;
    }

    if (s)
    {
        s += strlen(token);
        e = strstr(s, "\n");

        if (e)
        {
            memmove(s + strlen(value), e, strlen(e) + 1);
            memcpy(s, value, strlen(value));
        }
        else
        {
            strcpy(s, value);
            strcat(buf, "\n");
        }
    }
    else
    {
        strcat(buf, token);
        strcat(buf, value);
        strcat(buf, "\n");
    }

    ftruncate(fc_context.fd, 0);
    lseek(fc_context.fd, SEEK_SET, 0);
    write(fc_context.fd, buf, strlen(buf));
    fsync(fc_context.fd);

    if (fc_context.handler)
    {
        fc_context.handler(key, value, fc_context.data);
    }

    return 1;
}

int ft_config_set_int(const char *key, int value)
{
    char text[BUFFER_SIZE];

    snprintf(text, BUFFER_SIZE, "%d", value);

    return ft_config_set(key, text);
}

char *ft_config_get(const char *key)
{
    assert(key != NULL);

    if (!fc_context.fd && !ft_config_init())
    {
        return NULL;
    }
    
    char token[BUFFER_SIZE + 2];
    char value[BUFFER_SIZE + 1];
    char *s, *e;

    strncpy(token, key, BUFFER_SIZE);
    strcat(token, "=");

    for (s = fc_context.buf; (s = strstr(s, token)); s++)
    {
        if (s == fc_context.buf || *(s - 1) == '\n')
            break;
    }

    if (!s)
    {
        return NULL;
    }

    s += strlen(token);
    e = strstr(s, "\n");

    if (e)
    {
        strncpy(value, s, e - s);
        value[e - s] = '\0';
    }
    else
    {
        strcpy(value, s);
    }

    return strdup(value);
}

int ft_config_get_int(const char *key)
{
    int value = -1;

    char *text = ft_config_get(key);

    if (text)
    {
        value = atoi(text);
        free(text);
    }

    return value;
}

void ft_config_set_handler(FCHandler handler, void *data)
{
    fc_context.handler = handler;
    fc_context.data = data;
}

int ft_config_reset()
{
    if (!fc_context.fd && !ft_config_init())
    {
        return 0;
    }

    fc_context.buf[0] = '\0';

    ftruncate(fc_context.fd, 0);
    fsync(fc_context.fd);

    return 1;
}

