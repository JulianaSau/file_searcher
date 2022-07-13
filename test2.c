#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


static void lookup(const char *arg)
{
    DIR *dirp;
    struct dirent *dp;


    if ((dirp = opendir(".")) == NULL) {
        perror("couldn't open '.'");
        return;
    }


    do {
        errno = 0;
        if ((dp = readdir(dirp)) != NULL) {
            if (strcmp(dp->d_name, arg) != 0)
                continue;


            (void) printf("found %s\n", arg);
            (void) closedir(dirp);
                return;


        }
    } while (dp != NULL);


    if (errno != 0)
        perror("error reading directory");
    else
        (void) printf("failed to find %s\n", arg);
    (void) closedir(dirp);
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
    int i;
    for (i = 1; i < argc; i++)
        lookup(argv[i]);
    return (0);
}