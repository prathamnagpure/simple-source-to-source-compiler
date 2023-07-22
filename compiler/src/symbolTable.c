#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/symbolTable.h"
mapNode *gst=NULL;
mapNode *lst=NULL;
mapNode *fst=NULL;

/* find in symbol table for functions */
mapNode* findfst(char *variableName)
{
    if(NULL == variableName)
    {
        return NULL; 
    }
    mapNode* ptr = fst;
    while(ptr != NULL)
    {
        if(0 == strcmp(ptr->key,variableName))
        {
            break;
        }
        ptr = ptr->next;
    }  
    return ptr;
}

/* insert in symbol table for functions */
int insertfst(char* variableName,int dim,int line)
{
    if(NULL != findfst(variableName))
    {
        return 1;
    }
    mapNode *newNode = (mapNode*)malloc(sizeof(mapNode));
    if(NULL == newNode)
    {
        printf("Out of memory\n");
        exit(1);
    }
    newNode->key = variableName;
    newNode->next = fst;
    newNode->data = dim;
    newNode->lineno = line;
    fst = newNode;
    return 0;
}

void freelst()
{
    while(NULL != lst)
    {
        mapNode* next=lst->next;
        free(lst);
        lst=next;
    }
}

/* find if variable is present in symbol table */
mapNode* findlst(char *variableName) 
{
    if(NULL == variableName)
    {
        return NULL; 
    }
    mapNode* ptr = lst;
    while(ptr != NULL)
    {
        if(0 == strcmp(ptr->key,variableName))
        {
            break;
        }
        ptr = ptr->next;
    }  
    return ptr;
}

/* Insert variable in symbol table */
int insertlst(char* variableName, int dim, int line) 
{
    if(NULL != findlst(variableName))
    {
        return 1;
    }
    mapNode *newNode = (mapNode*)malloc(sizeof(mapNode));
    if(NULL == newNode)
    {
        printf("Out of memory\n");
        exit(1);
    }
    newNode->key = variableName;
    newNode->next = lst;
    newNode->data = dim;
    newNode->lineno = line;
    lst = newNode;
    return 0;
}
/* find if variable is present in symbol table */
mapNode* findgst(char *variableName) 
{
    if(NULL == variableName)
    {
        return NULL; 
    }
    mapNode* ptr = gst;
    while(ptr != NULL)
    {
        if(0 == strcmp(ptr->key,variableName))
        {
            break;
        }
        ptr = ptr->next;
    }  
    return ptr;
}

/* Insert variable in symbol table */
int insertgst(char* variableName, int dim, int line) 
{
    if(NULL != findgst(variableName))
    {
        return 1;
    }
    mapNode *newNode = (mapNode*)malloc(sizeof(mapNode));
    if(NULL == newNode)
    {
        printf("Out of memory\n");
        exit(1);
    }
    newNode->key = variableName;
    newNode->next = gst;
    newNode->data = dim;
    newNode->lineno = line;
    gst = newNode;
    return 0;
}