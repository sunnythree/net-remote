/************************************************************
** FileName: parse.c
**Author:      Liu Jinwei
**Version :    1.0
**Company:  KonKa
**Date:           2015.12.16
**Description:   to parse html request and respond
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0  build this moudle 
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#include <convert2ekey.h>
#include <my_dfb.h>


#define HTTP_FILENAME_LEN   256     /* 文件名长度 */
#define HTTP_BUF_SIZE      1024     /* 缓冲区的大小 */

/* 定义文件类型对应的 Content-Type */
struct doc_type
{
    char *suffix; /* 文件后缀 */
    char *type;   /* Content-Type */
};
 
struct doc_type file_type[] = 
{
    {"html",    "text/html"  },
    {"css",     "text/css"  },
    {"js",      "text/js"  },
    {"gif",     "image/gif"  },
    {"jpeg",    "image/jpeg" },
    {"ico",     "image/ico"  },
    { NULL,      NULL        }
};
 
char *http_res_get = "HTTP/1.1 200 OK\r\nServer: Huiyong's Server <0.1>\r\n"
    "Accept-Ranges: bytes\r\nContent-Length: %d\r\nConnection: close\r\n"
    "Content-Type: %s\r\n\r\n";

char *http_res_post = "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\nConnection:  keep-alive\r\nContent-Length: "
    "%d\r\n\r\n";

char * data="<!DOCTYPE html><meta http-equiv='Content-Type' content='text/html; charset=utf-8'><head><title>Fail Page</title></head><body>The Page you access can not find!<p><p><a href='/index.html'>If you want to access index Page,Please click!</a></body></html>";
 
 
 /*************************************************
 **Function: http_get_type_by_suffix
 **Description: get file suffix in URL
 **Input:   filename(include suffix)
 **Output:	NULL
 **Return: NULL to fial, suffix to sucess
 **Modify History: 
 **<author> <time> <version > <desc>
 **Liu Jinwei  15/12/16 1.0  build this moudle 
 *************************************************/

char *http_get_type_by_suffix(const char *suffix)
{
    struct doc_type *type;
 
    for (type = file_type; type->suffix; type++)
    {
        if (strcmp(type->suffix, suffix) == 0)
            return type->type;
    }
 
    return NULL;
}
 
 /*************************************************
 **Function: http_parse_request_cmd
 **Description: parse html request,get filename and file suffix
 **Input:  
 **char *buf   -->html request
 **int buflen  -->html request length
 **Output:
 **char *file_name  -->getted file name
 **char *suffix         -->getted file suffix
 **Return: NULL
 **Modify History: 
 **<author> <time> <version > <desc>
 **Liu Jinwei  15/12/16 1.0  build this moudle 
 *************************************************/

void http_parse_request_cmd(char *buf, int buflen, char *file_name, char *suffix)
{
    int length = 0;
    char *begin, *end;

    memset(file_name,0,sizeof(file_name));
    memset(suffix,0,sizeof(suffix));

    begin = strchr(buf, ' ');
    begin += 1;
         
    end = strchr(begin, ' ');
    *end = '\0';
 
    length = end - begin;

    if (length > 0)
    {
        memcpy(file_name, begin, length);
        file_name[length] ='\0';
        printf("filename =%s\n",file_name);
        begin = strchr(file_name, '.');
        if (begin)
            strcpy(suffix, begin + 1);
	
    }  
}
 /*************************************************
 **Function: deal_web_get
 **Description: deal get request
 **Input:  
 **int soc       -->connection socket fd
 **char *buf  -->html request data
 **int buf_len  -->html request length
 **Output: NULL
 **Return: 0 to sucess,negative to fail
 **Modify History: 
 **<author> <time> <version > <desc>
 **Liu Jinwei  15/12/16 1.0  build this moudle 
 *************************************************/

int deal_web_get(int soc, char *buf, int buf_len)
{
    int read_len, file_len, hdr_len, send_len;
    char *type;
    char read_buf[HTTP_BUF_SIZE];
    char http_header[HTTP_BUF_SIZE];
    char file_name[HTTP_FILENAME_LEN] = "index.html", suffix[16] = "html";
    char file_name1[HTTP_FILENAME_LEN];
    FILE *res_file=NULL;

  /* 得到文件名和后缀 */
    if(buf && buf_len>0)
    	http_parse_request_cmd(buf, buf_len, file_name1, suffix);
    else  printf("no buf input\n");
    sprintf(file_name,"%s%s","/Customer/remoteC",file_name1);
    res_file = fopen(file_name, "rb+"); /* 用二进制格式打开文件 */
    if (res_file == NULL)
    {
        printf("[Web] The file [%s] is not existed\n", file_name);
	hdr_len = sprintf(http_header, "HTTP/1.1 404 FAIL\r\nContent-Type: text/html;charset=utf-8\r\nConnection:  keep-alive\r\nContent-Length: %d\r\n\r\n", strlen(data));
	send_len = send(soc, http_header, hdr_len, 0);
        if(send_len != hdr_len)
	    printf("socket send http_header err\n");
        printf("first send length is %d\n",send_len);
        send_len = send(soc, data, strlen(data), 0);
        if(send_len != hdr_len)
	    printf("socket send data err\n");
        printf("second send length is %d\n",send_len);
        return -1;
    }
 
    fseek(res_file, 0, SEEK_END);
    file_len = ftell(res_file);
    fseek(res_file, 0, SEEK_SET);
     
    type = http_get_type_by_suffix(suffix); 
    if (type == NULL)
    {
        printf("[Web] There is not the related content type\n");
        return 0;
    }
 
    hdr_len = sprintf(http_header, http_res_get, file_len, type);
    send_len = send(soc, http_header, hdr_len, 0);

 
    do 
    {
        read_len = fread(read_buf, sizeof(char), HTTP_BUF_SIZE, res_file);
 
        if (read_len > 0)
        {
            send_len = send(soc, read_buf, read_len, 0);
            file_len -= read_len;
        }
    } while ((read_len > 0) && (file_len > 0));
 
    fclose(res_file);
    return 0;
}

