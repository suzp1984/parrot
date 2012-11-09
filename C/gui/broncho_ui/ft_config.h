#ifndef _FT_CONFIG_H_
#define _FT_CONFIG_H_

#define FT_CONFIG_FILE  "/data/factory_test.dat"

typedef void (*FCHandler)(const char *key, const char *value, void *data);

int ft_config_init();

int ft_config_set(const char *key, const char *value);

int ft_config_set_int(const char *key, int value);

char *ft_config_get(const char *key);

int ft_config_get_int(const char *key);

void ft_config_set_handler(FCHandler handler, void *data);

int ft_config_reset();

#endif/*_FT_CONFIG_H_*/
