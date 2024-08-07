#ifndef _SOCK_H_
#define _SOCK_H_

#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include<pthread.h>


int open_clientfd(char *hostname, char *port) __attribute__((unused));
int open_serverfd(char *port) __attribute__((unused));
void *response(void *socket_desc);
void processing(char [], char, char [], char *);
void setProcessing(char, char [], char *);
void getProcessing(char, char []);
void deleteProcessing(char, char []);
void showDatabase(void);


struct KeyValue
{
    struct KeyValue *next;
    struct Value *value;
    char  key[];
};
struct Value
{
    int ignore;
    char number[];
};


#endif

