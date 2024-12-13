#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>

#define DEFAULT_BUF_SIZE 4096

void saveFile(char *originalFilename, int clientSocket, int bufferSize) {
    int duplicateCounter = 1;
    char uniqueFilename[256];
    FILE *outputFile;

    // Check for duplicate filename and create unique name if needed
    strcpy(uniqueFilename, originalFilename);
    while (access(uniqueFilename, F_OK) != -1) {
        sprintf(uniqueFilename, "%s(%d)", originalFilename, duplicateCounter++);
    }

    // Open file for writing
    outputFile = fopen(uniqueFilename, "w");
    if (outputFile == NULL) {
        perror("Failed to open file");
        close(clientSocket);
        return;
    }

    char dataBuffer[bufferSize];
    ssize_t bytesReceived;
    ssize_t totalBytesTransferred = 0;

    // Receive data in chunks and write to the file
    while ((bytesReceived = recv(clientSocket, dataBuffer, bufferSize, 0)) > 0) {
        fwrite(dataBuffer, 1, bytesReceived, outputFile);
        totalBytesTransferred += bytesReceived;
    }

    fclose(outputFile);

    printf("Received file: %s\n", uniqueFilename);
    printf("Total size: %ld bytes\n", totalBytesTransferred);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s port-number [bufSize]\n", argv[0]);
        exit(1);
    }

    int portNumber = atoi(argv[1]);
    int bufferSize = (argc > 2) ? atoi(argv[2]) : DEFAULT_BUF_SIZE;

    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    // Create socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address structure
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(portNumber);

    // Bind socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Bind failed");
        close(serverSocket);
        exit(1);
    }

    // Listen for connections
    if (listen(serverSocket, 5) == -1) {
        perror("Listen failed");
        close(serverSocket);
        exit(1);
    }

    printf("Server listening on port %d with buffer size %d\n", portNumber, bufferSize);

    while (1) {
        // Accept client connection
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if (clientSocket == -1) {
            perror("Accept failed");
            continue;
        }

        char clientIpAddress[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddress.sin_addr, clientIpAddress, INET_ADDRSTRLEN);
        printf("Connected to client: %s\n", clientIpAddress);

        // Receive filename first
        char incomingFilename[256];
        recv(clientSocket, incomingFilename, sizeof(incomingFilename), 0);

        // Save file and close connection
        saveFile(incomingFilename, clientSocket, bufferSize);
        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}
