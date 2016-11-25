/************************************************************
** FileName: my_socket.c
**Author:      Liu Jinwei
**Version :    1.0
**Company:  KonKa
**Date:           2015.12.16
**Description:   a little webserver
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
**Liu Jinwei  15/12/23 1.0  build this moudle 
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <my_socket.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <parse.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/types.h> 
#include <signal.h>   

#define BUF_SIZE 4096  
struct sockaddr_in	   servaddr;  
#define BACKLOG 100     // how many pending connections queue will hold
int fd_A[BACKLOG];    // accepted connection fd
static pthread_mutex_t httplock; 
static pthread_mutex_t sleeplock;
static pthread_cond_t  http_cond;
static volatile int connect_fd=0;
static volatile int client_fd[20];

/*************************************************
**Function: server_init
**Description: init server
**Input: server sport 
**Output:  NULL
**Return: server fd
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
*************************************************/

int server_init(int port)
{
    int    socket_fd;  

    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  
    printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);  
    exit(0);  
    }  
    memset(&servaddr, 0, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。  
    servaddr.sin_port = htons(port);//设置的端口为DEFAULT_PORT  
  
    //将本地地址绑定到所创建的套接字上  
    if( bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){  
    printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);  
    exit(0);  
    }  
	
    if( listen(socket_fd, 10) == -1){  
    printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);  
    exit(0);  
    }  

    return socket_fd;
}
/*************************************************
**Function: clean_client
**Description: clean chile thread
**Input: NULL
**Output:  NULL
**Return: NULL
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/23 1.0  build this moudle 
*************************************************/

void *clean_client(void *arg)
{
    printf("thread %d exit\n",(int)pthread_self());
    pthread_mutex_destroy(&httplock); 
    pthread_mutex_destroy(&sleeplock);
    pthread_cond_destroy(&http_cond);
    close(connect_fd); 
   return NULL;
}

/*************************************************
**Function: client_handle
**Description: child thread,deal request
**Input: NULL
**Output:  NULL
**Return: NULL
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
**Liu Jinwei  15/12/23 1.0  build this moudle 
*************************************************/
static void client_handle(void * client)
{
char run=1;
int n,i;
char	buff[4096];  
struct timeval timeout = {0,100}; 
    pthread_detach(pthread_self());
    pthread_cleanup_push((void *)clean_client,NULL);
    while(run){
    	    pthread_mutex_lock(&sleeplock);
   	    pthread_cond_wait(&http_cond,&sleeplock);
   	    pthread_mutex_unlock(&sleeplock);	
	    printf("wake up\n");
	    for(i=0;i<20;i++){
		if(client_fd[i]>0){
		    if(setsockopt(client_fd[i],SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout))==-1)printf("sockopt set recv error\n");
		    if(setsockopt(client_fd[i],SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeout))==-1)printf("sockopt set send error\n");
		    for(;;){
		        printf("begin to get data\n");
			memset(buff,0,sizeof(buff));
			n = recv(client_fd[i], buff, BUF_SIZE, 0);  
			if(n==BUF_SIZE){
			//    printf("%s\n",buff);
			    printf("-------------------start deal-----------------------\n");
			    http_send_response(client_fd[i], buff, n);
			    printf("get enoucht data continue\n");
			    continue;
			}else if(n>0){
			    printf("-------------------start deal-----------------------\n");
			    http_send_response(client_fd[i], buff, n);
			    close(client_fd[i]);
		    	    pthread_mutex_lock(&httplock);
		   	    client_fd[i]=0;
		    	    pthread_mutex_unlock(&httplock);
			    printf("get little data break\n");
			    break;
			}
			else {
			    printf("other port disconnect\n");
			    close(client_fd[i]);
		    	    pthread_mutex_lock(&httplock);
		   	    client_fd[i]=0;
		   	    pthread_mutex_unlock(&httplock);	
			    break;
			} 
	            }				
		}
	    }
	printf("another wile\n");
    }

    pthread_cleanup_pop(1);
}
/*************************************************
**Function: server_deal_request
**Description: this function is nessary for accept SIGPIPE when newwork interrupt,else process will exit; 
**Input: NULL
**Output:  NULL
**Return: NULL
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/23 1.0  build this moudle 
*************************************************/
void signal_func(){ 
   printf("haha func\n");
}

/*************************************************
**Function: server_deal_request
**Description: web server 
**Input: NULL
**Output:  NULL
**Return: 0 to sucess,negative to fail
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
**Liu Jinwei  15/12/23 1.0  build this moudle 
*************************************************/

int server_deal_request(int socket_fd)
{
pthread_t thread_id;
int i=0;
      signal(SIGPIPE,signal_func);
      if(pthread_mutex_init(&httplock,NULL)){
	printf("mutex init error");
	return -1;
	} 
      if(pthread_mutex_init(&sleeplock,NULL)){
	printf("mutex init error");
	return -1;
	} 
      if(pthread_cond_init(&http_cond,NULL)){
	printf("cond init error");
	return -1;
	} 
      if(pthread_create(&thread_id,NULL,(void *)(&client_handle),(void *)(&connect_fd)) == -1)
      {
	    printf("pthread_create return err\n");
	    return -2;                               //break while loop
      }
      for(i=0;i<20;i++)
	    client_fd[i] = 0;
	    printf("======waiting for client's request======\n");  
	    while(1)
	    {  
	           if( (connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1){  
		          printf("accept socket error: %s(errno: %d)",strerror(errno),errno);  
		          continue;  
	            }  
		   printf("request arrived here\n");
		   i=0;
		   while(client_fd[i] != 0 && i<20)i++;
		   if(i<20){
		        pthread_mutex_lock(&httplock);
			client_fd[i] = connect_fd;
		        pthread_mutex_unlock(&httplock);	
		   }else{
			printf("server busy\n");
		   }
		   printf("server i = %d\n",i);
    	           pthread_mutex_lock(&sleeplock);
   	    	   pthread_cond_signal(&http_cond);
   	   	   pthread_mutex_unlock(&sleeplock);	
	    }  
        close(socket_fd);
	return 0;
}
/*************************************************
**Function: server_exit
**Description: web server exit 
**Input: NULL
**Output:  NULL
**Return: 0 to sucess
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
**Liu Jinwei  15/12/23 1.0  build this moudle 
*************************************************/

int server_exit(int socket_fd)
{
	close(socket_fd);
	return 0;
}
