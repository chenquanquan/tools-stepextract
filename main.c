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


#define FILENAMEHEAD            "Record_step-"
#define NUMBER_MEMBER           3
#define FILE_NUMBER_MAX         200

char *file_name_list[FILE_NUMBER_MAX];
int file_count = 0;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  printdir
 *  Description:  
 * =====================================================================================
 */
void printdir(char *dir, int depth)
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
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode)) {
            /* Found a directory, but ignore . and .. */
            if (strcmp(".", entry->d_name) == 0 ||
                    strcmp("..", entry->d_name) == 0)
                continue;
            //printf("directory: %*s%s/\n", depth, " ", entry->d_name);
            /* Recurse at a new indent level */
            printdir(entry->d_name, depth+4);
            chdir("..");
        } else {
            //printf("file: %d%s%s\n", depth, " - ", entry->d_name);
            //printf("%s/%s\n", dir, entry->d_name);

            /* Output the matching file name */
            if (strncmp(FILENAMEHEAD, entry->d_name, sizeof(FILENAMEHEAD)/sizeof (char)-1) == 0) {
                printf("%s/%s\n", dir, entry->d_name);
            }
        }
    }
    closedir(dp);
}       /* -----  end of function printdir  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_file_name
 *  Description:  
 * =====================================================================================
 */
void get_file_name(char *dir)
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
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode)) {
            /* Found a directory, but ignore . and .. */
            if (strcmp(".", entry->d_name) == 0 ||
                    strcmp("..", entry->d_name) == 0)
                continue;
            /* Recurse at a new indent level */
            get_file_name(entry->d_name);
            chdir("..");
        } else {
            /* Output the matching file name */
            if (strncmp(FILENAMEHEAD, entry->d_name, sizeof(FILENAMEHEAD)/sizeof (char)-1) == 0) {
                file_name_list[file_count] = (char *)malloc((strlen(dir)+strlen(entry->d_name)+2)*sizeof (char)); /* The 2 byte is the '\0' and '\' in string */
                //file_name_list[file_count] = (char *)malloc(FILE_NAME_MAX * sizeof (char));
                sprintf(file_name_list[file_count], "%s/%s", dir, entry->d_name);
                file_count++;
            }
        }
    }
    closedir(dp);
}		/* -----  end of function get_file_name  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  extract_file
 *  Description:  
 * =====================================================================================
 */
int extract_file(char *filename, int len)
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
        if (i%NUMBER_MEMBER == 2) {
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
 *         Name:  extract_all
 *  Description:  
 * =====================================================================================
 */
int extract_all(char *filename)
{
    //short buffer[NUMBER_MEMBER];
    short *buffer;
    int filp = 0;
    int i, len, count;


    buffer = (short *)malloc(NUMBER_MEMBER*sizeof (short));
    filp = open(filename, O_RDONLY);

    len = NUMBER_MEMBER*2;
    for (count=0; 1; count+=NUMBER_MEMBER) {
        if (read(filp, buffer, len) != len) {
            break;
        }
        for (i=0; i<NUMBER_MEMBER; i++) {
            printf("%d,", buffer[i]);
        }
        printf("\n");
    }

    close(filp);
    free(buffer);

    return count;
}		/* -----  end of function extract_all  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  file_compar
 *  Description:  
 * =====================================================================================
 */
int file_compar(const void *arry1, const void *arry2)
{
    int num1, num2;
    char *const*str1 = arry1;
    char *const*str2 = arry2;


    num1 = atoi(rindex(*str1, '-')+1); //Get file number
    num2 = atoi(rindex(*str2, '-')+1);

    if (num1 > num2)
        return 1;
    else if (num1 < num2)
        return -1;
    else
        return 0;
}		/* -----  end of function file_compar  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
    int i;


    //printdir(".", 0);
    get_file_name(".");
    qsort(file_name_list, file_count, sizeof (char *), file_compar);
    for (i=0; i<file_count; i++){
        printf("%s\n", file_name_list[i]);
        //extract_all(file_name_list[i]);
        free(file_name_list[i]);
    }

    return EXIT_SUCCESS;
}               /* ----------  end of function main  ---------- */
