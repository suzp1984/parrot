/*
 * File:    rotate.c
 * Author:  zxsu <suzp1984@gmail.com>
 * Brief:   rotate alogrithm
 *
 * Copyright (c) zxsu
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2013-08-09 15:54 zxsu <suzp1984@gmail.com> created.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* str_rotate_left(char* str, int n);
char* str_rotate_right(char* str, int n);

char* str_rotate_left(char* str, int n)
{
    int i = 0;
    int m =0;
    char tmp;
    int len = strlen(str);

    n = n % len;

    if (n < 0) {
        n = len + n;
    }

    m = n / 2;
    for (i = 0; i < m; i++) {
        tmp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = tmp;
    }

    m = (len - n) / 2 + n;
    for (i = n; i < m; i++) {
        tmp = str[i];
        str[i] = str[len - i - 1 + n];
        str[len - i + n - 1] = tmp;
    }

    m = len / 2;
    for (i = 0; i < m; i++) {
        tmp = str[i];
        str[i] = str[len - i - 1];
        str[len-i-1] = tmp;
    }

    return str;
}

char* str_rotate_right(char* str, int n)
{
    return str_rotate_left(str, strlen(str) - n);
}

#ifdef ALOGRITHM_TEST

#include <assert.h>
#include <string.h>

int main(int argc, char* argv[])
{
    char* str = strdup("abcdefg");

    char* ret = str_rotate_left(str, 2);
    assert(strcmp("cdefgab", ret) == 0);
    free(str);
    str = strdup("abcdef");
    ret = str_rotate_right(str, 2);
    assert(strcmp("efabcd", ret) == 0);
    free(str);
    str = strdup("abcdef");
    ret = str_rotate_right(str, -2);
    assert(strcmp("cdefab", ret) == 0);
    free(str);

    return 0;
}

#endif
