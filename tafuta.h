/**
 * @file tafuta.h
 * @author JULIANA SAU NYANGE (SCT211-0010/2019)
 * @brief - header file for tafuta.c that handles storing directories to search for files from
 * @version 0.1
 * @date 2022-07-13
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef TAFUTA_H
#define TAFUTA_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

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

/**
 * @brief
 *
 */
int search_file(const char *filename, const char *search)
{
    FILE *fp;
    char ch;
    char word[50];
    int count = 0;
    int pos[10];
    int pointer = 0;
    int loop;

    /*  open for writing */
    fp = fopen(filename, "r");

    /* Exit if file not opened successfully */
    if (fp == NULL)
    {
        printf("Unable to open file.\n");
        printf("Please ensure that you have read/write previleges.\n");

        exit(EXIT_FAILURE);
    }

    do
    {
        ch = fscanf(fp, "%s", word);
        if (strcmp(word, search) == 0)
        {
            pos[count] = pointer;
            count++;
        }
        pointer++;
        // printf("%s",word);
    } while (ch != EOF);

    if (count == 0)
    {
        // printf("'%s' not found in %s\n", search, filename);
        exit(EXIT_FAILURE);
    }

    fclose(fp);

    return 0;
}

#endif /* TAFUTA_H */