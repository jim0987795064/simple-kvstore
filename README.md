# HW2 Simple Key-value Store

## Overview

This code implements a simple key-value store server that can handle `SET`, `GET`, and `DELETE` operations through a client-server architecture using sockets. The server is multithreaded, allowing it to handle multiple client connections simultaneously.

## Directories
- /server ->	server program related sources
- /client ->	client program related sources
- /common ->	common inclusions
- /util ->	common utilization
- /build ->	target build directory

## Building the Project
Code out your `/server/server.c` and `/client/client.c`, then
```shell
$ make
```
Test your `/build/server` and `build/client`.

## Implementations
### Please briefly describe your multi-threading design pattern
**Key-Value Storage**:
   - The key-value pairs are stored in a linked list structure. There are 26 possible linked lists (one for each letter of the alphabet) to handle different keys.
   - Each linked list node (`KeyValue`) holds a key and a pointer to a `Value` structure that contains the associated value.
### Please briefly describe your data structure implementation
1.**Server Functions**:
   - `open_serverfd(char *port)`: This function sets up the server to listen on the specified port. It creates a socket, binds it to the port, and starts listening for incoming connections.
   - `response(void *socket_desc)`: This function is the entry point for a new thread handling a client request. It reads commands from the client, processes them, and sends the appropriate response back.
   - `processing(char operation[], char firstLetter, char key[], char *value)`: This function determines which operation (SET, GET, DELETE) the client requested and calls the appropriate processing function.

2. **Client Functions**:
   - `open_clientfd(char *hostname, char *port)`: This function connects the client to the server at the specified hostname and port. It handles reading and writing data to the server.
   - The client interacts with the server by sending commands like `SET key value`, `GET key`, or `DELETE key`.

3. **Command Processing**:
   - **SET Operation**: `setProcessing` adds a new key-value pair to the linked list or returns an error if the key already exists.
   - **GET Operation**: `getProcessing` searches for a key in the linked list and retrieves its value.
   - **DELETE Operation**: `deleteProcessing` removes a key-value pair from the linked list.

4. **Multithreading**:
   - The server uses the `pthread_create` function to create a new thread for each client connection. This allows the server to handle multiple clients concurrently.

5. **Error Handling**:
   - The server and client functions include basic error handling, such as checking if a socket was created successfully or if the connection to the server was successful.

6. **Command Parsing**:
   - The client and server use `strtok` to parse incoming command strings, splitting them into command, key, and value components.

## References
* [POSIX thread man pages](https://man7.org/linux/man-pages/man7/pthreads.7.html)
* [socket man pages](https://linux.die.net/man/7/socket)

## Summary

This code demonstrates a basic client-server model using sockets to implement a simple key-value store. The server can handle multiple clients concurrently using threads and supports basic operations like setting, getting, and deleting key-value pairs. The implementation includes error handling and command parsing to ensure smooth operation.

