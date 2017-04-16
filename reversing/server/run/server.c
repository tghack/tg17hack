#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

struct proto_t {
    char type;
    uint8_t len;
    char payload[];
};

char* read_file(int len, char* name) {
    if(!name || !*name) return NULL;

    if (strchr(name, "."))
	    return NULL;
    else if (strchr(name, "/"))
	    return NULL;

    FILE* fp = fopen(name, "r");
    if(!fp) return NULL;
    char* buf = calloc(len + 1, 1);

    fread(buf, 1, len, fp);

    fclose(fp);
    return buf;
}

void write_file(char* name) {
    for (int i = 0; i < 10; i++) {
        do {
            if(name) break;
            continue;
        } while(0);
    }
    return;
}

int listcontents(char** res) {
    uint32_t size = 128;
    while(1) {
        char tempres[size];
        memset(tempres, 0, size);
        int cont = 1;

        DIR* dp = opendir("./");
        if(dp != NULL) {
            struct dirent* ep = readdir(dp);
            while(ep && cont) {
                if((strlen(tempres) + strlen(ep->d_name) + 1) > size) {
                    size *=2;
                    cont = 0;
                    break;
                }
                strcat(tempres, ep->d_name);
                strcat(tempres, "\n");
                ep = readdir(dp);
            }
            closedir(dp);
            if(cont) {
                *res = calloc(size, sizeof(char));
                tempres[strlen(tempres) - 1] = '\0';
                strcpy(*res, tempres);
                return 1;
            }
        } else {
            break;
        }
    }
    *res = 0;
    return 0;
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("USAGE: %s [port]\n", argv[0]);
        exit(EXIT_SUCCESS);
    }

    socklen_t client_len = sizeof(struct sockaddr_storage);
    struct sockaddr_storage client;
    struct addrinfo hints;
    struct addrinfo* res;
    int sock, ret;
    char buf[256];
    struct proto_t* header = (struct proto_t*)buf;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;

    getaddrinfo(NULL, argv[1], &hints, &res);

    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(sock, res->ai_addr, res->ai_addrlen);

    freeaddrinfo(res);

    for(;;) {
        ret = recvfrom(sock, &buf, 256, 0, (struct sockaddr*)&client, &client_len);
        if(ret == -1)
            continue;

        char host[NI_MAXHOST], service[NI_MAXSERV];
        getnameinfo((struct sockaddr*)&client, client_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);

        switch(header->type) {
            case 'r':
                ;
                char* file_buf = read_file(header->len, header->payload);
                if(!file_buf) break;
                sendto(sock, file_buf, strlen(file_buf), 0, (struct sockaddr*)&client, client_len);
                free(file_buf);
                break;
            case 'w':
                ;
                write_file(header->payload);
                break;
            default:
                ;
                char* res;
                listcontents(&res);
                if(!res) break;
                sendto(sock, res, strlen(res), 0, (struct sockaddr*)&client, client_len);
                free(res);
                continue;
        }
    }
}
