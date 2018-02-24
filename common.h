/******************************************************************************
*
* FILENAME:
*     common.h
*
* DESCRIPTION:
*     Common definition.
*
* REVISION(MM/DD/YYYY):
*     08/04/2017  Shengkui Leng (shengkui.leng@advantech.com.cn)
*     - Initial version
*
******************************************************************************/
#ifndef _COMMON_H_
#define _COMMON_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

size_t read_config_file(char *file, char **out_buf, char comment_char);

#endif /* _COMMON_H_ */
