/**
 * @file tafuta.c
 * @author JULIANA SAU NYANGE (SCT211-0010/2019)
 * @brief - a program that runs a search in a given directory
 * @version 0.1
 * @date 2022-07-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <string.h>
#include <time.h>
#include <fnmatch.h>
#include "tafuta.h"

#define TRUE 1
#define FALSE 0

/**
 * @brief file_searcher is a function that searches for files given wildcards
 *
 * @param rootPath  - path to seach for the file
 * @param fileName - name of the file to search for
 * @return int
 *
 */
int file_searcher(char *rootPath, char *fileName, char *searchParam)
{
    POrderNode directory_list = NULL;
    int fileFound = FALSE;

    DIR *dir = opendir(rootPath);
    if (!dir)
        return (printError(rootPath, "erroropendir", rootPath), -1);

    // Skipping . and ..
    readdir(dir);
    readdir(dir);

    for (struct dirent *entity = readdir(dir); entity; entity = readdir(dir))
    {
        char fullPath[PATH_MAX];
        sprintf(fullPath, "%s/%s", strcmp(rootPath, "/") == 0 ? "" : rootPath, entity->d_name);
        struct stat ed = {0};
        int return_value = stat(fullPath, &ed);

        if (return_value == 0)
        {

            unsigned char dMode = ed.st_mode >> 15 & 7;

            if (!dMode)
            {
                // Catalog

                char *buffer = (char *)malloc(sizeof(char) * PATH_MAX);
                if (!buffer)
                    return fprintf(stderr, "Error when trying to malloc mem for temp catalog path!\n");
                strcpy(buffer, fullPath);
                PushOrder(&directory_list, buffer);
            }
            else if (fnmatch(fileName, entity->d_name, 0) == 0)
            // (strcmp(entity->d_name, fileName) == 0)
            {

                fileFound = TRUE;

                unsigned char uMode = ed.st_mode >> 6 & 7;
                unsigned char gMode = ed.st_mode >> 3 & 7;
                unsigned char aMode = ed.st_mode >> 0 & 7;

                char formattedTime[256];
                struct tm *tmp;
                tmp = localtime(&ed.st_ctime);
                if (tmp == NULL)
                {
                    fprintf(stderr, "Error on converting to localtime...");
                    continue;
                }
                if (strftime(formattedTime, sizeof(formattedTime), "%H:%M:%S %d/%m/%Y", tmp) == 0)
                {

                    fprintf(stderr, "strftime returned 0");
                    continue;
                }

                printf(
                    "path: %s, size: %ld, bytes, date created: %s, mode: %d%d%d, index descriptor number: %ld\n",
                    fullPath,
                    ed.st_size,
                    formattedTime,
                    uMode, gMode, aMode,
                    ed.st_ino);
            }
        }
        else
        {
            printError(fullPath, "errorgetstat", entity->d_name);
        }
    }

    // Checking subfolders recursively
    while (directory_list)
    {

        char *fullPath = (char *)PopOrder(&directory_list);
        int return_value = file_searcher(fullPath, fileName);
        free(fullPath);
        if (return_value == 0)
            fileFound = TRUE;
    }

    // Cleaning folders if we have them
    while (directory_list)
    {
        free(PopOrder(&directory_list));
    }

    return fileFound ? 0 : 1;
}
/**
 * @brief
 *
 */
void search_file()
{
}

/**
 * @brief main function that implements the file searching program
 *
 * @param argc
 * @param argv
 * @param envp
 * @return int
 */
int main(int argc, char *argv[], char *envp[])
{
    // calculating the amount of time needed to run the search
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    if (argc < 2 || argc > 3)
        return fprintf(stderr, "Usage: tafuta <FILE> <PATH> <SEARCH>\n\n");

    char path[PATH_MAX];
    sprintf(path, "%s", argv[2]);

    char search_param[255];
    if (argv[3] != NULL)
    {
        sprintf(search_param, "%s", argv[3]);
        printf(search_param);
    }

    int return_value = file_searcher(path, argv[1]);

    if (return_value == 1)
    {

        printf("%s could NOT be found\n", argv[2]);
    }
    else if (return_value != 0)
    {

        fprintf(stderr, "Error occured while searching for the file\n");
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("tafuta took %f seconds to execute \n", cpu_time_used);

    return 0;
}