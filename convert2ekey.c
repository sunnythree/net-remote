/************************************************************
** FileName: convert2ekey.c
**Author:      Liu Jinwei
**Version :    1.0
**Company:  KonKa
**Date:           2015.12.16
**Description:   convert value from network to native
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <convert2ekey.h>


struct ekey_type ekey_type_arr[] = {
    {"POWER",   0xb},
    {"INPUT",   0xf0},
    {"0",      0x0},
    {"1",      0x1},
    {"2",      0x2},
    {"3",      0x3},
    {"4",      0x4},
    {"5",      0x5},
    {"6",      0x6},
    {"7",      0x7},
    {"8",      0x8},
    {"9",      0x9},
    {".",      0x6C},
    {"CH_RTN",   0xe},
    {"VOL_UP",   0x13},
    {"VOL_DOWN", 0x12},
    {"MUTE",     0x14},
    {"EPG",      0x22},
    {"LIST",     0x25},
    {"CH_UP",    0x11},
    {"CH_DOWN",  0x10},
    {"INTERNET", 0x63},
    {"YOUTUBE",  0xf1},
    {"Q.MENU",   0xf2},
    {"INFO",     0xa },
    {"MENU",     0x15},
    {"UP_ARROW", 0x2b},
    {"LEFT_ARROW",0x2d},
    {"RIGHT_ARROW",0x2e},
    {"DOWN_ARROW", 0x2c},
    {"ENTER",    0xf3},
    {"BACK",     0xf4},
    {"EXIT",     0x30},
    {"RED",      0x1a},
    {"GREEN",    0x16},
    {"YELLOW",   0xc },
    {"BLUE",     0x1f},
    {"FREEZE",   0x19},
    {"PICTURE",  0x1d},
    {"ZOOM",     0x1e},
    {"SLEEP",    0x61},
    {"AUDIO",    0x17},
    {"PGUP",     0x27},
    {"SUBTITLE", 0x38},
    {"CCD",      0x62},
    {"PGDN",     0x24},
    {"BACKWARD", 0x68},
    {"PAUSE",    0x66},
    {"FORWARD",  0x67},
    {"BIGBACKWARD",0x69},
    {"STOP",     0x65},
    {"BIGFORWARD", 0x6A}
};

/*************************************************
**Function: convert_name2ekey
**Description: convert key value from network to native
**Input:  
**char *name	-->network key name
**Output: NULL
**Return: positive means return crrect native key value,negative to fail
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0	build this moudle 
*************************************************/


int convert_name2ekey(char *name)
{
    struct ekey_type *type;
    printf("ekey get name=%s\n",name);
 
    for (type = ekey_type_arr; type->name; type++)
    {
        if (strcmp(type->name, name) == 0){
//	    printf("ok,get ekey=%d\n",type->ekey);
            return type->ekey;
	}
    }
 
    return -1;
}
 
