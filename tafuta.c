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
 * @param countFilesWatched - counts the files that have been searched
 * @param fileName - name of the file to search for
 * @return int
 */

int file_searcher(char *rootPath, int *countFilesWatched, char *fileName)
{
    POrderNode dirsToLook = NULL;
    int fileFound = FALSE;

    DIR *dd = opendir(rootPath);
    if (!dd)
        return (printError(rootPath, "erroropendir", rootPath), -1);

    // Skipping . and ..
    readdir(dd);
    readdir(dd);

    for (struct dirent *entity = readdir(dd); entity; entity = readdir(dd))
    {

        (*countFilesWatched)++;

        char fullPath[PATH_MAX];
        sprintf(fullPath, "%s/%s", strcmp(rootPath, "/") == 0 ? "" : rootPath, entity->d_name);
        struct stat ed = {0};
        int retVal = stat(fullPath, &ed);

        if (retVal == 0)
        {

            unsigned char dMode = ed.st_mode >> 15 & 7;

            if (!dMode)
            {
                // Catalog

                char *buffer = (char *)malloc(sizeof(char) * PATH_MAX);
                if (!buffer)
                    return fprintf(stderr, "Error when trying to malloc mem for temp catalog path!\n");
                strcpy(buffer, fullPath);
                PushOrder(&dirsToLook, buffer);
            }
            else if (fnmatch(fileName, entity->d_name, 0) == 0)
            // (strcmp(entity->d_name, fileName) == 0)
            {

                fileFound = TRUE;

                unsigned char uMode = ed.st_mode >> 6 & 7;
                unsigned char gMode = ed.st_mode >> 3 & 7;
                unsigned char aMode = ed.st_mode >> 0 & 7;

                char formatedTime[256];
                struct tm *tmp;
                tmp = localtime(&ed.st_ctime);
                if (tmp == NULL)
                {

                    fprintf(stderr, "Error on converting to localtime...");
                    continue;
                }
                if (strftime(formatedTime, sizeof(formatedTime), "%H:%M:%S %d/%m/%Y", tmp) == 0)
                {

                    fprintf(stderr, "strftime returned 0");
                    continue;
                }

                printf(
                    "Full Path: %s\nSize: %ld bytes\nCreated: %s\nMode: %d%d%d\nIndex descriptor number: %ld\n",
                    fullPath,
                    ed.st_size,
                    formatedTime,
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
    while (dirsToLook)
    {

        char *fullPath = (char *)PopOrder(&dirsToLook);
        int returnValue = file_searcher(fullPath, countFilesWatched, fileName);
        free(fullPath);
        if (returnValue == 0)
            fileFound = TRUE;
    }

    // Cleaning folders if we have them
    while (dirsToLook)
    {

        free(PopOrder(&dirsToLook));
    }

    printf("File has been found! Scanned: %d files and folders\n", *countFilesWatched);

    return fileFound ? 0 : 1;
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

    if (argc != 3)
        return fprintf(stderr, "Invalid arguments count! Should be 2 arguments!\n");

    int countFilesWatched = 0;

    char path[PATH_MAX];
    sprintf(path, "%s", argv[2]);

    int retVal = file_searcher(path, &countFilesWatched, argv[1]);

    if (retVal == 1)
    {

        printf("File has NOT been found!\n");
    }
    else if (retVal != 0)
    {

        fprintf(stderr, "Fatal error occured while trying to search file!\n");
    }
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("tafuta took %f seconds to execute \n", cpu_time_used);

    return 0;
}