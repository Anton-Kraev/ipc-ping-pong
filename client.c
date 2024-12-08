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
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 8

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        error("ERROR invalid server IP address");
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
    }

    printf("Client: Connection established\n");

    while (1) {
        strcpy(buffer, "ping");
        if (send(sockfd, buffer, strlen(buffer), 0) <= 0) {
            error("ERROR sending to socket");
        }

        memset(buffer, 0, BUFFER_SIZE);
        if (recv(sockfd, buffer, BUFFER_SIZE - 1, 0) <= 0) {
            error("ERROR receiving from socket");
        }

        printf("Client received: %s\n", buffer);

        sleep(1);
    }

    close(sockfd);
    return 0;
}
