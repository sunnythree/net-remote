/************************************************************
** FileName: convert2ekey.h
**Author:      Liu Jinwei
**Company:  KonKa
**Version :    1.0
**Date:           2015.12.11
**Description:   provide some functions   to convert network key to native key
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
***********************************************************/

#ifndef MY_CONVERT2EKEY_H
#define MY_CONVERT2EKEY_H

struct ekey_type  
{
    char *name; /* 键名 */
    int  ekey;   /* key value */
};

int convert_name2ekey(char *name);

#endif
