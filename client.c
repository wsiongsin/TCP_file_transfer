#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#define DEFAULT_BUF_SIZE 4096 // default buffer size

void sendFileContents(FILE *sourceFile, int socketFd, int bufferSize) {
    char buffer[bufferSize];
    ssize_t bytesRead;

    while ((bytesRead = fread(buffer, 1, bufferSize, sourceFile)) > 0) {
        if (send(socketFd, buffer, bytesRead, 0) == -1) {
            perror("Failed to send file contents");
            return;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s sourceFileName hostName:port-number [bufferSize]\n", argv[0]);
        exit(1);
    }

    char *sourceFileName = argv[1];
    char *hostAndPort = argv[2];
    int bufferSize = (argc > 3) ? atoi(argv[3]) : DEFAULT_BUF_SIZE;
    char *hostName = strtok(hostAndPort, ":");
    int portNumber = atoi(strtok(NULL, ":"));

    struct addrinfo connectionHints, *serverInfo;
    int clientSocket;

    // Set up connection hints
    memset(&connectionHints, 0, sizeof(connectionHints));
    connectionHints.ai_family = AF_INET;       // IPv4
    connectionHints.ai_socktype = SOCK_STREAM; // TCP

    // Convert port number to string
    char portString[6];
    snprintf(portString, sizeof(portString), "%d", portNumber);
    
    // Resolve the hostname to an IP address
    if (getaddrinfo(hostName, portString, &connectionHints, &serverInfo) != 0) {
        perror("Failed to resolve host address");
        exit(1);
    }

    // Create client socket
    if ((clientSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol)) == -1) {
        perror("Failed to create client socket");
        freeaddrinfo(serverInfo);
        exit(1);
    }

    // Connect to server
    if (connect(clientSocket, serverInfo->ai_addr, serverInfo->ai_addrlen) == -1) {
        perror("Failed to connect to server");
        close(clientSocket);
        freeaddrinfo(serverInfo);
        exit(1);
    }

    // Free the server info as it's no longer needed
    freeaddrinfo(serverInfo);

    // Send filename to server
    if (send(clientSocket, sourceFileName, strlen(sourceFileName) + 1, 0) == -1) {
        perror("Failed to send filename to server");
        close(clientSocket);
        exit(1);
    }

    // Open source file for reading
    FILE *sourceFile = fopen(sourceFileName, "rb");
    if (sourceFile == NULL) {
        perror("Failed to open source file");
        close(clientSocket);
        exit(1);
    }

    // Transfer file contents
    sendFileContents(sourceFile, clientSocket, bufferSize);
    fclose(sourceFile);
    close(clientSocket);

    printf("File transfer completed successfully.\n");
    return 0;
}

