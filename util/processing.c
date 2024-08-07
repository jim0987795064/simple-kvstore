#include "sock.h"


struct KeyValue *content[26];
char output[300] = {""};

//must use char [],don't use char *.
void processing(char operation[], char firstLetter, char key[], char *value)
{
    int set, get, delete;
    set = strcmp("SET", operation);
    get = strcmp("GET", operation);
    delete = strcmp("DELETE", operation);


    if(set == 0)
        return setProcessing(firstLetter, key, value);
    else if(get == 0)
        getProcessing(firstLetter, key);
    else if(delete == 0)
        deleteProcessing(firstLetter, key);

}


void setProcessing(char firstLetter, char key[], char *value)
{

    if(isdigit(firstLetter))
    {
        printf("%d", firstLetter);
        return;
    }
    firstLetter = toupper(firstLetter);
    int position = firstLetter - 65, comparison;
    struct KeyValue *current = NULL;
    current = content[position];

    //search for final position && check the same key if it exists
    while(current->next != NULL)
    {
        current = current->next;
        comparison = strcmp(current->key, key);

        if(comparison == 0)
        {
            printf("SET %s\n", "unknown/invalid command");
            strcat(output, "SET unknown/invalid command");
            return;
        }

    }
    //create new linked-list node
    current->next = malloc(sizeof(struct KeyValue));
    //move current to new linked-list node
    current = current->next;
    //Set Key Value
    strcpy(current->key, key);
    current->value = malloc(sizeof(struct Value));
    strcpy(current->value->number, value);
    current->next = NULL;

    //show sucessful storing message
    printf("\nKey value pair (%s, %s) is stored!\n", current->key, current->value->number);
    strcat(output, "Key value pair (");
    strcat(output, current->key);
    strcat(output, ", ");
    strcat(output, current->value->number);
    strcat(output, ") is stored!\n");
    showDatabase();
    return;



}


void getProcessing(char firstLetter, char key[])
{

    firstLetter = toupper(firstLetter);
    int position = firstLetter - 65, comparison;
    struct KeyValue *current = NULL;
    current = content[position];


    while(current->next != NULL)
    {
        current = current->next;
        comparison = strcmp(current->key, key);

        if(comparison == 0)
        {
            printf("\nThe value of %s is %s\n", current->key, current->value->number);
            strcat(output, "The value of ");
            strcat(output, current->key);
            strcat(output, " is ");
            strcat(output, current->value->number);
            showDatabase();
            return;
        }

    }

    printf("\nget %s\n", "unknown/invalid command");
    strcat(output, "GET unknown/invalid command");
}

void deleteProcessing(char firstLetter, char key[])
{
    firstLetter = toupper(firstLetter);
    int position = firstLetter - 65, comparison;
    struct KeyValue *current = NULL;
    struct KeyValue *previous = NULL;
    current = content[position];


    while(current->next != NULL)
    {
        //record last node
        previous = current;
        //move to next node
        current = current->next;
        comparison = strcmp(current->key, key);



        if(comparison == 0)
        {
            printf("\nKey \"%s\" is removed\n", current->key);
            strcat(output, "Key \"");
            strcat(output, current->key);
            strcat(output, "\" is removed");

            previous->next = current->next;
            free(current);
            showDatabase();
            return;
        }

    }

    printf("\ndelete %s\n", "unknown/invalid command");
    strcat(output, "DELETE unknown/invalid command");

}

void showDatabase()
{

    struct KeyValue *current = NULL;

    //read database
    for(int i = 0; i < 26; i++)
    {
        if(content[i]->next != NULL)
            printf("%d: ", i);
        for(current = content[i]->next; current != NULL; current = current->next)
            printf("%s %s | ", current->key, current->value->number);
        if(content[i]->next != NULL)
            printf("\n");
    }
    printf("\n");
}