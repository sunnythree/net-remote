/************************************************************
** FileName: my_socket.h
**Author:      Liu Jinwei
**Company:  KonKa
**Version :    1.0
**Date:           2015.12.11
**Description:   socket server point
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
***********************************************************/

#ifndef MY_SOCKET_H
#define MY_SOCKET_H


int server_init(int port);
int server_deal_request(int socket_fd);
int server_exit(int socket_fd);

#endif
