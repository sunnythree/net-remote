/************************************************************
** FileName: remote-C.c
**Author:      Liu Jinwei
**Version :    1.0
**Company:  KonKa
**Date:           2015.12.16
**Description:   network remote control
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <my_socket.h>
#include <my_dfb.h>

int socket_fd;

/*************************************************
**Function: print_usage
**Description: print usage
**Input: NULL
**Output:  NULL
**Return: NULL
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
*************************************************/

void print_usage()
{
    printf("usage: ./server <port>\n");  
    exit(0);  
}

/*************************************************
**Function: cleanserver
**Description: clean when server exit
**Input: NULL
**Output:  NULL
**Return: NULL
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
*************************************************/

void cleanserver(void)
{
	server_exit(socket_fd);
	printf("cleanserver cleaned server\n");
}

/*************************************************
**Function: main
**Description: a little web server
**Input: 
**usage ./remote-C.bin 80 
**argv[1] = 80  --> server port
**Output:  NULL
**Return: 0 to sucess ,negative fo err
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
*************************************************/

int main(int argc, char** argv)  
{
int port;
	if(argc != 2)print_usage();
	port = atoi(argv[1]);
	printf("you input port is %d\n",port);
	socket_fd = server_init(port);
	atexit(cleanserver);
	if(socket_fd < 0){
	    printf("server init error,socket_fd = %d\n",socket_fd);
	    return -1;
	}
	directfb_init();
	server_deal_request(socket_fd);
return 0;
}  


