#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>

int main(int argc, char* argv[])
{
	MD5_CTX ctx;
	char* buf = "123456789abcdefgh\n";
	unsigned char md[MD5_DIGEST_LENGTH];
	int i = 0;

	MD5_Init(&ctx);
	MD5_Update(&ctx, (void*)buf, strlen(buf));
	MD5_Final(md, &ctx);

	printf("%s\n", buf);
	for (i = 0; i < MD5_DIGEST_LENGTH; i++) {
		printf("%02x", md[i]);
	}
	printf("\n");
	return 0;
}
