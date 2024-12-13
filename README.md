# File Transfer System

A simple client-server file transfer system implemented in C that supports concurrent file transfers with automatic file renaming to prevent overwrites.

## Features

- TCP-based file transfer
- Automatic handling of duplicate filenames on the server side
- Configurable buffer size for transfer optimization
- Support for concurrent client connections
- Simple command-line interface

## Prerequisites

- GCC compiler
- UNIX-like operating system (Linux, macOS)
- Make utility

## Running the Server

Start the server by specifying a port number and optional buffer size:

```bash
./server port-number bufSize
```

Example:

```bash
./server 3000 4096
```

## Running the Client

Send a file using the client:

```bash
./sendFile fileName IP-address:port-number bufSize
```

Example:

```bash
./sendFile smallTest.txt localhost:3000 4096
```

## Running Multiple Clients (Concurrent Transfers)

A bash script is provided to test multiple concurrent transfers:

1. Make the script executable:
   ```bash
   chmod +x run_multiple_client.sh
   ```
2. Run the script:
   ```bash
   ./run_multiple_client.sh
   ```
   `  Note: You can modify the script parameters in`run_multiple_client.sh`:

- `NUM_CLIENTS`: Number of concurrent clients
- `SERVER_ADDRESS`: Server address and port
- `FILE_NAME`: File to transfer
- `BUFFER_SIZE`: Buffer size for transfers

## Clean Up

Remove compiled executables:

```bash
make clean
```

## Notes

- The server automatically handles duplicate filenames by appending a number in parentheses
- The default buffer size is 4096 bytes if not specified
- The server continues running until manually terminated
- Files are saved in the same directory where the server is running

## Error Handling

The system includes error handling for common scenarios:

- Invalid command-line arguments
- Connection failures
- File access issues
- Network transfer problems
