#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090  // Load balancer port
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    
    // Create socket for client
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address (load balancer)
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);  // Load balancer's port
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost IP

    // Connect to the load balancer server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Enter a lowercase string: ");
    fgets(buffer, sizeof(buffer), stdin);

    // Remove the newline character if present
    buffer[strcspn(buffer, "\n")] = '\0';

    // Send the lowercase string to the load balancer
    send(sock, buffer, strlen(buffer), 0);

    // Receive the uppercase string from the load balancer
    int len = recv(sock, buffer, sizeof(buffer), 0);
    buffer[len] = '\0';  // Null-terminate the string

    printf("Received uppercase string: %s\n", buffer);

    // Close the socket
    close(sock);
    return 0;
}

