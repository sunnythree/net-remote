/************************************************************
** FileName: my_dfb.h
**Author:      Liu Jinwei
**Company:  KonKa
**Version :    1.0
**Date:           2015.12.11
**Description:   provide some functions   to produce events to MUF with dfb functions
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
***********************************************************/

#ifndef MY_DFB_H
#define MY_DFB_H
#include <directfb.h>

void dfb_sendir(int ekey);
DFBEnumerationResult   input_device_callback(DFBInputDeviceID id, DFBInputDeviceDescription desc, void* data);
void directfb_init();

#endif
