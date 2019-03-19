#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<string.h>

int main(int argc,char* argv[])
{
  if(argc<3)
  {
    printf("./server [IP] [port]\n");
    return 1;
  }
   //创建套接字
  int serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(serv_sock<0)
  {
    printf("socket!!\n");
    return 2;
  }
  //绑定地址和端口号
 struct sockaddr_in serv_addr;
 struct sockaddr_in cli_addr;
 serv_addr.sin_family = AF_INET;
 serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
 serv_addr.sin_port = htons(atoi(argv[2]));
 socklen_t len = sizeof(struct sockaddr_in);
 int ret = bind(serv_sock,(const struct sockaddr*)&serv_addr,len);
 if(ret<0)
 {
   printf("bind!\n");
   return 3;
 }
 //监听套接字
 ret = listen(serv_sock,5);
 if(ret<0)
 {
   printf("listen!\n");
   return 4;
 }
 //循环处理client数据
 while(1)
 {
   int client_sock = accept(serv_sock,(struct sockaddr*)&cli_addr,&len);
   if(client_sock<0)
   {
     printf("accept!\n");
     return 5;
   }
   while(1)
   {
    char buf[1024] = {0};
    ssize_t rs = recv(client_sock,buf,1023,0);
    if(rs<0)
    {
      printf("read!\n");
      return 6;
    }
    else if(rs == 0)
    {
      printf("client close!\n");
      break;
    }
    printf("cllient[say]:%s",buf);
    fflush(stdout);
    printf("\n");
    char* resp = "hello";
    send(client_sock,resp,strlen(resp),0);
   }
 }
 close(serv_sock);
 return 0;
}
