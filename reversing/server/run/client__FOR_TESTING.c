#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if(argc != 3) {
        printf("USAGE: %s [host] [port]\n", argv[0]);
        exit(EXIT_SUCCESS);
    }

    struct addrinfo hints;
    struct addrinfo* res;
    int sock;
    char buf[256];

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    getaddrinfo(argv[1], argv[2], &hints, &res);

    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    connect(sock, res->ai_addr, res->ai_addrlen);

    freeaddrinfo(res);

    char* msg = "sAflag.txt";
    strcpy(buf, msg);
    buf[strlen(buf) + 1] = 0;

    send(sock, &buf, strlen(buf) + 1, 0);
    memset(buf, 0, 256);
    recv(sock, buf, 256, 0);
    printf("%s\n", buf);
}
