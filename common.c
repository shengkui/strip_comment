/******************************************************************************
*
* FILENAME:
*     common.c
*
* DESCRIPTION:
*     Define some useful functions.
*
* REVISION(MM/DD/YYYY):
*     08/04/2017  Shengkui Leng (lengshengkui@outlook.com)
*     - Create the file
*     10/12/2017  Shengkui Leng (lengshengkui@outlook.com)
*     - Add "trim space" code into remove_comment function
*
******************************************************************************/
#include "common.h"

#define MAX_LINE        4096


/***************************************************************************
 * NAME:
 *      get_file_size
 *
 * DESCRIPTION:
 *      Get size of a file.
 *
 * PARAMETERS:
 *      filename - The file.
 *
 * RETURN:
 *      Size of file.
 **************************************************************************/
static size_t get_file_size(char *filename)
{
    struct stat sb;

    if (!filename) {
        return 0;
    }

    if (stat(filename, &sb) == -1) {
        perror("stat");
        return 0;
    }
    if ((sb.st_mode & S_IFMT) != S_IFREG) {
        return 0;
    }

    return ((sb.st_size > 0) ? sb.st_size : 0);
}


/******************************************************************************
* NAME:
*     remove_comment
*
* DESCRIPTION:
*     Remove comment and trim trailling whitespace chars from a string.
*     This function support comment string started with a specified char
*     This function remove space, \r, \t', \f, and \v, but add a '\n' to the
*     end of string.
*     NOTES: The string will be modified.
*
* PARAMETERS:
*     str          - The string.
*     comment_char - A leading char of comment string
*
* RETURN:
*     None
******************************************************************************/
static void remove_comment(char *str, char comment_char)
{
    if (str == NULL) {
        return;
    }
    char *s = str;

    /* If there is a comment_char, there is a comment string */
    if (strchr(s, comment_char)) {
        /* Search comment string and remove it */
        int start = 1;
        while (*s) {
            if (*s == comment_char) {
                if (start) {
                    /*
                     * If the comment_char at the begin of line, just remove the
                     * whole line, no '\n' inserted.
                     */
                    *str = '\0';
                } else {
                    /* Added a '\n' to the end of line. */
                    *s = '\n';
                    *(s+1) = '\0';
                }
                break;
            }

            s++;
            start = 0;
        }
    }

    /* Trim trailing whitespace chars */
    char *p = str + strlen(str) - 1;
    while (isspace(*p))
        p--;
    *(p + 1) = '\n';
    *(p + 2) = '\0';
}


/******************************************************************************
* NAME:
*     read_config_file
*
* DESCRIPTION:
*     Read config file, and preprocess the data:
*     (1) Remove comments.
*     (2) Remove leading/trailing spaces of each line.
*
* PARAMETERS:
*     file         - The file to read.
*     out_buf      - Output a buffer store the data readed from file.
*                    This buffer should be freed by the caller.
*     comment_char - A leading char of comment string
*
* RETURN:
*     >0 - The bytes readed
*     0  - No data readed(error occured)
******************************************************************************/
size_t read_config_file(char *file, char **out_buf, char comment_char)
{
    if ((file == NULL) || (out_buf == NULL)) {
        return 0;
    }

    /* Get size of file */
    size_t file_size = get_file_size(file);
    if (file_size == 0) {
        printf("get conf file size error\n");
        return 0;
    }

    /* Reserve space for a null-terminator. */
    file_size++;

    char *buf = malloc(file_size);
    if (buf == NULL) {
        perror("malloc error");
        return 0;
    }
    memset(buf, 0, file_size);

    /* Open file */
    FILE *fp = fopen(file, "r");
    if (NULL == fp) {
        perror("open file error");
        free(buf);
        return 0;
    }

    /*
     * Read the file line-by-line, remove comment, trailing spaces chars and
     * empty line.
     */
    size_t total_bytes = 0;         /* Total bytes in the buf */
    int space_left = file_size - 1; /* Space left in the buf */
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp) != NULL) {
        remove_comment(line, comment_char);

        int len = strlen(line);
        if (len > 1) {
            strncat(buf, line, space_left);
            total_bytes += len;
            space_left -= len;
        }
    }
    fclose(fp);

    if (total_bytes > 0) {
        *out_buf = buf;
    } else {
        *out_buf = NULL;
        free(buf);
    }
    return total_bytes;
}
