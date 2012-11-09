#include <stdlib.h>
#include <mmintrin.h>

int mmx_add_byte(int a, int b);

int main(int argc, char* argv[])
{
	int a = mmx_add_byte(0x11223344, 0x44332211);
	printf("a=%p\n", a);
	return 0;
}
