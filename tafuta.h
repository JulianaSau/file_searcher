#ifndef TAFUTA_H
#define TAFUTA_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

typedef struct _OrderNode
{
    struct _OrderNode *pNext;
    void *pValue;
} OrderNode, *POrderNode;

void PushOrder(POrderNode *pHead, void *pValue)
{
    if (*pHead)
    {

        POrderNode node = *pHead;
        while (node->pNext)
            node = node->pNext;
        node->pNext = (POrderNode)malloc(sizeof(OrderNode));
        if (!node->pNext)
        {

            fprintf(stderr, "Error when trying to malloc mem for the next node!\n");
            return;
        }
        node->pNext->pValue = pValue;
        node->pNext->pNext = NULL;
    }
    else
    {

        *pHead = (POrderNode)malloc(sizeof(OrderNode));
        if (!(*pHead))
        {

            fprintf(stderr, "Error when trying to malloc mem for node in HEAD!\n");
            return;
        }
        (*pHead)->pValue = pValue;
        (*pHead)->pNext = NULL;
    }
}

void *PopOrder(POrderNode *pHead)
{

    if (!*pHead)
        return NULL;

    POrderNode node = *pHead;
    *pHead = (*pHead)->pNext;
    void *retVal = node->pValue;

    free(node);
    return retVal;
}

void printError(char *fullPath, char *message, char *filename)
{
    fprintf(stderr, "%s:%s:%s\n", fullPath, message, filename);
}

#endif /* TAFUTA_H */