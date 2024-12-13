#!/bin/bash

# Number of clients to simulate
NUM_CLIENTS=20

# Server address and port
SERVER_ADDRESS="127.0.0.1:3000"

# File to be sent by each client
FILE_NAME="smallTest.txt"\

# Buffer size to use for each client
BUFFER_SIZE=4096

# Loop to start each client in the background
for i in $(seq 1 $NUM_CLIENTS); do
    echo "Starting client $i to send file $FILE_NAME"
    ./sendFile "$FILE_NAME" "$SERVER_ADDRESS" "$BUFFER_SIZE" &
done

# Wait for all background processes to complete
wait
echo "All clients have finished sending files."
