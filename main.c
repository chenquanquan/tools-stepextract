/* main.c -  
 *
 *     1.0 - 2015/2/15 16:06:30
 *
 */
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  printdir
 *  Description:  
 * =====================================================================================
 */
void printdir (char *dir, int depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;


    if ((dp = opendir(dir)) == NULL) {
        fprintf(stderr, "cannot open directory: %s\n", dir);
        return;
    }
    chdir(dir);
    while ((entry = readdir(dp)) != NULL) {
        stat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode)) {
            /* Found a directory, but ignore . and .. */
            if (strcmp(".", entry->d_name) == 0 ||
                    strcmp("..", entry->d_name) == 0)
                continue;
            printf("directory: %*s%s/\n", depth, " ", entry->d_name);
            /* Recurse at a new indent level */
            printdir(entry->d_name, depth+4);
        } else {
            printf("file: %*s%s\n", depth, " ", entry->d_name);
        }
    }
    chdir("..");
    closedir(dp);
}       /* -----  end of function printdir  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  extract_file
 *  Description:  
 * =====================================================================================
 */
int extract_file (char *filename, int len)
{
    int i;
    int nlen;
    int filp = 0;
    short buffer[1000];


    filp = open(filename, O_RDONLY);

    nlen = read(filp, buffer, len*2);
    if (nlen != len*2) {
        printf("Error len %d: the file %s only read %d!\n", len, filename, nlen/2);
    }

    printf("\n");
    for (i=0; i<nlen/2; i++) {
        printf("%d", buffer[i]);
        if (i%3 == 2) {
            printf("\n");
        } else {
            printf(",");
        }
    }
    printf("\n");

    close(filp);


    return nlen;
}       /* -----  end of function extract_file  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
    int len;


    /* Input the data long */
    if (argc > 1) {
        len = atoi(argv[1]);
    } else {
        len = 9;
    }

    printdir(".", 0);
    extract_file("./file.txt", len);

    return EXIT_SUCCESS;
}               /* ----------  end of function main  ---------- */
