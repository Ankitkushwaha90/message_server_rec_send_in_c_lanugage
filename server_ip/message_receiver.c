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
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        error("Error creating socket");
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        error("Error binding socket");
    }

    // Listen for connections
    if (listen(server_fd, 1) == -1) {
        error("Error listening for connections");
    }

    printf("Waiting for incoming connections...\n");

    // Accept incoming connection
    client_len = sizeof(client_addr);
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len)) == -1) {
        error("Error accepting connection");
    }

    printf("Connected to client\n");

    while (1) {
        // Receive message from client
        ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (bytes_received == -1) {
            error("Error receiving message from client");
        } else if (bytes_received == 0) {
            printf("Client disconnected\n");
            break;
        } else {
            buffer[bytes_received] = '\0';
            printf("Client: %s", buffer);

            // Send message to client
            printf("Enter message to send to client: ");
            fgets(buffer, BUFFER_SIZE, stdin);
            send(client_fd, buffer, strlen(buffer), 0);
        }
    }

    // Close sockets
    close(client_fd);
    close(server_fd);

    return 0;
}
