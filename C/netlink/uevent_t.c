#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <linux/netlink.h>

int main(int argc, char* argv[])
{
	int mSocket;
	int sz = 64 * 1024;
	int rc;
	int count;
	char mBuffer[64 * 1024];
	fd_set read_fds;

	struct sockaddr_nl nladdr;

	if((mSocket = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT)) < 0) {
		printf("error to create netlink socket\n");
		return -1;
	}

	memset(&nladdr, 0, sizeof(nladdr));
	nladdr.nl_family = AF_NETLINK;
	nladdr.nl_pid = getpid();
	nladdr.nl_groups = 0xffffffff;

	if(setsockopt(mSocket, SOL_SOCKET, SO_RCVBUFFORCE, &sz, sizeof(sz)) < 0) {
		printf("Unable to bind uevent socket\n");
		return -1;
	}

	if (bind(mSocket, (struct sockaddr *) &nladdr, sizeof(nladdr)) < 0) {
		printf("Unable to bind uevent socket\n");
		return -1;
	}

	while(1)
	{
		FD_ZERO(&read_fds);
		FD_SET(mSocket, &read_fds);

		if((rc = select(mSocket+1, &read_fds, NULL, NULL, NULL)) < 0) {
			printf("select failed\n");
			sleep(1);
			continue;
		} else if(!rc) {
			continue;
		}

		if(FD_ISSET(mSocket, &read_fds)) {
			if((count = recv(mSocket, mBuffer, sizeof(mBuffer), 0)) < 0) {
				printf("recevice uevent buffer failed\n");
				continue;
			}

			printf("\trecevice uevent:\t %s\n", mBuffer);
		}
	}

	return 0;
}
