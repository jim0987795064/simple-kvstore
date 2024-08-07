#include "sock.h"

#define BUFFER_SIZE 200


extern struct KeyValue *content[26];

void error(const char *msg)
{
    perror(msg);
    exit(0);
}


extern void processing(char operation[], char firstLetter, char key[], char *value);
extern void setProcessing(char firstLetter, char key[], char *value);
extern void getProcessing(char firstLetter, char key[]);
extern void deleteProcessing(char firstLetter, char key[]);
extern char output[300];

int open_clientfd(char *hostname, char *port)
{

    int clientfd, writeBytes = 0;
    struct addrinfo hints, *listp, *p;
    char clientBuffer[BUFFER_SIZE];

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_flags |= AI_ADDRCONFIG;
    getaddrinfo(hostname, port, &hints, &listp);

    for (p = listp; p; p = p->ai_next)
    {
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;
        else
            printf("%s\n", "create client socket sucessfully...");

        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1)
        {
            printf("%s\n", "client connect server sucessfully...");
            break;
        }
    }

    while(1)
    {
        bzero(clientBuffer, 200);
        printf("\n\n%s", "enter:");
        gets(clientBuffer);

        writeBytes = write(clientfd, clientBuffer, strlen(clientBuffer));

        //write
        if (writeBytes < 0)
            printf("%s\n", "ERROR writing to socket");

        bzero(clientBuffer, 200);
        writeBytes = read(clientfd, clientBuffer, 200);
        printf("%s", clientBuffer);
        //read
        if (writeBytes < 0)
            printf("%s\n", "ERROR reading from socket");

    }

    close(clientfd);
    freeaddrinfo(listp);
    if (!p)
        return -1;
    else
        return clientfd;


}

int open_serverfd(char *port)
{
    struct addrinfo hints, *listp, *p;
    struct sockaddr_in clientAdd;
    socklen_t clientLength;
    int serverfd, receiveSockfd, optval = 1;



    for(int i = 0; i < 26; i++)
    {
        content[i] = malloc(sizeof(struct KeyValue));
        content[i]->next = NULL;
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
    hints.ai_flags |= AI_NUMERICSERV;
    getaddrinfo(NULL, port, &hints, &listp);

    for (p = listp; p; p = p->ai_next)
    {

        if ((serverfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;
        setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));

        if (bind(serverfd, p->ai_addr, p->ai_addrlen) == 0)
        {
            printf("%s\n", "create server socket sucessfully...");
            break;
        }


    }

    if (!p)
        return -1;

    if (listen(serverfd, 1024) < 0)
    {
        close(serverfd);
        return -1;
    }
    else
        printf("%s%s\n", "server listening on port ", port);

    clientLength = sizeof(clientAdd);
    pthread_t thread_id;

    while((receiveSockfd = accept(serverfd,
                                  (struct sockaddr *) &clientAdd,
                                  &clientLength)))
    {
        if (receiveSockfd < 0)
            error("ERROR on accept");
        else
            printf("%s\n", "accept successfully...");

        if( pthread_create( &thread_id, NULL,  response, (void*) &receiveSockfd) < 0)
        {
            error("could not create thread");
            return 1;
        }

        printf("Server PID:%ld\n", pthread_self());

    }

    close(receiveSockfd);
    close(serverfd);
    freeaddrinfo(listp);


    return serverfd;
}

void *response(void *socket_desc)
{

    int clientSock = *(int*)socket_desc;
    int writeBytes = 0, command = 0, comparison,
        setComparison, getComparison, deleteComparison;
    char writeBuffer[512], *firstCommand, *secondCommand, *thirdCommand, *token;
    const char *parser = " ";
    printf("Client PID:%ld\n", pthread_self());

    while(1)
    {

        command = 0;
        bzero(writeBuffer, 512);
        writeBytes = read(clientSock, writeBuffer, 200);

        if (writeBytes < 0)
            error("ERROR writing to socket");


        token = strtok(writeBuffer, parser);

        while(token != NULL)
        {
            if(command == 0)
                firstCommand = token;
            else if(command == 1)
                secondCommand = token;
            else if(command == 2)
                thirdCommand = token;

            token = strtok(NULL, parser);
            command++;
        }


        comparison = strcmp(firstCommand, "exit");
        setComparison = strcmp(firstCommand, "SET");
        getComparison = strcmp(firstCommand, "GET");
        deleteComparison = strcmp(firstCommand, "DELETE");
        /*printf("set:%d\n", setComparison);
        printf("command:%d\n", command);
        printf("%s\n", secondCommand);*/

        if(comparison == 0)
            return 0;

        if(command != 3 && command != 2)
        {
            printf("enter %s\n", "unknown/invalid command");
            strcat(output, "enter unknown/invalid command");
        }
        else if(command == 3 && setComparison != 0)
        {
            printf("enter %s\n", "unknown/invalid command");
            strcat(output, "enter unknown/invalid command");
        }
        else if(command == 2 && (getComparison != 0 && deleteComparison != 0))
        {
            printf("enter %s\n", "unknown/invalid command");
            strcat(output, "enter unknown/invalid command");
        }
        else
            processing(firstCommand, secondCommand[0], secondCommand, thirdCommand);

        writeBytes = write(clientSock, output, strlen(output));
        strcpy(output, "");
    }

}


