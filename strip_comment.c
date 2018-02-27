/******************************************************************************
*
* FILENAME:
*     strip_comment.c
*
* DESCRIPTION:
*     A demo program to strip all comments string started with a special char:
*     '#' or others.
*
* REVISION(MM/DD/YYYY):
*     10/12/2017  Shengkui Leng (lengshengkui@outlook.com)
*     - Initial version
*
******************************************************************************/
#include "common.h"


int main(int argc, char *argv[])
{
    char *cfg_file = "test.conf";
    if (argc == 2) {
        cfg_file = argv[1];
    }

    /* Read config file and remove comment string */
    char *buf = NULL;
    size_t bytes = read_config_file(cfg_file, &buf, '#');
    if (!bytes) {
        printf("read config file error, or no data in file\n");
        return -1;
    }

    /* Dump the converted file */
    printf("%s\n", buf);
    free(buf);

    return 0;
}
