/* DIRTREE.C  V 1.0 by Glen E. Gardner, Jr. */
/* A recursive directory tree traversal that finds the path and names */
/* for all files in the specified path. */

/* this program was written and compiled using Borland C++ 5.0 on */
/* Microsoft Windows NT 4.0 */
/* The program was written with no MS API function calls, for as much */
/* OS independence as was practical. */
/* It should compile and run on DOS, Win 3.X , Win 95/98 and Win NT. */

/* This program was written in ansi C as much as was possible, UNIX users might */
/* find this program easy to port. */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <dir.h>
#include <string.h>

int scandir(char *, char *);
int getsize(char *);
void doit(char *);
int scanstring(char *, char *);

void main(int argc, char *argv[])
{
    chdir(argv[1]);
    if (argc != 2)
    {
        printf("usage: DIRTREE <PATH>\n");
        exit(1);
    }
    doit(".");
    exit(0);
}

int getsize(char *dirname)
{ /* finds and returns the size of the directory entry (in bytes) */

    DIR *dir;
    struct dirent *ent;
    int size = 0;

    if ((dir = opendir(dirname)) == NULL)
    {
        perror("Unable to open directory");
        return (NULL);
    }
    while ((ent = readdir(dir)) != NULL)
        ;
    rewinddir(dir);
    while ((ent = readdir(dir)) != NULL)
        size = size + sizeof(ent->d_name) + 1;

    if (closedir(dir) != 0)
        perror("Unable to close directory");
    return (size);
}

int scandir(char *dirname, char *entries)
{ /* scan the directory and store the entries in a buffer */
    DIR *dir;
    struct dirent *ent;
    int count = 1;
    char name[256];

    if ((dir = opendir(dirname)) == NULL)
    {
        perror("Unable to open directory");
        return (0);
    }

    while ((ent = readdir(dir)) != NULL)
        count++;

    rewinddir(dir);

    while ((ent = readdir(dir)) != NULL)
    {
        strcpy(name, ent->d_name);
        sprintf(entries, "%s", name);
        entries = entries + strlen(name) + 1;
        count++;
    }

    if (closedir(dir) != 0)
        perror("Unable to close directory");

    return (count);
}

void doit(char *dirname)
{ /* scan the buffer and recursively enter any directories found */

    int size;
    char *entries;
    char name[256];
    char Path[256];
    char *path;
    char oldpath[256];
    char old[256] = "none";
    FILE *filein;
    path = Path;
    size = getsize(dirname);
    entries = (char *)malloc(size);
    scandir(dirname, entries);
    getcwd(Path, 256);
    strcpy(old, Path);

    while (scanstring(name, entries) != EOF)
    {

        /* store the path for use later */
        getcwd(Path, 256);
        if ((strcmp(name, ".")) && (strcmp(name, "..")))
        {

            /* this is where the valid path variable can be found */
            if (!strcmp(Path, old))
                printf("PATH: %s\n", Path);
            strcpy(oldpath, Path);

            /* add the next entry to the path for testing */
            strcat(Path, "\\");
            strcat(Path, name);

            /* see if the entry is a file */
            /* here is where the valid filename associated with the path can be found */
            if ((filein = fopen(Path, "r")) != 0)
            {
                printf("NAME: %s\n", name);
                fclose(filein);
            }
            else
            {
                if (chdir(Path) == 0)
                { /* if the entry is a valid directory, go there */
                    getcwd(path, 256);

                    /* start the recursive traversal */
                    doit(".");

                    /* restore the path */
                    strcpy(Path, oldpath);
                    chdir(Path);
                    getcwd(path, 256);
                }
            }
            /* set the sentinel variable for state changes */
            strcpy(old, Path);
            strcpy(Path, oldpath);
            chdir(Path);
            getcwd(path, 256);
        }

        entries = entries + strlen(name) + 1;
    }

    return;
}

int scanstring(char *string, char *buffer)
{
    int i;
    int size;
    char *temp;

    size = 0;
    temp = buffer;
    while (*buffer != NULL)
    {
        size++;
        buffer++;
    }

    buffer = temp;

    for (i = 0; i <= size; i++)
    {
        *string = *buffer;
        buffer++;
        string++;
    }
    *string = *buffer;
    buffer = buffer - 2;

    if (*buffer == NULL)
        return (EOF);
    return (size);
}