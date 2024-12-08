#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 12345
#define BUFFER_SIZE 8

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    socklen_t clilen;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr, cli_addr;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }

    listen(sockfd, 1);
    clilen = sizeof(cli_addr);

    int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) {
        error("ERROR on accept");
    }

    printf("Server: Connection established\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        if (recv(newsockfd, buffer, BUFFER_SIZE - 1, 0) <= 0) {
            error("ERROR receiving from socket");
        }

        printf("Server received: %s\n", buffer);

        if (strcmp(buffer, "ping") == 0 && send(newsockfd, "pong", 4, 0) <= 0) {
            error("ERROR sending to socket");
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}
