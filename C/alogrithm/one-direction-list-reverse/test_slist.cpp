/*
 * File:    test_slist.cpp
 * Author:  zxsu <suzp1984@gmail.com>
 * Brief:   test case for slist
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
 * 2013-08-20 21:42 zxsu <suzp1984@gmail.com> created.
 */

#include <gtest/gtest.h>

#include "slist.h"

TEST(SListTest, simple_case) {
    SList* slist = slist_create(NULL, NULL);
    
    int i = 0; 
    int n = -1;
    for (i = 1; i < 11; i++) {
        ASSERT_EQ(RET_OK, slist_append(slist, (void*)i));
    }
    
    ASSERT_EQ(10, slist_get_count(slist));

    for (i = 0; i < 10; i++) {
        n = 100;
        ASSERT_EQ(RET_OK, slist_get_by_index(slist, i, (void**)&n));
        ASSERT_EQ(1, (n - i));
    }

    slist_destroy(slist);
}

TEST(SListTest, reverse_recursion_case) {
    SList* slist = slist_create(NULL, NULL);
    
    int i = 0; 
    int n = -1;
    for (i = 1; i < 11; i++) {
        ASSERT_EQ(RET_OK, slist_append(slist, (void*)i));
    }
    
    ASSERT_EQ(10, slist_get_count(slist));

    for (i = 0; i < 10; i++) {
        n = 100;
        ASSERT_EQ(RET_OK, slist_get_by_index(slist, i, (void**)&n));
        ASSERT_EQ(1, (n - i));
    }
    
    ASSERT_EQ(RET_OK,  slist_reverse_recursion(slist));

    for (i = 9; i >= 0; i--) {
        ASSERT_EQ(RET_OK, slist_get_by_index(slist, i, (void**)&n));
        ASSERT_EQ(10, (i +n));
        printf("n[%d] = %d\n", i, n);
    }

    slist_destroy(slist);
}

TEST(SListTest, reverse_stack_case) {
}