/*************************************************
**Function: deal_web_post
**Description: deal post request
**Input:  
**int soc		-->connection socket fd
**char *buf  -->html request data
**int buf_len  -->html request length
**Output: NULL
**Return: 0 to sucess,negative to fail
**Modify History: 
**<author> <time> <version > <desc>
**Liu Jinwei  15/12/16 1.0	build this moudle 
*************************************************/

int deal_web_post(int soc, char *buf, int buf_len)
{
    char * cmd;
    char * addr_clength;
    char res[HTTP_BUF_SIZE];
    int  send_len=0,cmd_str_l,clength;
    int ekey=-1;

    addr_clength = strstr(buf,"Content-Length:");
    addr_clength += 15;
    while((*addr_clength) == ' ')addr_clength++;
    clength = atoi(addr_clength);
//    printf("i convert clength =%d\n",clength);
    
    cmd = strstr(buf,"\r\n\r\n");
    cmd += 4;
    cmd_str_l = clength-2;
//    printf("cmd str length = %d\n",cmd_str_l);
    if(cmd_str_l < 0)return -1;
    cmd[cmd_str_l] = '\0';
//    printf("accept cmd =%s\n",cmd);
    sprintf(res,http_res_post,strlen(res));

    send_len = send(soc, res, strlen(res), 0);
    if(send_len != strlen(res)){
	printf("send error\n");
    }

    ekey = convert_name2ekey(cmd);
    if(ekey >= 0){
	dfb_sendir(ekey);
    }else{
	printf("get convert ekey fail,ekey = %d\n",ekey);
    }
    return 0;
}
 
 
 /*************************************************
 **Function: http_send_response
 **Description: deal http request firstly,to distribute request
 **Input:  
 **int soc		 -->connection socket fd
 **char *buf  -->html request data
 **int buf_len	-->html request length
 **Output: NULL
 **Return: 0 to sucess,negative to fail
 **Modify History: 
 **<author> <time> <version > <desc>
 **Liu Jinwei  15/12/16 1.0  build this moudle 
 *************************************************/

int http_send_response(int soc, char *buf, int buf_len)
{
	
 //   printf("get buff=%s\n",buf);
    if(strncmp(buf,"GET",3)==0){
	if(deal_web_get(soc,buf,buf_len)<0);
	    return -1;
    }else if(strncmp(buf,"POST",4)==0){
	if(deal_web_post(soc,buf,buf_len)<0);
	    return -1;
    }
       
    return 0;
}
 
 /*************************************************
 **Function: http_send_server_busy
 **Description: when server busy,call it to tell client server is busy
 **Input:  
 **int soc		 -->connection socket fd
 **Output: NULL
 **Return: 0 to sucess,negative to fail
 **Modify History: 
 **<author> <time> <version > <desc>
 **Liu Jinwei  15/12/16 1.0  build this moudle 
 *************************************************/

int http_send_server_busy(int soc)
{
    int hdr_len,send_len;
    char http_header[HTTP_BUF_SIZE];
    char * datal="<!DOCTYPE html><meta http-equiv='Content-Type' content='text/html; charset=utf-8'><head><title>Fail Page</title></head><body>The Server is busy,please wait!</body></html>";
	hdr_len = sprintf(http_header, "HTTP/1.1 404 FAIL\r\nContent-Type: text/html;charset=utf-8\r\nConnection:  keep-alive\r\nContent-Length: %d\r\n\r\n", strlen(datal));
	send_len = send(soc, http_header, hdr_len, 0);
        if(send_len != hdr_len)
	    printf("socket send http_header err\n");
        printf("first send length is %d\n",send_len);
        send_len = send(soc, datal, strlen(datal), 0);
        if(send_len != strlen(datal)){
	    printf("socket send data err\n");
	    return -1;
	}
        printf("second send length is %d\n",send_len);
        return 0;
}
 
