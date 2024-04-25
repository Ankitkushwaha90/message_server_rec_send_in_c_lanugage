#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        error("Error creating socket");
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.77.28"); // Loopback address
    server_addr.sin_port = htons(PORT);

    // Connect to server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        error("Error connecting to server");
    }

    printf("Connected to server\n");

    while (1) {
        // Send message to server
        printf("Enter message to send to server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(client_fd, buffer, strlen(buffer), 0);

        // Receive message from server
        ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (bytes_received == -1) {
            error("Error receiving message from server");
        } else if (bytes_received == 0) {
            printf("Server disconnected\n");
            break;
        } else {
            buffer[bytes_received] = '\0';
            printf("Server: %s", buffer);
        }
    }

    // Close socket
    close(client_fd);

    return 0;
}
