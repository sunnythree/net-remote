/************************************************************
** FileName: parse.h
**Author:      Liu Jinwei
**Company:  KonKa
**Version :    1.0
**Date:           2015.12.11
**Description:  parse html and respond
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
***********************************************************/

#ifndef MY_PARSE_H
#define MY_PARSE_H
int http_send_server_busy(int soc);
int http_send_response(int soc, char *buf, int buf_len);

#endif
