#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) 
{
    int socketfd;
    int len;
    struct sockaddr_un address;
    int result;
    char ch = 'A';

    socketfd = socket(AF_UNIX, SOCK_STREAM, 0);
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, "server_socket");
    len = sizeof(address);

    result = connect(socketfd, (struct sockaddr*)&address, len);

    if (result == -1) {
        perror("oop: client");
        exit(1);
    }

    write(socketfd, &ch, 1);
    read(socketfd, &ch, 1);
    printf("char from server = %c\n", ch);
    close(socketfd);

    return 0;
}
