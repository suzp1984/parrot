#include <stdio.h>

int main() 
{
	int a = 10, b;

	__asm__("mov %1, %r0\n\t"
			"mov %r0, %0\n\t"
			:"=r"(b)        /* output */
			:"r"(a)         /* input */
			:"r0"         /* clobbered register */
		   );
	printf("Result: %d, %d\n", a, b);
	return 0;
}
