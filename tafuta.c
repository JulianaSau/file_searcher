#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <string.h>
#include <time.h>
#include <tafuta.h>

#define TRUE 1
#define FALSE 0

void SearchFunction(char *rootPath, int *countFilesWatched, const char *filename)
{
    DIR *dir = opendir(rootPath);

    if (dir == NULL)
    {
        return;
    }

    printf("Reading files in %s\n", rootPath);
    //     //Skipping . and ..
    // 	readdir(dd);
    // 	readdir(dd);

    struct dirent *entity; // It could be a file or a directory
    entity = readdir(dir); // could give null

    while (entity != NULL)
    {
        printf("%hhd %s/%s\n", entity->d_type, rootPath, entity->d_name);
        // if there is a directory in the results of search, search in that file also
        if(entity->d_type == DT_DIR && strcmp(entity->d_name, "." && strcmp(entity->d_name, "..") != 0){
            char path[100] = {0};
            strcat(path, rootPath);
            strcat(path, "/");
            strcat(path, entity->d_name);
            listFiles(path);
        }
        entity = readdir(dir);
    }

    closedir(dir);
}

int main(int argc, char *argv[])
{
    // ensuring correct number of arguments are passed
    if (argc != 3)
        return fprintf(stderr, "Invalid arguments count! Should be 2 arguments!\n");

    // listFiles(".");

    int noOfFilesWatched = 0;

    // printing the path to the directory we are searching in
    char path[PATH_MAX];
    sprintf(path, "%s", argv[2]);

    int returnVal = SearchFunction(path, &noOfFilesWatched, argv[1]);

    // checking for correctness of the program
    if (returnVal == 1)
    {

        printf("File has NOT been found!\n");
    }
    else if (returnVal != 0)
    {

        fprintf(stderr, "Fatal error occured while trying to search file!\n");
    }
    return 0;
}