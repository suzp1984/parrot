/*
 * Copyright (C) 2005 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_UTILS_ATOMIC_H
#define ANDROID_UTILS_ATOMIC_H

#include <stdint.h>
#include <sys/types.h>

#ifndef __cplusplus
extern "C" {
#endif

int32_t android_atomic_inc(volatile int32_t* addr);
int32_t android_atomic_dec(volatile int32_t* addr);

extern inline int32_t android_atomic_add(int32_t increment,
		                                         volatile int32_t *ptr)
{
	__asm__ __volatile__ ("lock; xaddl %0, %1"
					: "+r" (increment), "+m" (*ptr)
					: : "memory");
	/* increment now holds the old value of *ptr */
	return increment;
}

extern inline int32_t android_atomic_inc(volatile int32_t *addr)
{
	    return android_atomic_add(1, addr);
}

extern inline int32_t android_atomic_dec(volatile int32_t *addr)
{
	    return android_atomic_add(-1, addr);
}

#ifdef __cplusplus
}
#endif

#endif // ANDROID_UTILS_ATOMIC_H
