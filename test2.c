#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fnmatch.h>

static void lookup(char *rootPath, const char *filename)
{
    DIR *dirp;
    struct dirent *entity;

    if ((dirp = opendir(rootPath)) == NULL)
    {
        perror("couldn't open '.'");
        return;
    }

    do
    {
        errno = 0;
        if ((entity = readdir(dirp)) != NULL)
        {
            if (fnmatch(filename, entity->d_name, 0) != 0)
            // if (strcmp(entity->d_name, filename) != 0)
            {
                printf("%hhd %s/%s\n", entity->d_type, filename, entity->d_name);
                continue;
            }
            // if there is a directory in the results of search, pritn out
            if (entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0)
            {
                char path[100] = {0};
                strcat(path, filename);
                strcat(path, "/");
                strcat(path, entity->d_name);
                lookup(path, filename);
            }
            entity = readdir(dirp);

            (void)printf("found %s\n", filename);
            (void)closedir(dirp);
            return;
        }
    } while (entity != NULL);

    if (errno != 0)
        perror("error reading directory");
    else
        (void)printf("failed to find %s\n", filename);
    (void)closedir(dirp);
    return;
}

// static int lookup(const char * dir, const char *arg)
// {
//     DIR *dirp;
//     struct dirent *dp;

//     if ((dirp = opendir(dir)) == NULL) {
//         perror(dir);
//         return -1;
//     }

//     while ((dp = readdir(dirp)) != NULL) {
//         if (!strcmp(dp->d_name, arg)) {
//             break;
//         }
//     }
//     (void) closedir(dirp);

//     printf("%s %s\n", dp ? "found" : "failed to find", arg);
//     return 0;
// }

int main(int argc, char *argv[])
{
    if (argc != 3)
        return fprintf(stderr, "Invalid arguments count! Should be 2 arguments!\n");

    // printing the path to the directory we are searching in
    char path[PATH_MAX];
    sprintf(path, "%s", argv[2]);

    lookup(path, argv[1]);
    return (0);
}